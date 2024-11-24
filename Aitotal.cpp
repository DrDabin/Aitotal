//---------------------------------------------------------------------------

#include <vcl.h>
#include <memory>    //std::auto_prt<>
#include <IniFiles.hpp>
#pragma hdrstop

#include <tchar.h>
#include "Aitotal1.h"
//---------------------------------------------------------------------------

USEFORM("Aitotal1.cpp", Form3);
USEFORM("AitotalInclude\About\ABOUT.cpp", AboutBox);
USEFORM("AitotalInclude\ScanResultat\ScanResult.cpp", FormResultScan);
USEFORM("AitotalInclude\Options\Options1.cpp", MyOptionsForm);
USEFORM("AitotalInclude\Password\paroll1.cpp", FParoll);
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
			   HWND hWnd = FindWindow(L"TForm3", 0);

            if (IsWindowVisible(hWnd))
			{
				SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
				SetForegroundWindow(hWnd);
			}
			else
			{
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
				  cds.cbData =sizeof(wchar_t)* (_tcslen(asqw) + sizeof(wchar_t)); //sizeof(char) * (_tcslen(asqw) + sizeof(char));
				  cds.lpData = asqw;
				  SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
			   }

				SendMessage(hWnd, MY_MESSAGE(), 0, 0);
			}

			return 0;
			}

		}


		 // Не переносить, а то не будет работать запрет запуска копий программ.
		 // а так все данные добавляются в главную программу.
         Application->Initialize();
		 Application->MainFormOnTaskBar = true;
		 Application->CreateForm(__classid(TForm3), &Form3);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->CreateForm(__classid(TMyOptionsForm), &MyOptionsForm);
		Application->CreateForm(__classid(TFormResultScan), &FormResultScan);
		Application->Run();
		ReleaseMutex(hMutex);
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
