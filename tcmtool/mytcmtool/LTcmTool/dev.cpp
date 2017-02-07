/*
 * Copyright (c) 2004 Intel Corporation. All rights reserved.
 *
 * This source code is Intel confidential material, and it is subject
 * to special licensing terms with Intel Corporation. Please refer to
 * the license file included with this release or contact Intel for
 * further details.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "stdafx.h"
#include <string.h>
#include "tcm.h"
#include "dev.h"
#include "bsg.h"

#ifndef DLL_D

typedef UINT32 TSS_RESULT;
//#ifndef TSS_RESULT
//#define TSS_RESULT UINT32
//#endif

#include "tddl.h"
#endif

#undef strdup
#define strdup(x) __strdup(x)

static char*
__strdup(const char* s)
{
	char* str = NULL;
	if (s)
	{
		int size = strlen(s) + 1;
		str = new char[size];
		memcpy(str, s, size);
	}
	return str;
}

static void
Pack(BYTE* dst, int size, int val)
{
	switch (size)
	{
	case 4:
		dst[0] = (BYTE)((val >> 24) & 0xff);
		dst[1] = (BYTE)((val >> 16) & 0xff);
		dst[2] = (BYTE)((val >> 8) & 0xff);
		dst[3] = (BYTE)(val & 0xff);
		break;
	case 2:
		dst[0] = (BYTE)((val >> 8) & 0xff);
		dst[1] = (BYTE)(val & 0xff);
		break;
	case 1:
		dst[0] = (BYTE)(val & 0xff);
		break;
	}
}

static int
Unpack(const BYTE* src, int size)
{
	int val = 0;
	switch (size)
	{
	case 4:
		val = (int)(((unsigned) src[0]) << 24
			    | ((unsigned) src[1]) << 16
			    | ((unsigned) src[2]) << 8
			    | (unsigned) src[3]);
		break;
	case 2:
		val = (int)(((unsigned) src[0]) << 8 | (unsigned) src[1]);
		break;
	case 1:
		val = src[0];
		break;
	}		
	return val;
}
static void
Pack16(BYTE* dst, int val)
{
	dst[0] = (BYTE)((val >> 8) & 0xff);
	dst[1] = (BYTE)(val & 0xff);
}

static void
Pack32(BYTE* dst, int val)
{
	dst[0] = (BYTE)((val >> 24) & 0xff);
	dst[1] = (BYTE)((val >> 16) & 0xff);
	dst[2] = (BYTE)((val >> 8) & 0xff);
	dst[3] = (BYTE)(val & 0xff);
}

static int
Unpack32(const BYTE* src)
{
	return (((UINT32) src[0]) << 24
		| ((UINT32) src[1]) << 16
		| ((UINT32) src[2]) << 8
		| (UINT32) src[3]);
}

Device::~Device()
{
}

int
Device::Cancel()
{
	return -1;
}

WinDevice::WinDevice()
	:m_flags(0), m_dllpath(NULL), m_handle(NULL),
	 m_open(NULL), m_close(NULL), m_transmit(NULL), m_cancel(NULL)
{
}

WinDevice::~WinDevice()
{
	delete [] m_dllpath;
#ifdef DLL_D
	if (m_handle)
	{
		(*m_close)();
		FreeLibrary((HMODULE) m_handle);
	}
#else
	if( m_close != NULL )
	{
		(*m_close)();
	}
#endif
}

int
WinDevice::Init(int flags, const char* dllpath)
{
	m_flags = flags;

#ifdef DLL_D
	if (m_handle)
		return 0;

	HMODULE h = 0;
	if (dllpath)
		h = LoadLibrary(dllpath);
	else
	{
		h = LoadLibrary("TCMTddl.dll");
		if (!h)
			h = LoadLibrary("ddl.dll");
		if (!h)
			h = LoadLibrary("../dll/i386/TCMTddl.dll");
		if (!h)
			h = LoadLibrary("../../dll/i386/TCMTddl.dll");
	}
	if (!h)
	{
		printf("Cannot load TDDI DLL\n");
		return -1;
	}
	
	m_open = (DevOpen) GetProcAddress(h, "Tddli_Open");
	if (m_open)
	{
		m_close	= (DevClose) GetProcAddress(h, "Tddli_Close");
		m_transmit = (DevTransmit)
			GetProcAddress(h, "Tddli_TransmitData");
		m_cancel = (DevCancel)
			GetProcAddress(h, "Tddli_Cancel");
	}
	else
	{
		m_open = (DevOpen) GetProcAddress(h, "?Tddli_Open@@YAIXZ");
		m_close = (DevClose) GetProcAddress(h, "?Tddli_Close@@YAIXZ");
		m_transmit = (DevTransmit)
			GetProcAddress(h, "?Tddli_TransmitData@@YAIPAEI0PAI@Z");
		m_cancel = (DevCancel) 
			GetProcAddress(h, "?Tddli_Cancel@@YAIXZ");
	}

	if (!m_open || !m_close || !m_transmit)
	{
		printf("Cannot find TDDI functions\n");
		FreeLibrary(h);
		return -1;
	}

	int status = (*m_open)();
	if (status != 0)
	{
		printf("TDDL_Open\n");
		FreeLibrary(h);
		return status;
	}

	m_handle = h;

#else //DLL_D

	m_open = Tddli_Open;
	m_close	= Tddli_Close;
	m_transmit = Tddli_TransmitData;
	m_cancel = Tddli_Cancel;

	int status = (*m_open)();
	if (status != 0)
	{
		printf("TDDL_Open\n");
		return status;
	}

#endif //DLL_D
	
	/*
	static char tmppath[MAX_PATH];
	if (GetModuleFileName(NULL, tmppath, sizeof(tmppath)))
		m_dllpath = strdup(tmppath);
	*/
	m_dllpath = strdup("./");

	return 0;
}

