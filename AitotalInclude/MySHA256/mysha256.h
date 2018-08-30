//---------------------------------------------------------------------------

#ifndef mysha256H
#define mysha256H

#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.SysUtils.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit
#include <System.Types.hpp>	// Pascal unit

namespace Usha256
{
   typedef Byte *PBYTE;
   typedef unsigned *PDWORD;

   const int SHA256_HASH = 8;
   const int SHA256_BLOCK = 16;
   const int SHA256_WORK = 64;
   const int SHA256_BUFFER_SIZE = SHA256_BLOCK*4;
   unsigned  c_dwInitH[SHA256_HASH] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

   unsigned c_dwK[SHA256_WORK]= {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};

   struct TSHA256Table
   {
	  public:
		 unsigned x[8];
   };
   class TSha256
   {
	  typedef System::TObject inherited;

	  protected:
				//System::StaticArray<unsigned, 8> m_dwH;
				unsigned m_dwH[8];
				unsigned m_dwLNumBits;
				unsigned m_dwHNumBits;
				unsigned m_aBlock[16];
				unsigned m_nNumChr;
				void __fastcall Generate(void);
				unsigned __fastcall ReverseEndian(unsigned x);

	   public:
			  __fastcall TSha256(void);
			  __fastcall virtual ~TSha256(void);
			  void __fastcall Init();
			  void __fastcall Load(PBYTE pBuffer, unsigned dwSIze);
			  void __fastcall Final();
			  System::UnicodeString __fastcall GetDigest(void);
			  TSHA256Table __fastcall GetTable(void);
};


   //function _GetSHA256(ss:TStream; size:INT64):String; overload;
   String  _GetSHA256(TStream *ss, __int64 size);
   String _GetSHA256(Pointer ptr, DWORD size);
   //function _GetSHA256(ptr:Pointer; size:DWORD):String; overload;

}
//---------------------------------------------------------------------------
#endif
