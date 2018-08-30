//---------------------------------------------------------------------------

#pragma hdrstop

#include "mysha256.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void __fastcall Usha256::TSha256::Init()
{

	for(int i=0; i< SHA256_HASH ;i++)
	   m_dwH[i] = c_dwInitH[i];

  m_dwLNumBits = 0;
  m_dwHNumBits = 0;
  m_nNumChr = 0;
}
void __fastcall Usha256::TSha256::Final()
{

  BYTE cZero,cOne;
  DWORD dwHNumBits,dwLNumBits;

  cZero = 0x00;
  cOne  = 0x80;

  dwHNumBits = ReverseEndian(m_dwHNumBits);
  dwLNumBits = ReverseEndian(m_dwLNumBits);

  Load(&cOne, 1);
  while (m_nNumChr == SHA256_BUFFER_SIZE - 8)
  {
	 Load(&cOne, 1);
  }

  Load(PBYTE(&dwHNumBits), 4);
  Load(PBYTE(&dwLNumBits), 4);
}

String  _GetSHA256(TStream *ss, __int64 size)
{
const int _MoveSize = 1024*1024*2;
  //ms : TMemoryStream;
  TMemoryStream * ms = new TMemoryStream();
  Usha256::TSha256 *SHA256 = new Usha256::TSha256();
  int s,rs;
  Usha256::PBYTE ptr;

  //RESULT := '';
  if (size<0)
	 return "";

  //ms := TmemoryStream.Create;
  ms->SetSize(_MoveSize);
  //Пока не знаю как перевести
  //ptr = ms->Memory;

  //SHA256 := TSHA256.Create;
  SHA256->Init();

  s = size;
  while (s>0)
  {
	if (s<_MoveSize)
	  rs = s;
    else
	  rs = _MoveSize;

	ss->Read(ptr,rs);
	SHA256->Load(ptr,rs);
	s = s - rs;
  }

  SHA256->Final();

  delete SHA256;
  delete ms;
  return SHA256->GetDigest();
}