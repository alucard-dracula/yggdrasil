/* LzmaLibExports.c -- LZMA library DLL Entry point
2023-03-05 : Igor Pavlov : Public domain */

#include "Precomp.h"

#if defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS)

#include "../../7zWindows.h"

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  UNUSED_VAR(hInstance)
  UNUSED_VAR(dwReason)
  UNUSED_VAR(lpReserved)
  return TRUE;
}

#endif // #if defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS)
