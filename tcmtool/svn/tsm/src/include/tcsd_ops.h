
/*
 * Licensed Materials - Property of IBM
 *
 * trousers - An open source TCG Software Stack
 *
 * (C) Copyright International Business Machines Corp. 2005, 2007
 *
 */


#ifndef _TCSD_COMMANDS_H_
#define _TCSD_COMMANDS_H_

#include "tcsd_wrap.h"

#define OPENCONTEXT			TCSD_ORD_OPENCONTEXT
#define CLOSECONTEXT			TCSD_ORD_CLOSECONTEXT
#define FREEMEMORY			TCSD_ORD_FREEMEMORY
#define TCSGETCAPABILITY		TCSD_ORD_TCSGETCAPABILITY
#define REGISTERKEY			TCSD_ORD_REGISTERKEY
#define UNREGISTERKEY			TCSD_ORD_UNREGISTERKEY
#define GETREGISTEREDKEYBLOB		TCSD_ORD_GETREGISTEREDKEYBLOB
#define GETREGISTEREDKEYBYPUBLICINFO	TCSD_ORD_GETREGISTEREDKEYBYPUBLICINFO
#define GETPUBKEY			TCSD_ORD_GETPUBKEY
#define LOADKEYBYBLOB			TCSD_ORD_LOADKEYBYBLOB
#define LOADKEYBYUUID			TCSD_ORD_LOADKEYBYUUID
#define CREATEWRAPKEY			TCSD_ORD_CREATEWRAPKEY
#define GETPCREVENTLOG			TCSD_ORD_GETPCREVENTLOG
#define OIAP				TCSD_ORD_OIAP
#define OSAP				TCSD_ORD_OSAP
#define APCREATE			TCSD_ORD_APCREATE
#define TERMINATEHANDLE			TCSD_ORD_TERMINATEHANDLE
#define PCRREAD				TCSD_ORD_PCRREAD
#define PCRRESET			TCSD_ORD_PCRRESET
#define QUOTE				TCSD_ORD_QUOTE
#define SEAL				TCSD_ORD_SEAL
#define UNSEAL				TCSD_ORD_UNSEAL
#define UNBIND				TCSD_ORD_UNBIND
#define CERTIFYKEY			TCSD_ORD_CERTIFYKEY
#define SIGN				TCSD_ORD_SIGN
#define GETRANDOM			TCSD_ORD_GETRANDOM
#define STIRRANDOM			TCSD_ORD_STIRRANDOM
#define GETCAPABILITY			TCSD_ORD_GETCAPABILITY
#define READPUBEK			TCSD_ORD_READPUBEK
#define SELFTESTFULL			TCSD_ORD_SELFTESTFULL
#define CERTIFYSELFTEST			TCSD_ORD_CERTIFYSELFTEST
#define CONTINUESELFTEST		TCSD_ORD_CONTINUESELFTEST
#define GETTESTRESULT			TCSD_ORD_GETTESTRESULT
/* below this line are ordinals that probably shouldn't be allowed for non-localhosts */
#if 0
#define OWNERREADPUBEK			TCSD_ORD_OWNERREADPUBEK
#define GETPCREVENT			TCSD_ORD_GETPCREVENT
#define GETPCREVENTBYPCR		TCSD_ORD_GETPCREVENTBYPCR
#define GETCAPABILITYSIGNED		TCSD_ORD_GETCAPABILITYSIGNED
#define ENUMREGISTEREDKEYS		TCSD_ORD_ENUMREGISTEREDKEYS
#define ENUMREGISTEREDKEYS2		TCSD_ORD_ENUMREGISTEREDKEYS2
#define GETREGISTEREDKEY		TCSD_ORD_GETREGISTEREDKEY
#define EXTEND				TCSD_ORD_EXTEND
#define LOGPCREVENT			TCSD_ORD_LOGPCREVENT
#define EVICTKEY			TCSD_ORD_EVICTKEY
#define DISABLEPUBEKREAD		TCSD_ORD_DISABLEPUBEKREAD
#define SETOWNERINSTALL			TCSD_ORD_SETOWNERINSTALL
#define MAKEIDENTITY			TCSD_ORD_MAKEIDENTITY
#define MAKEIDENTITY2			TCSD_ORD_MAKEIDENTITY2
#define TAKEOWNERSHIP			TCSD_ORD_TAKEOWNERSHIP
#define CREATEENDORSEMENTKEYPAIR	TCSD_ORD_CREATEENDORSEMENTKEYPAIR
#define GETCAPABILITYOWNER		TCSD_ORD_GETCAPABILITYOWNER
#define ACTIVATETPMIDENTITY		TCSD_ORD_ACTIVATETPMIDENTITY
#define AUTHORIZEMIGRATIONKEY		TCSD_ORD_AUTHORIZEMIGRATIONKEY
#define CHANGEAUTH			TCSD_ORD_CHANGEAUTH
#define CHANGEAUTHOWNER			TCSD_ORD_CHANGEAUTHOWNER
#define CHANGEAUTHASYMSTART		TCSD_ORD_CHANGEAUTHASYMSTART
#define CHANGEAUTHASYMFINISH		TCSD_ORD_CHANGEAUTHASYMFINISH
#define DIRREAD				TCSD_ORD_DIRREAD
#define DIRWRITEAUTH			TCSD_ORD_DIRWRITEAUTH
#define CREATEMIGRATIONBLOB		TCSD_ORD_CREATEMIGRATIONBLOB
#define CONVERTMIGRATIONBLOB		TCSD_ORD_CONVERTMIGRATIONBLOB
#define OWNERSETDISABLE			TCSD_ORD_OWNERSETDISABLE
#define OWNERCLEAR			TCSD_ORD_OWNERCLEAR
#define DISABLEOWNERCLEAR		TCSD_ORD_DISABLEOWNERCLEAR
#define FORCECLEAR			TCSD_ORD_FORCECLEAR
#define DISABLEFORCECLEAR		TCSD_ORD_DISABLEFORCECLEAR
#define PHYSICALDISABLE			TCSD_ORD_PHYSICALDISABLE
#define PHYSICALENABLE			TCSD_ORD_PHYSICALENABLE
#define PHYSICALSETDEACTIVATED		TCSD_ORD_PHYSICALSETDEACTIVATED
#define SETTEMPDEACTIVATED		TCSD_ORD_SETTEMPDEACTIVATED
#define PHYSICALPRESENCE		TCSD_ORD_PHYSICALPRESENCE
#define FIELDUPGRADE			TCSD_ORD_FIELDUPGRADE
#define SETRIDIRECTION			TCSD_ORD_SETRIDIRECTION
#define CREATEMAINTENANCEARCHIVE	TCSD_ORD_CREATEMAINTENANCEARCHIVE
#define LOADMAINTENANCEARCHIVE		TCSD_ORD_LOADMAINTENANCEARCHIVE
#define KILLMAINTENANCEFEATURE		TCSD_ORD_KILLMAINTENANCEFEATURE
#define LOADMANUFECTURERMAINTENANCEPUB	TCSD_ORD_LOADMANUFECTURERMAINTENANCEPUB
#define READMANUFECTURERMAINTENANCEPUB	TCSD_ORD_READMANUFECTURERMAINTENANCEPUB
#define SETTEMPDEACTIVATED2		TCSD_ORD_SETTEMPDEACTIVATED2
#endif

