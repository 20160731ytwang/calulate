/*


#ifndef _TDDL_H_
#define _TDDL_H_

struct tpm_device_node {
	char *path;
#define TDDL_TRANSMIT_IOCTL	1
#define TDDL_TRANSMIT_RW	2
	int transmit;
	int fd;
};

#define TDDL_TXBUF_SIZE		2048
#define TDDL_UNDEF		-1

TSS_RESULT Tddli_Open(void);

TSS_RESULT Tddli_TransmitData(BYTE *pTransmitBuf,
			UINT32 TransmitBufLen,
			BYTE *pReceiveBuf,
			UINT32 *pReceiveBufLen);

TSS_RESULT Tddli_Close(void);

#endif
*/


#ifndef __TDDLI_H__
#define __TDDLI_H__

#ifdef WIN32

#ifdef TDDLI_EXPORTS
#define TDDLI __declspec(dllexport)
#else
#define TDDLI __declspec(dllimport)
#endif

#else //WIN32

#define TDDLI

#endif //WIN32


#define TDDL_CAP_VERSION   0x0100
#define TDDL_CAP_VER_DRV   0x0101
#define TDDL_CAP_VER_FW    0x0102
#define TDDL_CAP_VER_FW_DATE   0x0103

#define TDDL_CAP_PROPERTY   0x0200
#define TDDL_CAP_PROP_MANUFACTURER  0x0201
#define TDDL_CAP_PROP_MODULE_TYPE  0x0202
#define TDDL_CAP_PROP_GLOBAL_STATE  0x0203


//--------------------------------------------------------------------
// TDDL specific helper redefinitions

#ifdef __cplusplus
extern "C" {
#endif

    //establish a connection to the TPM device driver
    TDDLI TSS_RESULT Tddli_Open(void);
 
    //close a open connection to the TPM device driver
    TDDLI TSS_RESULT Tddli_Close(void);

    //cancels the last outstanding TPM command
    TDDLI TSS_RESULT Tddli_Cancel(void);

    // send any data to the TPM module
    TDDLI TSS_RESULT Tddli_TransmitData(
        BYTE         *pTransmitBuf,
        UINT32        TransmitBufLen,
        BYTE         *pReceiveBuf,
        UINT32       *puntReceiveBufLen);

    
#ifdef __cplusplus
}
#endif

#endif // __TDDLI_H__




