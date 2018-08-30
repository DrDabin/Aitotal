// CodeGear C++Builder
// Copyright (c) 1995, 2016 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'uSha256.pas' rev: 32.00 (Windows)

#ifndef Usha256HPP
#define Usha256HPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>
#include <SysInit.hpp>
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <System.Types.hpp>

//-- user supplied -----------------------------------------------------------

namespace Usha256
{
//-- forward type declarations -----------------------------------------------
struct TSHA256Table;
class DELPHICLASS TSha256;
//-- type declarations -------------------------------------------------------
typedef System::Byte *PBYTE;

typedef unsigned *PDWORD;

#pragma pack(push,1)
struct DECLSPEC_DRECORD TSHA256Table
{
public:
	System::StaticArray<unsigned, 8> x;
};
#pragma pack(pop)


#pragma pack(push,4)
class PASCALIMPLEMENTATION TSha256 : public System::TObject
{
	typedef System::TObject inherited;
	
protected:
	System::StaticArray<unsigned, 8> m_dwH;
	unsigned m_dwLNumBits;
	unsigned m_dwHNumBits;
	System::StaticArray<unsigned, 16> m_aBlock;
	unsigned m_nNumChr;
	void __fastcall Generate(void);
	unsigned __fastcall ReverseEndian(unsigned x);
	
public:
	__fastcall TSha256(void);
	__fastcall virtual ~TSha256(void);
	void __fastcall Init(void);
	void __fastcall Load(PBYTE pBuffer, unsigned dwSIze);
	void __fastcall Final(void);
	System::UnicodeString __fastcall GetDigest(void);
	TSHA256Table __fastcall GetTable(void);
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
static const System::Int8 SHA256_HASH = System::Int8(0x8);
static const System::Int8 SHA256_BLOCK = System::Int8(0x10);
static const System::Int8 SHA256_WORK = System::Int8(0x40);
static const System::Int8 SHA256_BUFFER_SIZE = System::Int8(0x40);
extern DELPHI_PACKAGE System::StaticArray<unsigned, 8> c_dwInitH;
extern DELPHI_PACKAGE System::StaticArray<unsigned, 64> c_dwK;
extern DELPHI_PACKAGE System::UnicodeString __fastcall SHA256String(System::UnicodeString filename);
extern DELPHI_PACKAGE System::UnicodeString __fastcall _GetSHA256(System::Classes::TStream* ss, __int64 size)/* overload */;
extern DELPHI_PACKAGE System::UnicodeString __fastcall _GetSHA256(void * ptr, unsigned size)/* overload */;
}	/* namespace Usha256 */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_USHA256)
using namespace Usha256;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Usha256HPP
