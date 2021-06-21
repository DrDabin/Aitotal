//---------------------------------------------------------------------------

#ifndef BaseParserH
#define BaseParserH
//---------------------------------------------------------------------------
#include "AitotalInclude/MyStruct.h"
class BaseClass{
  private:
	 UnicodeString Mystroka;
	 UnicodeString PathLogFileBase;
	 TStringList *SpisokBase;// (new TStringList(NULL) );
	 __int64 PosiciyaPoisk;
  public:
	 void BaseClass::AddBase(Base StructBase);
	 void BaseClass::SaveToLog();
	 void BaseClass::AddSaveTolog(Base StructBase);
	 Base BaseClass::LogToMyStruct(UnicodeString StrokaSave);
	 Base BaseClass::LogToMyStruct(int number);
	 UnicodeString BaseClass::TexTega(UnicodeString text, UnicodeString beginTeg, UnicodeString endTeg,__int64 Posiciya);
	 void BaseClass::AddToListItem();
	 void BaseClass::Delete(int number);
	 int BaseClass::PostBase(UnicodeString PosPatchFile, UnicodeString PostMD5, UnicodeString PostDate);
     void BaseClass::Clear();
	 BaseClass::BaseClass();
	 ~BaseClass();
};





#endif
