//---------------------------------------------------------------------------

#include <System.hpp>
#include <memory>
#include <DBXJSON.hpp>
#include <DateUtils.hpp>

#include <IdComponent.hpp>


#pragma hdrstop

#include "Aitotal1.h"
#include "Aitotal.cpp"
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

   ScanFiles();

}
//+++++++++++++++++++++++++++++++++++++++++++++
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


//Общии функции не зависимые от версий VT
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
//+++++++++++++++++++++++++++++++++++
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++
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
		//И ссылка для всех одинаковая. заношу при добавление данных в таблицу2
		VtBase.BaseAdress = "https://www.virustotal.com/gui/file/" +VtBase.BaseSHA256.LowerCase()+ "/detection";
		ListItem->Caption = VtBase.BaseFileName;
		ListItem->SubItems->Add(VtBase.BasePatchFileName);
		ListItem->SubItems->Add(VtBase.BaseSizeFile);
		ListItem->SubItems->Add(VtBase.BaseMD5);
		ListItem->SubItems->Add(VtBase.BaseSHA256.LowerCase());
		ListItem->SubItems->Add(Progress);
		ListItem->SubItems->Add(VtBase.BaseAdress);
		ListItem->Data = (void*) FileNumber;
	}
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++
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
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
//+++++++++++++++++++++++++++++++++++++++++++++++++
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
//++++++++++++++++++++++++++++++++++++++++++++

// Функции для 3 версии


