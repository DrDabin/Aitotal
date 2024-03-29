//---------------------------------------------------------------------------
#include <vcl.h>
#include <memory>    //std::auto_prt<>
#include <System.IOUtils.hpp>
#include <StrUtils.hpp>
#pragma hdrstop

#include "AitotalInclude/MyStruct.h"
#include "Aitotal1.h"
#include "BaseParser.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
void BaseClass::AddBase(Base StructBase)
{
   SpisokBase->Add("<MynameFile>" + StructBase.BaseFileName +"</MynameFile>"+
				   "<MyPathFileName>" + StructBase.BasePatchFileName + "</MyPathFileName>"+
				   "<MyMD5>" + StructBase.BaseMD5 + "</MyMD5>"+
				   "<MySHA256>" + StructBase.BaseSHA256 + "</MySHA256>" +
				   "<MySizeFile>" + StructBase.BaseSizeFile + "</MySizeFile>" +
				   "<MyDate>" + StructBase.BaseDataProverki +"</MyDate>" +
				   "<MyDetect>" + StructBase.BaseDetect + "</MyDetect>" +
				   "<MyAdress>" + StructBase.BaseAdress +"</MyAdress>" +
				   "<MyPatchFileArchiv>"+StructBase.BasePatchFileArchiv+"</MyPatchFileAchiv>"+
				   "<MyJesson>" + StructBase.BaseJesson+ "</MyJesson>"
   );
}
void BaseClass::SaveToLog()
{
   //TDirectory::Exists
   if(TFile::Exists(PathLogFileBase))
   {
	   std::auto_ptr<TStringList>FileSpisokBase(new TStringList(NULL));
	   FileSpisokBase->LoadFromFile(PathLogFileBase);
	   FileSpisokBase->Add(SpisokBase->Text.Trim());
	   SpisokBase->Text = FileSpisokBase->Text;
	   SpisokBase->SaveToFile(PathLogFileBase);
   }
   else
	  SpisokBase->SaveToFile(PathLogFileBase);
}
BaseClass::BaseClass()
{
   SpisokBase =	new TStringList(NULL);
   PosiciyaPoisk = 0;
   PathLogFileBase = ExtractFilePath(Application->ExeName) + "AitotalTMP\\Base.txt";
}

BaseClass::~BaseClass()
{
	SpisokBase->Clear();
	delete SpisokBase;
}
//�������� � ������ ��������.
void BaseClass::AddSaveTolog(Base StructBase)
{
   AddBase(StructBase);
   SaveToLog();
}
UnicodeString BaseClass::TexTega(UnicodeString text, UnicodeString beginTeg, UnicodeString endTeg,__int64 Posiciya)
{
	int begin = PosEx(beginTeg,text,Posiciya);
	if (begin == 0)
		return "";
	else
	   begin+= beginTeg.Length();

	int end = PosEx(endTeg,text,begin);
	if(end==0)
	   return "";
	PosiciyaPoisk = end + endTeg.Length();
	return text.SubString(begin, end - begin);
}

Base BaseClass::LogToMyStruct(UnicodeString StrokaSave)
{
  Base Base;

  Base.BaseFileName = TexTega(StrokaSave,"<MynameFile>","</MynameFile>",1);
  Base.BasePatchFileName = TexTega(StrokaSave,"<MyPathFileName>","</MyPathFileName>",1);
  Base.BaseMD5 = TexTega(StrokaSave,"<MyMD5>","</MyMD5>",1);
  Base.BaseSHA256 = TexTega(StrokaSave,"<MySHA256>","</MySHA256>",1);
  Base.BaseSizeFile = TexTega(StrokaSave,"<MySizeFile>","</MySizeFile>",1);
  Base.BaseDataProverki = TexTega(StrokaSave,"<MyDate>","</MyDate",1);
  Base.BaseDetect = TexTega(StrokaSave,"<MyDetect>","</MyDetect>",1);
  Base.BaseAdress = TexTega(StrokaSave,"<MyAdress>","</MyAdress>",1);
  Base.BasePatchFileArchiv = TexTega(StrokaSave,"<MyPatchFileArchiv>","</MyPatchFileArchiv>",1);
  Base.BaseJesson = TexTega(StrokaSave,"<MyJesson>","</MyJesson>",1);

  return Base;
}

