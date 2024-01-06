//---------------------------------------------------------------------------

#include <System.hpp>
#include <memory>
#include <DBXJSON.hpp>
#include <DateUtils.hpp>

#include <IdComponent.hpp>

#include <IdConnectThroughHttpProxy.hpp>
#include <urlmon.h> //Для настроек прокси как в IE.
#pragma comment(lib, "urlmon.lib")

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
UnicodeString LnMesEror = "Ошибка";
UnicodeString LnMesErrorCalcSHA256 = "Ошибка расчета SHA256" ;
UnicodeString LnMesSize = "Размер" ;
UnicodeString LnMesNoInterConect = "Нет соединение с интернетом";

UnicodeString LnMesNouInternet = "Нет интернета";

 UnicodeString LnMesStartOfScaning = "Начало сканирование";
UnicodeString LnMesScaningIsStoped = "Проверка остановлена" ;
UnicodeString LnMesUplodCompl = "Загрузка завершена в";
UnicodeString LnMesPerenosVTablScan = "Переношу всё в вкладку сканирование";
UnicodeString LnMesErrorNetwork = "Ошибка сети.";
UnicodeString LnMesSearchHessshBaseVT = "Ищем хеш в базе VT";
UnicodeString LnMesDisplToTablScan = "Вывожу в таблицу о начале сканирования";
UnicodeString LnMesScanStopped = "Проверка остановлена";
UnicodeString LnMesNouLimitApiKey = "Превышен лимит";
UnicodeString LnMesUploadFile = "Загрузка файла";
UnicodeString LnMesFileMax32MB = "Файл более 32МБ";
UnicodeString LnMesErrorUploadFile = "Ошибка в загрузки файла.";
UnicodeString LnMesAnalizFile = "Идет анализ файла";
UnicodeString LnMesObrabotResult = "Обработка результата" ;
UnicodeString LnMesNoSovpadSHA256 = "Не совпадает sha256: у меня =";
UnicodeString LnMesToVT =  "На сайте =";
UnicodeString LnMesFileVerified = "Файл проверен";
UnicodeString LnMesOtwetNarescan = "Ждём ответа на повторный анализ";
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

	TimeLogirovanie  =  VtBase.BasePredScanData;//сразу изменил в запуске потока из Aitotal1.cpp
	if(logirovanie)
	{
		LogMessage = LnMesStartOfScaning;
		Synchronize(&Logirovanie);
	}

	//ScanFiles();
	NewScanFiles();

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
		Progress =LnMesScaningIsStoped;
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
	Progress = LnMesUplodCompl;
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
			LogMessage = " в AtScanBegin()\n" + LnMesPerenosVTablScan ;
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
   Progress = LnMesEror;
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
		ErrorMessage = LnMesErrorCalcSHA256 +". ScanVTIndy::OtwetErrorSizeFile() THashSHA2::GetHashStringFromFile" + e.ToString();
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

   ListItem->SubItems->Add(LnMesSize + " = " + VtBase.BaseSizeFile);// ссылка на результат.

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

// Функции для 3 версии  +++++++++++++++++++++

