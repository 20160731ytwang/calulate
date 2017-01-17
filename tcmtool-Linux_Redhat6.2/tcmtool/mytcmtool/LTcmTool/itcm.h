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

#ifndef __ITCM_H__
#define __ITCM_H__

#ifdef __cplusplus
extern "C"
{
#endif

void itcm_pack_const(BYTE* dst, int val, int size);
int itcm_unpack_const(const BYTE* src, int size);
int itcm_pack_simple(int cmd, int argsize, int arg, BYTE* dst, int max);
int itcm_unpack_simple(const BYTE* src, int size, int argsize, int* arg);

const char* itcm_ord_to_str(int ord);
const char* itcm_status_to_str(int status);
const char* itcm_tag_to_str(int tag);
const char* itcm_manu_to_str(int value);

#ifdef __cplusplus
}
#endif

#endif /* __ITCM_H__ */