/* TCSD ordinal sub-command sets */
#define SUBOP_CONTEXT			OPENCONTEXT, CLOSECONTEXT
#define SUBOP_RANDOM			STIRRANDOM, GETRANDOM
#define SUBOP_AUTHSESS			OIAP, OSAP, TERMINATEHANDLE
#define SUBOP_LOADKEYBYUUID		LOADKEYBYUUID, GETREGISTEREDKEYBLOB, FREEMEMORY
#define SUBOP_SELFTEST			SELFTESTFULL, CERTIFYSELFTEST, CONTINUESELFTEST, GETTESTRESULT 

/* Top level TCSD operations which can be enabled to be used by remote hosts. Each of these
 * should have a corresponding on/off switch in the tcsd.conf file
 */
#define TCSD_OP_SEAL				SEAL, SUBOP_LOADKEYBYUUID, SUBOP_RANDOM, SUBOP_AUTHSESS, SUBOP_CONTEXT, 0
#define TCSD_OP_UNSEAL				UNSEAL, SUBOP_LOADKEYBYUUID, SUBOP_RANDOM, SUBOP_AUTHSESS, SUBOP_CONTEXT, 0
#define TCSD_OP_GETREGISTEREDKEYBYPUBLICINFO	GETREGISTEREDKEYBYPUBLICINFO, SUBOP_CONTEXT, 0
#define TCSD_OP_GETPUBKEY			GETPUBKEY, SUBOP_RANDOM, SUBOP_AUTHSESS, SUBOP_CONTEXT, 0
#define TCSD_OP_LOADKEY				LOADKEYBYBLOB, SUBOP_LOADKEYBYUUID, SUBOP_CONTEXT, SUBOP_AUTHSESS, SUBOP_RANDOM, 0
#define TCSD_OP_REGISTERKEY			REGISTERKEY, SUBOP_CONTEXT, SUBOP_LOADKEYBYUUID, LOADKEYBYBLOB, 0
#define TCSD_OP_UNREGISTERKEY			UNREGISTERKEY, SUBOP_CONTEXT, 0
#define TCSD_OP_CREATEKEY			CREATEWRAPKEY, SUBOP_CONTEXT, SUBOP_AUTHSESS, SUBOP_LOADKEYBYUUID, SUBOP_RANDOM, 0
#define TCSD_OP_SIGN				SIGN, SUBOP_CONTEXT, SUBOP_AUTHSESS, SUBOP_RANDOM, FREEMEMORY, 0
#define TCSD_OP_RANDOM				SUBOP_RANDOM, SUBOP_CONTEXT, FREEMEMORY, 0
#define TCSD_OP_GETCAPABILITY			GETCAPABILITY, TCSGETCAPABILITY, SUBOP_CONTEXT, FREEMEMORY, 0
#define TCSD_OP_UNBIND				UNBIND, SUBOP_CONTEXT, SUBOP_AUTHSESS, SUBOP_RANDOM, 0
#define TCSD_OP_QUOTE				QUOTE, SUBOP_CONTEXT, SUBOP_AUTHSESS, SUBOP_RANDOM, 0
#define TCSD_OP_READPUBEK			READPUBEK, SUBOP_CONTEXT, SUBOP_AUTHSESS, SUBOP_RANDOM, 0
#define TCSD_OP_SELFTEST			SUBOP_SELFTEST, SUBOP_CONTEXT, FREEMEMORY, 0