int
WinDevice::Transmit(const BYTE* in, int insize, BYTE* out, int* outsize)
{
	if (!m_transmit)
		return -1;

	unsigned outs = *outsize;
	int status = (int)((*m_transmit)((BYTE*) in, insize, out, &outs));
	if (status != 0 && (m_flags & FLAG_RETRY))
	{
		(*m_close)();
		status = (int)((*m_open)());
		if (status != 0)
		{
			return -1;
		}
		status = (int)((*m_transmit)((BYTE*) in, insize, out, &outs));
	}

	if (status != 0)
	{
		return status;
	}

	if (status == 0)
		*outsize = outs;
	return status;
}

int
WinDevice::Cancel()
{
	if (!m_cancel)
		return -1;
	return (*m_cancel)();
}

int
WinDevice::SubmitSimple(int cmd, int argsize, int arg)
{
	BYTE in[14];
	int insize = 10 + argsize;
	Pack(in, 2, TCM_TAG_RQU_COMMAND);
	Pack(in + 2, 4, insize);
	Pack(in + 6, 4, cmd);
	if (argsize > 0)
		Pack(in + 10, argsize, arg);

	BYTE out[10];
	int outsize = sizeof(out);
	int status = Transmit(in, insize, out, &outsize);
	if (status == 0)
	{
		if (outsize == 10
		    && Unpack(out, 2) == TCM_TAG_RSP_COMMAND
		    && Unpack(out + 2, 4) == 10)
			status = Unpack(out + 6, 4);
		else
			status = -1;
	}
	return status;
}

int
WinDevice::Submit(const BYTE* in, int insize, BYTE** out, int* outsize)
{
	int outsz = 2048;
	BYTE* outbuf = new BYTE[outsz];
	int status = Transmit(in, insize, outbuf, &outsz);
   
	if (status == 0)
		status = (int)BSG::UnpackConst(outbuf + 6, 4);

	if (outsize)
		*outsize = (int)BSG::Unpack32(outbuf + 2);

	if (status != 0 || !out)
	{
		delete [] outbuf;
		outbuf = NULL;
	}
	if (out)
		*out = outbuf;
	else
		delete [] outbuf;
	return status;
}

