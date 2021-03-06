
/*
 * Licensed Materials - Property of IBM
 *
 * trousers - An open source TCG Software Stack
 *
 * (C) Copyright International Business Machines Corp. 2004, 2005
 *
 */


#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>

#include "trousers/tss.h"
#include "trousers_types.h"
#include "linux/tpm.h"
#include "tcslog.h"
#include "tddl.h"

struct tpm_device_node tpm_device_nodes[] = {
	{"/dev/tpm0", TDDL_UNDEF, TDDL_UNDEF},
	{"/udev/tpm0", TDDL_UNDEF, TDDL_UNDEF},
	{"/dev/tpm", TDDL_UNDEF, TDDL_UNDEF},
	{"/dev/tcm0", TDDL_UNDEF, TDDL_UNDEF},
	{"/udev/tcm0", TDDL_UNDEF, TDDL_UNDEF},
	{"/dev/tcm", TDDL_UNDEF, TDDL_UNDEF},
	{NULL, 0, 0}
};

struct tpm_device_node *opened_device = NULL;

BYTE txBuffer[TDDL_TXBUF_SIZE];

int
open_device(void)
{
	int i;

	/* tpm_device_paths is filled out in tddl.h */
	for (i = 0; tpm_device_nodes[i].path != NULL; i++) {
		errno = 0;
		if ((tpm_device_nodes[i].fd = open(tpm_device_nodes[i].path, O_RDWR)) < 0)
			continue;

		opened_device = &(tpm_device_nodes[i]);
		return opened_device->fd;
	}

	return -1;
}

TSS_RESULT
Tddli_Open()
{
	int rc;

	if (opened_device != NULL) {
		LogDebug("attempted to re-open the TPM driver!");
		return TDDLERR(TDDL_E_ALREADY_OPENED);
	}

	rc = open_device();
	if (rc < 0) {
		LogError("Could not find a device to open!");
		if (errno == ENOENT) {
			/* File DNE */
			return TDDLERR(TDDL_E_COMPONENT_NOT_FOUND);
		}

		return TDDLERR(TDDL_E_FAIL);
	}

	return TSS_SUCCESS;
}

TSS_RESULT
Tddli_Close()
{
	if (opened_device == NULL) {
		LogDebug("attempted to re-close the TPM driver!");
		return TDDLERR(TDDL_E_ALREADY_CLOSED);
	}

	close(opened_device->fd);
	opened_device->fd = TDDL_UNDEF;
	opened_device = NULL;

	return TSS_SUCCESS;
}

TSS_RESULT
Tddli_TransmitData(BYTE * pTransmitBuf, UINT32 TransmitBufLen, BYTE * pReceiveBuf,
		   UINT32 * pReceiveBufLen)
{
	int sizeResult;

	if (TransmitBufLen > TDDL_TXBUF_SIZE) {
		LogError("buffer size handed to TDDL is too large! (%u bytes)", TransmitBufLen);
		return TDDLERR(TDDL_E_FAIL);
	}

	memcpy(txBuffer, pTransmitBuf, TransmitBufLen);
	LogDebug("Calling write to driver");

	switch (opened_device->transmit) {
		case TDDL_UNDEF:
			/* fall through */
		case TDDL_TRANSMIT_IOCTL:
			errno = 0;
			if ((sizeResult = ioctl(opened_device->fd, TPMIOC_TRANSMIT, txBuffer)) != -1) {
				opened_device->transmit = TDDL_TRANSMIT_IOCTL;
				break;
			}
			LogWarn("ioctl: (%d) %s", errno, strerror(errno));
			LogInfo("Falling back to Read/Write device support.");
			/* fall through */
		case TDDL_TRANSMIT_RW:
			if ((sizeResult = write(opened_device->fd,
						txBuffer,
						TransmitBufLen)) == (int)TransmitBufLen) {
				opened_device->transmit = TDDL_TRANSMIT_RW;
				sizeResult = read(opened_device->fd, txBuffer,
						  TDDL_TXBUF_SIZE);
				break;
			} else {
				if (sizeResult == -1) {
					LogError("write to device %s failed: %s",
						 opened_device->path,
						 strerror(errno));
				} else {
					LogError("wrote %d bytes to %s (tried "
						 "to write %d)", sizeResult,
						 opened_device->path,
						 TransmitBufLen);
				}
			}
			/* fall through */
		default:
			return TDDLERR(TDDL_E_IOERROR);
	}

	if (sizeResult < 0) {
		LogError("read from device %s failed: %s", opened_device->path, strerror(errno));
		return TDDLERR(TDDL_E_IOERROR);
	} else if (sizeResult == 0) {
		LogError("Zero bytes read from device %s", opened_device->path);
		return TDDLERR(TDDL_E_IOERROR);
	}

	if ((unsigned)sizeResult > *pReceiveBufLen) {
		LogError("read %d bytes from device %s, (only room for %d)", sizeResult,
				opened_device->path, *pReceiveBufLen);
		return TDDLERR(TDDL_E_INSUFFICIENT_BUFFER);
	}

	*pReceiveBufLen = sizeResult;

	memcpy(pReceiveBuf, txBuffer, *pReceiveBufLen);
	return TSS_SUCCESS;
}

TSS_RESULT
Tddli_GetStatus(UINT32 ReqStatusType, UINT32 *pStatus)
{
	return TDDLERR(TSS_E_NOTIMPL);
}

TSS_RESULT
Tddli_SetCapability(UINT32 CapArea, UINT32 SubCap,
		    BYTE *pSetCapBuf, UINT32 SetCapBufLen)
{
	return TDDLERR(TSS_E_NOTIMPL);
}

TSS_RESULT
Tddli_GetCapability(UINT32 CapArea, UINT32 SubCap,
		    BYTE *pCapBuf, UINT32 *pCapBufLen)
{
	return TDDLERR(TSS_E_NOTIMPL);
}

TSS_RESULT Tddli_Cancel(void)
{
	int rc;

	if (opened_device->transmit == TDDL_TRANSMIT_IOCTL) {
		if ((rc = ioctl(opened_device->fd, TPMIOC_CANCEL, NULL)) == -1) {
			LogError("ioctl: (%d) %s", errno, strerror(errno));
			return TDDLERR(TDDL_E_FAIL);
		} else if (rc == -EIO) {
			/* The driver timed out while trying to tell the chip to cancel */
			return TDDLERR(TDDL_E_COMMAND_COMPLETED);
		}

		return TSS_SUCCESS;
	} else {
		return TDDLERR(TSS_E_NOTIMPL);
	}
}
