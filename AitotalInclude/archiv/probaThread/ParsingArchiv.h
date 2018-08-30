//---------------------------------------------------------------------------

#ifndef ParsingArchivH
#define ParsingArchivH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class ParsingArchiv : public TThread
{
private:
	UnicodeString NameLogError; //Путь до файла с ошибкой
	Base VtBase;
	UnicodeString ErrorMessage; // текст ошибки.
	TListItem *ListItem;
	int FileNumber;
	void __fastcall ParsingArchiv::ExtArchiv(UnicodeString ArcName);
	AnsiString ParsingArchiv::MyFiledDirArchiv(UnicodeString Dir);
	AnsiString ParsingArchiv::ExstractFileNameBezExt(UnicodeString NameFile);
	void __fastcall ParsingArchiv::OtWetErrorSizeFile(UnicodeString FileNime, UnicodeString MoveFileName,  __int64 sizefile, UnicodeString Md5);
	void __fastcall ParsingArchiv::ZanoshuVListWiew3PriOshibki(String ArcName, __int64 MysizaFile , String rezult, String Heh, bool date);
protected:
	void __fastcall Execute();
	void __fastcall ParsingArchiv::ErrorLog();
public:
	__fastcall ParsingArchiv(bool CreateSuspended);
	Options AtOptions;
};
//---------------------------------------------------------------------------
#endif
