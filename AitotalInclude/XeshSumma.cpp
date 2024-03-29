﻿//---------------------------------------------------------------------------
#include <vcl.h>
#include <Wincrypt.h>

#pragma hdrstop

#include "XeshSumma.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)



bool XeshSumma::CalcHash(UnicodeString FileName, DWORD dwProvType, ALG_ID Algid, UnicodeString &OutHash, UnicodeString &OutError)
{
   bool bResult = true;
   HCRYPTPROV hCryptProv = NULL;
   HCRYPTHASH hHash = NULL;

   int cbRead; // кол-во прочитанных байтов из файла в буфер
   const int BUFSIZE = 65536; // размер буфера
   UCHAR *szBuffer = new char[BUFSIZE]; // буфера

   byte *szHash = NULL; // хеш
   DWORD cbHash = 0; // размер хеша в байтах
   DWORD dwCount = 4;

   TFileStream *fs = NULL;

   try
   {
	  fs = new TFileStream(FileName, fmOpenRead | fmShareDenyWrite);

	  //--------------------------------------------------------------------
	  // Get a handle to a cryptography provider context.
	  if ( !CryptAcquireContext(&hCryptProv, NULL, NULL, dwProvType, 0) )
	  {
		 throw Exception( "Error CryptAcquireContext: " + SysErrorMessage(GetLastError()) );
	  }


	  //--------------------------------------------------------------------
	  // Acquire a hash object handle.
	  if ( !CryptCreateHash(hCryptProv, Algid, 0, 0, &hHash))
	  {
		  throw Exception( "Error CryptCreateHash: " + SysErrorMessage(GetLastError()) );
	  }

	  // Хешируем
	  while ( (cbRead = fs->Read(szBuffer, BUFSIZE)) != 0 )
	  {
		 if ( !CryptHashData(hHash, szBuffer, cbRead, 0) )
		 {
			throw Exception( "Error CryptHashData: " + SysErrorMessage(GetLastError()) );
		 }
	  }

	  // Узнаём кол-во байтов в хеше
	  if ( !CryptGetHashParam(hHash, HP_HASHSIZE, (byte*)&cbHash, &dwCount, 0) )
	  {
		 throw Exception( "Error CryptGetHashParam(HP_HASHSIZE): " + SysErrorMessage(GetLastError()) );
	  }

	  // Получаем результат хеширования
	  szHash = new byte[cbHash];
	  OutHash.SetLength(cbHash*2);
	  if ( !CryptGetHashParam(hHash, HP_HASHVAL, szHash, &cbHash, 0) )
	  {
		 throw Exception( "Error CryptGetHashParam(HP_HASHVAL): " + SysErrorMessage(GetLastError()) );
	  }
	  BinToHex((char*) szHash, OutHash.c_str(), cbHash);
   }
   catch (Exception &ex)
   {
	  bResult = false;
	  OutError = ex.Message;
   }

   // освобождаем ресурсы
   delete fs;
   delete[] szBuffer;
   if (szHash) delete[] szHash;

   if(hHash)
	  CryptDestroyHash(hHash);
   if(hCryptProv)
	  CryptReleaseContext(hCryptProv,0);

   // возвращаем true или falses
   return bResult;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool XeshSumma::SHA256(UnicodeString FileName, UnicodeString &sha256, UnicodeString &error)
{
   bool rezult = false;
   if ( CalcHash(FileName, PROV_RSA_AES, /*CALG_SHA_256*/0x0000800c, sha256, error) )
   {
	  rezult = true;
   }
   return rezult;
}
