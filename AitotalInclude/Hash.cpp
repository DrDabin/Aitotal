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

        int cbRead = 0; // ���-�� ����������� ������ �� ����� � �����
        const int BUFSIZE = 65536; // ������ ������
        UCHAR *szBuffer = new char[BUFSIZE]; // ������

        byte *szHash = NULL; // ���
        DWORD cbHash = 0; // ������ ���� � ������       
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

                // ��������
                while ( (cbRead = fs->Read(szBuffer, BUFSIZE)) != 0 )
                {
                        if ( !CryptHashData(hHash, szBuffer, cbRead, 0) )
                        {
                                throw Exception( "Error CryptHashData: " + SysErrorMessage(GetLastError()) );
                        }
                }

                // ����� ���-�� ������ � ����
                if ( !CryptGetHashParam(hHash, HP_HASHSIZE, (byte*)&cbHash, &dwCount, 0) )
                {
                        throw Exception( "Error CryptGetHashParam(HP_HASHSIZE): " + SysErrorMessage(GetLastError()) );
                }

                // �������� ��������� �����������
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

        // ����������� �������
        delete fs;
        delete[] szBuffer;
        if (szHash) delete[] szHash;

        if(hHash)
                CryptDestroyHash(hHash);
        if(hCryptProv)
        CryptReleaseContext(hCryptProv,0);

        // ���������� true ��� falses
        return bResult;
}
