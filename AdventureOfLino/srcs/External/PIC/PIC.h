/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * プロセス間通信制御
 * @author SPATZ.
 * @data   2014/04/20 00:20:27
 */
//---------------------------------------------------
#ifndef __PIC_H__
#define __PIC_H__

#include <windows.h>

#ifdef PIC_DLL_EXPORT
#define PIC_DLL_API extern "C" __declspec(dllexport)
#else
#define PIC_DLL_API extern "C" __declspec(dllimport)
#endif

namespace PIC_LIB
{
PIC_DLL_API bool CreatePICPipe(const char* strPipeName);
PIC_DLL_API bool OpenPICPipe(const char* strPipeName);
PIC_DLL_API void WaitConnectClient(void);
PIC_DLL_API bool SendCommand(char* pBuffer, size_t bufferSize);
PIC_DLL_API bool RecvCommand(char* pBuffer, size_t bufferSize);
PIC_DLL_API bool CloseNamedPipe(void);	
}

#endif // __PIC_H__

 /**** end of file ****/

