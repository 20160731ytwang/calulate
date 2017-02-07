#ifndef SMS4_H
#define SMS4_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "tcm_alg_type.h"

#define	SMS4_BLOCK_SIZE 16
#define SMS4_MAX_LEN	512

#define SMS4_MODE_CBC	0
#define	SMS4_MODE_ECB	1


/* UINT4 defines a four byte word */
typedef struct {
	UINT32 rk[32];
}SMS4_KEY;

INT32 SMS4_E(BYTE *IV, BYTE *M, INT32 mLen, BYTE *S, BYTE *key, BYTE mode);
INT32 SMS4_D(BYTE *IV, BYTE *M, INT32 mLen, BYTE *S, BYTE *key, BYTE mode);

#ifdef	__cplusplus
}
#endif


#endif /* SMS4_H */