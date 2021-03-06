/*
 *
 * Copyright (C) 2009 Nation Technologies Inc.
 *
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/pnp.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include "tcm.h"


#define TCM_HEADER_SIZE 10

enum tis_access {
	TCM_ACCESS_VALID = 0x80,
	TCM_ACCESS_ACTIVE_LOCALITY = 0x20,
	TCM_ACCESS_REQUEST_PENDING = 0x04,
	TCM_ACCESS_REQUEST_USE = 0x02,
};

enum tis_status {
	TCM_STS_VALID = 0x80,
	TCM_STS_COMMAND_READY = 0x40,
	TCM_STS_GO = 0x20,
	TCM_STS_DATA_AVAIL = 0x10,
	TCM_STS_DATA_EXPECT = 0x08,
};

enum tis_int_flags {
	TCM_GLOBAL_INT_ENABLE = 0x80000000,
	TCM_INTF_BURST_COUNT_STATIC = 0x100,
	TCM_INTF_CMD_READY_INT = 0x080,
	TCM_INTF_INT_EDGE_FALLING = 0x040,
	TCM_INTF_INT_EDGE_RISING = 0x020,
	TCM_INTF_INT_LEVEL_LOW = 0x010,
	TCM_INTF_INT_LEVEL_HIGH = 0x008,
	TCM_INTF_LOCALITY_CHANGE_INT = 0x004,
	TCM_INTF_STS_VALID_INT = 0x002,
	TCM_INTF_DATA_AVAIL_INT = 0x001,
};

enum tis_defaults {
	TIS_MEM_BASE = 0xFED40000,
	TIS_MEM_LEN = 0x5000,
	TIS_SHORT_TIMEOUT = 750,	/* ms */
	TIS_LONG_TIMEOUT = 2000,	/* 2 sec */
};

#define	TCM_ACCESS(l)			(0x0000 | ((l) << 12))
#define	TCM_INT_ENABLE(l)		(0x0008 | ((l) << 12)) //interpert
#define	TCM_INT_VECTOR(l)		(0x000C | ((l) << 12))
#define	TCM_INT_STATUS(l)		(0x0010 | ((l) << 12))
#define	TCM_INTF_CAPS(l)		(0x0014 | ((l) << 12))
#define	TCM_STS(l)			(0x0018 | ((l) << 12))
#define	TCM_DATA_FIFO(l)		(0x0024 | ((l) << 12))

#define	TCM_DID_VID(l)			(0x0F00 | ((l) << 12))
#define	TCM_RID(l)			(0x0F04 | ((l) << 12))

static LIST_HEAD(tis_chips);
static DEFINE_SPINLOCK(tis_lock);

static int check_locality(struct tcm_chip *chip, int l)
{
	if ((ioread8(chip->vendor.iobase + TCM_ACCESS(l)) &
	     (TCM_ACCESS_ACTIVE_LOCALITY | TCM_ACCESS_VALID)) ==
	    (TCM_ACCESS_ACTIVE_LOCALITY | TCM_ACCESS_VALID))
		return chip->vendor.locality = l;

	return -1;
}

static void release_locality(struct tcm_chip *chip, int l, int force)
{
	if (force || (ioread8(chip->vendor.iobase + TCM_ACCESS(l)) &
		      (TCM_ACCESS_REQUEST_PENDING | TCM_ACCESS_VALID)) ==
	    (TCM_ACCESS_REQUEST_PENDING | TCM_ACCESS_VALID))
		iowrite8(TCM_ACCESS_ACTIVE_LOCALITY,
			 chip->vendor.iobase + TCM_ACCESS(l));
}

static int request_locality(struct tcm_chip *chip, int l)
{
	unsigned long stop;

	if (check_locality(chip, l) >= 0)
		return l;

	iowrite8(TCM_ACCESS_REQUEST_USE,
		 chip->vendor.iobase + TCM_ACCESS(l));

	/* wait for burstcount */
	stop = jiffies + chip->vendor.timeout_a;
	do {
		if (check_locality(chip, l) >= 0)
			return l;
		msleep(TCM_TIMEOUT);
	}while (time_before(jiffies, stop));

	return -1;
}

static u8 tcm_tis_status(struct tcm_chip *chip)
{
	return ioread8(chip->vendor.iobase +
		       TCM_STS(chip->vendor.locality));
}

static void tcm_tis_ready(struct tcm_chip *chip)
{
	/* this causes the current command to be aborted */
	iowrite8(TCM_STS_COMMAND_READY,
		 chip->vendor.iobase + TCM_STS(chip->vendor.locality));
}

