﻿// ---------------------------------------------------------------------------

#pragma hdrstop

#include "extract.h"
#include "AitotalInclude/Struct.h"
#include "extract.h"
#include "CallbackClassesExtract.h"
#include "CallbackClassesOpen.h"

#pragma package(smart_init)




// ---------------------------------------------------------------------------
extern void toLog(const wchar_t* line); // ф-ция логирования из файла формы
//extern void toExtracting(const wchar_t* line);
// ---------------------------------------------------------------------------

DEFINE_GUID(CLSID_CFormat7z, 0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x07, 0x00, 0x00);
DEFINE_GUID(CLSID_CFormatZip, 0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x01, 0x00, 0x00);
DEFINE_GUID(CLSID_CFormatRar, 0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x03, 0x00, 0x00);
DEFINE_GUID(CLSID_CFormatRar5, 0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0xCC, 0x00, 0x00);

//typedef UINT32(WINAPI* Func_CreateObject)(const GUID* clsID, const GUID* interfaceID, void** outObject);

const wchar_t DllName[]= L"tools/7z.dll";
// ---------------------------------------------------------------------------

 int extArchivToPassword( wchar_t* archiv, wchar_t* dir, wchar_t *pass, unsigned int CLSID_CFormat)
 {
	NWindows::NDLL::CLibrary lib;
	int result;
	if (!lib.Load(DllName))
	{
		toLog(L"Can not load 7-zip library");
		result = errorlibdll;
	}
	else
	{
		Func_CreateObject createObjectFunc = (Func_CreateObject)lib.GetProc("CreateObject");

		if (createObjectFunc == 0)
		{
			toLog(L"Can not get CreateObject");
			result = errorlibdll;
		}
		else
		{
			UString archiveName = GetUnicodeString(archiv);

			UString toDir = GetUnicodeString(dir);
			_GUID cid;


			if(CLSID_CFormat == 1)
				cid = CLSID_CFormat7z;
			if(CLSID_CFormat == 2)
				cid = CLSID_CFormatZip;
			if(CLSID_CFormat == 3)
				cid = CLSID_CFormatRar;
			if(CLSID_CFormat == 4)
				cid = CLSID_CFormatRar5;

			CMyComPtr<IInArchive>archive;

			if (createObjectFunc(&cid, &IID_IInArchive, (void**)&archive) != S_OK)
			{
			   toLog(L"Can not get class object");
			   result = errorlibdll;
			}
			else
			{
				CInFileStream* fileSpec = new CInFileStream;
				CMyComPtr<IInStream>file = fileSpec;

				if (!fileSpec->Open(archiveName))
				{
					toLog(L"Can not open archive file");
					result = erroropenarchiv;
				}
				else
				{
						 // Open

					CArchiveOpenCallback* openCallbackSpec = new CArchiveOpenCallback;
					CMyComPtr<IArchiveOpenCallback>openCallback(openCallbackSpec);
					openCallbackSpec->PasswordIsDefined = true;
					openCallbackSpec->Password = GetUnicodeString(pass);

					if (archive->Open(file, 0, openCallback) != S_OK)
					{
						toLog(L"Can not open archive callback");
						result = erroropenarchiv;
					}
					  // End Open
					else
					{
						// Extract
						CArchiveExtractCallback* extractCallbackSpec = new CArchiveExtractCallback;
						CMyComPtr<IArchiveExtractCallback>extractCallback(extractCallbackSpec);
						extractCallbackSpec->Init(archive, toDir);
						extractCallbackSpec->PasswordIsDefined = true;
						extractCallbackSpec->Password = GetUnicodeString(pass);

						HRESULT resultat = archive->Extract(NULL, (UInt3232)(Int3232)(-1), false, extractCallback);

						 if (resultat == errorDataPass)
						{
							toLog(L"Password incorrect.");
							return errorDataPass;
						}

						if(resultat == S_OK)
							result = EX_OK;

						if( resultat == errorHead)
							return errorHead;
					}
				}
				delete fileSpec;
			}
		}
	}
	return result;
}

	//+++++++++++++++
