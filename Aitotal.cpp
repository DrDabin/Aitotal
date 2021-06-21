﻿//---------------------------------------------------------------------------

#include <vcl.h>
#include <memory>    //std::auto_prt<>
#include <IniFiles.hpp>
#pragma hdrstop

#include <tchar.h>
//---------------------------------------------------------------------------

USEFORM("AitotalInclude\Options\Options1.cpp", MyOptionsForm);
USEFORM("Aitotal1.cpp", Form3);
USEFORM("AitotalInclude\About\ABOUT.cpp", AboutBox);
USEFORM("AitotalInclude\Password\paroll1.cpp", FParoll);
USEFORM("AitotalInclude\ScanResultat\ScanResult.cpp", FormResultScan);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR par, int)
{
	try
	{

		bool Dublikat = false;

		try
		{
		   std::auto_ptr<TIniFile>Ini(new TIniFile(ExtractFilePath(Application->ExeName) + "tools\\options.ini"));
		   Dublikat = Ini->ReadBool("Tools", "RunDuplicateApplication", false);
		}
		catch(...)
		{
		   ;
		}
		 //должен быть как-можно сложнее
		HANDLE hMutex=OpenMutexW(MUTEX_ALL_ACCESS, false, L"ProgAitotal");

		if(!hMutex)
		  hMutex = CreateMutexW(NULL, false, L"ProgAitotal");

		else
		{
			if(!Dublikat)
			{
				//придумай классу формы уникальное имя
			   HWND__ *hWnd = FindWindowW(L"TForm3",NULL);
			   int cout;

			   if (ParamCount() >0)
			   {
				  wchar_t * asqw  = new wchar_t[_tcslen(par)];
				  StrCopy(asqw,L" ");

				  for (int i = 1; i <= ParamCount(); i++)
				  {
					 wcscat(asqw ,UnicodeString(ParamStr(i)).w_str());
					 wcscat(asqw, L"\n");
				  }

				  COPYDATASTRUCT cds;
				  cds.dwData = 0;
				  cds.cbData =sizeof(wchar_t) * (wcslen(asqw) + sizeof(wchar_t)); //sizeof(char) * (_tcslen(asqw) + sizeof(char));
				  cds.lpData = asqw;
				  SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)(LPVOID)&cds);
			   }
			   else
				  SendMessage(hWnd, WM_USER+1, 0, 0);

			   ReleaseMutex(hMutex);
			   return 0;
			}

		}
		 ReleaseMutex(hMutex);

		 // Не переносить, а то не будет работать запрет запуска копий программ.
		 // а так все данные добавляются в главную программу.
         Application->Initialize();
		 Application->MainFormOnTaskBar = true;
		 Application->CreateForm(__classid(TForm3), &Form3);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->CreateForm(__classid(TMyOptionsForm), &MyOptionsForm);
		Application->CreateForm(__classid(TFormResultScan), &FormResultScan);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
