//---------------------------------------------------------------------------
 #include <io.h>
 #include <vcl.h>

#pragma hdrstop

#include "SizeFile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


/*__int64 MyFileSize(UnicodeString Path)
{
   struct _finddata_t fData;
   int res = _findfirst(AnsiString (L"\\\\?\\"+Path).c_str(), &fData);

   if (res > 0)
	  return fData.size;

   else
	  return -1;
} */
__int64 MyFileSize2(UnicodeString Path)
{
	HANDLE file;
	Path = L"\\\\?\\" + Path;

	file = CreateFileW(Path.w_str(), GENERIC_READ,FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

	if(file == INVALID_HANDLE_VALUE)
	{
		CloseHandle(file);;
		return -1;
	}
	   //ShowMessage("Non fatal error - File cannot be opened because this file is in use.");

	else
	{
	  __int64 dwSize;
	  dwSize = GetFileSize(file, NULL);
	  CloseHandle(file);

	  if (dwSize == INVALID_FILE_SIZE)
	  {
		 // Получим код ошибки.
		 return -2;
	  }
	  else
	  {
		 return dwSize;
	  }

	}

}