static int get_burstcount(struct tcm_chip *chip)
{
	unsigned long stop;
	int burstcnt;

	/* wait for burstcount */
	/* which timeout value, spec has 2 answers (c & d) */
	stop = jiffies + chip->vendor.timeout_d;
	do {
		burstcnt = ioread8(chip->vendor.iobase +
				   TCM_STS(chip->vendor.locality) + 1);
		burstcnt += ioread8(chip->vendor.iobase +
				    TCM_STS(chip->vendor.locality) +
				    2) << 8;
		if (burstcnt)
			return burstcnt;
		msleep(TCM_TIMEOUT);
	} while (time_before(jiffies, stop));
	return -EBUSY;
}

static int wait_for_stat(struct tcm_chip *chip, u8 mask, unsigned long timeout,
			 wait_queue_head_t *queue)
{
	unsigned long stop;
	u8 status;

	/* check current status */
	status = tcm_tis_status(chip);
	if ((status & mask) == mask)
		return 0;

	stop = jiffies + timeout;
	do {
		msleep(TCM_TIMEOUT);
		status = tcm_tis_status(chip);
		if ((status & mask) == mask)
			return 0;
	} while (time_before(jiffies, stop));
	return -ETIME;
}

static int recv_data(struct tcm_chip *chip, u8 *buf, size_t count)
{
	int size = 0, burstcnt;
	while (size < count &&
	       wait_for_stat(chip,
			     TCM_STS_DATA_AVAIL | TCM_STS_VALID,
			     chip->vendor.timeout_c,
			     &chip->vendor.read_queue)
	       == 0) {
		burstcnt = get_burstcount(chip);
		for (; burstcnt > 0 && size < count; burstcnt--)
			buf[size++] = ioread8(chip->vendor.iobase +
					      TCM_DATA_FIFO(chip->vendor.
							    locality));
	}
	return size;
}

static int tcm_tis_recv(struct tcm_chip *chip, u8 *buf, size_t count)
{
	//dev_info(chip->dev,"-Call tcm_tis_recv\n");
	int size = 0;
	int expected, status;
	unsigned long stop;
	if (count < TCM_HEADER_SIZE) {
		size = -EIO;
		goto out;
	}

	/* read first 10 bytes, including tag, paramsize, and result */
	if ((size =
	     recv_data(chip, buf, TCM_HEADER_SIZE)) < TCM_HEADER_SIZE) {
		dev_err(chip->dev, "Unable to read header\n");
		goto out;
	}

	expected = be32_to_cpu(*(__be32 *) (buf + 2));
	if (expected > count) {
		size = -EIO;
		goto out;
	}

	if ((size +=
	     recv_data(chip, &buf[TCM_HEADER_SIZE],
		       expected - TCM_HEADER_SIZE)) < expected) {
		dev_err(chip->dev, "Unable to read remainder of result\n");
		size = -ETIME;
		goto out;
	}

	wait_for_stat(chip, TCM_STS_VALID, chip->vendor.timeout_c,
		      &chip->vendor.int_queue);

	stop = jiffies + chip->vendor.timeout_c;
	do 
	{
		msleep(TCM_TIMEOUT);
		status = tcm_tis_status(chip);
		if( (status & TCM_STS_DATA_AVAIL) == 0 )
		  break;
		
	}while(time_before(jiffies,stop));

	status = tcm_tis_status(chip);
	if (status & TCM_STS_DATA_AVAIL) {	/* retry? */
		dev_err(chip->dev, "Error left over data\n");
		size = -EIO;
		goto out;
	}

out:
	tcm_tis_ready(chip);
	release_locality(chip, chip->vendor.locality, 0);
	return size;
}

/*
 * If interrupts are used (signaled by an irq set in the vendor structure)
 * tcm.c can skip polling for the data to be available as the interrupt is
 * waited for here
 */
