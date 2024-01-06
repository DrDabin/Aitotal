//---------------------------------------------------------------------------
#include <vcl.h>
#include <_str.h>
#include <io.h>
#include <memory>
#pragma hdrstop

#include "ParseHeadrArchiv.h"
#include "extract.h"
#include "Aitotal1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
 //---------------------------------------------------------------------------
UnicodeString FilePatch;
UnicodeString LnMesArchivErrorUnpacking = "Ошибка распаковки архива. ";
UnicodeString LnMesErroeReadSignArchiv = "\n Ошибка. Чтения сигнатуры архива.\n";
UnicodeString LnMesErrorArchivCorupted = "Возможно битый архив";
UnicodeString LnMesErrorNouSign = "\n Ошибка. Нет совпадение сигнатур архива";
UnicodeString LnMesErrorNouArchivSign = "Не известный архив";

 bool Read_head_file_New(UnicodeString FileName, UnicodeString &head)
 {
   try
   {
	  //Создаем поток и загружаем в него файл
	  std::auto_ptr<TFileStream> tfile (new TFileStream(FileName,fmOpenRead|fmShareDenyWrite));
	  //TFileStream  *tfile=new TFileStream(FileName,fmOpenRead);
	  unsigned char *pszBuffer;
	  //Выделяем память для буфера информации
	  pszBuffer=(char *)malloc(9);
	  //Можно не применять - при открытии указатель и так на нуле
	  tfile->Seek(0,0);
	  //Читаем информацию в буфер
	  tfile->Read(pszBuffer,9);
	  //переводим в 16 систему
	  unsigned int r = 0;

	  for(unsigned int i=0;i <7;i++)
	  {
		 head += IntToHex(pszBuffer[i],2);
	  }

	  free(pszBuffer);
	  delete pszBuffer;

	  head = head.LowerCase();
	  return true;

   }
   catch(Exception &E)
	{
	   Form3->ErrorLog(FileName + LnMesErroeReadSignArchiv+ E.Message);
	   MessageBoxW(0, E.Message.w_str(), 0, MB_OK + MB_ICONSTOP);
	}
   return false;
 }
 //++++++++++++++++++++++++++++++++++++++++++++++++++
bool Read_head_file(UnicodeString FileName, UnicodeString &head)
{
   FilePatch =  FileName;
   try
   {
	  //Создаем поток и загружаем в него файл
	  std::auto_ptr<TFileStream> tfile (new TFileStream(FileName,fmOpenRead|fmShareDenyWrite));
	  //TFileStream  *tfile=new TFileStream(FileName,fmOpenRead);
	  unsigned char *pszBuffer;
	  //Выделяем память для буфера информации
	  pszBuffer=(char *)malloc(9);
	  //Можно не применять - при открытии указатель и так на нуле
	  tfile->Seek(0,0);
	  //Читаем информацию в буфер
	  tfile->Read(pszBuffer,9);
	  //переводим в 16 систему
	  unsigned int r = 0;
	  for(unsigned int i=0;i <7;i++)
	  {
		 if(pszBuffer[i] ==NULL)
			++r;

		 head += IntToHex(pszBuffer[i],2);

	  }

	  free(pszBuffer);
	  delete pszBuffer;

	  if(r<3)
	  {
		 head = head.LowerCase();
		 return true;
	  }
	  else
	  {
		 ShowMessage(LnMesErroeReadSignArchiv + LnMesErrorArchivCorupted);
		 return false ;
	  }
   }
   catch(Exception &E)
	{
	   Form3->ErrorLog(FileName + LnMesErroeReadSignArchiv+ E.Message);
	   MessageBoxW(0, E.Message.w_str(), 0, MB_OK + MB_ICONSTOP);
	}
   return false;
}
//CLSID_CFormat == 1 == 7z
//CLSID_CFormat == 2 == zip
//CLSID_CFormat == 3 == rar
//CLSID_CFormat ==4 == rar5
bool Parse_Head_Guid(UnicodeString guidParse,unsigned int &GuidArchiv)
{
	   if(    AnsiCompareStr(guidParse,"504b0304140000")==0
		   || AnsiCompareStr(guidParse,"504b0304140009")==0
		   || AnsiCompareStr(guidParse,"504b0304140001")==0
		   || AnsiCompareStr(guidParse,"504b0304140002")==0
		   || AnsiCompareStr(guidParse,"504b0304140300")==0
		   || AnsiCompareStr(guidParse,"504b03040a0000")==0
		   || AnsiCompareStr(guidParse,"504b03040a0009")==0
		   || AnsiCompareStr(guidParse,"504b3030504b03")==0


		   )

	   {
		   GuidArchiv = 2;
		   return true;
	   }
	   else if(AnsiCompareStr(guidParse, L"526172211a0700")==0)
	   {
		  GuidArchiv = 3;
		  return true;
	   }
	   else if(AnsiCompareStr(guidParse, L"526172211a0701")==0)
	   {
		  GuidArchiv = 4;
		  return true;
	   }
	   else if(AnsiCompareStr(guidParse, L"377abcaf271c00")==0)
	   {
		  GuidArchiv = 1;
		  return true;
	   }

	  Form3->ErrorLog(FilePatch + LnMesErrorNouSign +" guidParse. /n" +guidParse);
	  ShowMessage(LnMesArchivErrorUnpacking);
	   return false;
}

//+++++++++++++++++++new
bool Parse_Head_Guid_New(UnicodeString guidParse,unsigned int &GuidArchiv)
{
	UnicodeString pars="";
	pars =guidParse.SubString(1,4);

	if(AnsiCompareStr(pars,L"504b") == 0)
	{
        GuidArchiv = 2;
		return true;
	}
	else if(AnsiCompareStr(pars,L"377a") == 0)
	{
        GuidArchiv = 1;
		return true;
	}
	else if(AnsiCompareStr(pars,L"5261") == 0)
	{
		/*int rar5 =  guidParse.SubString(14,1).ToInt();

		if(rar5 ==0)
		{
           GuidArchiv = 3;
		   return true;
		}
		else if(rar5 == 1)
		{
            GuidArchiv = 4;
			return true;
		}*/
        if(AnsiCompareStr(guidParse, L"526172211a0700")==0)
		{
		  GuidArchiv = 3;
		  return true;
		}
		else if(AnsiCompareStr(guidParse, L"526172211a0701")==0)
		{
		  GuidArchiv = 4;
		  return true;
	    }
	}

	Form3->ErrorLog(FilePatch + LnMesErrorNouArchivSign + " его ./n guidParse = " +guidParse);
	ShowMessage(LnMesArchivErrorUnpacking +LnMesErrorNouArchivSign);
	return false;
}
