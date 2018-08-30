﻿//---------------------------------------------------------------------------

#include <System.hpp>
#include <memory>

/*#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp> */
#include <DBXJSON.hpp>
#include <DateUtils.hpp>
//#include <IdHashSHA.hpp>

#include <IdComponent.hpp>


#pragma hdrstop

#include "Aitotal1.h"
#include "Aitotal.cpp"
//#include "sha256.h"
#include "AitotalInclude/InternetConnected.h"
#include "AitotalInclude/RabotaSfilam.h"
#include "AitotalInclude/AitotalVersion.h"

#include "ScanVTIndy1.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall ScanVTIndy::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall ScanVTIndy::ScanVTIndy(bool CreateSuspended)
	: TThread(CreateSuspended)
{
   FreeOnTerminate = true;
}
//---------------------------------------------------------------------------
void __fastcall ScanVTIndy::Execute()
{
   NameLogError = ExtractFilePath(ParamStr(0)) + "AitotalTMP\\Error.log";
   VtBase.BaseFileName = ExtractFileName(VtBase.BasePatchFileName);
   FileUpload = false;

   TimeLogirovanie  =  StringReplace(VtBase.BasePredScanData,":","_",TReplaceFlags()<<rfReplaceAll);;
   if(logirovanie)
   {
	  LogMessage = "Начало сканирование";
	  Synchronize(&Logirovanie);
   }

   ScanApiZapros();

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::FileUploadBool()
{
   if(Form3->SpisokNamePotok->Count < Form3->AtOptions.FileCount)
   {
	  FileUpload = true;
	  Form3->SpisokNamePotok->Add(PotokNumber);
      if(logirovanie)
	  {
		 LogMessage = "FileUpload = true в FileUploadBool()";
		 Synchronize(&Logirovanie);
	  }
   }
   else
   {
	  FileUpload = false;
	  if(logirovanie)
	  {
		 LogMessage = "FileUpload = false в FileUploadBool()";
		 Synchronize(&Logirovanie);
	  }
   }

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++
bool __fastcall ScanVTIndy::PostHesh(UnicodeString Hesh)
{
   bool scan = false;
   while(!Terminated)
   {
	  if(Terminated)
	  {
		 Progress ="Проверка остановлена";
		 Synchronize(&ScanProgres);
		 if(logirovanie)
		 {
			LogMessage = "в PostHesh(UnicodeString Hesh) Проверка остановлена";
			Synchronize(&Logirovanie);
		 }
		 break;
	  }

	  if(!GetStatusConnect())
	  {
		 Progress ="Ошибка сети. Нет Интернета.";
		 // Вывожу в таблицу о начале загрузке.
		 ErrorMessage = "Ошибка сети. Нет Интернета.";
		 Synchronize(&ErrorLog);
		 Synchronize(&ScanProgres);
         if(logirovanie)
		 {
			LogMessage = "в PostHesh(UnicodeString Hesh) \n Ошибка сети.";
			Synchronize(&Logirovanie);
		 }
		 break;
	  }

	  if(ReportVT(VtBase.BaseMD5))
	  {
		 if(logirovanie)
		 {
			LogMessage = "в PostHesh(UnicodeString Hesh) \n ReportVT == true.";
			Synchronize(&Logirovanie);
		 }
		 if(ScanIndyVT.http_response_code != 204)
		 {
			if(logirovanie)
			{
			   LogMessage = "в PostHesh(UnicodeString Hesh) \n ScanIndyVT.http_response_code != 204";
			   Synchronize(&Logirovanie);
			}
			if(JSONParseResponzeCode() == 1 )
			{
			   if(logirovanie)
			   {
				  LogMessage = "в PostHesh(UnicodeString Hesh)\n JSONParseResponzeCode() == 1 \n" + VtBase.BaseJesson;
				  Synchronize(&Logirovanie);
			   }
			   scan = true;
			}
			else
			{
			   scan = false;
			   if(logirovanie)
			   {
				  LogMessage = "в PostHesh(UnicodeString Hesh)\n else JSONParseResponzeCode() == 1 \n" + VtBase.BaseJesson;
				  Synchronize(&Logirovanie);
			   }
			}
			break;
		 }
		 else
			Sleep(2000);
	  }
   }
   if(Terminated)
   {
	  Progress ="Проверка остановлена";
	  Synchronize(&ScanProgres);
	  if(logirovanie)
	  {
		 LogMessage = "в PostHesh(UnicodeString Hesh) Проверка остановлена";
		 Synchronize(&Logirovanie);
	  }
   }
   return scan;
}
//+++++++++++++++++++++++++++++++++++++
int __fastcall ScanVTIndy::JSONParseResponzeCode()
{
   try
   {
	  std::auto_ptr<TJSONObject> object(static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(VtBase.BaseJesson)));
      // ++++++ START заносим джейсон в файл.
	  //std::auto_ptr<TStringList>FileSpisokProwerki(new TStringList(NULL));
	  //FileSpisokProwerki->Add(object->ToString());
	  //FileSpisokProwerki->SaveToFile("alex");
	  //++++++++++END++
	  if(logirovanie)
	  {
		 LogMessage = "JSONParseResponzeCode() true \n" + object->ToString();
		 Synchronize(&Logirovanie);
	  }
	  ScanIndyVT.VT_response_code  = (object->Get("response_code")->JsonValue->Value()).Trim().ToInt();

	  //return ScanIndyVT.VT_response_code;
   }
   catch(...)
   {
	   if(logirovanie)
	   {
		  LogMessage = "JSONParseResponzeCode() catch(..) \n Ошибка парсинга " + VtBase.BaseJesson;
		  Synchronize(&Logirovanie);
	   }
	   ScanIndyVT.VT_response_code = 5;
	   ErrorMessage = "error: JSONParseResponzeCode\n JSON = " + VtBase.BaseJesson +"\n http_response_code = " + ScanIndyVT.http_response_code;
	   Synchronize(&ErrorLog);
   }
   return ScanIndyVT.VT_response_code;
}
//+++++++++++++++++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::ScanProgres()
{
   if(logirovanie)
   {
	  LogMessage = "ScanProgress() begin \n" + Progress;
	  Synchronize(&Logirovanie);
   }
   int i = 0;
   for(i; i < Form3->ListView2->Items->Count ; i++)
   {
	  if((int)Form3->ListView2->Items->Item[i]->Data == FileNumber)
	  {
		 Form3->ListView2->Items->Item[i]->SubItems->Strings[4] = Progress;
		 break;
	  }
   }
   if(logirovanie)
   {
	  LogMessage = IntToStr((int)Form3->ListView2->Items->Item[i]->Data)+ "+++ ScanProgres() end \n " + Progress;
	  Synchronize(&Logirovanie);
   }
   Progress = "";
}
//+++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::OtwetOshibka()
{
   Progress = "Ошибка";
   Synchronize(&ScanProgres);
   Synchronize(&DelSpisokNamePotok);
   if(logirovanie)
	  {
		 LogMessage = "OtwetOshibka()\n";
		 Synchronize(&Logirovanie);
	  }
}
//+++++++++++++++++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::JSONGetPermalink()
{
   try
   {
	  std::auto_ptr<TJSONObject> ResRescan(static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(VtBase.BaseJesson)));
	  VtBase.BaseAdress = ResRescan->Get("permalink")->JsonValue->Value();
	  if(logirovanie)
	  {
		 LogMessage = "JSONGetPermalink() try \n " + ResRescan->ToString();
		 Synchronize(&Logirovanie);
	  }
   }
   catch(...)
   {
       if(logirovanie)
	  {
		 LogMessage = "JSONGetPermalink() catch(...) \n " + VtBase.BaseJesson;
		 Synchronize(&Logirovanie);
	  }
	   ErrorMessage = "error: JSONGetPermalink\n JSON = " + VtBase.BaseJesson + "\n http_response_code = " + ScanIndyVT.http_response_code;
	   Synchronize(&ErrorLog);
   }
}
//---------------------------------------------------------------------------
void __fastcall ScanVTIndy::ScanApiZapros()
{
   VtBase.BaseDataProverki = "";
   Progress = "Ищем хеш в базе VT";
   // Вывожу в таблицу о начале сканирования
   Synchronize(&AtScanBegin);
   if(logirovanie)
   {
	  LogMessage = "Вывожу в таблицу о начале сканирования \n ScanApiZapros()";
	  Synchronize(&Logirovanie);
   }

   while(!Terminated)
   {
	 if(!GetStatusConnect())
	 {
		Progress ="Ошибка сети. Нет Интернета.";
		// Вывожу в таблицу о начале загрузке.
		ErrorMessage = "Ошибка сети. Нет Интернета.";
		Synchronize(&ErrorLog);
		Synchronize(&ScanProgres);
		if(logirovanie)
		{
		   LogMessage = "Ошибка сети ScanApiZapros() ";
		   Synchronize(&Logirovanie);
		}
		break;
	 }
	 bool proverka =PostHesh(VtBase.BaseMD5.c_str());

	 if(!proverka)
	 {
        if(logirovanie)
		{
		   LogMessage = "bool proverka == false \n Файл не проверялся";
		   Synchronize(&Logirovanie);
		}
		Synchronize(&FileUploadBool);

		if(FileUpload)
		{
           if(logirovanie)
		   {
			  LogMessage = "Начало загрузки файла ScanApiZapros()";
			  Synchronize(&Logirovanie);
		   }
		   Progress ="Загрузка файла";
		   // Вывожу в таблицу о начале загрузке.
		   Synchronize(&ScanProgres);
		   //отправляем файл.
		   if(VtBase.BaseSizeFile.ToInt() > 33554432)
		   {
			 if(logirovanie)
			 {
				LogMessage = "Ошибка проверки. Размер файла превышает 32Мб ScanApiZapros() ";
				Synchronize(&Logirovanie);
			 }
			  Synchronize(&OtwetErrorSizeFile);
			  break;
		   }
		   if(LoadingFile())
		   {
			   if(logirovanie)
			   {
				  LogMessage = "if(LoadingFile()) ScanApiZapros() ";
				  Synchronize(&Logirovanie);
			   }
			   JSONGetPermalink();
			   if(logirovanie)
			   {
				LogMessage = "после JSONGetPermalink(); ScanApiZapros() ";
				Synchronize(&Logirovanie);
			   }

			   Synchronize(&ZapisPermalinkLV);
			   Synchronize(&DelSpisokNamePotok);
			   ApiReportFile(); // Ждем результат
			   if(logirovanie)
			   {
				LogMessage = "после ApiReportFile(); ScanApiZapros() ";
				Synchronize(&Logirovanie);
			   }
		   }
		   else
		   {
              if(logirovanie)
			 {
				LogMessage = "Ошибка загрузки файла. ScanApiZapros() ";
				Synchronize(&Logirovanie);
			 }
			  ErrorMessage = "Ошибка загрузки файла";
			  Synchronize(&ErrorLog);
			  OtwetOshibka();
		   }
		   break;
		}
		else
		   Sleep(2000);
	 }
	 //Рескан
	 else
	 {
		Synchronize(&DelSpisokNamePotok);
		// Если Рескан ==false
		if(!ScanIndyVT.Rescan)
		{
			if(logirovanie)
			{
			   LogMessage = "Рескан рескан == фалсе \n после if(!ScanIndyVT.Rescan) ScanApiZapros() ";
			   Synchronize(&Logirovanie);
			}
			
			ApiReScanFalse();

			if(logirovanie)
			{
			   LogMessage = "после ApiReScanFalse(); ScanApiZapros() ";
			   Synchronize(&Logirovanie);
			}
		}
		// Если Рескан ==true
		else
		{
			if(logirovanie)
			{
			   LogMessage = "после // Если Рескан ==true \n else ScanApiZapros() ";
			   Synchronize(&Logirovanie);
			}
			JSONGetPermalink();
			if(logirovanie)
			{
			   LogMessage = "после JSONGetPermalink(); ScanApiZapros() ";
			   Synchronize(&Logirovanie);
			}
			Synchronize(&ZapisPermalinkLV);
			ApiReScan();
			if(logirovanie)
			{
			   LogMessage = "после ApiReScan(); ScanApiZapros() ";
			   Synchronize(&Logirovanie);
			}
		}
		break;
	 }
	 //break;
   }

   if(Terminated)
   {
	  Progress ="Проверка остановлена";
	  Synchronize(&ScanProgres);
	  if(logirovanie)
	  {
		 LogMessage = "Проверка остановлена ScanApiZapros() ";
		 Synchronize(&Logirovanie);
	  }
   }


}
//+++++++++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::ZapisPermalinkLV()
{

   for(int i = 0; i < Form3->ListView2->Items->Count ; i++)
   {
	  if((int)Form3->ListView2->Items->Item[i]->Data == FileNumber)
	  {
		 Form3->ListView2->Items->Item[i]->SubItems->Strings[5] = VtBase.BaseAdress;
		 if(logirovanie)
		 {
			LogMessage = "в ZapisPermalinkLV()";
			Synchronize(&Logirovanie);
		 }
		 break;
	  }
   }
}
//+++++++++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::OtwetErrorSizeFile()
{
	UnicodeString er;
	UnicodeString SHA256="";
	//UnicodeString SHA256 = Mysha256(VtBase.BasePatchFileName,er);
	//Использую стандартную функцию расчета хеш суммы.
	try{
		SHA256 = THashSHA2::GetHashStringFromFile(TFileName(VtBase.BasePatchFileName), THashSHA2::TSHA2Version::SHA256);
	}
	catch(Exception &e)
	{
		ErrorMessage = "Ошибка расчета Sha256. ScanVTIndy::OtwetErrorSizeFile() THashSHA2::GetHashStringFromFile" + e.ToString();
		Synchronize(&ErrorLog);;
	}

	if(logirovanie)
	{
	   LogMessage = "в OtwetErrorSizeFile() после UnicodeString SHA256 = THashSHA2::GetHashStringFromFile();\n SHA256 = " + SHA256 ;
	   Synchronize(&Logirovanie);
	}


   TListItem *ListItem;
   ListItem = Form3->ListView3->Items->Add();
   ListItem->Checked = false;
   ListItem->Caption = ExtractFileName(VtBase.BasePatchFileName); // Имя файла
   ListItem->SubItems->Add(VtBase.BasePatchFileName);// пулный путь.
   ListItem->SubItems->Add(VtBase.BaseDetect);// результат сканирования
   ListItem->SubItems->Add(VtBase.BaseSizeFile);// размер
   ListItem->SubItems->Add(VtBase.BaseMD5);// хеш сумма
   ListItem->SubItems->Add(SHA256);//Хеш SHA256

   ListItem->SubItems->Add(VtBase.BaseDataProverki);//Дата проверки

   ListItem->SubItems->Add("Размер файла превышает 32Мб");// ссылка на результат.

  for(int i = 0; i < Form3->ListView2->Items->Count ; i++)
   {
	  if( (int)Form3->ListView2->Items->Item[i]->Data == FileNumber)
	  {
		 Form3->ListView2->Items->Item[i]->Delete();
		 break;
	  }
   }

}

