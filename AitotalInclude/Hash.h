//---------------------------------------------------------------------------

#ifndef HashH
#define HashH
//---------------------------------------------------------------------------

#include <vcl.h>
#include <Wincrypt.h>

#ifndef CALG_SHA_256
        #define CALG_SHA_256  0x0000800c
#endif

#ifndef CALG_SHA_384
        #define CALG_SHA_384  0x0000800d
#endif

#ifndef CALG_SHA_512
        #define CALG_SHA_512  0x0000800e
#endif

namespace Hash
{
        static const unsigned int MD2 = CALG_MD2;
        static const unsigned int MD4 = CALG_MD4;
        static const unsigned int MD5 = CALG_MD5;
        static const unsigned int SHA = CALG_SHA;
        static const unsigned int SHA_256 = CALG_SHA_256;
        static const unsigned int SHA_384 = CALG_SHA_384;
        static const unsigned int SHA_512 = CALG_SHA_512;

        bool CalcHash(wchar_t *szFileName, const unsigned int HashType, AnsiString &OutHash, AnsiString &OutError);
}

#endif