struct tcsd_op {
	char *name;
	int op[];
};

struct tcsd_op tcsd_op_seal = {"seal", {TCSD_OP_SEAL}};
struct tcsd_op tcsd_op_unseal = {"unseal", {TCSD_OP_UNSEAL}};
struct tcsd_op tcsd_op_registerkey = {"registerkey", {TCSD_OP_REGISTERKEY}};
struct tcsd_op tcsd_op_unregisterkey = {"unregisterkey", {TCSD_OP_UNREGISTERKEY}};
struct tcsd_op tcsd_op_getregisteredkeybypublicinfo = {"getregisteredkeybypublicinfo", {TCSD_OP_GETREGISTEREDKEYBYPUBLICINFO}};
struct tcsd_op tcsd_op_getpubkey = {"getpubkey", {TCSD_OP_GETPUBKEY}};
struct tcsd_op tcsd_op_loadkey = {"loadkey", {TCSD_OP_LOADKEY}};
struct tcsd_op tcsd_op_createkey = {"createkey", {TCSD_OP_CREATEKEY}};
struct tcsd_op tcsd_op_sign = {"sign", {TCSD_OP_SIGN}};
struct tcsd_op tcsd_op_random = {"random", {TCSD_OP_RANDOM}};
struct tcsd_op tcsd_op_getcapability = {"getcapability", {TCSD_OP_GETCAPABILITY}};
struct tcsd_op tcsd_op_unbind = {"unbind", {TCSD_OP_UNBIND}};
struct tcsd_op tcsd_op_quote = {"quote", {TCSD_OP_QUOTE}};
struct tcsd_op tcsd_op_readpubek = {"readpubek", {TCSD_OP_READPUBEK}};
struct tcsd_op tcsd_op_selftest = {"selftest", {TCSD_OP_SELFTEST}};

struct tcsd_op *tcsd_ops[] = {
	&tcsd_op_seal,
	&tcsd_op_unseal,
	&tcsd_op_registerkey,
	&tcsd_op_unregisterkey,
	&tcsd_op_getregisteredkeybypublicinfo,
	&tcsd_op_getpubkey,
	&tcsd_op_loadkey,
	&tcsd_op_createkey,
	&tcsd_op_sign,
	&tcsd_op_random,
	&tcsd_op_getcapability,
	&tcsd_op_unbind,
	&tcsd_op_quote,
	&tcsd_op_readpubek,
	&tcsd_op_selftest,
	NULL
};

#endif