bool __fastcall ScanVTIndy::SearchHesh(UnicodeString Hesh)
{
   bool scan = false;
   while(!Terminated)
   {
	  if(Terminated)
	  {
		 Progress =LnMesScanStopped;
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
		 Progress =LnMesErrorNetwork + LnMesNoInterConect;
		 // Вывожу в таблицу о начале загрузке.
		 ErrorMessage = LnMesErrorNetwork + LnMesNoInterConect;
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

	  //if(VTFilesID(VtBase.BaseMD5))
	  if(PosGetZapross("https://www.virustotal.com/api/v3/files/" + VtBase.BaseMD5,'G',""))
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
	  Progress =LnMesScanStopped;
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
void __fastcall ScanVTIndy::NewScanFiles()
{
	VtBase.BaseDataProverki = "";
	Progress = LnMesSearchHessshBaseVT;
	// Вывожу в таблицу о начале сканирования
	Synchronize(&AtScanBegin);
	if(logirovanie)
    {
		LogMessage = LnMesDisplToTablScan +" \n ScanFiles()";
		Synchronize(&Logirovanie);
    }

	if(Terminated)
    {
		Progress =LnMesScanStopped;
		Synchronize(&ScanProgres);
	    if(logirovanie)
	    {
			LogMessage = LnMesScanStopped + " NewScanFiles() ";
			Synchronize(&Logirovanie);
	    }
	}

	else
	{
		bool proverka =SearchHesh(VtBase.BaseMD5.c_str());
		//Проверка на  превышение лимита. код будет 429
		if(ScanIndyVT.http_response_code == 429)
		{
			Progress =LnMesScanStopped +"." + LnMesNouLimitApiKey + "code=429";
			Synchronize(&ScanProgres);
			if(logirovanie)
			{
				LogMessage = " в ReScanVT() в" + LnMesScanStopped +"." + LnMesNouLimitApiKey + "code=429";
				Synchronize(&Logirovanie);
			}
			//останавливаю проверку. так как произошло превышение лимита.
			return;
		}
		//Файл проверялся ранее
		if(proverka)
		{
			Synchronize(&DelSpisokNamePotok);

			// Если Рескан ==false
			if(!ScanIndyVT.Rescan)
				ReScanFalse(); //Просто выводим отчет.
			else
				ReScanVT();//Проводим повторное сканирование.

		}
		//Файл не проверялся ранее
		else
		{
			while(!Terminated)
			{
				Progress ="Подготовка к загрузке файла";
				Synchronize(&ScanProgres);
				Synchronize(&FileUploadBool);
				if(FileUpload)
				{
					if(VtBase.BaseSizeFile.ToInt()<= 32*1024*1024)
					{
							Progress =LnMesUploadFile;
							Synchronize(&ScanProgres);
							PosGetZapross("https://www.virustotal.com/api/v3/files",'P',VtBase.BasePatchFileName);
					}
					else if(VtBase.BaseSizeFile.ToInt()> 32*1024*1024 && VtBase.BaseSizeFile.ToInt() <= VtBase.MaxFileSize*1024*1024 &&VtBase.BaseSizeFile.ToInt() <= 650*1024*1024)
					{
						Progress = LnMesFileMax32MB;
						Synchronize(&ScanProgres);
						PosGetZapross("https://www.virustotal.com/api/v3/files/upload_url",'G',"");

						if(ScanIndyVT.http_response_code == 200)
						{
							Progress = "ScanIndyVT.http_response_code == 200" + VtBase.BaseJesson;
							Synchronize(&ScanProgres);
							if(VTAnalysasFileUploadID ())
							{
								PosGetZapross(VtBase.BaseUploadFile,'P',VtBase.BasePatchFileName);
							}

						}

					}
					if(ScanIndyVT.http_response_code == 429)
					{
						Progress =LnMesErrorUploadFile +"." +  LnMesNouLimitApiKey +"code=429";
						Synchronize(&ScanProgres);
						if(logirovanie)
						{
							LogMessage = LnMesErrorUploadFile +"." +  LnMesNouLimitApiKey +"code=429";
							Synchronize(&Logirovanie);
						}
						ErrorMessage = "http_response_code == 429\n" +VtBase.BaseJesson+ "\n http_response_code = " + ScanIndyVT.http_response_code;
						Synchronize(&ErrorLog);
						//останавливаю проверку. так как произошло превышение лимита.
						break;
					}
					if(ScanIndyVT.http_response_code == 200)
					{
						Synchronize(&DelSpisokNamePotok);
						if(JSONParseAnalysisID())
						{
							Progress =LnMesAnalizFile;
							Synchronize(&ScanProgres);
							while(!Terminated)
							{
								Sleep(15000);
								PosGetZapross("https://www.virustotal.com/api/v3/analyses/"+ VtBase.BaseIDVT,'G',"");
								if(ScanIndyVT.http_response_code == 429)
								{
									Progress =LnMesErrorUploadFile +"." +  LnMesNouLimitApiKey +"code=429";
									Synchronize(&ScanProgres);
									if(logirovanie)
									{
										LogMessage = LnMesErrorUploadFile +"." +  LnMesNouLimitApiKey +"code=429";
										Synchronize(&Logirovanie);
									}
                                    ErrorMessage = "http_response_code == 429\n" +VtBase.BaseJesson+ "\n http_response_code = " + ScanIndyVT.http_response_code;
									Synchronize(&ErrorLog);
									//останавливаю проверку. так как произошло превышение лимита.
									break;
								}
								if(ScanIndyVT.http_response_code == 200)
								{
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
										}//if(VtBase.BaseStatusAnalyzeID == "completed")
									}//if(JSONParseAnalysisStatus())
								}//if(ScanIndyVT.http_response_code == 200)

							}//while(!Terminated)
							break;
						}//if(JSONParseAnalysisID())
					}//if(ScanIndyVT.http_response_code == 200)
					else
					{
						Progress =ScanIndyVT.http_response_code;
						Synchronize(&ScanProgres);
						ErrorMessage = "http_response_code\n" +VtBase.BaseJesson+ "\n http_response_code = " + ScanIndyVT.http_response_code;
						Synchronize(&ErrorLog);
					}

				}//if(FileUpload)
				else Sleep(5000);//Ставим паузу, перед повторной проверкой на количество загружаемых файлов.
			}//while(!Terminated)
		}//Файл не проверялся ранее
		//else
	}
}
//+++++++++++++++++++++++++++++++++++++++++++++
void __fastcall ScanVTIndy::ReScanFalse()
{
   Progress ="ReScan false. "+LnMesObrabotResult;

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
			ErrorMessage = LnMesNoSovpadSHA256
			+ Form3->ListView2->Items->Item[i]->SubItems->Strings[3]
			+ LnMesToVT + VtBase.BaseSHA256 + "\n"
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
   ListItem->Data = new String(VtBase.BaseJesson);// результат

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
	  Form3->TrayIcon1->BalloonTitle = LnMesFileVerified;
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
		  TJSONObject *obj = static_cast<TJSONObject*>(ResRescan->GetValue("data"));
		try
		{
			VtBase.BaseIDVT = obj->GetValue("id")->Value();
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
	   ErrorMessage = "catch(...) error: JSONParseAnalysisID\n JSON = " +VtBase.BaseJesson+ "\n NameFile = " + VtBase.BaseFileName;// + "\n Error = " +;
	   Synchronize(&ErrorLog);
   }
   return rezult;

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
///+++++++++++++++++++++++++++++++++++
bool __fastcall ScanVTIndy::VTAnalysasFileUploadID ()
{
	bool rezult = false;
	try
	{
		std::auto_ptr<TJSONObject> ResRescan(static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(VtBase.BaseJesson)));
		if(logirovanie)
		{
			LogMessage = " в VTAnalysasFileUploadID() try std::auto_ptr<TJSONObject> ResRescan \n" +  ResRescan->ToString();
			Synchronize(&Logirovanie);
		}
		try
		{
			VtBase.BaseUploadFile = ResRescan->Get("data")->JsonValue->Value();
			rezult = true;
			if(logirovanie)
			{
				LogMessage = "VTAnalysasFileUploadID() try ResRescan->Get(\"data\")\n" +  VtBase.BaseUploadFile;

				Synchronize(&Logirovanie);
			}
		}
		catch (Exception &E)
		{
			if(logirovanie)
			{
				LogMessage = "VTAnalysasFileUploadID() catch TJSONObject *ResRescan\n" + E.Message + String("\n") + ResRescan->ToString();
				Synchronize(&Logirovanie);
			}
		}

	}
	catch(Exception &E)
	{
		if(logirovanie)
		{
			LogMessage = " JSONParseAnalysisStatus() catch TJSONObject *asd\n" + E.ToString();
			Synchronize(&Logirovanie);
		}
	   //ErrorMessage = "error: JSONParseDetect\n JSON = " +VtBase.BaseJesson+ "\n http_response_code = " + ScanIndyVT.http_response_code;
	   //Synchronize(&ErrorLog);
	}
	return rezult;

}
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
	int I;

	Progress = LnMesOtwetNarescan;
	// Вывожу в таблицу о начале сканирования
	Synchronize(&ScanProgres);
    if(Terminated)
	{
	  Progress =LnMesScanStopped +"11";
	  Synchronize(&ScanProgres);
	}
	do
	{
		if(!GetStatusConnect())
		{
			Progress =LnMesErrorNetwork +"." + LnMesNouInternet;
			// Вывожу в таблицу о начале загрузке.
			Synchronize(&ScanProgres);
			ErrorMessage = LnMesErrorNetwork +"." + LnMesNouInternet;
			Synchronize(&ErrorLog);
			return;
		}


		//if(VTAnalyse(VtBase.BaseMD5))
		//Повторное сканирование.
		if(PosGetZapross("https://www.virustotal.com/api/v3/files/" + VtBase.BaseMD5 + "/analyse",'P',""))
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
						Progress =LnMesErrorNetwork +"." + LnMesNouInternet;
						// Вывожу в таблицу о начале загрузке.
						Synchronize(&ScanProgres);
						ErrorMessage = LnMesErrorNetwork +"." + LnMesNouInternet;
						Synchronize(&ErrorLog);
						return;
					 }
					 //VTAnalysasID();
					 PosGetZapross("https://www.virustotal.com/api/v3/analyses/"+ VtBase.BaseIDVT,'G',"");
                     //Проверка на  превышение лимита. код будет 429
					 if(ScanIndyVT.http_response_code == 429)
					 {
						Progress =LnMesErrorUploadFile +"." +  LnMesNouLimitApiKey +"code=429";
						Synchronize(&ScanProgres);
						if(logirovanie)
						{
							LogMessage = " в ReScanVT() " +LnMesErrorUploadFile +"." +  LnMesNouLimitApiKey +"code=429";
							Synchronize(&Logirovanie);
						}

						//останавливаю проверку. так как произошло превышение лимита.
						break;
					 }
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
						VtBase.BaseJesson = JessonInSearchHech;
						VTJSONParseDetect();
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
					 Progress =LnMesScanStopped;
					 Synchronize(&ScanProgres);
					 if(logirovanie)
					 {
						LogMessage = " в ReScanVT() " + LnMesScanStopped;
						Synchronize(&Logirovanie);
					 }
				  }
				  //Endwhile
				  break;

			   }
			   else
				  break;//Sleep(10000);
			}
			//Проверка на  превышение лимита. код будет 429
			if(ScanIndyVT.http_response_code == 429)
			{
				Progress =LnMesErrorUploadFile +"." +  LnMesNouLimitApiKey +"code=429";
				Synchronize(&ScanProgres);
                if(logirovanie)
				{
					LogMessage = " в ReScanVT() " +  LnMesErrorUploadFile +"." +  LnMesNouLimitApiKey +"code=429";
					Synchronize(&Logirovanie);
				}
				//останавливаю проверку. так как произошло превышение лимита.
				break;
			}
			//else
			   //Sleep(2000);
		}
	}while(!Terminated);
}
//++++++++++++++++++++++++++++++++++++++++
// Новая функция запроса.
// Объединил все запросы в одну функцию.
//url ссылка запроса, Zapros тип запроса(Pos/Get), PatchFile Имя файла если надо загрузить его.
bool __fastcall ScanVTIndy::PosGetZapross (UnicodeString Url, const wchar_t Zapros, UnicodeString PatchFile)
{

	std::auto_ptr<TIdHTTP> IndyVT (new TIdHTTP(NULL) );
	std::auto_ptr<TIdSSLIOHandlerSocketOpenSSL> ssl ( new TIdSSLIOHandlerSocketOpenSSL(NULL));
	std::auto_ptr<TIdSocksInfo> soketInfo (new TIdSocksInfo(NULL));
	bool resultat= false;
	IndyVT->HandleRedirects = 1;
	IndyVT->Request->ContentType = L"application/x-msdownload";
	IndyVT->Request->Accept = "text/html,application/json,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
	IndyVT->Request->UserAgent = "Mozilla/5.0 (Windows; U; Windows NT 5.1;en-US; rv:1.8.1.1) Gecko/20061204 Firefox/2.0.0.1";
	IndyVT->Request->CustomHeaders->AddValue("x-apikey", ScanIndyVT.ApiKey);

	if(Zapros == 'P'&& PatchFile !="")
	{
		IndyVT->OnWorkBegin = InOnWorkBegin;
		IndyVT->OnWork = InWork;
		IndyVT->OnWorkEnd = InWorkEnd;
	}
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

	if(!ScanIndyVT.ProxyVT.ProxiChecked)
	{
		INTERNET_PROXY_INFO dfdg;
		dfdg.dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG;
		UrlMkSetSessionOption(INTERNET_OPTION_PROXY,&dfdg, sizeof(dfdg),0);
	}
	//удалил все пробелы по краям
	PatchFile = PatchFile.Trim();

	if(logirovanie)
	{
		LogMessage = " в ScanVTIndy::PosGetZapross";
		Synchronize(&Logirovanie);
	}
	try
	{
		/*Место добавление получение настроек по умолчанию.*/
		switch(Zapros)
		{
			case 'G':
				JessonInSearchHech = VtBase.BaseJesson = IndyVT->Get(Url);
				ScanIndyVT.http_response_code = IndyVT->ResponseCode;

				if(logirovanie)
				{
					LogMessage = " ScanVTIndy::PosGetZapross прошла успешно. response code = " + ScanIndyVT.http_response_code + String("\n") + VtBase.BaseJesson;
					Synchronize(&Logirovanie);
				}
				resultat = true;
				break;

			case 'P':
				if(PatchFile =="")
				{
					std::auto_ptr<TIdMultiPartFormDataStream> PostData(new TIdMultiPartFormDataStream);
					VtBase.BaseJesson = IndyVT->Post(Url,PostData.get());
					ScanIndyVT.http_response_code = IndyVT->ResponseCode;
					resultat = true;
                }
				else
				{
					std::auto_ptr<TIdMultiPartFormDataStream> PostData(new TIdMultiPartFormDataStream);
					std::auto_ptr<TFileStream> fs (new TFileStream(PatchFile, fmOpenRead | fmShareDenyNone));
					PostData->AddFormField("file","","",fs.get(),PatchFile);
					VtBase.BaseJesson = IndyVT->Post(Url,PostData.get());
					ScanIndyVT.http_response_code = IndyVT->ResponseCode;
					resultat = true;
				}
                if(logirovanie)
				{
					LogMessage = " ScanVTIndy::PosGetZapross прошла успешно. response code = " + ScanIndyVT.http_response_code + String("\n") + VtBase.BaseJesson;
					Synchronize(&Logirovanie);
				}
				break;
		}

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
		//ScanVTIndy::Terminate();
	}

	catch(Exception &E)   // Другие НЕ Indy исключения
	{
		ErrorMessage = "Ошибка: try (nouIn) Res\""+E.Message+"\""+ "Класс ошибки = " +E.ClassName()
						+ "\n" + PatchFile + "---" + Zapros + "\n" + Url;
		Synchronize(&ErrorLog);
		if(logirovanie)
		{
			LogMessage = "Ошибка: try (nouIn) Res\""+E.Message+"\""+ "Класс ошибки = " +E.ClassName();
			Synchronize(&Logirovanie);
		}
		//ScanVTIndy::Terminate();
	}

	return resultat;

}