BYTE*
WinDevice::Build(int* bufsize, int cmd, va_list *args)
{
	BYTE* buffer = NULL;
	BYTE* bp = NULL;

	// tag + size + cmd + auth
	int total = 10;
	int tag = TCM_TAG_RQU_COMMAND;
	
	for (;;)
	{
		va_list args2;
		memcpy(&args2, args, sizeof(args2));
		for (int argc = 0;; argc++)
		{
			int flags = (int) va_arg(args2, int);
			int psize = 0;
			if (flags & __BSG_SIZE32)
				psize = 4;
			else if (flags & __BSG_SIZE16)
				psize = 2;
			if (psize && bp)
				bp += psize;
			int dsize = (flags & __BSG_MASK);

			if (flags & __BSG_CONST)
			{
				int val = (int) va_arg(args2, int);
				if (bp)
					BSG::PackConst(val, dsize, bp);
			}
			else if (flags & __BSG_DATA)
			{
				BYTE* val = va_arg(args2, BYTE*);
				if (bp)
				{
					if (val)
						memcpy(bp, val, dsize);
					else
						memset(bp, 0, dsize);
				}
			}
			else if (flags & __BSG_MASK)
			{
				void* val = va_arg(args2, void*);
				if (val)
				{
					dsize = BSG::Pack(
						(BSG::Type)
						 (flags & __BSG_MASK),
						val,
						bp);
				}
				else
					dsize = 0;
			}
			else
				break;

			if (bp)
			{
				if (psize)
				{
					BSG::PackConst(
						dsize, psize, bp - psize);
				}
				if (dsize)
				{
					bp += dsize;
				}
			}
			else
				total += (psize + dsize);
		}

		if (bp)
			break;

		// allocate buffer
		buffer = new BYTE[total];
		if (!buffer)
			return NULL;
		if (bufsize)
			*bufsize = total;
		bp = buffer;

		// build tag+size
		BSG::Pack16(bp, tag);
		bp += 2;
		BSG::Pack32(bp, total);
		bp += 4;
		BSG::Pack32(bp, cmd);
		bp += 4;
	}
	return buffer;	
}

BYTE* 
WinDevice::BuildSimple(int* bufsize, int cmd, ...)
{
	va_list args;
	va_start(args, cmd);
	BYTE* x = Build(bufsize, cmd, &args);
	va_end(args);
	return x;

}
int
WinDevice::VerifySimple(const BYTE* buffer, int bufsize, int cmd, ...)
{
	va_list args;
	va_start(args, cmd);
	int x = Verify(buffer, bufsize, cmd, &args);
	va_end(args);
	return x;
}
int
WinDevice::Verify(
	const BYTE* buffer,
	int bufsize,
	int cmd,
	va_list *args)
{
	const BYTE* bp = buffer;

	int status = 0;

	// verify tag+size+result
	int tag = TCM_TAG_RSP_COMMAND;

	if (BSG::Unpack16(bp) != tag)
	{
		status = -1;
	}
	bp += 2;
	if (BSG::Unpack32(bp) != bufsize)
	{
		status = -1;
	}
	bp += 4;
	int res = BSG::Unpack32(bp);
	if (res != 0)
		return (int) res;
	bp += 4;

	for (;;)
	{
		int flags = (int) va_arg(*args, int);
		int psize = 0;
		if (flags & __BSG_SIZE32)
			psize = 4;
		else if (flags & __BSG_SIZE16)
			psize = 2;
		if (psize && bp)
			bp += psize;
		int dsize = (flags & __BSG_MASK);

		if (flags & __BSG_CONST)
		{
			int val2 = (int) BSG::UnpackConst(bp, dsize);
			if (flags & __BSG_OUT)
			{
				int* val = va_arg(*args, int*);
				if (val)
					*val = val2;
			}
			else
			{
				int val	= (int) va_arg(*args, int);
				if (val2 != val)
					return -1;
			}
		}		
		else if (psize)
		{
			if (flags & __BSG_OUT)
			{
				BYTE* val = va_arg(*args, BYTE*);
				int* osize = va_arg(*args, int*);
				int vsize = BSG::UnpackConst(
						bp - psize, psize);
				if (val && vsize)
					memcpy(val, bp, vsize);
				if (osize)
					*osize = vsize;
				dsize += vsize; 
			}
		}		
		else if (flags & __BSG_DATA)
		{
			BYTE* val = va_arg(*args, BYTE*);
			if (val && dsize)
			{
				if (flags & __BSG_OUT)
					memcpy(val, bp, dsize);
				else if (memcmp(val, bp, dsize) != 0)
					return -1;
			}
		}
		else if (flags & __BSG_MASK)
		{
			void* val = va_arg(*args, void*);
			dsize = BSG::Unpack(
				(BSG::Type)(flags & __BSG_MASK),
				bp,
				val);
		}
		else
			break;

		bp += dsize;
	}

	return status;
}

int WinDevice::UnpackArr(BYTE* buffer, int size, int res)
{
	int i = 0;
	res = 0;
	int temp = 1;
	while(i <= size)
	{
		if(buffer[i] == 1)
		{
			res |= (temp <<= i);
			temp = 1;
		}
		i++;	
	}
	return res;
}