static int tcm_tis_send(struct tcm_chip *chip, u8 *buf, size_t len)
{

	int rc, status, burstcnt;
	size_t count = 0;
	u32 ordinal;
       	unsigned long stop;
	if (request_locality(chip, 0) < 0)
	{
		return -EBUSY;
	}
	status = tcm_tis_status(chip);
	
	if ((status & TCM_STS_COMMAND_READY) == 0) {
		tcm_tis_ready(chip);
		if (wait_for_stat
		    (chip, TCM_STS_COMMAND_READY, chip->vendor.timeout_b,
		     &chip->vendor.int_queue) < 0) {
			rc = -ETIME;
			goto out_err;
		}
	}
	while (count < len - 1) {
		burstcnt = get_burstcount(chip);
		for (; burstcnt > 0 && count < len - 1; burstcnt--) {
			iowrite8(buf[count], chip->vendor.iobase +
				 TCM_DATA_FIFO(chip->vendor.locality));
			count++;
		}
		
		wait_for_stat(chip, TCM_STS_VALID, chip->vendor.timeout_c,
			      &chip->vendor.int_queue);
	}


	/* write last byte */
	iowrite8(buf[count],chip->vendor.iobase +TCM_DATA_FIFO(chip->vendor.locality));
	wait_for_stat(chip, TCM_STS_VALID, chip->vendor.timeout_c,  &chip->vendor.int_queue);
	stop = jiffies + chip->vendor.timeout_c;
	do 
	{
		msleep(TCM_TIMEOUT);
		status = tcm_tis_status(chip);
		if( (status & TCM_STS_DATA_EXPECT) == 0 )
		  break;
		
	}while(time_before(jiffies,stop));

	if ((status & TCM_STS_DATA_EXPECT) != 0)
	{
		rc = -EIO;
		goto out_err;
	}
	
	/* go and do it */
	iowrite8(TCM_STS_GO,
		 chip->vendor.iobase + TCM_STS(chip->vendor.locality));

	ordinal = be32_to_cpu(*((__be32 *) (buf + 6)));
	ordinal = buf[9];
	if (wait_for_stat
		    (chip, TCM_STS_DATA_AVAIL | TCM_STS_VALID,
		       tcm_calc_ordinal_duration(chip, ordinal),
		      &chip->vendor.read_queue) < 0) {
			rc = -ETIME;
			goto out_err;
		}
	return len;
out_err:
	tcm_tis_ready(chip);
	release_locality(chip, chip->vendor.locality, 0);
	return rc;
}

static struct file_operations tis_ops = {
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.open = tcm_open,
	.read = tcm_read,
	.write = tcm_write,
	.release = tcm_release,
};

static DEVICE_ATTR(pubek, S_IRUGO, tcm_show_pubek, NULL);
static DEVICE_ATTR(pcrs, S_IRUGO, tcm_show_pcrs, NULL);
static DEVICE_ATTR(enabled, S_IRUGO, tcm_show_enabled, NULL);
static DEVICE_ATTR(active, S_IRUGO, tcm_show_active, NULL);
static DEVICE_ATTR(owned, S_IRUGO, tcm_show_owned, NULL);
static DEVICE_ATTR(temp_deactivated, S_IRUGO, tcm_show_temp_deactivated,
		   NULL);
static DEVICE_ATTR(caps, S_IRUGO,tcm_show_caps, NULL);
static DEVICE_ATTR(cancel, S_IWUSR | S_IWGRP, NULL, tcm_store_cancel);

static struct attribute *tis_attrs[] = {
	&dev_attr_pubek.attr,
	&dev_attr_pcrs.attr,
	&dev_attr_enabled.attr,
	&dev_attr_active.attr,
	&dev_attr_owned.attr,
	&dev_attr_temp_deactivated.attr,
	&dev_attr_caps.attr,
	&dev_attr_cancel.attr, NULL,
};

static struct attribute_group tis_attr_grp = {
	.attrs = tis_attrs
};

static struct tcm_vendor_specific tcm_tis = {
	.status = tcm_tis_status,
	.recv = tcm_tis_recv,
	.send = tcm_tis_send,
	.cancel = tcm_tis_ready,
	.req_complete_mask = TCM_STS_DATA_AVAIL | TCM_STS_VALID,
	.req_complete_val = TCM_STS_DATA_AVAIL | TCM_STS_VALID,
	.req_canceled = TCM_STS_COMMAND_READY,
	.attr_group = &tis_attr_grp,
	.miscdev = {
		    .fops = &tis_ops,},
};

