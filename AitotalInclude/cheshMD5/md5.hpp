// CodeGear C++Builder
// Copyright (c) 1995, 2016 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'md5.pas' rev: 32.00 (Windows)

#ifndef Md5HPP
#define Md5HPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>
#include <SysInit.hpp>
#include <Winapi.Windows.hpp>
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>

//-- user supplied -----------------------------------------------------------

namespace Md5
{
//-- forward type declarations -----------------------------------------------
struct TMD5Digest;
//-- type declarations -------------------------------------------------------
typedef TMD5Digest *PMD5Digest;

struct DECLSPEC_DRECORD TMD5Digest
{
	
public:
	union
	{
		struct 
		{
			System::StaticArray<System::Byte, 16> v;
		};
		struct 
		{
			int A;
			int B;
			int C;
			int D;
		};
		
	};
};


//-- var, const, procedure ---------------------------------------------------
extern DELPHI_PACKAGE System::UnicodeString __fastcall MD5DigestToStr(const TMD5Digest &Digest);
extern DELPHI_PACKAGE TMD5Digest __fastcall MD5String(const System::UnicodeString S);
extern DELPHI_PACKAGE TMD5Digest __fastcall MD5File(const System::UnicodeString FileName);
extern DELPHI_PACKAGE TMD5Digest __fastcall MD5Stream(System::Classes::TStream* const Stream);
extern DELPHI_PACKAGE TMD5Digest __fastcall MD5Buffer(const void *Buffer, int Size);
extern DELPHI_PACKAGE bool __fastcall MD5DigestCompare(const TMD5Digest &Digest1, const TMD5Digest &Digest2);
extern DELPHI_PACKAGE System::UnicodeString __fastcall CalkFileMD5(System::UnicodeString AFileName);
extern DELPHI_PACKAGE System::UnicodeString __fastcall CalksStreamMD5(System::Classes::TStream* const Stream);
}	/* namespace Md5 */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_MD5)
using namespace Md5;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Md5HPP
