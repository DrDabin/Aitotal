// ---------------------------------------------------------------------------

#ifndef extractH
#define extractH
#include <vector>
#include <vcl.h>
#include <StrUtils.hpp>
#include <System.IOUtils.hpp>
#include <memory>    //std::auto_prt<>

#define EX_OK 0
#define errorlibdll 1
#define erroropenarchiv 3
#define errorDataPass 9
#define errorHead 8
// ---------------------------------------------------------------------------
   //CLSID_CFormat == 1 == 7z
   //CLSID_CFormat == 2 == zip
   //CLSID_CFormat == 3 == rar
   //CLSID_CFormat ==4 == rar5

   //bool extract2( wchar_t* archiv, wchar_t* dir, unsigned int CLSID_CFormat,UnicodeString PatchProgramma);
   int extArchivToPassword( wchar_t* archiv, wchar_t* dir, wchar_t *pass, unsigned int CLSID_CFormat);
// ---------------------------------------------------------------------------
#endif
