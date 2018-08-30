//---------------------------------------------------------------------------


#pragma hdrstop

#include "Hash.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

bool Hash::CalcHash(wchar_t*szFileName, unsigned int HashType, AnsiString &OutHash, AnsiString &OutError)
{
        bool bResult = true;
        
        HCRYPTPROV hCryptProv = NULL;
        HCRYPTHASH hHash = NULL;

        int cbRead = 0; // кол-во прочитанных байтов из файла в буфер
        const int BUFSIZE = 65536; // размер буфера
        UCHAR *szBuffer = new char[BUFSIZE]; // буфера

        byte *szHash = NULL; // хеш
        DWORD cbHash = 0; // размер хеша в байтах       
        DWORD dwCount = 4;

        TFileStream *fs = NULL; 

        try
        {
                fs = new TFileStream(szFileName, fmOpenRead | fmShareDenyWrite);

                //--------------------------------------------------------------------
                // Get a handle to a cryptography provider context.
                if ( !CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, 0) )
                {
                        throw Exception( "Error CryptAcquireContext: " + SysErrorMessage(GetLastError()) );
                }


                //--------------------------------------------------------------------
                // Acquire a hash object handle.
                if ( !CryptCreateHash(hCryptProv, HashType, 0, 0, &hHash))
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
                BinToHex(szHash, OutHash.c_str(), cbHash);
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
