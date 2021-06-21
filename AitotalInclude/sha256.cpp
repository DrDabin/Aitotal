//---------------------------------------------------------------------------
#include "Aitotal1.h"
 #include "uSha256.hpp"
#pragma hdrstop

#include "sha256.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

UnicodeString Mysha256(UnicodeString filename,UnicodeString &error)
{
	UnicodeString sha = "";
	error = "";
	if(FileExists(filename))
	{
	   try
	   {  //begin
		  TFileStream *fs = new TFileStream(filename, fmOpenRead | fmShareDenyNone);

		  try
		  {

			 //f := ExpandFileName(filename);
			 if(fs->Size >0)
				sha = _GetSHA256(fs,fs->Size);
			 else
			 {
				error = "Ошибка рсчёта SHA256.3\n Имя файл = " + filename + ": Сообщение ошибки  = \"Размер FileStream < 0 \":";
				Form3->ErrorLog(error);
			 }

		  }

		  catch(EFOpenError &E)
		  {
			error = "Ошибка расчёта SHA256.\n Имя файл = " + filename + " Сообщение ошибки  = " + E.Message;
			Form3->ErrorLog(error);
			return sha;
		  }
		  catch(Exception &E)
		  {
			error = "Ошибка расчёта SHA256.\n Имя файл = " + filename + " Сообщение ошибки  = " + E.Message;
			Form3->ErrorLog(error);
		  }

		  delete fs;
	   }
	   catch(EFOpenError &E)
	   {
		  error = "Ошибка расчёта SHA256.\n Имя файл = " + filename + " Сообщение ошибки  = " + E.Message;
		  Form3->ErrorLog(error);
		  return sha;
	   }
	   catch(Exception &E)
	   {
		  error = "Ошибка расчёта SHA256.\n Имя файл = " + filename + " Сообщение ошибки  = " + E.Message;
		  Form3->ErrorLog(error);
	   }
	}
	else
	{
	   error = "name file = \"" + filename +"\"" +"\n nou file";
	   Form3->ErrorLog(error);
    }
	return sha;

}
///++++++++++++++++++++++++++++++++++
UnicodeString Mysha256ver2(UnicodeString filename)
{
	UnicodeString sha = "";

	if(FileExists(filename))
	{
	   try
	   {
		  TFileStream *fs = new TFileStream(filename, fmOpenRead | fmShareDenyNone);

		  try
		  {
			 if(fs->Size >0)
				sha = _GetSHA256(fs,fs->Size);
			 else
				Form3->ErrorLog("Ошибка рсчёта SHA256.3\n Имя файл = " + filename + ": Сообщение ошибки  = \"Размер FileStream < 0 \":");
		  }

		  catch(EFOpenError &E)
		  {
			Form3->ErrorLog("Ошибка расчёта SHA256.\n Имя файл = " + filename + " Сообщение ошибки  = " + E.Message);
		  }
		  catch(Exception &E)
		  {
			Form3->ErrorLog("Ошибка расчёта SHA256.\n Имя файл = " + filename + " Сообщение ошибки  = " + E.Message);
		  }

		  delete fs;
	   }
	   catch(EFOpenError &E)
	   {
		  Form3->ErrorLog("Ошибка расчёта SHA256.\n Имя файл = " + filename + " Сообщение ошибки  = " + E.Message);
	   }
	   catch(Exception &E)
	   {
		  Form3->ErrorLog("Ошибка расчёта SHA256.\n Имя файл = " + filename + " Сообщение ошибки  = " + E.Message);
	   }
	}
	else
	   Form3->ErrorLog("Ошибка расчёта SHA256.\n Имя файл = \"" + filename +"\"" +"\n nou file");

	return sha;

}
 //Надо будет потом просто этот срр удалить и использовать код дельфи uSha256.pas
UnicodeString Streamsha256(TStream *filename)
{
	UnicodeString sha = "";

	try
	{
	   sha = _GetSHA256(filename, filename->Size);
	}
	catch(Exception &E)
	{
	   Form3->ErrorLog("Ошибка расчёта SHA256.\n  Сообщение ошибки  = " + E.Message);
	}
	return sha;
}