bool __fastcall ScanVTIndy::SearchHesh(UnicodeString Hesh)
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
			LogMessage = "в SearchHesh(UnicodeString Hesh) Проверка остановлена";
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
			LogMessage = "в SearchHesh(UnicodeString Hesh) \n Ошибка сети.";
			Synchronize(&Logirovanie);
		 }
		 break;
	  }
      if(logirovanie)
	  {
			LogMessage = "в SearchHesh(UnicodeString Hesh) \n перед if(VTFilesID(VtBase.BaseMD5))";
			Synchronize(&Logirovanie);
	  }

	  if(VTFilesID(VtBase.BaseMD5))
	  {
		 if(logirovanie)
		 {
			LogMessage = "в SearchHesh(UnicodeString Hesh) \n ReportVT == true.";
			Synchronize(&Logirovanie);
		 }
		 if(ScanIndyVT.http_response_code == 404)
		 {
            if(logirovanie)
			{
			   LogMessage = "в SearchHesh(UnicodeString Hesh) \n ScanIndyVT.http_response_code == 404";
			   Synchronize(&Logirovanie);
			}
			scan = false;
			break;
		 }

		 if(ScanIndyVT.http_response_code == 200)
		 {
			if(logirovanie)
			{
			   LogMessage = "в SearchHesh(UnicodeString Hesh) \n ScanIndyVT.http_response_code == 200";
			   Synchronize(&Logirovanie);
			}
			scan = true;
			break;
         }

		 //посмотреть код ответа при исчерпании лимита.
		 //else
			//Sleep(2000);
	  }
	  else
	  {
        if(logirovanie)
			{
			   LogMessage = "в SearchHesh(UnicodeString Hesh) \n else (VTFilesID(VtBase.BaseMD5))";
			   Synchronize(&Logirovanie);
			}

		break;
	  }
   }
   if(Terminated)
   {
	  Progress ="Проверка остановлена";
	  Synchronize(&ScanProgres);
	  if(logirovanie)
	  {
		 LogMessage = "в SearchHesh(UnicodeString Hesh) Проверка остановлена";
		 Synchronize(&Logirovanie);
	  }
   }
   return scan;
}
//+++++++++++++++++++++++++++++++++++++
//Загрузка файла
bool __fastcall ScanVTIndy::VTFiles(UnicodeString FileName)
{
	std::auto_ptr<TIdHTTP> IndyVT (new TIdHTTP(NULL) );
	std::auto_ptr<TIdSSLIOHandlerSocketOpenSSL> ssl ( new TIdSSLIOHandlerSocketOpenSSL(NULL));
	std::auto_ptr<TIdSocksInfo> soketInfo (new TIdSocksInfo(NULL));
	bool resultat= false;
	IndyVT->HandleRedirects = 1;
	IndyVT->Request->ContentType = L"application/x-msdownload";
	IndyVT->Request->Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
	IndyVT->Request->UserAgent = "Mozilla/5.0 (Windows; U; Windows NT 5.1;en-US; rv:1.8.1.1) Gecko/20061204 Firefox/2.0.0.1";
	IndyVT->Request->CustomHeaders->AddValue("x-apikey", ScanIndyVT.ApiKey);
    IndyVT->OnWorkBegin = InOnWorkBegin;
	IndyVT->OnWork = InWork;
	IndyVT->OnWorkEnd = InWorkEnd;

	if(ScanIndyVT.ProxyVT.ProxiChecked)
	{
		switch(ScanIndyVT.ProxyVT.Socket)
		{
			case 0:
			{
				soketInfo->Version = svNoSocks;
				break;
			}

			case 1:
			{
				soketInfo->Version = svSocks4;
				break;
			}
			case 2:
			{
				soketInfo->Version = svSocks5;
				break;
			}
		}
		if(ScanIndyVT.ProxyVT.Proxy !=" " || ScanIndyVT.ProxyVT.Proxy.Length() !=0)
		{
			soketInfo->Host = ScanIndyVT.ProxyVT.Proxy;
		}

		if(ScanIndyVT.ProxyVT.IpPort !=0)
			soketInfo->Port = ScanIndyVT.ProxyVT.IpPort;

		if(ScanIndyVT.ProxyVT.OptProxiLogin !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			soketInfo->Username = ScanIndyVT.ProxyVT.OptProxiLogin;

		if(ScanIndyVT.ProxyVT.OptProxiPassword !=" " || ScanIndyVT.ProxyVT.OptProxiPassword.Length() != 0)
		{
			soketInfo->Password = ScanIndyVT.ProxyVT.OptProxiPassword;
			soketInfo->Authentication = saUsernamePassword;
		}
		else
			soketInfo->Authentication = saNoAuthentication;
	}
	ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
	ssl->SSLOptions->VerifyMode = TIdSSLVerifyModeSet();
	ssl->SSLOptions->Mode =  sslmClient;
	IndyVT->IOHandler = ssl.get();
	ssl->TransparentProxy = soketInfo.get();


	std::auto_ptr<TIdMultiPartFormDataStream> PostData(new TIdMultiPartFormDataStream);



	std::auto_ptr<TFileStream> fs (new TFileStream(FileName, fmOpenRead | fmShareDenyNone));

	PostData->AddFormField("file","","",fs.get(),FileName);

	try
	{
		VtBase.BaseJesson = IndyVT->Post("https://www.virustotal.com/api/v3/files",PostData.get());
		ScanIndyVT.http_response_code = IndyVT->ResponseCode;
		if(logirovanie)
		{
			LogMessage = " в VTFiles (UnicodeString FileName) прошла успешно. response code = " + ScanIndyVT.http_response_code + String("\n") + VtBase.BaseJesson;
			Synchronize(&Logirovanie);
		}
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
bool __fastcall ScanVTIndy::VTFilesID (UnicodeString chesch)
{
	std::auto_ptr<TIdHTTP> IndyVT (new TIdHTTP(NULL) );
	std::auto_ptr<TIdSSLIOHandlerSocketOpenSSL> ssl ( new TIdSSLIOHandlerSocketOpenSSL(NULL));
	std::auto_ptr<TIdSocksInfo> soketInfo (new TIdSocksInfo(NULL));
	bool resultat= false;
	IndyVT->HandleRedirects = 1;
	IndyVT->Request->ContentType = L"application/x-msdownload";
	IndyVT->Request->Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
	IndyVT->Request->UserAgent = "Mozilla/5.0 (Windows; U; Windows NT 5.1;en-US; rv:1.8.1.1) Gecko/20061204 Firefox/2.0.0.1";
	IndyVT->Request->CustomHeaders->AddValue("x-apikey", ScanIndyVT.ApiKey);

	if(ScanIndyVT.ProxyVT.ProxiChecked)
	{
		switch(ScanIndyVT.ProxyVT.Socket)
		{
			case 0:
			{
				soketInfo->Version = svNoSocks;
				break;
			}

			case 1:
			{
				soketInfo->Version = svSocks4;
				break;
			}
			case 2:
			{
				soketInfo->Version = svSocks5;
				break;
			}
		}
		if(ScanIndyVT.ProxyVT.Proxy !=" " || ScanIndyVT.ProxyVT.Proxy.Length() !=0)
		{
			soketInfo->Host = ScanIndyVT.ProxyVT.Proxy;
		}

		if(ScanIndyVT.ProxyVT.IpPort !=0)
			soketInfo->Port = ScanIndyVT.ProxyVT.IpPort;

		if(ScanIndyVT.ProxyVT.OptProxiLogin !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			soketInfo->Username = ScanIndyVT.ProxyVT.OptProxiLogin;

		if(ScanIndyVT.ProxyVT.OptProxiPassword !=" " || ScanIndyVT.ProxyVT.OptProxiPassword.Length() != 0)
		{
			soketInfo->Password = ScanIndyVT.ProxyVT.OptProxiPassword;
			soketInfo->Authentication = saUsernamePassword;
		}
		else
			soketInfo->Authentication = saNoAuthentication;
	}
	ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
	ssl->SSLOptions->VerifyMode = TIdSSLVerifyModeSet();
	ssl->SSLOptions->Mode =  sslmClient;
	IndyVT->IOHandler = ssl.get();
	ssl->TransparentProxy = soketInfo.get();

	try
	{
		VtBase.BaseJesson = IndyVT->Get("https://www.virustotal.com/api/v3/files/"+ chesch);
		ScanIndyVT.http_response_code = IndyVT->ResponseCode;
		if(logirovanie)
		{
			LogMessage = " в VTFilesID (UnicodeString chesch) прошла успешно. response code = " + ScanIndyVT.http_response_code + String("\n") + VtBase.BaseJesson;
			Synchronize(&Logirovanie);
		}
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
bool __fastcall ScanVTIndy::VTAnalyse (UnicodeString chesch)
{
	std::auto_ptr<TIdHTTP> IndyVT (new TIdHTTP(NULL) );
	std::auto_ptr<TIdSSLIOHandlerSocketOpenSSL> ssl ( new TIdSSLIOHandlerSocketOpenSSL(NULL));
	std::auto_ptr<TIdSocksInfo> soketInfo (new TIdSocksInfo(NULL));
	bool resultat= false;
	IndyVT->HandleRedirects = 1;
	IndyVT->Request->ContentType = L"application/x-msdownload";
	IndyVT->Request->Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
	IndyVT->Request->UserAgent = "Mozilla/5.0 (Windows; U; Windows NT 5.1;en-US; rv:1.8.1.1) Gecko/20061204 Firefox/2.0.0.1";
	IndyVT->Request->CustomHeaders->AddValue("x-apikey", ScanIndyVT.ApiKey);

	if(ScanIndyVT.ProxyVT.ProxiChecked)
	{
		switch(ScanIndyVT.ProxyVT.Socket)
		{
			case 0:
			{
				soketInfo->Version = svNoSocks;
				break;
			}

			case 1:
			{
				soketInfo->Version = svSocks4;
				break;
			}
			case 2:
			{
				soketInfo->Version = svSocks5;
				break;
			}
		}
		if(ScanIndyVT.ProxyVT.Proxy !=" " || ScanIndyVT.ProxyVT.Proxy.Length() !=0)
		{
			soketInfo->Host = ScanIndyVT.ProxyVT.Proxy;
		}

		if(ScanIndyVT.ProxyVT.IpPort !=0)
			soketInfo->Port = ScanIndyVT.ProxyVT.IpPort;

		if(ScanIndyVT.ProxyVT.OptProxiLogin !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			soketInfo->Username = ScanIndyVT.ProxyVT.OptProxiLogin;

		if(ScanIndyVT.ProxyVT.OptProxiPassword !=" " || ScanIndyVT.ProxyVT.OptProxiPassword.Length() != 0)
		{
			soketInfo->Password = ScanIndyVT.ProxyVT.OptProxiPassword;
			soketInfo->Authentication = saUsernamePassword;
		}
		else
			soketInfo->Authentication = saNoAuthentication;
	}
	ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
	ssl->SSLOptions->VerifyMode = TIdSSLVerifyModeSet();
	ssl->SSLOptions->Mode =  sslmClient;
	IndyVT->IOHandler = ssl.get();
	ssl->TransparentProxy = soketInfo.get();


	std::auto_ptr<TIdMultiPartFormDataStream> PostData(new TIdMultiPartFormDataStream);


	UnicodeString Url= "https://www.virustotal.com/api/v3/files/" + chesch + "/analyse";

	try
	{
		VtBase.BaseJesson = IndyVT->Post(Url,PostData.get());
		ScanIndyVT.http_response_code = IndyVT->ResponseCode;
		if(logirovanie)
		{
			LogMessage = " в VTFilesID (UnicodeString chesch) прошла успешно. response code = " + ScanIndyVT.http_response_code + String("\n") + VtBase.BaseJesson;
			Synchronize(&Logirovanie);
		}
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
bool __fastcall ScanVTIndy::VTAnalysasID ()
{
	std::auto_ptr<TIdHTTP> IndyVT (new TIdHTTP(NULL) );
	std::auto_ptr<TIdSSLIOHandlerSocketOpenSSL> ssl ( new TIdSSLIOHandlerSocketOpenSSL(NULL));
	std::auto_ptr<TIdSocksInfo> soketInfo (new TIdSocksInfo(NULL));
	bool resultat= false;
	IndyVT->HandleRedirects = 1;
	IndyVT->Request->ContentType = L"application/x-msdownload";
	IndyVT->Request->Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
	IndyVT->Request->UserAgent = "Mozilla/5.0 (Windows; U; Windows NT 5.1;en-US; rv:1.8.1.1) Gecko/20061204 Firefox/2.0.0.1";
	IndyVT->Request->CustomHeaders->AddValue("x-apikey", ScanIndyVT.ApiKey);

	if(ScanIndyVT.ProxyVT.ProxiChecked)
	{
		switch(ScanIndyVT.ProxyVT.Socket)
		{
			case 0:
			{
				soketInfo->Version = svNoSocks;
				break;
			}

			case 1:
			{
				soketInfo->Version = svSocks4;
				break;
			}
			case 2:
			{
				soketInfo->Version = svSocks5;
				break;
			}
		}
		if(ScanIndyVT.ProxyVT.Proxy !=" " || ScanIndyVT.ProxyVT.Proxy.Length() !=0)
		{
			soketInfo->Host = ScanIndyVT.ProxyVT.Proxy;
		}

		if(ScanIndyVT.ProxyVT.IpPort !=0)
			soketInfo->Port = ScanIndyVT.ProxyVT.IpPort;

		if(ScanIndyVT.ProxyVT.OptProxiLogin !=" " || ScanIndyVT.ProxyVT.OptProxiLogin.Length() != 0)
			soketInfo->Username = ScanIndyVT.ProxyVT.OptProxiLogin;

		if(ScanIndyVT.ProxyVT.OptProxiPassword !=" " || ScanIndyVT.ProxyVT.OptProxiPassword.Length() != 0)
		{
			soketInfo->Password = ScanIndyVT.ProxyVT.OptProxiPassword;
			soketInfo->Authentication = saUsernamePassword;
		}
		else
			soketInfo->Authentication = saNoAuthentication;
	}
	ssl->SSLOptions->SSLVersions = TIdSSLVersions() << sslvSSLv23;
	ssl->SSLOptions->VerifyMode = TIdSSLVerifyModeSet();
	ssl->SSLOptions->Mode =  sslmClient;
	IndyVT->IOHandler = ssl.get();
	ssl->TransparentProxy = soketInfo.get();


	try
	{
		VtBase.BaseJesson = IndyVT->Get("https://www.virustotal.com/api/v3/analyses/"+ VtBase.BaseIDVT);
		ScanIndyVT.http_response_code = IndyVT->ResponseCode;
		if(logirovanie)
		{
			LogMessage = " в VTAnalysasID () прошла успешно. response code = " + ScanIndyVT.http_response_code + String("\n") + VtBase.BaseJesson;
			Synchronize(&Logirovanie);
		}
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
void __fastcall ScanVTIndy::ScanFiles()
{
   VtBase.BaseDataProverki = "";
   Progress = "Ищем хеш в базе VT";
   // Вывожу в таблицу о начале сканирования
   Synchronize(&AtScanBegin);
   if(logirovanie)
   {
	  LogMessage = "Вывожу в таблицу о начале сканирования \n ScanFiles()";
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
		   LogMessage = "Ошибка сети ScanFiles() ";
		   Synchronize(&Logirovanie);
		}
		break;
	 }
	 bool proverka =SearchHesh(VtBase.BaseMD5.c_str());
	 if (!proverka)
	 {
		if(logirovanie)
		{
		   LogMessage = "bool proverka == false \n Файл не проверялся";
		   Synchronize(&Logirovanie);
		}
		Synchronize(&FileUploadBool);

		if(FileUpload)
		{
			;
		}
        if(logirovanie)
		{
			  LogMessage = "Начало загрузки файла ScanFiles()";
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
				LogMessage = "Ошибка проверки. Размер файла превышает 32Мб ScanFiles()";
				Synchronize(&Logirovanie);
			}
			Synchronize(&OtwetErrorSizeFile);
			break;
		}

		VTFiles(VtBase.BasePatchFileName);
		JSONParseAnalysisID();

		Progress ="Идет анализ файла";
		Synchronize(&ScanProgres);
		while(!Terminated)
		{
			Sleep(15000);
			if(!GetStatusConnect())
			{
				Progress ="Ошибка сети. Нет Интернета.";
				// Вывожу в таблицу о начале загрузке.
				Synchronize(&ScanProgres);
				ErrorMessage = "Ошибка сети. Нет Интернета.";
				Synchronize(&ErrorLog);
				return;
			}
			VTAnalysasID();

			if(logirovanie)
			{
				LogMessage = " в ScanFiles() VTAnalysasID()";
				Synchronize(&Logirovanie);
			}
			if(JSONParseAnalysisStatus())
			{
				if(VtBase.BaseStatusAnalyzeID == "completed")
				{
					if(logirovanie)
					{
						LogMessage = " в ScanFiles() в if(VtBase.BaseStatusAnalyzeID == \"completed\")\n" + VtBase.BaseJesson;
						Synchronize(&Logirovanie);
					}

					VTJSONParseDetectAnalysasID();
					if(logirovanie)
					{
						LogMessage = " в ScanFiles() в if(VtBase.BaseStatusAnalyzeID == \"completed\")  После VTJSONParseDetectAnalysasID()\n" + VtBase.BaseJesson;
						Synchronize(&Logirovanie);
					}
					Synchronize(&VIOtwet);
					break;
				}
			}

		}
		break;
	 }

	 if(proverka)
	 {
		Synchronize(&DelSpisokNamePotok);
		// Если Рескан ==false
		if(!ScanIndyVT.Rescan)
		{
			if(logirovanie)
			{
			   LogMessage = "Рескан рескан == фалсе \n после if(!ScanIndyVT.Rescan) ScanFiles() ";
			   Synchronize(&Logirovanie);
			}

			ReScanFalse();

			if(logirovanie)
			{
			   LogMessage = "после ReScanFalse(); ScanFiles() ";
			   Synchronize(&Logirovanie);
			}
			break;
		}
        // Если Рескан ==true
		else
		{
			if(logirovanie)
			{
			   LogMessage = "после // Если Рескан ==true \n else ScanFiles() ";
			   Synchronize(&Logirovanie);
			}
			//Закоментировал, так как ссылки сейчас нет. только по IP сылка.
			//JSONGetPermalink();
			if(logirovanie)
			{
			   LogMessage = "после JSONGetPermalink(); ScanFiles() ";
			   Synchronize(&Logirovanie);
			}
			//Synchronize(&ZapisPermalinkLV);

			ReScanVT();
			if(logirovanie)
			{
			   LogMessage = "после ApiReScan(); ScanFiles() ";
			   Synchronize(&Logirovanie);
			}
			break;
		}

	 }

	 //break;
   }

   if(Terminated)
   {
	  Progress ="Проверка остановлена";
	  Synchronize(&ScanProgres);
	  if(logirovanie)
	  {
		 LogMessage = "Проверка остановлена ScanFiles() ";
		 Synchronize(&Logirovanie);
	  }
   }

}
//+++++++++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::ReScanFalse()
{
   Progress ="ReScan false. Обработка результата.";

   if(logirovanie)
	{
	   LogMessage = "в ReScanFalse() " ;
	   Synchronize(&Logirovanie);
	}
	Synchronize(&ScanProgres);

	VTJSONParseDetect();
	if(logirovanie)
	{
		  LogMessage = " в ReScanFalse() после JSONParseDetect();";
		  Synchronize(&Logirovanie);
	}

	Synchronize(&VIOtwet);
}
//++++++++++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::VTJSONParseDetect()
{
	try
	{
		std::auto_ptr<TJSONObject> ResRescan(static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(VtBase.BaseJesson)));
		if(logirovanie)
		{
			LogMessage = " в VTJSONParseDetect()\n" +  ResRescan->ToString();
			Synchronize(&Logirovanie);
		}

		TJSONObject *obj = static_cast<TJSONObject*>(ResRescan->Get("data")->JsonValue);
		TJSONObject *qwe = static_cast<TJSONObject*>(obj->Get("attributes")->JsonValue);
		//TJSONObject *asd = static_cast<TJSONObject*>(obj->Get("links")->JsonValue);
		TJSONObject *zxc = static_cast<TJSONObject*>(qwe->Get("last_analysis_stats")->JsonValue);
		TJSONObject *result = static_cast<TJSONObject*>(qwe->Get("last_analysis_results")->JsonValue);
		try
		{
			VtBase.BaseDetect = zxc->Get("malicious")->JsonValue->Value() + "/" +zxc->Get("undetected")->JsonValue->Value();
			__int64 as=qwe->Get("last_modification_date")->JsonValue->Value().ToInt();
			VtBase.BaseDataProverki =FormatDateTime("YYYY.MM.DD HH:NN:SS",UnixToDateTime(as,false));
			VtBase.BaseSHA256 = qwe->Get("sha256")->JsonValue->Value();
			VtBase.BaseJesson = result->ToString();

			if(logirovanie)
			{
				LogMessage = " в VTJSONParseDetect()\n" +  VtBase.BaseDataProverki + VtBase.BaseSHA256 + VtBase.BaseAdress;

				Synchronize(&Logirovanie);
			}
		}
		catch(Exception &E)
		{
			if(logirovanie)
			{
				LogMessage = " try{ TJSONObject *obj\n" + E.Message + String("\n") + obj->ToString();
				Synchronize(&Logirovanie);
			}
			ErrorMessage = "error: VTJSONParseDetect() catch(Exception &E)\n JSON = " +VtBase.BaseJesson+ "\n http_response_code = " + ScanIndyVT.http_response_code;
			Synchronize(&ErrorLog);
		}

	}
	catch(Exception &T)
	{
		if(logirovanie)
		{
			LogMessage = " в VTJSONParseDetect() catch(Exception &T)\n" + T.Message;
			Synchronize(&Logirovanie);
		}
		ErrorMessage = "error: VTJSONParseDetect() catch(Exception &T)\n JSON = " +VtBase.BaseJesson+ "\n http_response_code = " + ScanIndyVT.http_response_code;
		Synchronize(&ErrorLog);
	}
}
//++++++++++++++++
void __fastcall ScanVTIndy::VIOtwet()
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

   //Закоментировал пока не прописал к новому детекту
   /*if(VtBase.BaseJesson !="")
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
   }*/
   ListItem->Data = new String(VtBase.BaseJesson);

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
	  LogMessage = " в VIOtwet()\n" + VtBase.BaseJesson;
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
//Извлекаю из ответа ID для Get analyses id
bool __fastcall ScanVTIndy::JSONParseAnalysisID()
{
   UnicodeString a="";
   bool rezult = false;
   try
   {
	  std::auto_ptr<TJSONObject> ResRescan(static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(VtBase.BaseJesson)));
	  if(logirovanie)
	  {
		 LogMessage = " в JSONParseAnalysisID() try std::auto_ptr<TJSONObject> ResRescan \n" +  ResRescan->ToString();
		 Synchronize(&Logirovanie);
	  }

	 TJSONObject *obj = static_cast<TJSONObject*>(ResRescan->Get("data")->JsonValue);

	 try
	 {
		VtBase.BaseIDVT = obj->Get("id")->JsonValue->Value();
		rezult = true;
		if(logirovanie)
		{
		 LogMessage = " в JSONParseAnalysisID() try obj->Get(\"id\")->JsonValue->Value();\n" +  VtBase.BaseIDVT;

		 Synchronize(&Logirovanie);
		}
	 }
	 catch(Exception &E)
	{
		if(logirovanie)
		{
			LogMessage = " JSONParseAnalysisID() catch TJSONObject *obj\n" + E.Message + String("\n") + obj->ToString();
			Synchronize(&Logirovanie);
		}
	   ErrorMessage = "error: JSONParseAnalysisID\n JSON = " +VtBase.BaseJesson+ "\n NameFile = " + VtBase.BaseFileName + "\n Error = "+ E.Message;
	   Synchronize(&ErrorLog);
	}

   }
   catch(...)
   {
	   if(logirovanie)
	  {
		 LogMessage = " в VTJSONParseDetect() catch(...)\n" +a;
		 Synchronize(&Logirovanie);
	  }
	   ErrorMessage = "error: JSONParseAnalysisID\n JSON = " +VtBase.BaseJesson+ "\n NameFile = " + VtBase.BaseFileName;// + "\n Error = " +;
	   Synchronize(&ErrorLog);
   }
   return rezult;

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Извлекаем статус проверки, в VTAnalysasID()
   // статусы
   // "completed" - завершен.
   // "queued" в ожидание.
   // "in-progress" в процессе.
bool __fastcall ScanVTIndy::JSONParseAnalysisStatus()
{
   UnicodeString a="";
   bool rezult = false;
   try
   {
	  std::auto_ptr<TJSONObject> ResRescan(static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(VtBase.BaseJesson)));
	  if(logirovanie)
	  {
		 LogMessage = " в JSONParseAnalysisStatus() try std::auto_ptr<TJSONObject> ResRescan \n" +  ResRescan->ToString();
		 Synchronize(&Logirovanie);
	  }

	 TJSONObject *obj = static_cast<TJSONObject*>(ResRescan->Get("data")->JsonValue);
	 TJSONObject *asd = static_cast<TJSONObject*>(obj->Get("attributes")->JsonValue);

	 try
	 {
		VtBase.BaseStatusAnalyzeID = asd->Get("status")->JsonValue->Value();
		rezult = true;
		if(logirovanie)
		{
		 LogMessage = " JSONParseAnalysisStatus() try asd->Get(\"status\")\n" +  VtBase.BaseStatusAnalyzeID;

		 Synchronize(&Logirovanie);
		}
	 }
	 catch(Exception &E)
	{
		if(logirovanie)
		{
			LogMessage = " JSONParseAnalysisStatus() catch TJSONObject *asd\n" + E.Message + String("\n") + asd->ToString();
			Synchronize(&Logirovanie);
		}
	   //ErrorMessage = "error: JSONParseDetect\n JSON = " +VtBase.BaseJesson+ "\n http_response_code = " + ScanIndyVT.http_response_code;
	   //Synchronize(&ErrorLog);
	}

   }
   catch(Exception &E)
   {
	   if(logirovanie)
	  {
		 LogMessage = " в VTJSONParseDetect() catch(...)\n" + E.ToString();
		 Synchronize(&Logirovanie);
	  }
	   //ErrorMessage = "error: JSONParseDetect\n JSON = " +VtBase.BaseJesson+ "\n http_response_code = " + ScanIndyVT.http_response_code;
	   //Synchronize(&ErrorLog);
   }
   return rezult;

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::VTJSONParseDetectAnalysasID()
{
   UnicodeString a="";
   try
   {
	  std::auto_ptr<TJSONObject> ResRescan(static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(VtBase.BaseJesson)));
	  if(logirovanie)
	  {
		 LogMessage = " в VTJSONParseDetect()\n" +  ResRescan->ToString();
		 Synchronize(&Logirovanie);
	  }

	 TJSONObject *obj = static_cast<TJSONObject*>(ResRescan->Get("data")->JsonValue);
	 TJSONObject *qwe = static_cast<TJSONObject*>(obj->Get("attributes")->JsonValue);
	 TJSONObject *zxc = static_cast<TJSONObject*>(qwe->Get("stats")->JsonValue);

	 TJSONObject *asd = static_cast<TJSONObject*>(qwe->Get("results")->JsonValue);

	 try{
		VtBase.BaseDetect = zxc->Get("malicious")->JsonValue->Value() + "/" + IntToStr(zxc->Get("malicious")->JsonValue->Value().ToInt()+ zxc->Get("undetected")->JsonValue->Value().ToInt());
		__int64 as=qwe->Get("date")->JsonValue->Value().ToInt();
		VtBase.BaseDataProverki = FormatDateTime("YYYY.MM.DD HH:NN:SS",UnixToDateTime(as,false));
		VtBase.BaseJesson = asd->ToString();
		if(logirovanie)
		{
		 LogMessage = " в VTJSONParseDetect()\n" +  VtBase.BaseDataProverki + VtBase.BaseSHA256 + VtBase.BaseAdress;

		 Synchronize(&Logirovanie);
		}
	 }
	 catch(Exception &E)
	{
		if(logirovanie)
		{
			LogMessage = " try{ TJSONObject *obj\n" + E.Message + String("\n") + obj->ToString();
			Synchronize(&Logirovanie);
		}
	   //ErrorMessage = "error: JSONParseDetect\n JSON = " +VtBase.BaseJesson+ "\n http_response_code = " + ScanIndyVT.http_response_code;
	   //Synchronize(&ErrorLog);
	}

   }
   catch(...)
   {
	   if(logirovanie)
	  {
		 LogMessage = " в VTJSONParseDetect() catch(...)\n" +a;
		 Synchronize(&Logirovanie);
	  }
	   //ErrorMessage = "error: JSONParseDetect\n JSON = " +VtBase.BaseJesson+ "\n http_response_code = " + ScanIndyVT.http_response_code;
	   //Synchronize(&ErrorLog);
   }

}
//++++++++++++++++
 //Проводит реанализ файла, и ждет отчета новой провеки?
void __fastcall ScanVTIndy::ReScanVT()
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


		if(VTAnalyse(VtBase.BaseMD5))
		{
			if(logirovanie)
			{
			   LogMessage = " в ReScanVT() VTAnalyse(VtBase.BaseMD5)\n" + VtBase.BaseJesson;
			   Synchronize(&Logirovanie);
			}

			if(ScanIndyVT.http_response_code == 200)
			{
			   if(logirovanie)
			   {
				  LogMessage = " в ReScanVT() после if(ScanIndyVT.http_response_code == 200)\n" + VtBase.BaseJesson;
				  Synchronize(&Logirovanie);
			   }

			   if(JSONParseAnalysisID())
			   {
				  if(logirovanie)
				  {
					 LogMessage = " в ReScanVT() после if(JSONParseAnalysisID())\n" + VtBase.BaseJesson;
					 Synchronize(&Logirovanie);
				  }
				  //Begin while
				  while(!Terminated)
				  {
					 Sleep(15000);
					 if(!GetStatusConnect())
					 {
						Progress ="Ошибка сети. Нет Интернета.";
						// Вывожу в таблицу о начале загрузке.
						Synchronize(&ScanProgres);
						ErrorMessage = "Ошибка сети. Нет Интернета.";
						Synchronize(&ErrorLog);
						return;
					 }
					 VTAnalysasID();

					 if(logirovanie)
					 {
						LogMessage = " в ReScanVT() VTAnalysasID()";
						Synchronize(&Logirovanie);
					 }
					 if(JSONParseAnalysisStatus())
					 {
						 if(VtBase.BaseStatusAnalyzeID == "completed")
						 {
							if(logirovanie)
							{
								LogMessage = " в ReScanVT() в if(VtBase.BaseStatusAnalyzeID == \"completed\")\n" + VtBase.BaseJesson;
								Synchronize(&Logirovanie);
							}

							VTJSONParseDetectAnalysasID();
							if(logirovanie)
							{
								LogMessage = " в ReScanVT() в if(VtBase.BaseStatusAnalyzeID == \"completed\")  После VTJSONParseDetectAnalysasID()\n" + VtBase.BaseJesson;
								Synchronize(&Logirovanie);
							}
							Synchronize(&VIOtwet);
							break;
						 }
					 }
					 int timeBrec = AnsiCompareStr(FormatDateTime("YYYY.MM.DD",Date())+ " " + FormatDateTime("HH:NN:SS",Time()),TimeBreak);
					 if(timeBrec >=0)
					 {
						//JSONParseDetect(); //Удалил так как ответ JSON другой
						//DateTime(); //Удалил так как ответ JSON другой
						Synchronize(&VIOtwet);
						if(logirovanie)
						{
							LogMessage = " в ReScanVT() в if(timeBrec >=0))\n" + VtBase.BaseJesson;
							Synchronize(&Logirovanie);
						}
						break;
					 }
				  }
				  if(Terminated)
				  {
					 Progress ="Проверка остановлена";
					 Synchronize(&ScanProgres);
					 if(logirovanie)
					 {
						LogMessage = " в ReScanVT() в Проверка остановлена";
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

