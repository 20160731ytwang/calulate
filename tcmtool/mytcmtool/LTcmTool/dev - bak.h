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

#ifndef __DEV_H__
#define __DEV_H__

//#include "os.h"
#include "stdafx.h"

class Device
{
public:
	virtual ~Device();
	virtual int Transmit(
		const BYTE* in, int insize, BYTE* out, int* outsize) = 0;
	virtual int Cancel();
};

class WinDevice:public Device
{
public:
	enum {FLAG_TEST = 0x1, FLAG_RETRY = 0x2};

	WinDevice();
	int Init(int flags = 0, const char* dllpath = NULL);
	virtual ~WinDevice();
	virtual int Transmit(
		const BYTE* in, int insize, BYTE* out, int* outsize);
	virtual int Cancel();
	//发送命令
	int SubmitSimple(int cmd, int argsize = 0, int arg = 0);
	int Submit(const BYTE* in, int insize, BYTE** out, int* outsize);
	//组合数据流
	BYTE* BuildSimple(int* bufsize, int cmd, ...);
	BYTE* Build(int* bufsize, int cmd, va_list args);
	//验证返回的数据
	int VerifySimple(const BYTE* buffer, int bufsize, int cmd, ...);
	int Verify(const BYTE* buffer, int bufsize, int cmd, va_list args);

	int UnpackArr(BYTE* buffer, int size, int res);

protected:
	typedef unsigned (*DevOpen)();
	typedef unsigned (*DevClose)();
	typedef unsigned (*DevTransmit)(BYTE*, unsigned, BYTE*, unsigned*);
	typedef unsigned (*DevCancel)();

	int m_flags;
	char* m_dllpath;
	void* m_handle;
	DevOpen m_open;
	DevClose m_close;
	DevTransmit m_transmit;
	DevCancel m_cancel;
};


#endif /* __DEV_H__ */