static int tcm_tis_init(struct device *dev, unsigned long start,
			unsigned long len)
{
	u32 vendor, intfcaps;
	int rc;
	struct tcm_chip *chip;

	start = TIS_MEM_BASE;
	len = TIS_MEM_LEN;

	if (!(chip = tcm_register_hardware(dev, &tcm_tis)))
		return -ENODEV;

	chip->vendor.iobase = ioremap(start, len);
	if (!chip->vendor.iobase) {
		rc = -EIO;
		goto out_err;
	}
	vendor = ioread32(chip->vendor.iobase + TCM_DID_VID(0));

	if( (vendor & 0xffff) != 0x19f5 && (vendor & 0xffff) != 0x1B4E )
		{
			dev_err(dev,"there is  no Nationz TCM on you computer");
			goto out_err;
		}
	/* Default timeouts */
	chip->vendor.timeout_a = msecs_to_jiffies(TIS_SHORT_TIMEOUT);
	chip->vendor.timeout_b = msecs_to_jiffies(TIS_LONG_TIMEOUT);
	chip->vendor.timeout_c = msecs_to_jiffies(TIS_SHORT_TIMEOUT);
	chip->vendor.timeout_d = msecs_to_jiffies(TIS_SHORT_TIMEOUT);



	/* Figure out the capabilities */
	intfcaps =
	    ioread32(chip->vendor.iobase +
		     TCM_INTF_CAPS(chip->vendor.locality));
	
	if (request_locality(chip, 0) != 0) {
		rc = -ENODEV;
		goto out_err;
	}

	
	INIT_LIST_HEAD(&chip->vendor.list);
	spin_lock(&tis_lock);
	list_add(&chip->vendor.list, &tis_chips);
	spin_unlock(&tis_lock);

	
	tcm_get_timeouts(chip);
	tcm_startup(chip);
	return 0;
out_err:
	if (chip->vendor.iobase)
		iounmap(chip->vendor.iobase);
	tcm_remove_hardware(chip->dev);
	return rc;
}

static int __devinit tcm_tis_pnp_init(struct pnp_dev *pnp_dev,
				      const struct pnp_device_id *pnp_id)
{
	unsigned long start, len;
	start = pnp_mem_start(pnp_dev, 0);
	len = pnp_mem_len(pnp_dev, 0);

	return tcm_tis_init(&pnp_dev->dev, start, len);
}

static int tcm_tis_pnp_suspend(struct pnp_dev *dev, pm_message_t msg)
{
	return tcm_pm_suspend(&dev->dev, msg);
}

static int tcm_tis_pnp_resume(struct pnp_dev *dev)
{
	return tcm_pm_resume(&dev->dev);
}

static struct pnp_device_id tcm_pnp_tbl[] __devinitdata = {
	{"ZIC0101", 0},		/* Natioinz TCM */
	/* Add new here */
	{"", 0},		/* User Specified */
	{"", 0}			/* Terminator */
};

static struct pnp_driver tis_pnp_driver = {
	.name = "tcm_tis",
	.id_table = tcm_pnp_tbl,
	.probe = tcm_tis_pnp_init,
	.suspend = tcm_tis_pnp_suspend,
	.resume = tcm_tis_pnp_resume,
};

#define TIS_HID_USR_IDX sizeof(tcm_pnp_tbl)/sizeof(struct pnp_device_id) -2

static struct device_driver tis_drv = {
	.name = "tcm_tis",
	.bus = &platform_bus_type,
	.owner = THIS_MODULE,
	.suspend = tcm_pm_suspend,
	.resume = tcm_pm_resume,
};

static struct platform_device *pdev;

static int force;
module_param(force, bool, 0444);
MODULE_PARM_DESC(force, "Force device probe rather than using ACPI entry");
static int __init init_tis(void)
{
	int rc;
	if (force) {
		rc = driver_register(&tis_drv);
		if (rc < 0)
			return rc;
		if (IS_ERR(pdev=platform_device_register_simple("tcm_tis", -1, NULL, 0)))
			return PTR_ERR(pdev);
		if((rc=tcm_tis_init(&pdev->dev, 0, 0)) != 0) {
			platform_device_unregister(pdev);
			driver_unregister(&tis_drv);
		}
		return rc;
	}

	return pnp_register_driver(&tis_pnp_driver);
}

static void __exit cleanup_tis(void)
{
	struct tcm_vendor_specific *i, *j;
	struct tcm_chip *chip;
	spin_lock(&tis_lock);
	list_for_each_entry_safe(i, j, &tis_chips, list) {
		chip = to_tcm_chip(i);
		iowrite32(~TCM_GLOBAL_INT_ENABLE &
			  ioread32(chip->vendor.iobase +
				   TCM_INT_ENABLE(chip->vendor.
						  locality)),
			  chip->vendor.iobase +
			  TCM_INT_ENABLE(chip->vendor.locality));
		release_locality(chip, chip->vendor.locality, 1);
		if (chip->vendor.irq)
			free_irq(chip->vendor.irq, chip);
		iounmap(i->iobase);
		list_del(&i->list);
		tcm_remove_hardware(chip->dev);
	}
	spin_unlock(&tis_lock);
	if (force) {
		platform_device_unregister(pdev);
		driver_unregister(&tis_drv);
	}
	else 
		pnp_unregister_driver(&tis_pnp_driver);
}

module_init(init_tis);
module_exit(cleanup_tis);
MODULE_AUTHOR("Nationz Technologies Inc.");
MODULE_DESCRIPTION("TCM Driver");
MODULE_VERSION("1.1.0.2");
MODULE_LICENSE("GPL");