Base BaseClass::LogToMyStruct(int number)
{
  Base Base;

  if(TFile::Exists(PathLogFileBase))
   {
	  std::auto_ptr<TStringList>FileSpisokBase(new TStringList(NULL));

	  FileSpisokBase->LoadFromFile(PathLogFileBase);

	  Base =  LogToMyStruct(FileSpisokBase->Strings[number]);
   }
  return Base;
}

void BaseClass::AddToListItem()
{
   if(TFile::Exists(PathLogFileBase))
   {
	  std::auto_ptr<TStringList>FileSpisokBase(new TStringList(NULL));

	  FileSpisokBase->LoadFromFile(PathLogFileBase);

	  if(FileSpisokBase->Text !="")
	  {
		 TListItem *ListItem;

		 for( int i =0; i < FileSpisokBase->Count ; i++)
		 {
			 if(FileSpisokBase->Strings[i] !="")
			 {
				Base Base = LogToMyStruct(FileSpisokBase->Strings[i]);

				if(Base.BaseFileName !="" && Base.BasePatchFileName !="")
				{
				   ListItem = Form3->ListView3->Items->Add();
				   ListItem->Checked = false;
				   ListItem->Caption = Base.BaseFileName; // ��� �����
				   ListItem->SubItems->Add(Base.BasePatchFileName);// ������ ����.
				   ListItem->SubItems->Add(Base.BaseMD5);// ��� �����
				   ListItem->SubItems->Add(Base.BaseSHA256);
				   ListItem->SubItems->Add(Base.BaseSizeFile);// ������
				   ListItem->SubItems->Add(Base.BaseDataProverki);
				   ListItem->SubItems->Add(Base.BaseDetect);// ��������� ������������
				   ListItem->SubItems->Add(Base.BaseAdress);// ������ �� ���������.
				}
			 }
		 }
	  }
   }
}

int BaseClass::PostBase(UnicodeString PosPatchFile, UnicodeString PostMD5, UnicodeString PostDate)
{
   if(TFile::Exists(PathLogFileBase))
   {
	  std::auto_ptr<TStringList>FileSpisokBase(new TStringList(NULL));

	  FileSpisokBase->LoadFromFile(PathLogFileBase);

	  if(FileSpisokBase->Text !="")
	  {
		 Base Poisk;
		 for(int i =0; i < FileSpisokBase->Count; i++)
		 {
			if(FileSpisokBase->Strings[i] !="")
			{
			   Poisk = LogToMyStruct(FileSpisokBase->Strings[i]);

			   if((AnsiCompareText(Poisk.BasePatchFileName, PosPatchFile) ==0) &&
				   (AnsiCompareText(Poisk.BaseMD5, PostMD5) ==0)&&
				   (AnsiCompareText(Poisk.BaseDataProverki, PostDate) == 0)
				  )
				   return i;
			}
		 }

	  }
   }
   return -1;
}

void BaseClass::Delete(int number)
{
   if(TFile::Exists(PathLogFileBase))
   {
	  std::auto_ptr<TStringList>FileSpisokBase(new TStringList(NULL));

	  FileSpisokBase->LoadFromFile(PathLogFileBase);

	  if(FileSpisokBase->Text !="")
	  {
		FileSpisokBase->Delete(number);
		FileSpisokBase->SaveToFile(PathLogFileBase);
	  }
   }
}

void BaseClass::Clear()
{
   if(TFile::Exists(PathLogFileBase))
   {
	  std::auto_ptr<TStringList>FileSpisokBase(new TStringList(NULL));

	  FileSpisokBase->LoadFromFile(PathLogFileBase);
	  FileSpisokBase->Clear();
	  FileSpisokBase->SaveToFile(PathLogFileBase);
   }
}