void __fastcall ScanVTIndy::ApiReScanFalse()
{
   Progress ="Отмена ReScana. Ждём результат.";

   if(logirovanie)
	{
	   LogMessage = "в ApiReScanFalse() " ;
	   Synchronize(&Logirovanie);
	}
	Synchronize(&ScanProgres);

	if(JSONParseResponzeCode() == 1 )
	{
	   if(logirovanie)
	   {
		  LogMessage = "if(JSONParseResponzeCode() == 1 ) в ApiReScanFalse() " ;
		  Synchronize(&Logirovanie);
	   }
	   JSONParseDetect();
	   if(logirovanie)
	   {
		  LogMessage = " в ApiReScanFalse() после JSONParseDetect();";
		  Synchronize(&Logirovanie);
	   }
	   DateTime();
	   if(logirovanie)
	   {
		  LogMessage = " в ApiReScanFalse() после DateTime();";
		  Synchronize(&Logirovanie);
	   }
	   Synchronize(&Otwet);
	}
	else
	{
	   if(logirovanie)
	   {
		  LogMessage = " в ApiReScanFalse() Ошибка при отмене ReScana.";
		  Synchronize(&Logirovanie);
	   }
	   Progress ="Ошибка при отмене ReScana.";
	   Synchronize(&ScanProgres);
	   ErrorMessage = "Ошибка при отмене ReScana. JSONParseResponzeCode() = " + StrToInt(ScanIndyVT.VT_response_code);
	   Synchronize(&ErrorLog);
	}
}
//++++++++++++++++++++++++++++++++++++++
AnsiString ScanVTIndy::JSONParseResponzeDate()
{
   try
   {
	  std::auto_ptr<TJSONObject> object(static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(VtBase.BaseJesson)));

	  if(logirovanie)
	  {
		 LogMessage = " в JSONParseResponzeDate() блок try \n" + object->ToString();
		 Synchronize(&Logirovanie);
	  }

	  return (object->Get("scan_date")->JsonValue->Value()).Trim();

   }
   catch(...)
   {
	   if(logirovanie)
	  {
		 LogMessage = " в JSONParseResponzeDate() catch(...) \n" + VtBase.BaseJesson;
		 Synchronize(&Logirovanie);
	  }
	   ErrorMessage = "error: JSONParseResponzeDate\n JSON" +VtBase.BaseJesson+ "\n http_response_code = " + ScanIndyVT.http_response_code;
	   Synchronize(&ErrorLog);

   }
   return "";
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
AnsiString ScanVTIndy::ApiPostDateToStr(AnsiString Hesh)
{
   AnsiString date = "";
   while(!Terminated)
   {
	  if(!GetStatusConnect())
	  {
		 Progress ="Ошибка сети. Нет Интернета.";
		 ErrorMessage = "Ошибка сети. Нет Интернета.";
		 Synchronize(&ErrorLog);
		 Synchronize(&ScanProgres);
		 break;
	  }

	  if(ReportVT(VtBase.BaseMD5))
	  {
		 if(logirovanie)
		 {
			LogMessage = " в ApiPostDateToStr() после if(ReportVT(VtBase.BaseMD5))";
			Synchronize(&Logirovanie);
		 }
		 if(ScanIndyVT.http_response_code != 204)
		 {
			if(logirovanie)
			{
			   LogMessage = " в ApiPostDateToStr() после if(ScanIndyVT.http_response_code != 204)";
			   Synchronize(&Logirovanie);
			}
			if(JSONParseResponzeCode() == 1 )
			{
			   if(logirovanie)
			   {
				  LogMessage = " в ApiPostDateToStr() после if(JSONParseResponzeCode() == 1 )\n" + VtBase.BaseJesson;
				  Synchronize(&Logirovanie);
			   }
			   date = JSONParseResponzeDate();
			   break;
			}
			else
			   Sleep(10000);
		 }
		 else
			Sleep(10000);
	  }// подумать, что здесь поставить.
   }

   if(Terminated)
   {
	  Progress ="Проверка остановлена";
	  Synchronize(&ScanProgres);
	  if(logirovanie)
			   {
				  LogMessage = " в ApiPostDateToStr() после if(Terminated)\n";
				  Synchronize(&Logirovanie);
			   }
   }
   return date;
}
//+++++++++++++++++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::ApiReScan()
{
   Progress = "Повторная проверка файла";
	// Вывожу в таблицу о начале сканирования
	Synchronize(&ScanProgres);
	if(logirovanie)
	{
	   LogMessage = " в ApiReScan() ";
	   Synchronize(&Logirovanie);
	}
	int I;

	Progress = "Ждём ответа на повторный анализ";
	// Вывожу в таблицу о начале сканирования
	Synchronize(&ScanProgres);

	do
	{
		if(!GetStatusConnect())
		{
			Progress ="Ошибка сети. Нет Интернета.";
			// Вывожу в таблицу о начале загрузке.
			Synchronize(&ScanProgres);
			ErrorMessage = "Ошибка сети. Нет Интернета.";
			Synchronize(&ErrorLog);
			return;
		}


		if(RescanVT(VtBase.BaseMD5))
		{
			if(logirovanie)
			{
			   LogMessage = " в ApiReScan() после if(logirovanie)\n" + VtBase.BaseJesson;
			   Synchronize(&Logirovanie);
			}

			if(ScanIndyVT.http_response_code != 204)
			{
			   if(logirovanie)
			   {
				  LogMessage = " в ApiReScan() после if(ScanIndyVT.http_response_code != 204)\n" + VtBase.BaseJesson;
				  Synchronize(&Logirovanie);
			   }
			   if(JSONParseResponzeCode() ==1)
			   {
				  if(logirovanie)
				  {
					 LogMessage = " в ApiReScan() после if(JSONParseResponzeCode() ==1))\n" + VtBase.BaseJesson;
					 Synchronize(&Logirovanie);
				  }
				  //Begin while
				  while(!Terminated)
				  {
					 int timeBrec;
					 if(!GetStatusConnect())
					 {
						Progress ="Ошибка сети. Нет Интернета.";
						// Вывожу в таблицу о начале загрузке.
						Synchronize(&ScanProgres);
						ErrorMessage = "Ошибка сети. Нет Интернета.";
						Synchronize(&ErrorLog);
						return;
					 }
					 UnicodeString PostReport = ApiPostDateToStr(VtBase.BaseMD5);
					 if(logirovanie)
					 {
						LogMessage = " в ApiReScan() после UnicodeString PostReport = ApiPostDateToStr(VtBase.BaseMD5);\n" + VtBase.BaseJesson;
						Synchronize(&Logirovanie);
					 }
					 if(PostReport !="")
					 {
						if(DateTimeR(PostReport))
						{
						   if(logirovanie)
						   {
							  LogMessage = " в ApiReScan() после if(DateTimeR(PostReport))\n" + VtBase.BaseJesson;
							  Synchronize(&Logirovanie);
						   }
						   I = AnsiCompareStr(VtBase.BasePredScanData, PostReport);
						   if(I<=0)
						   {
							  JSONParseDetect();
							  DateTime();
							  Synchronize(&Otwet);
							  if(logirovanie)
							  {
								 LogMessage = " в ApiReScan() в if(I<=0)\n" + VtBase.BaseJesson;
								 Synchronize(&Logirovanie);
							  }
							  break;
						   }
							timeBrec = AnsiCompareStr(FormatDateTime("YYYY.MM.DD",Date())+ " " + FormatDateTime("HH:NN:SS",Time()),TimeBreak);
							if(timeBrec >=0)
							{
							   JSONParseDetect();
							   DateTime();
							   Synchronize(&Otwet);
							   if(logirovanie)
							   {
								 LogMessage = " в ApiReScan() в if(timeBrec >=0))\n" + VtBase.BaseJesson;
								 Synchronize(&Logirovanie);
							   }
							   break;
							}
							else
							   Sleep(20000);
						}
						else
						  Sleep(10000);
					 }
				  }
				  if(Terminated)
				  {
					 Progress ="Проверка остановлена";
					 Synchronize(&ScanProgres);
					 if(logirovanie)
					 {
						LogMessage = " в ApiReScan() в Проверка остановлена";
						Synchronize(&Logirovanie);
					 }
				  }
				  //Endwhile
				  break;

			   }
			   else
				  Sleep(10000);
			}
			//else
			   //Sleep(2000);
		}
	}while(!Terminated);

	if(Terminated)
	{
	  Progress ="Проверка остановлена";
	  Synchronize(&ScanProgres);
	}
}
//++++++++++++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::JSONParseDetect()
{
   try
   {
	  std::auto_ptr<TJSONObject> ResRescan(static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(VtBase.BaseJesson)));
	  if(logirovanie)
	  {
		 LogMessage = " в JSONParseDetect()\n" +  ResRescan->ToString();
		 Synchronize(&Logirovanie);
	  }

	  VtBase.BaseDetect = ResRescan->Get("positives")->JsonValue->Value() + "/" +
						  ResRescan->Get("total")->JsonValue->Value();

	  VtBase.BaseAdress = ResRescan->Get("permalink")->JsonValue->Value();

	  VtBase.BaseSHA256 =  ResRescan->Get("sha256")->JsonValue->Value();

	 VtBase.BaseDataProverki =  ResRescan->Get("scan_date")->JsonValue->Value();

   }
   catch(...)
   {
	   if(logirovanie)
	  {
		 LogMessage = " в JSONParseDetect() catch(...)\n" +  VtBase.BaseJesson;
		 Synchronize(&Logirovanie);
	  }
	   ErrorMessage = "error: JSONParseDetect\n JSON = " +VtBase.BaseJesson+ "\n http_response_code = " + ScanIndyVT.http_response_code;
	   Synchronize(&ErrorLog);
   }

}
//+++++++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::ApiReportFile()
{

	if(JSONParseResponzeCode() ==1)
	{
         Progress = "Проверка файла";
		// Вывожу в таблицу о начале сканирования
		Synchronize(&ScanProgres);
		if(logirovanie)
		{
		   LogMessage = " в ApiReportFile()\n" +  VtBase.BaseJesson;
		   Synchronize(&Logirovanie);
		}

		Sleep(10000);
		//Begin while
		while(!Terminated)
		{
			if(!GetStatusConnect())
			{
				Progress ="Ошибка сети. Нет Интернета.";
				// Вывожу в таблицу о начале загрузке.
				ErrorMessage = "Ошибка сети. Нет Интернета.";
				Synchronize(&ErrorLog);
				Synchronize(&ScanProgres);
				return;
			}

			if(Terminated)
			{
			   if(logirovanie)
			   {
				  LogMessage = " в ApiReportFile() в if(Terminated)";
				  Synchronize(&Logirovanie);
			   }
			   Progress ="Проверка остановлена";
			   Synchronize(&ScanProgres);
			   break;
		   }

			if(PostHesh(VtBase.BaseMD5))
			{
			   if(logirovanie)
			   {
				  LogMessage = " в ApiReportFile() в if(PostHesh(VtBase.BaseMD5))\n" +  VtBase.BaseJesson;
				  Synchronize(&Logirovanie);
			   }
			   if(ScanIndyVT.http_response_code == 200)
			   {
				  JSONParseDetect();
				  DateTime();
				  Synchronize(&Otwet);
				  if(logirovanie)
				  {
					 LogMessage = " в ApiReportFile() в if(ScanIndyVT.http_response_code == 200)\n" +  VtBase.BaseJesson;
					 Synchronize(&Logirovanie);
				  }
				  break;
			   }
			}
			else
			   Sleep(10000);
		}//Endwhile

		if(Terminated)
		{
		   if(logirovanie)
		   {
			  LogMessage = " в ApiReportFile() в if(Terminated)";
			  Synchronize(&Logirovanie);
		   }
		   Progress ="Проверка остановлена";
		   Synchronize(&ScanProgres);
		}
	}
}
//+++++++++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::Otwet()
{
   for(int i = 0; i < Form3->ListView2->Items->Count ; i++)
   {
	  if((int)Form3->ListView2->Items->Item[i]->Data == FileNumber)
	  {
		 if(AnsiCompareStr(Form3->ListView2->Items->Item[i]->SubItems->Strings[3].LowerCase(),VtBase.BaseSHA256.LowerCase()) !=0)
		 {
			ErrorMessage = "Не совпадает sha256: у меня = " 
			+ Form3->ListView2->Items->Item[i]->SubItems->Strings[3] 
			+ " На сайте = " + VtBase.BaseSHA256 + "\n"
			+ "md5Tablica = " + Form3->ListView2->Items->Item[i]->SubItems->Strings[2]
			+ " md5VTBase = " + VtBase.BaseMD5
			+ "\n"
			+ VtBase.BaseJesson
			;
			Synchronize(&ErrorLog);
		 }
		 break;
	  }
   }

   TListItem *ListItem;
   ListItem = Form3->ListView3->Items->Add();
   ListItem->Checked = false;
   ListItem->Caption = ExtractFileName(VtBase.BasePatchFileName); // Имя файла
   ListItem->SubItems->Add(VtBase.BasePatchFileName);// пулный путь.
   ListItem->SubItems->Add(VtBase.BaseDetect);// результат сканирования
   ListItem->SubItems->Add(VtBase.BaseSizeFile);// размер
   ListItem->SubItems->Add(VtBase.BaseMD5);// хеш сумма
   ListItem->SubItems->Add(VtBase.BaseSHA256);//Хеш SHA256
   ListItem->SubItems->Add(VtBase.BaseDataProverki);//Дата проверки
   ListItem->SubItems->Add(VtBase.BaseAdress);// ссылка на результат.

   if(VtBase.BaseJesson !="")
   {
	   TJSONObject * ResRescan =(TJSONObject*) TJSONObject::ParseJSONValue(VtBase.BaseJesson);
	   if(ResRescan !=NULL)
	   {
		  TJSONObject *obj = (TJSONObject*)ResRescan->Get("scans")->JsonValue;
		  if(obj !=NULL)
		  {
			 ListItem->Data = new String(obj->ToString());
             VtBase.BaseJesson = obj->ToString();

		  }
	   }
   }
	//Закоментировал, так как сразу из ответа джейсона удаляю лишнее, оставляю только детект антивирусов()
	// Всё из объекта scan. но без scan {Avast{}, drweb{} и т.д.}
   //ListItem->Data = new String(VtBase.BaseJesson);

   for(int i = 0; i < Form3->ListView2->Items->Count ; i++)
   {
	  if( (int)Form3->ListView2->Items->Item[i]->Data == FileNumber)
	  {
		 Form3->ListView2->Items->Item[i]->Delete();
		 break;
	  }
   }

   for(int qwer = 0; qwer< Form3->ListView3->Items->Count; qwer++)
   {
	  if(Form3->ListView3->Items->Item[qwer]->SubItems->Strings[1] =="")
	  {
		 if(Form3->ListView3->Items->Item[qwer]->SubItems->Strings[3].LowerCase()
			 == VtBase.BaseMD5.LowerCase())
		 {
            if(VtBase.BaseDetect !="")
			   Form3->ListView3->Items->Item[qwer]->SubItems->Strings[1] = VtBase.BaseDetect;
			else
			{
			   ErrorMessage = "BaseDetect";
			   Synchronize(&ErrorLog);
			}

			if(VtBase.BaseSHA256 !="")
			   Form3->ListView3->Items->Item[qwer]->SubItems->Strings[4] = VtBase.BaseSHA256;
			else
			{
			   ErrorMessage = "BaseSHA256";
			   Synchronize(&ErrorLog);
			}
			if(VtBase.BaseDataProverki !="")
			   Form3->ListView3->Items->Item[qwer]->SubItems->Strings[5] = VtBase.BaseDataProverki;
			else
			{
			   ErrorMessage = "BaseDataProverki";
			   Synchronize(&ErrorLog);
			}

			if(VtBase.BaseAdress !="")
			   Form3->ListView3->Items->Item[qwer]->SubItems->Strings[6] = VtBase.BaseAdress;
			else
			{
			   ErrorMessage = "BaseAdress";
			   Synchronize(&ErrorLog);
			}
			if(VtBase.BaseJesson !="")
			   Form3->ListView3->Items->Item[qwer]->Data = new String(VtBase.BaseJesson);
			else
			{
			   ErrorMessage = "BaseJesson";
			   Synchronize(&ErrorLog);
			}
		 }
	  }
   }
   if(logirovanie)
   {
	  LogMessage = " в Otwet()" + VtBase.BaseJesson;
	  Synchronize(&Logirovanie);
   }

   if(Form3->AtOptions.BallonHint ==true)
   {
	  Form3->TrayIcon1->Visible = true;
	  Form3->TrayIcon1->BalloonTimeout = 300; // Ставлю на 3 секунды
	  Form3->TrayIcon1->BalloonTitle = "Файл проверен.";
	  Form3->TrayIcon1->BalloonHint = VtBase.BasePatchFileName + " : " + VtBase.BaseDetect;
	  Form3->TrayIcon1->ShowBalloonHint();
	  Form3->TrayIcon1->FreeOnRelease();
   }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++


void __fastcall ScanVTIndy::DateTime()
{
   //Begin перевод из строки в дату.
   TDateTime DataReport;
   FormatSettings.DateSeparator = '.';
   FormatSettings.TimeSeparator = ':';
   FormatSettings.LongDateFormat = "YYYY.MM.DD";
   FormatSettings.ShortDateFormat = "YYYY.MM.DD";
   FormatSettings.ShortTimeFormat = "HH:MM:SS";
   FormatSettings.LongTimeFormat = "HH:MM:SS";
   VtBase.BaseDataProverki = StringReplace(VtBase.BaseDataProverki,"-",".",TReplaceFlags()<<rfReplaceAll);//Обработака строки

   if(TryStrToDateTime(VtBase.BaseDataProverki,DataReport,FormatSettings))
   {
	  VtBase.BaseDataProverki = DateTimeToStr(UTCToLocalTime(DataReport),FormatSettings);
   }
   else
   {
	  ErrorMessage = "Ошибка перекодирование даты в DateTime()" +VtBase.BaseDataProverki;
	  Synchronize(&ErrorLog);
   }
}
//+++++++++++++++++++++++++++++++++++
bool __fastcall ScanVTIndy::DateTimeR(UnicodeString &Date)
{
   //Begin перевод из строки в дату.
   TDateTime DataReport;
   FormatSettings.DateSeparator = '.';
   FormatSettings.TimeSeparator = ':';
   FormatSettings.LongDateFormat = "YYYY.MM.DD";
   FormatSettings.ShortDateFormat = "YYYY.MM.DD";
   FormatSettings.ShortTimeFormat = "HH:MM:SS";
   FormatSettings.LongTimeFormat = "HH:MM:SS";
   Date = StringReplace(Date,"-",".",TReplaceFlags()<<rfReplaceAll);//Обработака строки

   if(TryStrToDateTime (Date,DataReport,FormatSettings))
   {
	  Date = DateTimeToStr(UTCToLocalTime(DataReport),FormatSettings);
	  return true;
   }
   else
   {
	  ErrorMessage = "Ошибка перекодирование даты в DateTimeR()" + Date;
      Synchronize(&ErrorLog);
   }
   return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
TDateTime ScanVTIndy::UTCToLocalTime(TDateTime AValue)
{
	TSystemTime ST1, ST2;
	TTimeZoneInformation TZ;

	// TZ - локальные настройки Windows
	GetTimeZoneInformationForYear(YearOf(AValue), NULL, &TZ);

	// Преобразование TDateTime к WindowsSystemTime
	DateTimeToSystemTime(AValue, ST1);

	// Применение локальных настроек ко времени
	SystemTimeToTzSpecificLocalTime(&TZ, &ST1, &ST2);

	// Приведение SystemTime к TDateTime
	return SystemTimeToDateTime(ST2);
}
//++++++++++++++++++++++++
// Удаляем значение из списка номера потока, что бы было ограничение на количество загрузок а не проверок.
void __fastcall ScanVTIndy::DelSpisokNamePotok()
{
   if(Form3->SpisokNamePotok->IndexOf(PotokNumber)!= -1)
   {
		 Form3->SpisokNamePotok->Delete(Form3->SpisokNamePotok->IndexOf(PotokNumber));
		 if(logirovanie)
		 {
			LogMessage = " в DelSpisokNamePotok()";
			Synchronize(&Logirovanie);
		 }
   }
}
//++++++++++++++++++++++++++++++++++++++++++
// Общая функция загрузки. идет проверка загрузился файл или нет.
// если ответ сервера 204, то проводим повторную отправку файла.
bool __fastcall ScanVTIndy::LoadingFile()
{
	while(!Terminated)
	{
		if(logirovanie)
		 {
			LogMessage = " в LoadingFile() начало загрузки";
			Synchronize(&Logirovanie);
		 }
		if(!GetStatusConnect())
		{
			Progress ="Ошибка сети. Нет интернета.";
			Synchronize(&ScanProgres);
			ErrorMessage = "Ошибка сети. Нет интернета.";
			Synchronize(&ErrorLog);
			return false;
		}

		if(UploadFileVT(VtBase.BasePatchFileName))
		{
			if(logirovanie)
			{
			   LogMessage = " в LoadingFile() в if(UploadFileVT(VtBase.BasePatchFileName))";
			   Synchronize(&Logirovanie);
			}
			if(ScanIndyVT.http_response_code !=204)
			{
			   if(logirovanie)
			   {
				  LogMessage = " в LoadingFile() в if(ScanIndyVT.http_response_code !=204)" + VtBase.BaseJesson;
				  Synchronize(&Logirovanie);
			   }
			   if(logirovanie)
			   {
				  LogMessage = "Загрузка завершена  LoadingFile()";
				  Synchronize(&Logirovanie);
			   }
			   return true;
			}
			else
			   Sleep(2000);
		}
		else
		{
			if(ScanIndyVT.CodeErrorUpload ==0)
			{
			   if(logirovanie)
			   {
				  LogMessage = " в LoadingFile() в if(ScanIndyVT.http_response_code !=204)\n  Смотреть исключение в UploadFileVT\n" + VtBase.BaseJesson;
				  Synchronize(&Logirovanie);
			   }
			   ErrorMessage = "Ошибка загрузки. " + IntToStr(ScanIndyVT.http_response_code);
			   Synchronize(&ErrorLog);
			   break;
			}
			else
			{
				Sleep(1000);// повторная итерация
            }
		}
	}

	if(Terminated)
	{
	  Progress ="Проверка остановлена";
	  Synchronize(&ScanProgres);
	  if(logirovanie)
	  {
		 LogMessage = " в LoadingFile() в if(Terminated)";
		 Synchronize(&Logirovanie);
	  }
	}

	return false;
}
//+++++++++++++++++++++++++++++++++++
bool __fastcall ScanVTIndy::UploadFileVT (UnicodeString file_path)
{
   std::auto_ptr<TIdHTTP> IndyVT (new TIdHTTP(NULL) );
   TIdSSLIOHandlerSocketOpenSSL *ssl = new TIdSSLIOHandlerSocketOpenSSL(NULL);
   TIdSocksInfo *soketInfo = new TIdSocksInfo(NULL);
   bool resultat= false;
   IndyVT->HandleRedirects = 1;
   IndyVT->Request->UserAgent = "Mozilla/5.0 (Windows; U; Windows NT 5.1;en-US; rv:1.8.1.1) Gecko/20061204 Firefox/2.0.0.1";
   IndyVT->OnWorkBegin = InOnWorkBegin;
   IndyVT->OnWork = InWork;
   IndyVT->OnWorkEnd = InWorkEnd;

   if(logirovanie)
   {
	  LogMessage = " в UploadFileVT (UnicodeString file_path)";
	  Synchronize(&Logirovanie);
   }
   if(ScanIndyVT.ProxyVT.ProxiChecked)
   {
	  switch(ScanIndyVT.ProxyVT.Socket)
	  {
		 case 0:
		 {
			if(ScanIndyVT.ProxyVT.Proxy !=" " || ScanIndyVT.ProxyVT.Proxy.Length() !=0)
			{
			   IndyVT->ProxyParams->ProxyServer = ScanIndyVT.ProxyVT.Proxy;
			}

			if(ScanIndyVT.ProxyVT.IpPort !=0)
			   IndyVT->ProxyParams->ProxyPort = ScanIndyVT.ProxyVT.IpPort;

			if(ScanIndyVT.ProxyVT.OptProxiLogin !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   IndyVT->ProxyParams->ProxyUsername = ScanIndyVT.ProxyVT.OptProxiLogin;

			}

			if(ScanIndyVT.ProxyVT.OptProxiPassword !=" " || ScanIndyVT.ProxyVT.OptProxiPassword.Length() != 0)
			{
			   IndyVT->ProxyParams->ProxyPassword = ScanIndyVT.ProxyVT.OptProxiPassword;
			   IndyVT->ProxyParams->BasicAuthentication = true;
			}
			else
			   IndyVT->ProxyParams->BasicAuthentication = false;

			ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
			ssl->SSLOptions->VerifyMode = TIdSSLVerifyModeSet();
			IndyVT->IOHandler = ssl;
			break;
		 }

		 case 1:
		 {
			soketInfo->Version = svSocks4;

			if(ScanIndyVT.ProxyVT.Proxy !=" " || ScanIndyVT.ProxyVT.Proxy.Length() !=0)
			{
			   soketInfo->Host = ScanIndyVT.ProxyVT.Proxy;
			}
			if(ScanIndyVT.ProxyVT.IpPort !=0)
			{
			   IndyVT->ProxyParams->ProxyPort = 0;
			   soketInfo->Port = ScanIndyVT.ProxyVT.IpPort;
			}

			if(ScanIndyVT.ProxyVT.OptProxiLogin !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Username = ScanIndyVT.ProxyVT.OptProxiLogin;
			}


			if(ScanIndyVT.ProxyVT.OptProxiPassword !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Password = ScanIndyVT.ProxyVT.OptProxiLogin;
			   soketInfo->Authentication = saUsernamePassword;
			}
			else
			{
			   soketInfo->Authentication = saNoAuthentication;
			}
			ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
			ssl->SSLOptions->VerifyMode = TIdSSLVerifyModeSet();
			IndyVT->IOHandler = ssl;
			ssl->TransparentProxy = soketInfo;
			break;
		 }
		 case 2:
		 {
			soketInfo->Version = svSocks5;

			if(ScanIndyVT.ProxyVT.Proxy !=" " || ScanIndyVT.ProxyVT.Proxy.Length() !=0)
			{
			   soketInfo->Host = ScanIndyVT.ProxyVT.Proxy;
			}
			if(ScanIndyVT.ProxyVT.IpPort !=0)
			{
			   IndyVT->ProxyParams->ProxyPort = 0;
			   soketInfo->Port = ScanIndyVT.ProxyVT.IpPort;
			}

			if(ScanIndyVT.ProxyVT.OptProxiLogin !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Username = ScanIndyVT.ProxyVT.OptProxiLogin;
			}


			if(ScanIndyVT.ProxyVT.OptProxiPassword !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Password = ScanIndyVT.ProxyVT.OptProxiLogin;
			   soketInfo->Authentication = saUsernamePassword;
			}
			else
			{
			   soketInfo->Authentication = saNoAuthentication;
			}
			ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
			ssl->SSLOptions->VerifyMode = TIdSSLVerifyModeSet();
			IndyVT->IOHandler = ssl;
			ssl->TransparentProxy = soketInfo;
			break;
		 }
	  }
   }
   else
   {
	   ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
	   ssl->SSLOptions->Mode =  sslmClient;
	   IndyVT->IOHandler = ssl;
   }

   std::auto_ptr<TIdMultiPartFormDataStream> PostData(new TIdMultiPartFormDataStream);

   if(logirovanie)
   {
	  LogMessage = "после  std::auto_ptr<TIdMultiPartFormDataStream> PostData(new TIdMultiPartFormDataStream) в UploadFileVT()" ;
	  Synchronize(&Logirovanie);
   }

	TFileStream *fs = new TFileStream(file_path, fmOpenRead | fmShareDenyNone);


	PostData->AddFormField("apikey",ScanIndyVT.ApiKey,"");
	PostData->AddFormField("file","","",fs,file_path);

	if(logirovanie)
	{
	   LogMessage = "после  PostData->AddFormField(\"apikey\",ScanIndyVT.ApiKey,"") в UploadFileVT()" ;
	   Synchronize(&Logirovanie);
	}

	if(logirovanie)
	{
	   LogMessage = "после  PostData->AddFile(Name,file_path) в UploadFileVT()" ;
	   Synchronize(&Logirovanie);
	}
	UnicodeString Url= "https://www.virustotal.com/vtapi/v2/file/scan";

	try
	{
	   IndyVT->Request->ContentType = L"application/x-msdownload";
	   VtBase.BaseJesson = IndyVT->Post(Url, PostData.get());
	   delete fs;

	   if(logirovanie)
	   {
		  LogMessage = "после  VtBase.BaseJesson = IndyVT->Post(Url, PostData.get()) в UploadFileVT()" ;
		  Synchronize(&Logirovanie);
	   }

	   ScanIndyVT.http_response_code = IndyVT->ResponseCode;

	   if(logirovanie)
	   {
		 LogMessage = " в UploadFileVT (UnicodeString file_path) загрузка прошла успешно" ;
		 Synchronize(&Logirovanie);
	   }
	   resultat = true;
	}
	catch(EIdHTTPProtocolException &E)
	{
	   ErrorMessage = "Обработка исключения: (<EIdHTTPProtocolException>) U\"";
	   Synchronize(&ErrorLog);
	   //resultat = false;
	   ScanIndyVT.CodeErrorUpload = 1;
	}

	catch(EIdOSSLConnectError &E)
	{
	   ErrorMessage = "Обработка исключения: (<EIdOSSLConnectError>) U\"";
	   Synchronize(&ErrorLog);
	   //resultat = false;
	   ScanIndyVT.CodeErrorUpload = 1;
	}
	catch (EIdSocketError &E)
	{
	   ErrorMessage = "Обработка исключения: (<EIdSocketError>) U\"";
	   Synchronize(&ErrorLog);
	   //resultat = false;
	   ScanIndyVT.CodeErrorUpload = 1;
	}
	 catch (EIdOpenSSLAPISSLError &E)
	{
	   ErrorMessage = "Обработка исключения: (<EIdOpenSSLAPISSLError>) U\"";
	   Synchronize(&ErrorLog);
	   //resultat = false;
	   ScanIndyVT.CodeErrorUpload = 1;
	}
	catch (EIdConnClosedGracefully &E)
	{
	   ErrorMessage = "Обработка исключения: (<EIdConnClosedGracefully>) U\"";
	   Synchronize(&ErrorLog);
	   ScanIndyVT.CodeErrorUpload = 1;
	}

	catch(EIdException &E)// Другие исключения Indy
	{
		  ErrorMessage = "Ошибка: try (DrIn)U\""+E.Message+"\"" + "Класс ошибки = " +E.ClassName();
		  Synchronize(&ErrorLog);
		  ScanIndyVT.CodeErrorUpload = 0;
		  ScanVTIndy::Terminate();
	}

	catch(Exception &E)   // Другие НЕ Indy исключения
	{
	   ErrorMessage = "Ошибка: try (nouIn) U\""+E.Message+"\""+ "Класс ошибки = " +E.ClassName();
	   Synchronize(&ErrorLog);
	   ScanIndyVT.CodeErrorUpload = 0;
	   ScanVTIndy::Terminate();
	}

	return resultat;
}
//+++++++++++++++++++++++++++++
bool __fastcall ScanVTIndy::ReportVT (UnicodeString chesch)
{
   std::auto_ptr<TIdHTTP> IndyVT (new TIdHTTP(NULL) );
   TIdSSLIOHandlerSocketOpenSSL *ssl = new TIdSSLIOHandlerSocketOpenSSL(NULL);
   TIdSocksInfo *soketInfo = new TIdSocksInfo(NULL);
   bool resultat= false;
   IndyVT->HandleRedirects = 1;
   IndyVT->Request->UserAgent = "Mozilla/5.0 (Windows; U; Windows NT 5.1;en-US; rv:1.8.1.1) Gecko/20061204 Firefox/2.0.0.1";

   if(ScanIndyVT.ProxyVT.ProxiChecked)
   {
	  switch(ScanIndyVT.ProxyVT.Socket)
	  {
		 case 0:
		 {
			if(ScanIndyVT.ProxyVT.Proxy !=" " || ScanIndyVT.ProxyVT.Proxy.Length() !=0)
			{
			   IndyVT->ProxyParams->ProxyServer = ScanIndyVT.ProxyVT.Proxy;
			}

			if(ScanIndyVT.ProxyVT.IpPort !=0)
			   IndyVT->ProxyParams->ProxyPort = ScanIndyVT.ProxyVT.IpPort;

			if(ScanIndyVT.ProxyVT.OptProxiLogin !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   IndyVT->ProxyParams->ProxyUsername = ScanIndyVT.ProxyVT.OptProxiLogin;

			}

			if(ScanIndyVT.ProxyVT.OptProxiPassword !=" " || ScanIndyVT.ProxyVT.OptProxiPassword.Length() != 0)
			{
			   IndyVT->ProxyParams->ProxyPassword = ScanIndyVT.ProxyVT.OptProxiPassword;
			   IndyVT->ProxyParams->BasicAuthentication = true;
			}
			else
			   IndyVT->ProxyParams->BasicAuthentication = false;

			ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
			ssl->SSLOptions->VerifyMode = TIdSSLVerifyModeSet();
			IndyVT->IOHandler = ssl;
			break;
		 }

		 case 1:
		 {
			soketInfo->Version = svSocks4;

			if(ScanIndyVT.ProxyVT.Proxy !=" " || ScanIndyVT.ProxyVT.Proxy.Length() !=0)
			{
			   soketInfo->Host = ScanIndyVT.ProxyVT.Proxy;
			}
			if(ScanIndyVT.ProxyVT.IpPort !=0)
			{
			   IndyVT->ProxyParams->ProxyPort = 0;
			   soketInfo->Port = ScanIndyVT.ProxyVT.IpPort;
			}

			if(ScanIndyVT.ProxyVT.OptProxiLogin !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Username = ScanIndyVT.ProxyVT.OptProxiLogin;
			}


			if(ScanIndyVT.ProxyVT.OptProxiPassword !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Password = ScanIndyVT.ProxyVT.OptProxiLogin;
			   soketInfo->Authentication = saUsernamePassword;
			}
			else
			{
			   soketInfo->Authentication = saNoAuthentication;
			}
			ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
			ssl->SSLOptions->VerifyMode = TIdSSLVerifyModeSet();
			IndyVT->IOHandler = ssl;
			ssl->TransparentProxy = soketInfo;
			break;
		 }
		 case 2:
		 {
			soketInfo->Version = svSocks5;

			if(ScanIndyVT.ProxyVT.Proxy !=" " || ScanIndyVT.ProxyVT.Proxy.Length() !=0)
			{
			   soketInfo->Host = ScanIndyVT.ProxyVT.Proxy;
			}
			if(ScanIndyVT.ProxyVT.IpPort !=0)
			{
			   IndyVT->ProxyParams->ProxyPort = 0;
			   soketInfo->Port = ScanIndyVT.ProxyVT.IpPort;
			}

			if(ScanIndyVT.ProxyVT.OptProxiLogin !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Username = ScanIndyVT.ProxyVT.OptProxiLogin;
			}


			if(ScanIndyVT.ProxyVT.OptProxiPassword !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Password = ScanIndyVT.ProxyVT.OptProxiLogin;
			   soketInfo->Authentication = saUsernamePassword;
			}
			else
			{
			   soketInfo->Authentication = saNoAuthentication;
			}
			ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
			ssl->SSLOptions->VerifyMode = TIdSSLVerifyModeSet();
			IndyVT->IOHandler = ssl;
			ssl->TransparentProxy = soketInfo;
			break;
		 }
	  }
   }
   else
   {
	   ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
	   ssl->SSLOptions->Mode =  sslmClient;
	   IndyVT->IOHandler = ssl;
   }

   IndyVT->Request->Accept = "text/html, */*";


   std::auto_ptr<TIdMultiPartFormDataStream>
					   PostData(new TIdMultiPartFormDataStream);

	PostData->AddFormField("resource",chesch,"");
	PostData->AddFormField("apikey",ScanIndyVT.ApiKey,"");

	UnicodeString Url= "https://www.virustotal.com/vtapi/v2/file/report";
	try
	{
	   IndyVT->Request->Host = "http://www.virustotal.com";
	   IndyVT->Request->ContentType = "application/x-www-form-urlencoded";
	   VtBase.BaseJesson = IndyVT->Post(Url, PostData.get());
	   ScanIndyVT.http_response_code = IndyVT->ResponseCode;

	   if(logirovanie)
	   {
		 LogMessage = " в ReportVT (UnicodeString chesch) прошла успешно" ;
		 Synchronize(&Logirovanie);
	   }
	   resultat = true;
	}
	/*catch(EIdHTTPProtocolException &E)
	{
	   ScanIndyVT.http_response_code = E.ErrorCode;
	   ErrorMessage = "Ошибка: try (In) Rep" + IntToStr(E.ErrorCode) + "Класс ошибки = " +E.ClassName();
	   Synchronize(&ErrorLog);
	   resultat = false;
	   ScanVTIndy::Terminate();
	}*/
	catch(EIdHTTPProtocolException &E)
	{
	   ;
	}

	catch(EIdOSSLConnectError &E)
	{
	   ;
	}
	 catch (EIdSocketError &E)
	{
	   ;
	}
	catch (EIdOpenSSLAPISSLError &E)
	{
	   ;
	}
	catch (EIdConnClosedGracefully &E)
	{
	   ;
	}

	catch(EIdException &E)// Другие исключения Indy
	{
		  ErrorMessage = "Ошибка: try (DrIn)Rep\""+E.Message+"\"" + "Класс ошибки = " +E.ClassName();
		  Synchronize(&ErrorLog);
		  if(logirovanie)
		  {
			 LogMessage = "Ошибка: try (DrIn)Rep\""+E.Message+"\"" + "Класс ошибки = " +E.ClassName();
			 Synchronize(&Logirovanie);
		  }
		  ScanVTIndy::Terminate();
	}

	catch(Exception &E)   // Другие НЕ Indy исключения
	{
	   ErrorMessage = "Ошибка: try (nouInd) Rep\""+E.Message+"\""+ "Класс ошибки = " +E.ClassName();
	   Synchronize(&ErrorLog);
	   if(logirovanie)
	   {
		  LogMessage = "Ошибка: try (nouInd)Rep\""+E.Message+"\"" + "Класс ошибки = " +E.ClassName();
		  Synchronize(&Logirovanie);
	   }
	   ScanVTIndy::Terminate();
	}

	return resultat;
}
///+++++++++++++++++++++++++++++++++++
bool __fastcall ScanVTIndy::RescanVT (UnicodeString chesch)
{
   std::auto_ptr<TIdHTTP> IndyVT (new TIdHTTP(NULL) );
   TIdSSLIOHandlerSocketOpenSSL *ssl = new TIdSSLIOHandlerSocketOpenSSL(NULL);
   TIdSocksInfo *soketInfo = new TIdSocksInfo(NULL);
   bool resultat= false;
   IndyVT->HandleRedirects = 1;
   IndyVT->Request->UserAgent = "Mozilla/5.0 (Windows; U; Windows NT 5.1;en-US; rv:1.8.1.1) Gecko/20061204 Firefox/2.0.0.1";

   if(ScanIndyVT.ProxyVT.ProxiChecked)
   {
	  switch(ScanIndyVT.ProxyVT.Socket)
	  {
		 case 0:
		 {
			if(ScanIndyVT.ProxyVT.Proxy !=" " || ScanIndyVT.ProxyVT.Proxy.Length() !=0)
			{
			   IndyVT->ProxyParams->ProxyServer = ScanIndyVT.ProxyVT.Proxy;
			}

			if(ScanIndyVT.ProxyVT.IpPort !=0)
			   IndyVT->ProxyParams->ProxyPort = ScanIndyVT.ProxyVT.IpPort;

			if(ScanIndyVT.ProxyVT.OptProxiLogin !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   IndyVT->ProxyParams->ProxyUsername = ScanIndyVT.ProxyVT.OptProxiLogin;

			}

			if(ScanIndyVT.ProxyVT.OptProxiPassword !=" " || ScanIndyVT.ProxyVT.OptProxiPassword.Length() != 0)
			{
			   IndyVT->ProxyParams->ProxyPassword = ScanIndyVT.ProxyVT.OptProxiPassword;
			   IndyVT->ProxyParams->BasicAuthentication = true;
			}
			else
			   IndyVT->ProxyParams->BasicAuthentication = false;
			ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
			ssl->SSLOptions->VerifyMode = TIdSSLVerifyModeSet();
			IndyVT->IOHandler = ssl;
			break;
		 }

		 case 1:
		 {
			soketInfo->Version = svSocks4;

			if(ScanIndyVT.ProxyVT.Proxy !=" " || ScanIndyVT.ProxyVT.Proxy.Length() !=0)
			{
				  //IndyVT->ProxyParams->ProxyServer = "";
			   soketInfo->Host = ScanIndyVT.ProxyVT.Proxy;
			}
			if(ScanIndyVT.ProxyVT.IpPort !=0)
			{
			   IndyVT->ProxyParams->ProxyPort = 0;
			   soketInfo->Port = ScanIndyVT.ProxyVT.IpPort;
			}

			if(ScanIndyVT.ProxyVT.OptProxiLogin !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Username = ScanIndyVT.ProxyVT.OptProxiLogin;
			}


			if(ScanIndyVT.ProxyVT.OptProxiPassword !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Password = ScanIndyVT.ProxyVT.OptProxiLogin;
			   soketInfo->Authentication = saUsernamePassword;
			}
			else
			{
			   soketInfo->Authentication = saNoAuthentication;
			}
			ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
			ssl->SSLOptions->VerifyMode = TIdSSLVerifyModeSet();
			IndyVT->IOHandler = ssl;
			ssl->TransparentProxy = soketInfo;
			break;
		 }
		 case 2:
		 {
			soketInfo->Version = svSocks5;

			if(ScanIndyVT.ProxyVT.Proxy !=" " || ScanIndyVT.ProxyVT.Proxy.Length() !=0)
			{
				  //IndyVT->ProxyParams->ProxyServer = "";
			   soketInfo->Host = ScanIndyVT.ProxyVT.Proxy;
			}
			if(ScanIndyVT.ProxyVT.IpPort !=0)
			{
			   IndyVT->ProxyParams->ProxyPort = 0;
			   soketInfo->Port = ScanIndyVT.ProxyVT.IpPort;
			}

			if(ScanIndyVT.ProxyVT.OptProxiLogin !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Username = ScanIndyVT.ProxyVT.OptProxiLogin;
			}


			if(ScanIndyVT.ProxyVT.OptProxiPassword !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Password = ScanIndyVT.ProxyVT.OptProxiLogin;
			   soketInfo->Authentication = saUsernamePassword;
			}
			else
			{
			   soketInfo->Authentication = saNoAuthentication;
			}
			ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
			ssl->SSLOptions->VerifyMode = TIdSSLVerifyModeSet();
			IndyVT->IOHandler = ssl;
			ssl->TransparentProxy = soketInfo;
			break;
		 }
	  }
   }
   else
   {
	   ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
	   ssl->SSLOptions->Mode =  sslmClient;
	   IndyVT->IOHandler = ssl;
   }

   std::auto_ptr<TIdMultiPartFormDataStream>
					   PostData(new TIdMultiPartFormDataStream);

	PostData->AddFormField("resource",chesch,"");
	PostData->AddFormField("apikey",ScanIndyVT.ApiKey,"");
	UnicodeString Url= "https://www.virustotal.com/vtapi/v2/file/rescan";
	try
	{
	   IndyVT->Request->ContentType = L"application/x-msdownload";
	   VtBase.BaseJesson = IndyVT->Post(Url, PostData.get());
	   ScanIndyVT.http_response_code = IndyVT->ResponseCode;
	   if(logirovanie)
	   {
		 LogMessage = " в RescanVT (UnicodeString chesch) прошла успешно" ;
		 Synchronize(&Logirovanie);
	   }
	   resultat = true;
	   resultat = true;
	}
	catch(EIdHTTPProtocolException &E)
	{
	   ;
	}

	catch(EIdOSSLConnectError &E)
	{
	   ;
	}
	catch (EIdSocketError &E)
	{
	   ;
	}
	catch (EIdOpenSSLAPISSLError &E)
	{
	   ;
	}
	catch (EIdConnClosedGracefully &E)
	{
	   ;
	}

	catch(EIdException &E)// Другие исключения Indy
	{
		  ErrorMessage = "Ошибка: try (DrIn)Res\""+E.Message+"\"" + "Класс ошибки = " +E.ClassName();
		  Synchronize(&ErrorLog);
		  if(logirovanie)
		  {
			 LogMessage = "Ошибка: try (DrIn)Res\""+E.Message+"\"" + "Класс ошибки = " +E.ClassName() ;
			 Synchronize(&Logirovanie);
		  }
		  ScanVTIndy::Terminate();
	}

	catch(Exception &E)   // Другие НЕ Indy исключения
	{
	   ErrorMessage = "Ошибка: try (nouIn) Res\""+E.Message+"\""+ "Класс ошибки = " +E.ClassName();
	   Synchronize(&ErrorLog);
	   if(logirovanie)
	   {
		  LogMessage = "Ошибка: try (nouIn) Res\""+E.Message+"\""+ "Класс ошибки = " +E.ClassName();
		  Synchronize(&Logirovanie);
	   }
	   ScanVTIndy::Terminate();
	}

	return resultat;
}
///+++++++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::InOnWorkBegin(TObject *ASender, TWorkMode AWorkMode,__int64 AWorkCountMax)
{
   VTFileSize = AWorkCountMax;
}
//+++++++++++++++++++++++++
void __fastcall ScanVTIndy::InWork(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCount)
{
	TIdHTTP *http = static_cast<TIdHTTP*>(ASender);

	if(Terminated)
	{
	   http->Socket->Close();
	   Progress ="Проверка остановлена";
	   Synchronize(&ScanProgres);
	}
	if(AWorkCount !=0 && VTFileSize !=0)
	{
	   VtZagruzki = (AWorkCount*100)/VTFileSize;
	   Synchronize(&UploadProgress);

	   if(logirovanie)
	  {
		 LogMessage = " в InWork()\n " + IntToStr(VtZagruzki);
		 Synchronize(&Logirovanie);
	  }

	}
}
//++++++++++++++++++
void __fastcall ScanVTIndy::InWorkEnd(TObject *Sender, TWorkMode AWorkMode)
{
	Progress = "Загрузка завершена в InWorkEnd";
	Synchronize(&ScanProgres);
}
//+++++++++++++++++++++++++
void __fastcall ScanVTIndy::UploadProgress()
{
   for(int i = 0; i < Form3->ListView2->Items->Count ; i++)
   {
	  if((int)Form3->ListView2->Items->Item[i]->Data == FileNumber)
	  {
		 Form3->ListView2->Items->Item[i]->SubItems->Strings[4] = VtZagruzki;
		 break;
	  }
   }
}
//++++++++++++++++++++++++++++
// Переношу всё во вторую вкладку "Сканирование"
void __fastcall ScanVTIndy::AtScanBegin()
{
   if(!ScanIndyVT.Proverka)
   {
	  if(logirovanie)
	  {
		 LogMessage = " в AtScanBegin()\n Переношу всё в вкладку сканирование." ;
		 Synchronize(&Logirovanie);
	  }
	  TListItem  *ListItem;
	  ListItem = Form3->ListView2->Items->Add();
	  ListItem->Caption = VtBase.BaseFileName;
	  ListItem->SubItems->Add(VtBase.BasePatchFileName);
	  ListItem->SubItems->Add(VtBase.BaseSizeFile);
	  ListItem->SubItems->Add(VtBase.BaseMD5);
	  ListItem->SubItems->Add(VtBase.BaseSHA256.LowerCase());
	  ListItem->SubItems->Add(Progress);
	  ListItem->SubItems->Add("");
	  ListItem->Data = (void*) FileNumber;
   }
}
//+++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::ErrorLog()
{
   try
   {
	  std::auto_ptr<TStringList> FileError (new TStringList(NULL));

	  if(!FileExists(NameLogError))
	  {
		 int FileCr;
		 FileCr = FileCreate(NameLogError);
		 FileClose(FileCr);
		 Sleep(100);
		 if(logirovanie)
		 {
			LogMessage = " в ErrorLog() в if(!FileExists(NameLogError))" ;
			Synchronize(&Logirovanie);
		 }
	  }
	  if(FileSizeStatic(NameLogError) > 1048576)
	  {
		 TFile::Delete(NameLogError);
		 int FileCr;
		 FileCr = FileCreate(NameLogError);
		 FileClose(FileCr);
		 Sleep(100);

		 if(logirovanie)
		 {
			LogMessage = " в ErrorLog() в if(MyFileSize2(NameLogError) > 1048576)" ;
			Synchronize(&Logirovanie);
		 }
	  }

	  FileError->LoadFromFile(NameLogError);


	  FileError->Add("+++"+ TOSVersion::Name +"+++"+TOSVersion::Major + "+++"+ TOSVersion::Minor +"+++"+ TOSVersion::Build +"+++" +TOSVersion::ServicePackMajor+ "+++" + "+++" +String(MY_VERSION)+ "++++" + String(MY_DATE)+ " +++");
	  FileError->Add(FormatDateTime("YYYY.MM.DD",Date())+ " " + FormatDateTime("HH:NN:SS",Time()));
	  FileError->Add(VtBase.BasePatchFileName);
	  //заношу в логировние имя лога, что бы легче искать.
	  if(logirovanie)
		 FileError->Add("LogTMP\\"+VtBase.BaseFileName + TimeLogirovanie + ".log");

	  FileError->Add(ErrorMessage);
	  FileError->SaveToFile(NameLogError, TEncoding::UTF8);
   }
   catch(EFOpenError &e)
   {
	  ShowMessage("No open file!");
	  if(logirovanie)
	  {
		 LogMessage = " в ErrorLog() в catch(EFOpenError &e)" ;
		 Synchronize(&Logirovanie);
	  }
   }
}
//---------------------------------------------------------------------------
void __fastcall ScanVTIndy::Logirovanie()
{
   try
   {
	  std::auto_ptr<TStringList> FileLog (new TStringList(NULL));

	  if(!FileExists(ExtractFilePath(ParamStr(0)) + "LogTMP\\"+VtBase.BaseFileName+ TimeLogirovanie + ".log"))
	  {
		 int FileCr;
		 if(!TDirectory::Exists(ExtractFilePath(ParamStr(0)) + "LogTMP",false))
			TDirectory::CreateDirectory(ExtractFilePath(ParamStr(0)) + "LogTMP");

		 FileCr = FileCreate(ExtractFilePath(ParamStr(0)) + "LogTMP\\"+VtBase.BaseFileName+ TimeLogirovanie + ".log");
		 FileClose(FileCr);
	  }
	  FileLog->LoadFromFile(ExtractFilePath(ParamStr(0)) + "LogTMP\\"+VtBase.BaseFileName+ TimeLogirovanie + ".log");

	  FileLog->Add( IntToStr(FileNumber) + "+++"+ TOSVersion::Name +"+++"+TOSVersion::Major + "+++"+ TOSVersion::Minor +"+++"+ TOSVersion::Build +"+++" +TOSVersion::ServicePackMajor+ "+++" + "+++" +String(MY_VERSION)+ "++++" + String(MY_DATE)+ " +++");
	  FileLog->Add(FormatDateTime("YYYY.MM.DD",Date())+ " " + FormatDateTime("HH:NN:SS",Time()));
	  FileLog->Add(LogMessage);
	  FileLog->SaveToFile(ExtractFilePath(ParamStr(0)) + "LogTMP\\"+VtBase.BaseFileName + TimeLogirovanie + ".log", TEncoding::UTF8);
   }
   catch(EFOpenError &e)
   {
	  ShowMessage("No open file Error.log!" + e.ToString());

   }

}



