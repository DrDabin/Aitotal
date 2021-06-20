// ---------------------------------------------------------------------------
#include <vcl.h>
#include <System.DateUtils.hpp>
#include <clipbrd.hpp>
#include <Tlhelp32.h>
#include <memory>    //std::auto_prt<>
#include <IniFiles.hpp>
#include <StrUtils.hpp>
#include <shlobj.h>
#include "Registry.hpp"
#include <IdSync.hpp>
#include <DBXJSON.hpp>

#pragma hdrstop

#include "Aitotal1.h"
#include "Aitotal.cpp"
//#include "AitotalInclude/sha256.h"
#include "AitotalInclude/ScanVTIndy/ScanVTIndy1.h"
#include "AitotalInclude/ScanResultat/ScanResult.h"// Форма результата.
#include "AitotalInclude/About/About.h"//справка о программе
#include "AitotalInclude/Password/paroll1.h"  //форма ввода пароля.
#include "AitotalInclude/InternetConnected.h"
#include "AitotalInclude/archiv/extract.h"
#include "AitotalInclude/Options/Options1.h"
#include "AitotalInclude/cheshMD5/md5.hpp"
#include "AitotalInclude/Struct.h"//Структуры. Перенесу туда все мои структуры. И надо будет их обьединить.
#include "AitotalInclude/RabotaSfilam.h"
#include "AitotalInclude/ParseHeadrArchiv.h"
#include "AitotalInclude/AitotalVersion.h"
#include "jessonLog.h"//запись из таблиц в файл

#pragma comment(lib,"rstrtmgr.lib")  //используется в функции определение на занятость файла.
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm3 *Form3;

TIniFile *IniOptions;

int nSortColumn=0;
bool bAscend = false;

int filenumber = 0;
int ApikeyNumber =0;


std::auto_ptr<TStringList>FileSpisokProwerki(new TStringList(NULL));

std::auto_ptr<TStringList>ErrorListArchivExtrakt(new TStringList(NULL));

//Удаление не пустой директории, вариант для Unicode
int Remove_dirW(wchar_t * pPath)
{
	SHFILEOPSTRUCTW sh;
	ZeroMemory(&sh,sizeof(SHFILEOPSTRUCTW));
	sh.wFunc = FO_DELETE;
	wcscat(pPath , L"\0"); //(путь в формате Unicode, завершающийся 2-мя NULL-терминаторами...)
	sh.pFrom =  pPath;//удаляемая директория
	sh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT | FOF_NORECURSION;
	return SHFileOperationW(&sh);
}

//++++++++++++

class TStatusBarNotify: public TIdNotify // Для потокобезопастностного добавления текста в лог
{
	public:
		__fastcall TStatusBarNotify( const String& ALine,TStatusBar* AStatusBar)
			 :FLine(ALine),FStatusBar(AStatusBar){}

	protected:
		virtual void __fastcall DoNotify()
		{
			 FStatusBar->Panels->Items[0]->Text = FLine;
		}

	private:
		String FLine;
		TStatusBar * FStatusBar;
};
// ---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner) : TForm(Owner)
{

}
// ---------------------------------------------------------------------------

void toLogA(const char* line)
{
	ErrorListArchivExtrakt->Add(line);
	//ErrorListArchivExtrakt->SaveToFile("fd",TEncoding::Unicode);
}

//++++++
void toLog(const wchar_t* line)
{
	ErrorListArchivExtrakt->Add(line);
	//ErrorListArchivExtrakt->SaveToFile("fd",TEncoding::Unicode);
}
//++++++++++++++++++++++++++++++++++++
void toExtracting(const wchar_t* line)
{
	//Form3->stat1->Panels->Items[0]->Text = String("Распаковка архива. ") + String(line);

	(new TStatusBarNotify(String("Распаковка архива. " + String(line)),Form3->stat1))->Notify();
	Application->ProcessMessages();
}
 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void toExtractingS(const wchar_t* nameFile, const __int64 line)
{
	//Form3->stat1->Panels->Items[0]->Text = String("Имя файла = ") + String(nameFile) + String("  Процент распаковки = ") + line;
	//(new TStatusBarNotify(String("Имя файла = "+String(nameFile) + String("  Процент распаковки = ") + String(line)),Form3->stat1))->Notify();
	(new TStatusBarNotify(String(" Процент распаковки = ") + String(line),Form3->stat1))->Notify();
	Application->ProcessMessages();
} 
//---------------------------------------------------------------------------
void __fastcall TForm3::GetMessage(TWMCopyData &msg)
{
	//Срабатывает когда стоит запрет на запуск несколько копий программ.
	// при этом всё передаётся в первую программу.

	std::auto_ptr<TStringList> SpisokFileArchiv (new TStringList(NULL) );
	SpisokFileArchiv->Text = (wchar_t*)msg.CopyDataStruct->lpData;
	int inrescan=1;
	for(int i = 1; i < SpisokFileArchiv->Count; i++)
	{
		if(SpisokFileArchiv->Strings[i].SubString(1,1)=="/")
		{
			if( SpisokFileArchiv->Strings[i].Trim()  == "/scan")
				inrescan=1;
			else if(SpisokFileArchiv->Strings[i].Trim() == "/norescan")
				inrescan=0;

			else if(SpisokFileArchiv->Strings[i].Trim()== "/min")
				ShowWindow(Handle, SW_SHOWMINNOACTIVE);
			else{
				ShowMessage(SpisokFileArchiv->Strings[i]+"\n Нет такого параметра коммандной строки.\n");
				return;
			}

		}
		else
			MyFiledDir(SpisokFileArchiv->Strings[i].Trim(),inrescan);
	}
	ScanFile();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::SetForeground(TMessage &msg)
{
   SetForegroundWindow(Handle);
}

void __fastcall TForm3::WM_QueryEndSession (TMessage &msg)
{
	AtOptions.Exit_Trei = true;

	Form3->Close();
	//TForm::Dispatch(&msg);
}

int Mycompare (int a, int b)
{
	if (a>b)
		 return 1;

	if (a<b)
		 return -1;

	return 0;
}
//ф-я сортировка столбца результат
int SortTabResultat(UnicodeString par1, UnicodeString par2)
{
	int per1;// = par1.ToInt();
	int per2;// = par2.ToInt();
	try
	{
		par1 = par1.SubString(0,par1.Pos("/")-1);
	   	per1 = par1.ToInt();
	}
	catch(...)
	{
		per1=-1;
	}
	try
	{
		par2 = par2.SubString(0,par2.Pos("/")-1);

		per2 = par2.ToInt();
	}
	catch(...)
	{
		per2=-1;
	}

	if (per1>per2)
		return 1;
	if(per1<per2)
		return -1;

	return 0;
}
int __stdcall SortProc(TListItem *Item1, TListItem *Item2, long Para)
{
   TDateTime dt1, dt2;
   int I=0;
   switch(nSortColumn)
   {
	  case 0:
		 I = CompareStr(((TListItem *)Item1)->Caption, ((TListItem *)Item2)->Caption);
		 break;
	  case 1:
		 I = AnsiCompareStr(((TListItem *)Item1)->SubItems->Strings[0], ((TListItem *)Item2)->SubItems->Strings[0]);
		 break;
	  case 2:
		 I = SortTabResultat(((TListItem *)Item1)->SubItems->Strings[1], ((TListItem *)Item2)->SubItems->Strings[1]);
		 break;
	  case 3:
		 I = Mycompare(((TListItem *)Item1)->SubItems->Strings[2].Trim().ToInt(), ((TListItem *)Item2)->SubItems->Strings[2].Trim().ToInt());
		 break;
		 case 4:
		 I = AnsiCompareStr(((TListItem *)Item1)->SubItems->Strings[3], ((TListItem *)Item2)->SubItems->Strings[3]);
		 break;
	  case 5:
		 I = AnsiCompareStr(((TListItem *)Item1)->SubItems->Strings[4], ((TListItem *)Item2)->SubItems->Strings[4]);
		 break;
	  case 6:
		 FormatSettings.DateSeparator = '.';
		 FormatSettings.TimeSeparator = ':';
		 FormatSettings.LongDateFormat =  "YYYY.DD.MM";
		 FormatSettings.ShortDateFormat = "YYYY.DD.MM";
		 FormatSettings.ShortTimeFormat = "HH:MM:SS";
		 FormatSettings.LongTimeFormat =  "HH:MM:SS";
		 dt1 = StrToDateTimeDef(Item1->SubItems->Strings[5], StrToDateTime(UnicodeString("1976.01.01 00:00:00"), FormatSettings), FormatSettings); // Обрати внимание на индекс, на 1 меньше чем case
		 dt2 = StrToDateTimeDef(Item2->SubItems->Strings[5], StrToDateTime(UnicodeString("1976.01.01 00:00:00"), FormatSettings), FormatSettings);
		 I = Idglobalprotocols::CompareDateTime(dt1, dt2);
		 break;
   }
   if(!bAscend)
	  I = -I;
   return I;// ...
}
//++++++++++++++++++++++++
int __stdcall SortProcListViewe2(TListItem *Item1, TListItem *Item2, long Para)
{
   char sep;
   int I=0;
   switch(nSortColumn)
   {
	  case 0:
		 I = CompareStr(((TListItem *)Item1)->Caption, ((TListItem *)Item2)->Caption);
		 break;
	  case 1:
		 I = AnsiCompareStr(((TListItem *)Item1)->SubItems->Strings[0], ((TListItem *)Item2)->SubItems->Strings[0]);
		 break;
	  case 2:
		 I = Mycompare(((TListItem *)Item1)->SubItems->Strings[1].Trim().ToInt(), ((TListItem *)Item2)->SubItems->Strings[1].Trim().ToInt());
		 break;
	  case 3:
		 I = AnsiCompareStr(((TListItem *)Item1)->SubItems->Strings[2], ((TListItem *)Item2)->SubItems->Strings[2]);
		 break;
	  case 4:
		 I = AnsiCompareStr(((TListItem *)Item1)->SubItems->Strings[3], ((TListItem *)Item2)->SubItems->Strings[3]);
		 break;
	  case 5:
		 I = AnsiCompareStr(((TListItem *)Item1)->SubItems->Strings[4], ((TListItem *)Item2)->SubItems->Strings[4]);
		 break;
   }
   if(!bAscend)
	  I = -I;
   return I;// ...
}
//++++++++++++++++++++++++++++++++++++++
int __stdcall SortProcListViewe1(TListItem *Item1, TListItem *Item2, long Para)
{
   int I=0;

   switch(nSortColumn)
   {
	  case 0:
		 I = CompareStr(((TListItem *)Item1)->Caption, ((TListItem *)Item2)->Caption);
		 break;

	  case 1:
		 I = AnsiCompareStr(((TListItem *)Item1)->SubItems->Strings[0], ((TListItem *)Item2)->SubItems->Strings[0]);
		 break;

	  case 2:
		 I = Mycompare(((TListItem *)Item1)->SubItems->Strings[1].Trim().ToInt(), ((TListItem *)Item2)->SubItems->Strings[1].Trim().ToInt());
		 break;

	  case 3:
		 I = AnsiCompareStr(((TListItem *)Item1)->SubItems->Strings[2], ((TListItem *)Item2)->SubItems->Strings[2]);
		 break;

	  case 4:
		 I = AnsiCompareStr(((TListItem *)Item1)->SubItems->Strings[3], ((TListItem *)Item2)->SubItems->Strings[3]);
		 break;
   }
   if(!bAscend)
	  I = -I;
   return I;// ...
}
//++++++++++++++++++++++++++++++++++++++
void __fastcall TForm3::FormActivate(TObject *Sender)
{
	//exit=false;

	ListView3->Columns->Items[0]->Width = 150;// Имя файла
	ListView3->Columns->Items[1]->Width = 180; //Путь к файлу
	ListView3->Columns->Items[2]->Width = 70;//Результат
	ListView3->Columns->Items[3]->Width = 85;//размер
	ListView3->Columns->Items[4]->Width = 230;//MD5
	ListView3->Columns->Items[5]->Width = 300;//SHA256
	ListView3->Columns->Items[6]->Width = 120;//Дата

	ListView2->Columns->Items[0]->Width = ListView2->Width * 15 / 100;//имя файла
	ListView2->Columns->Items[1]->Width = ListView2->Width * 20 / 100;// Полное имя файла
	ListView2->Columns->Items[2]->Width = 85;//размер
	ListView2->Columns->Items[3]->Width = 230;//МД5
	ListView2->Columns->Items[4]->Width = 300;//SHA256
	ListView2->Columns->Items[6]->Width = 85;//permalink
	ListView2->Columns->Items[5]->Width = 150; //% загрузки

	ListView1->Columns->Items[0]->Width = ListView1->Width * 20 / 100;//имя файла
	ListView1->Columns->Items[1]->Width = ListView1->Width * 47 / 100; //Полный путь к файлу
	ListView1->Columns->Items[2]->Width = 85;//размер
	ListView1->Columns->Items[3]->Width =230;//MD5

	Form3->Caption = "Aitotal [v" + UnicodeString(MY_VERSION) + "  " + UnicodeString(MY_DATE) + "]";
   PageControl1->ActivePage = TabSheet1;
   FileNumber = 0;
   ListView1->FullDrag = true;
   ListView2->FullDrag = true;
   ListView3->FullDrag = true;

   PatchProgramma = L"\\\\?\\" + ExtractFilePath(ParamStr(0));

	//Использую ChangeWindowMessageFilterEx которая объявлена начиная с  Windows7. Надо потестировать.

		if(TOSVersion::Check(6,1))//проверяю Windows 7 и выше.
		{
			HINSTANCE hUser =  LoadLibrary(TEXT("user32.dll"));
			if (hUser != NULL)
			{
				typedef BOOL (WINAPI *ChangeWindowMessageFilterEx)(HWND hWnd,UINT message, DWORD action, PCHANGEFILTERSTRUCT pChangeFilterStruct);
				ChangeWindowMessageFilterEx filterEx = (ChangeWindowMessageFilterEx)GetProcAddress(hUser, "ChangeWindowMessageFilterEx");
				if (filterEx != NULL)
				{
					filterEx (Handle, WM_DROPFILES, MSGFLT_ALLOW, NULL);
					filterEx (Handle, WM_COPYDATA, MSGFLT_ALLOW, NULL);
					filterEx (Handle, 0x0049, MSGFLT_ALLOW,NULL);

					FreeLibrary(hUser);
				}
			}

		}
		else if(TOSVersion::Check(6,0))  // Только для  Висты.
		{
			HINSTANCE hUser =  LoadLibrary(TEXT("user32.dll"));
			if (hUser != NULL)
			{
				typedef BOOL (WINAPI *ChangeMessageFilter)(UINT message, DWORD dwFlag);
				ChangeMessageFilter filter = (ChangeMessageFilter)GetProcAddress(hUser, "ChangeWindowMessageFilter");
				if (filter != NULL)
				{
					filter(WM_DROPFILES, MSGFLT_ADD);
					filter(WM_COPYDATA, MSGFLT_ADD);
					filter(0x0049, MSGFLT_ADD);
                    FreeLibrary(hUser);
				}
			}
		}

   CreateDirectoryW(UnicodeString(PatchProgramma + "AitotalTMP\\").c_str(), NULL);
   CreateDirectoryW(UnicodeString(PatchProgramma + "tools\\").c_str(), NULL);

   //Устанавливаю директорию, где лежат файлы длл для SSL.
   try
   {
	  IdOpenSSLSetLibPath(UnicodeString(PatchProgramma + "tools\\"));
   }
   catch(  Exception &E)
   {
	   ErrorLog("Ошибка в функции IdOpenSSLSetLibPath = " + E.Message);
	   ShowMessage(E.Message);
	   ShowMessage(E.ToString());
   }
}
// ---------------------------------------------------------------------------
bool __fastcall TForm3::Kill(char * procname)
{
	#define PROCESS_TERMINATE (0x0001)
	BOOL ContinueLoop;
	HANDLE FSnapshotHandle;
	PROCESSENTRY32 FProcessEntry32;
	FSnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	FProcessEntry32.dwSize = sizeof(FProcessEntry32);

	ContinueLoop = Process32First(FSnapshotHandle, &FProcessEntry32);

	while (int(ContinueLoop) != 0) {
		if ((UpperCase(ExtractFileName(FProcessEntry32.szExeFile)) ==
			(UpperCase(procname)) | (UpperCase(FProcessEntry32.szExeFile)
			== UpperCase(procname)))) {
			return int(TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, BOOL(0),
				FProcessEntry32.th32ProcessID), 0));
		}
		ContinueLoop = Process32Next(FSnapshotHandle, &FProcessEntry32);
	}
	CloseHandle(FSnapshotHandle);
	return false;
}

// ++++++++++++++++++++++++++++++++++++++


// ---------------------------------------------------------------------------
void __fastcall TForm3::FormCreate(TObject *Sender)
{
   IzvlechenieResursToFile();
   OptionReadIni();
   Scan = false; // если труе идет обработка.
   StopInet = false; // если труе то не выводить больше сообщение о прекращение доступа к интернету.
   SpisokFileName = new TList();
   SpisokNamePotok = new TStringList(NULL);
   SpisokArchivFolder =  new TStringList(NULL);
   DragAcceptFiles(this->Handle, true);
   // праметры передаваемые через батник или командную строку.
	//При каждой новой копии программы
	//  Передаю значение делать рескан или нет.
	int inrescan = 1;

	for(int i = 1; i<=ParamCount(); i++)
	{
		if(ParamStr(i).SubString(1,1)=="/")
		{
			if( ParamStr(i)  == "/scan")
				inrescan=1;
			else if(ParamStr(i) == "/norescan")
				inrescan=0;

			else if(ParamStr(i)== "/min")
				ShowWindow(Handle, SW_SHOWMINNOACTIVE);
			else{
				ShowMessage(ParamStr(i)+"\n Нет такого параметра коммандной строки.\n");
				return;
			}

		}
		else
			MyFiledDir(UnicodeString(ParamStr(i)).Trim(),inrescan);
	}
	ScanFile();
}

// ++++++++++++++++++++++++

void __fastcall TForm3::MmOpenFile(TObject *Sender)
{
	if (GetStatusConnect())
	{
		if (OpenDialog1->Execute())
		{
			for (int i = 0; i < OpenDialog1->Files->Count; i++)
			{
				MyFiledDir(OpenDialog1->Files->Strings[i],1);
			}
			ScanFile();
		}
	}
	else
		MessageBoxA(0, "Нет соединение с интернетом", 0, MB_OK + MB_ICONSTOP);
}

// ---------------------------------------------------------------------------
void __fastcall TForm3::MmOpenDir(TObject *Sender)
{
	if (GetStatusConnect())
	{
		BROWSEINFOW info;
		wchar_t szDir[MAX_PATH];
		wchar_t szDisplayName[MAX_PATH];
		LPITEMIDLIST pidl;
		LPMALLOC pShellMalloc;

		if (SHGetMalloc(&pShellMalloc) == NO_ERROR)
		{
			memset(&info, 0x00, sizeof(info));
			info.hwndOwner = 0;
			info.pidlRoot = NULL;
			info.pszDisplayName = szDisplayName;
			info.lpszTitle = L"Выбор папки";
			info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_USENEWUI;
			pidl = SHBrowseForFolderW(&info);

			if (pidl)
			{
				if (SHGetPathFromIDListW(pidl, szDir))
				{
					MyFiledDir(szDir,1);
				}
				pShellMalloc->Free(pidl);
				pShellMalloc->Release();
			}
			ScanFile();
		}
	}
	else
		MessageBoxW(0, L"Нет соединение с интернетом", L"Внимание ошибка",
		MB_OK + MB_ICONSTOP);
}

// ---------------------------------------------------------------------------
bool TForm3::ExtractResource(UnicodeString NameResource, UnicodeString NameFile)
{
   wchar_t *pchResType = L"BINARY";

   HRSRC hSrsrc=FindResourceW(HInstance,NameResource.c_str(),pchResType);
   if(hSrsrc ==NULL)
   {
	  ShowMessage("Ресурс не найден");
	  ErrorLog("Ресурс не найден" + NameResource);
	  return false;
   }

   else
   {
	   HGLOBAL hGlobal=LoadResource(HInstance,hSrsrc);
	   if(hGlobal!=NULL)
	   {
		   void *pvResPtr=LockResource(hGlobal);
		   if(pvResPtr==NULL)
		   {
			  return false;
		   }
		   else
		   {
			  std::auto_ptr<TMemoryStream> Zapros (new TMemoryStream() );
			   //Размер ресурса
			  unsigned long  vulResourceSize=SizeofResource(NULL,hSrsrc);
			  //Поток бит помещаем в массив по умолчанию для Stream
			  Zapros->WriteBuffer(pvResPtr,vulResourceSize);
			  //На этом этапе можно сохранить ресурс в памяти
			  Zapros->SaveToFile(NameFile);
			  FreeResource(hGlobal);
			  return true;
		   }
	   }
	   else
	   {
		  MessageBoxW(NULL,L"Not resource",L"error",MB_OK);
		  ErrorLog("Ресурс не найден" + NameResource);
		  return false;
	   }
   }
}

void TForm3::IzvlechenieResursToFile()
{
   UnicodeString PathResous = ExtractFilePath(ParamStr(0));

   if(!TDirectory::Exists(PathResous + "tools",false))
	  TDirectory::CreateDirectory(PathResous + "tools");


   if(!TFile::Exists(PathResous + "tools\\7z.dll"))
	  ExtractResource("7zdll",PathResous + "tools\\7z.dll");

   if(!TFile::Exists(PathResous + "tools\\libeay32.dll"))
	  ExtractResource("libeay32",PathResous + "tools\\libeay32.dll");

   if(!TFile::Exists(PathResous + "tools\\ssleay32.dll"))
	  ExtractResource("ssleay32",PathResous + "tools\\ssleay32.dll");


}
//++++++++++++++++++++++++++++++++
void __fastcall TForm3::Button1Click(TObject *Sender)
{
   StopThread = true;
   StopThreadPopupMenu = true;
   Button1->Enabled =false;

   for(int i = 0; i< 100; i++)
   {
	   if(VTIndy[i] != NULL)
	   {
		  VTIndy[i]->Terminate();
		  Sleep(10);
	   }
   }
   Button1->Enabled = true;

}

// ---------------------------------------------------------------------------
// функция обработки перетаскивания файлов на форму.
void __fastcall TForm3::Response(TWMDropFiles &Message)
{
	if (GetStatusConnect())
	{
		// Переменная для хранения имени файла
		wchar_t chName[MAX_PATH];
		HDROP hdropHandle = (HDROP)Message.Drop;
		// Число буксируемых файлов
		int viNumber = DragQueryFile(hdropHandle, -1, NULL, NULL);

		for (int i = 0; i < viNumber; i++)
		{
			Application->ProcessMessages();
			DragQueryFileW(hdropHandle, i, chName, MAX_PATH);
			MyFiledDir(chName,1);
		}
		DragFinish(hdropHandle);
		ScanFile();
	}
	else
		MessageBoxA(0, "Нет соединение с интернетом", 0, MB_OK + MB_ICONSTOP);
}
// ---------------------------------------------------------------------------
void __fastcall TForm3::Pm1Select(TObject *Sender)
{
	for (int i = 0; i < ListView1->Items->Count; i++)
		ListView1->Items->Item[i]->Checked = true;
}

// ---------------------------------------------------------------------------
void __fastcall TForm3::Pm1Scan(TObject *Sender)
{
	;//ItemScan();
}

// ---------------------------------------------------------------------------
void __fastcall TForm3::Pm1Reset(TObject *Sender)
{
	for (int i = 0; i < ListView1->Items->Count; i++)
		ListView1->Items->Item[i]->Checked = false;
}

// ---------------------------------------------------------------------------
UnicodeString TForm3::MyFiledDirArchiv(UnicodeString Dir)
{
	TStringDynArray Files;
	UnicodeString Spisok;
	String Mask= "*"; //  "*.txt"
	std::auto_ptr<TStringList> SpisokFileArchiv (new TStringList(NULL) );

	try
	{
	   Files = TDirectory::GetFiles(Dir, Mask ,TSearchOption::soAllDirectories );

	   for(int i=0; i<Files.Length; i++)
	   {
		  SpisokFileArchiv->Add(Files[i].LowerCase());
	   }
	}
	catch (EDirectoryNotFoundException  &d)
	{
	   //Закоментировал тка как не надо распаковывать вложенные архивы.
	   /*if(ExtractFileExt(Dir.LowerCase()) == ".rar" ||
		  ExtractFileExt(Dir.LowerCase()) == ".zip" ||
		  ExtractFileExt(Dir.LowerCase()) == ".7z")
	   {
		  ExtArchiv(Dir);
	   }
	   else*/
		  SpisokFileArchiv->Add(Dir.LowerCase());
	}

	if(SpisokFileArchiv->Text !=" ")
	{
		Spisok = SpisokFileArchiv->Text;
	}
	else
	   Spisok="";

	return Spisok;
}
//++++++++++++++++++++++++++++++++++
void __fastcall TForm3::ZanoshuVListWiew3PriOshibki(String ArcName, __int64 MysizaFile , String rezult, String Heh, UnicodeString Sha256, bool date)
{
	ListItem = Form3->ListView3->Items->Add();
	ListItem->Caption =ExtractFileName(ArcName);
	ListItem->Checked = false;
	ListItem->SubItems->Add(ArcName);
	ListItem->SubItems->Add("");
	ListItem->SubItems->Add(MysizaFile);

	if(Heh =="")
	   ListItem->SubItems->Add("");
	else
	   ListItem->SubItems->Add(Heh);

	if(Sha256 =="")
	   ListItem->SubItems->Add("");
	else
	   ListItem->SubItems->Add(Sha256);

	if(date)
	   ListItem->SubItems->Add(FormatDateTime("YYYY.MM.DD",Date())+ " " + FormatDateTime("HH:NN:SS",Time()));
	else
	   ListItem->SubItems->Add("");

	ListItem->SubItems->Add(rezult);
}

//
void __fastcall TForm3::OtWetErrorSizeFile(UnicodeString FileNime, UnicodeString MoveFileName,  __int64 sizefile, UnicodeString Md5)
{
   if(MoveFileName !="")
   {
	  UnicodeString nameFy = RenameFileName(MoveFileName);
	  MoveFileName = nameFy;
	  RenameFile(FileNime,MoveFileName);
   }
   else
	 MoveFileName = FileNime;

   if(sizefile == 0)
   {
	  String resultat = "Размер файла = 0 байт";
	  ZanoshuVListWiew3PriOshibki(MoveFileName, sizefile ,  resultat , Md5, "", false);
   }
   if(sizefile > 134271701)
   {

	  String resultat = "Размер файла больше 128 Мб";
	  ZanoshuVListWiew3PriOshibki(MoveFileName, sizefile , resultat , Md5, "",false);
   }
   if(sizefile < 0)
   {
	  String resultat = "Ошибка определения размера файла";
	  ErrorLog(MoveFileName + "\n Ошибка определения размера файла");
	  ZanoshuVListWiew3PriOshibki(MoveFileName, sizefile , resultat , Md5, "", false );
   }
}
//-----------------------------------------------------
void __fastcall TForm3::ParsingArchivAVZ_dta(UnicodeString FileName, TList *SpisokFileKarantin, int rescan )
{
   //++++++++Begin AVZ .dta++++++++
   try
   {
			stat1->Panels->Items[0]->Text = L"Парсинг архива AVZ. Файл " + ExtractFileName(FileName);

			std::auto_ptr<TIniFile>  Ini  (new TIniFile(ExtractFilePath(FileName) + ExstractFileNameBezExt(ExtractFileName(FileName)) + ".ini"));
			UnicodeString name = Ini->ReadString("InfectedFile","Src","").LowerCase();
			__int64 sizefile = FileSizeStatic(FileName);
			UnicodeString MyMd5 = "";
			UnicodeString SHA256 = "";

			if(FileExists(FileName))
			{
			   if((sizefile >0) && (sizefile < 134271701))
			   {
				  MyMd5 = CalkFileMD5(FileName).LowerCase();
				  //SHA256 = Mysha256ver2(FileName);
				  //Использую стандартную функцию расчета хеш суммы.
				  try{
						SHA256 = THashSHA2::GetHashStringFromFile(TFileName(FileName), THashSHA2::TSHA2Version::SHA256);
				  }
				  catch(Exception &e)
				  {
					  ErrorLog("Ошибка расчета SHA256.  ParsingArchivAVZ_dta()" +e.ToString());
				  }
				  //Application->ProcessMessages();
				  if( (MyMd5 !="") && (MyMd5) != "nou")
				  {
					 bool postFileArchiv = false;
					 for( int st = 0; st < SpisokFileKarantin->Count ; st++)
					 {
						Application->ProcessMessages();

						if(MyMd5 == ((DuplicatListFile*)SpisokFileKarantin->Items[st])->MD5)
						{
						   if(name != ((DuplicatListFile*)SpisokFileKarantin->Items[st])->FilePatch)
						   {
							  UnicodeString fileOut;
							  fileOut = RenameFileName(ExtractFilePath(FileName) + ExtractFileName(name) + ".vir");

							  if(RenameFile(FileName,fileOut))
							  {
								 postFileArchiv = true;
								 String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
								 ZanoshuVListWiew3PriOshibki(fileOut, sizefile, result , MyMd5, SHA256,false);
								 break;
							  }
							  else
							  {
								 ErrorLog("Ошибка переименовывания файла " + FileName + "\n в " +fileOut +"\n");
								 break;
							  }
						   }
						   else
						   {
							  postFileArchiv = true;
							  break;
						   }
						}
					 }
					 if(!postFileArchiv)
					 {
						UnicodeString nameFy = RenameFileName(ExtractFilePath(FileName) + ExtractFileName(name) + ".vir");

						if(RenameFile(FileName,nameFy))
						{
						   SpisokFileName->Add(new StructFileList(1,rescan,nameFy, sizefile,MyMd5,SHA256));
						   SpisokFileKarantin->Add(new DuplicatListFile(name,MyMd5));
						}
						else
						   ErrorLog("Ошибка переименовывания файла " + FileName+ "\n в " +nameFy +"\n");
					 }
				  }
			   }
			   else
				  OtWetErrorSizeFile(FileName, ExtractFilePath(FileName) + ExtractFileName(name) + ".vir", sizefile, MyMd5);
			}
   }
   catch(...)
   {
	  ;
   }
	  ////++++++++End AVZ .dta++++++++
}
//---------------------------------------------------------------------------
void __fastcall TForm3::ParsingArchivAVZ_dat(UnicodeString FileName, TList *SpisokFileKarantin , int rescan)
{
		 try
		 {
			std::auto_ptr<TIniFile>  Ini  (new TIniFile(ExtractFilePath(FileName) + ExstractFileNameBezExt(ExtractFileName(FileName)) + ".ini"));
			UnicodeString name = Ini->ReadString("InfectedFile","Src","").LowerCase();
			__int64 sizefile = FileSizeStatic(FileName);
			UnicodeString MyMd5 = "";
			UnicodeString SHA256="";

			stat1->Panels->Items[0]->Text = L"Парсинг архива AVZ. Файл " + ExtractFileName(FileName);
			//Application->ProcessMessages();

			if(FileExists(FileName))
			{
			   if((sizefile >0) && (sizefile < 134271701))
			   {
				  MyMd5 = CalkFileMD5(FileName).LowerCase();
				  //SHA256 = Mysha256ver2(FileName);
				  //Использую стандартную функцию расчета хеш суммы
				  try{
						SHA256 = THashSHA2::GetHashStringFromFile(TFileName(FileName), THashSHA2::TSHA2Version::SHA256);
				  }
				  catch(Exception &e)
				  {
					  ErrorLog("Ошибка расчета SHA256.  ParsingArchivAVZ_dat()\n" +e.ToString());
				  }
				  if( (MyMd5 !=""))
				  {
					 bool postFileArchiv = false;
					 for( int st = 0; st < SpisokFileKarantin->Count ; st++)
					 {
						if(MyMd5 == ((DuplicatListFile*)SpisokFileKarantin->Items[st])->MD5)
						{
						   if(name != ((DuplicatListFile*)SpisokFileKarantin->Items[st])->FilePatch)
						   {
							  UnicodeString fileOut;
							  fileOut = RenameFileName(ExtractFilePath(FileName) + ExtractFileName(name) + ".vir");

							  if(RenameFile(FileName,fileOut))
							  {
								 postFileArchiv = true;
								 String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
								 ZanoshuVListWiew3PriOshibki(fileOut, sizefile, result , MyMd5,SHA256,false);
								 break;
							  }
							  else
							  {
								 ErrorLog("Ошибка переименовывания файла " + FileName + "\n в " +fileOut +"\n");
								 break;
							  }
						   }
						   else
						   {
							  postFileArchiv = true;
							  break;
						   }
						}
						Application->ProcessMessages();
					 }
					 if(!postFileArchiv)
					 {
						UnicodeString nameFy = RenameFileName(ExtractFilePath(FileName) + ExtractFileName(name) + ".vir");

						if(RenameFile(FileName,nameFy))
						{
						   ////FileSpisokProwerki->Add(IntToStr(rescan) + nameFy);
						   //FileSpisokProwerki->Add(IntToStr(rescan)+";" + nameFy+";" + IntToStr(sizefile) +";"+MyMd5);
						   SpisokFileName->Add(new StructFileList(1,rescan,nameFy, sizefile,MyMd5,SHA256));
						   SpisokFileKarantin->Add(new DuplicatListFile(name,MyMd5));
						}
						else
						   ErrorLog("Ошибка переименовывания файла " + FileName + "\n в " +nameFy +"\n");
					 }
				  }
			   }
			   else
				  OtWetErrorSizeFile(FileName, ExtractFilePath(FileName) + ExtractFileName(name) + ".vir", sizefile, MyMd5);
			}
		 }
		 catch(...)
		 {
			;
		 }
}
//-----------------------------------------------------------------------
void __fastcall TForm3::ParsingArchivUVS(UnicodeString FileName, TList *SpisokFileKarantin, int rescan )
{
   UnicodeString name = "";
   __int64 sizefile = FileSizeStatic(FileName);
   UnicodeString MyMd5 = "";
   UnicodeString SHA256="";

   stat1->Panels->Items[0]->Text = L"Парсинг архива UVS. Файл " + ExtractFileName(FileName);

   if(FileExists(FileName))
   {
	  if((sizefile >0) && (sizefile < 134271701))
	  {
		 MyMd5 = CalkFileMD5(FileName).LowerCase();
		 //SHA256 = Mysha256ver2(FileName);
		 //Использую стандартную функцию расчета хеш суммы
		 try
		 {
			SHA256 = THashSHA2::GetHashStringFromFile(TFileName(FileName), THashSHA2::TSHA2Version::SHA256);
		 }
		 catch(Exception &e)
		 {
			ErrorLog("Ошибка расчета SHA256.  ParsingArchivUVS()" +e.ToString());
		 }

		 if(MyMd5 !="")
		 {
			if(FileExists(FileName + ".txt"))
			{
			   std::auto_ptr<TStringList> Zapros (new TStringList(NULL) );
			   Zapros->LoadFromFile( FileName + ".txt");

			   for(int total = 0; total <Zapros->Count ; total++)
			   {
				  UnicodeString stroka = Zapros->Strings[total];
				  int qwer = stroka.Pos("Полное имя");

				  if( qwer > 0)
				  {
					 name = stroka.SubString(qwer+11, stroka.Length()-(qwer+10)).Trim();
					 break;
				  }
			   }
			   if(name !="")
			   {
				  bool postFileArchiv = false;

				  for( int st = 0; st < SpisokFileKarantin->Count ; st++)
				  {
					 //TStringDynArray arr(SplitString(SpisokFileKarantin->Strings[st], "|"));

					 if(MyMd5 == ((DuplicatListFile*)SpisokFileKarantin->Items[st])->MD5)
					 {
						if(name != ((DuplicatListFile*)SpisokFileKarantin->Items[st])->FilePatch)
						{
						   UnicodeString fileOut;
						   fileOut = RenameFileName(ExtractFilePath(FileName) + ExtractFileName(name) + ".vir");

						   if(RenameFile(FileName,fileOut))
						   {
							  postFileArchiv = true;
							  String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
							  ZanoshuVListWiew3PriOshibki(fileOut, sizefile, result , MyMd5,SHA256,false);
							  break;
						   }
						   else
						   {
							  ErrorLog("Ошибка переименовывания файла " + FileName + "\n в " +fileOut +"\n");
							  break;
						   }
						}
						else
						{
						   postFileArchiv = true;
						   break;
						}
					 }//if(MyMd5 == arr[0])
					 Application->ProcessMessages();
				  }
				  if(!postFileArchiv)
				  {
					 UnicodeString fileOut;
					 fileOut = RenameFileName(ExtractFilePath(FileName) + ExtractFileName(name) + ".vir");

					 if(RenameFile(FileName,fileOut))
					 {
						////FileSpisokProwerki->Add(IntToStr(rescan) + fileOut);
						//FileSpisokProwerki->Add(IntToStr(rescan)+";" + fileOut+";" + IntToStr(sizefile) +";"+MyMd5);
						SpisokFileName->Add(new StructFileList(1,rescan,fileOut, sizefile,MyMd5,SHA256));
						SpisokFileKarantin->Add(new DuplicatListFile(name, MyMd5));
					 }
					 else
						ErrorLog("Ошибка переименовывания файла " + FileName + "\n в " +fileOut +"\n");

				  }
			   }//if(name !="")
			   else
			   {
				  bool postFileArchiv = false;

				  for( int st = 0; st < SpisokFileKarantin->Count ; st++)
				  {
					 //TStringDynArray arr(SplitString(SpisokFileKarantin->Strings[st], "|"));

					 if(MyMd5 == ((DuplicatListFile*)SpisokFileKarantin->Items[st])->MD5)
					 {
						if(name != ((DuplicatListFile*)SpisokFileKarantin->Items[st])->FilePatch)
						{
						   UnicodeString fileOut;
						   fileOut = RenameFileName(ExtractFilePath(FileName) + ExtractFileName(name) + ".vir");

						   if(RenameFile(FileName,fileOut))
						   {
							  postFileArchiv = true;
							  String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
							  ZanoshuVListWiew3PriOshibki(fileOut, sizefile, result , MyMd5,SHA256,false);
							  break;
						   }
						   else
						   {
							  ErrorLog("Ошибка переименовывания файла " + FileName + "\n в " +fileOut +"\n");
							  break;
						   }

						}
						else
						{
						   postFileArchiv = true;
						   break;
						}
					 }
					 Application->ProcessMessages();
				  }
				  if(!postFileArchiv)
				  {
					 UnicodeString fileOut;
					 fileOut = RenameFileName(ExtractFilePath(FileName) + ExtractFileName(ExstractFileNameBezExt(FileName)) + ".vir");

					 if(RenameFile(FileName,fileOut))
					 {
						////FileSpisokProwerki->Add(IntToStr(rescan) + fileOut);
						//FileSpisokProwerki->Add(IntToStr(rescan)+";" + fileOut+";" + IntToStr(sizefile) +";"+MyMd5);
						SpisokFileName->Add(new StructFileList(1,rescan,fileOut, sizefile,MyMd5,SHA256));
						SpisokFileKarantin->Add(new DuplicatListFile(name,MyMd5));
					 }
					 else
						ErrorLog("Ошибка переименовывания файла " + FileName + "\n в " +fileOut +"\n");
				  }
			   }
			}//if(FileExists(FileName + ".txt"))
			else
			{
			   bool postFileArchiv = false;

			   for( int st = 0; st < SpisokFileKarantin->Count ; st++)
			   {
				  //TStringDynArray arr(SplitString(SpisokFileKarantin->Strings[st], "|"));

				  if(MyMd5 == ((DuplicatListFile*)SpisokFileKarantin->Items[st])->MD5)
				  {
					 if(name != ((DuplicatListFile*)SpisokFileKarantin->Items[st])->FilePatch)
					 {
						UnicodeString fileOut;
						fileOut = RenameFileName(ExtractFilePath(FileName) + ExtractFileName(ExstractFileNameBezExt(FileName)) + ".vir");

						if(RenameFile(FileName,fileOut))
						{
						   postFileArchiv = true;
						   String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
						   ZanoshuVListWiew3PriOshibki(fileOut, sizefile, result , MyMd5, SHA256, false);
						   break;
						}
						else
						{
						   ErrorLog("Ошибка переименовывания файла " + FileName + "\n в " +fileOut +"\n");
						   break;
						}
					 }
					 else
					 {
						postFileArchiv = true;
						break;
					 }
				  }
				  Application->ProcessMessages();
			   }
			   if(!postFileArchiv)
			   {
				  UnicodeString fileOut;
				  fileOut = RenameFileName(ExtractFilePath(FileName) + ExtractFileName(ExstractFileNameBezExt(FileName)) + ".vir");

				  if(RenameFile(FileName,fileOut))
				  {
					 ////FileSpisokProwerki->Add(IntToStr(rescan) + fileOut);
					 //FileSpisokProwerki->Add(IntToStr(rescan)+";" + fileOut+";" + IntToStr(sizefile) +";"+MyMd5);
					 SpisokFileName->Add(new StructFileList(1,rescan,fileOut, sizefile,MyMd5,SHA256));
					 SpisokFileKarantin->Add(new DuplicatListFile(name, MyMd5));
				  }
				  else
					 ErrorLog("Ошибка переименовывания файла " + FileName + "\n в " +fileOut +"\n");
			   }
			}

		 }//if(MyMd5 !="")

	  }//if((sizefile >0) && (sizefile < 134271701))
	  else
		 OtWetErrorSizeFile(FileName, ExtractFilePath(FileName) + ExtractFileName(name) + ".vir", sizefile, MyMd5);

   }//if(FileExists(FileName))
}
//---------------------------------------------------------------------
void __fastcall TForm3::ParsingArchivTDSciller(UnicodeString FileName, TList *SpisokFileKarantin, TStringList *SpisokFileArchiv, int &i, int rescan)
{
   // проверяю присутствует файл ини или нет.
   if(FileExists(FileName))
   {
	  stat1->Panels->Items[0]->Text = L"Парсинг архива TDSciller. Файл " + ExtractFileName(FileName);
	  //Application->ProcessMessages();

	  __int64 sizefile = FileSizeStatic(FileName);
	  UnicodeString MyMd5="";
	  UnicodeString Sha256="";

	  if((sizefile >0) && (sizefile < 134271701))
	  {
		 MyMd5 = CalkFileMD5(FileName).LowerCase();
		 //Sha256 = Mysha256ver2(FileName);
		 //Использую стандартную функцию расчета хеш суммы
		 try
		 {
			Sha256 = THashSHA2::GetHashStringFromFile(TFileName(FileName), THashSHA2::TSHA2Version::SHA256);
		 }
		 catch(Exception &e)
		 {
			ErrorLog("Ошибка расчета SHA256.  ParsingArchivTDSciller()" +e.ToString());
		 }
		 //Application->ProcessMessages();
		 if( (MyMd5 !=""))
		 {
			if(FileExists(ExtractFilePath(FileName) + ExstractFileNameBezExt(ExtractFileName(FileName)) + ".ini") ==false)
			{

			   bool postFileArchiv = false;

			   for( int st = 0; st < SpisokFileKarantin->Count ; st++)
			   {
				  if(MyMd5 == ((DuplicatListFile*)SpisokFileKarantin->Items[st])->MD5)
				  {
					 if(FileName != ((DuplicatListFile*)SpisokFileKarantin->Items[st])->FilePatch)
					 {
						postFileArchiv = true;
						String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
						ZanoshuVListWiew3PriOshibki(FileName, sizefile, result , MyMd5, Sha256, false);
						break;
					 }
					 else
					 {
						postFileArchiv = true;
						break;
					 }
				  }
				  Application->ProcessMessages();
			   }
			   if(!postFileArchiv)
			   {
				  try
				  {
					 //RenameFile(SpisokFileArchiv->Strings[i],ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir");
					 SpisokFileKarantin->Add(new DuplicatListFile(FileName,MyMd5));
					 ////FileSpisokProwerki->Add(IntToStr(rescan) + FileName);
					 //FileSpisokProwerki->Add(IntToStr(rescan)+";" + FileName+";" +IntToStr(sizefile) +";"+MyMd5);
					 SpisokFileName->Add(new StructFileList(1,rescan,FileName, sizefile,MyMd5,Sha256));
				  }
				  catch(...)
				  {
					 ;
				  }
			   }

			} // if(FileExists(ExtractFilePath(FileName) + ExstractFileNameBezExt(ExtractFileName(FileName)) + ".ini") ==false)

			std::auto_ptr<TStringList> Zapros (new TStringList(NULL) );
			UnicodeString name = "";
			bool boolextName = false;

			try
			{
			   Zapros->LoadFromFile(ExtractFilePath(FileName) + ExstractFileNameBezExt(ExtractFileName(FileName)) + ".ini");
			}
			catch(...)
			{
			   ;
			}

			for(int q=0; q<Zapros->Count;q++)
			{
			   if(Zapros->Strings[q].Pos("Src") !=0)
			   {
				  name = Zapros->Strings[q].SubString(Zapros->Strings[q].Pos("Src")+4, Zapros->Strings[q].Length()-(Zapros->Strings[q].Pos("Src")+3));

				  if(name !="")
				  {
					  boolextName = true;
					  bool postFileArchiv = false;

					  for( int st = 0; st < SpisokFileKarantin->Count ; st++)
					  {
						 //TStringDynArray arr(SplitString(SpisokFileKarantin->Strings[st], "|"));
						 if(MyMd5 == ((DuplicatListFile*)SpisokFileKarantin->Items[st])->MD5)
						 {
							if(name != ((DuplicatListFile*)SpisokFileKarantin->Items[st])->FilePatch)
							{
							   UnicodeString fileOut;
							   fileOut = RenameFileName(ExtractFilePath(FileName) + ExtractFileName(name) + ".vir");

							   if(RenameFile(FileName,fileOut))
							   {
								  postFileArchiv = true;
								  String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
								  ZanoshuVListWiew3PriOshibki(fileOut, sizefile, result , MyMd5, Sha256, false);
								  break;
							   }
							   else
							   {
								  ErrorLog("Ошибка переименовывания файла " + FileName + "\n в " +fileOut +"\n");
							   }
							}
							else
							{
							   postFileArchiv = true;
							   break;
							}
						 }
					  }
					  if(!postFileArchiv)
					  {
						 UnicodeString nameFy = RenameFileName(ExtractFilePath(FileName) + ExtractFileName(name) + ".vir");

						 if(RenameFile(FileName,nameFy))
						 {
							////FileSpisokProwerki->Add(IntToStr(rescan) + nameFy);
							//FileSpisokProwerki->Add(IntToStr(rescan)+";" + nameFy+";" + IntToStr(sizefile) +";"+MyMd5);
							SpisokFileName->Add(new StructFileList(1,rescan,nameFy, sizefile,MyMd5,Sha256));
							SpisokFileKarantin->Add(new DuplicatListFile(name, MyMd5));
						 }
						 else
							ErrorLog("Ошибка переименовывания файла " + FileName + "\n в " +nameFy +"\n");

					  }
					  // Надо будет переправить на использования TList.
					  //if(FileSpisokProwerki->IndexOf(ExtractFilePath(FileName) + "object.ini") != -1)
						 //FileSpisokProwerki->Delete(FileSpisokProwerki->IndexOf(ExtractFilePath(FileName)+ "Object.ini"));

					  // этот участок протестировать
					  //------------------Begin.

					  if(SpisokFileArchiv->IndexOf(ExtractFilePath(ExtractFileDir(FileName)) + "object.ini") != -1)
					  {
						 SpisokFileArchiv->Delete(SpisokFileArchiv->IndexOf(ExtractFilePath(ExtractFileDir(FileName))+ "Object.ini"));
						 --i;
					  }
					  //------------------End
					  // Надо будет переправить на использования TList.
					  //if(FileSpisokProwerki->IndexOf(ExtractFilePath(ExtractFileDir(FileName)) + "object.ini") != -1)
						 //FileSpisokProwerki->Delete(FileSpisokProwerki->IndexOf(ExtractFilePath(ExtractFileDir(FileName)) + "object.ini"));
				  }
			   }
			   // Добавить условие если в ини нет Src. То есть пути к файлу.
			   // Надо будет обробатывать "object.ini"
			}
			if(!boolextName)
			{
			   UnicodeString NameIni = "";
			   UnicodeString PatchIni = "";

			   for(int q=0; q<Zapros->Count;q++)
			   {
				  int post = Zapros->Strings[q].Pos("Name:");

				  if( post !=0)
				  {
					 NameIni = Zapros->Strings[q].SubString(post + 5, Zapros->Strings[q].Length() - (post+4));
				  }
			   }

			   if(NameIni =="")
			   {
				  for(int q=0; q<Zapros->Count;q++)
				  {
					 int post = Zapros->Strings[q].Pos("Type:");

					 if( post !=0)
					 {
						NameIni = Zapros->Strings[q].SubString(post + 5, Zapros->Strings[q].Length() - (post+4));
					 }
				  }
			   }
			   Zapros->Clear();

			   try
			   {
				  Zapros->LoadFromFile(ExtractFilePath(FileName) + "object.ini");
			   }
			   catch(...)
			   {
				  ;
			   }

			   for(int q=0; q<Zapros->Count; q++)
			   {
				  int post = Zapros->Strings[q].Pos("Name:");

				  if( post !=0)
				  {
					 PatchIni = Zapros->Strings[q].SubString(post + 5, Zapros->Strings[q].Length() - (post+4));
				  }
			   }

			   if(NameIni !="" && PatchIni !="")
			   {
				  name = PatchIni + "\\" + NameIni;
				  bool postFileArchiv = false;

				  for( int st = 0; st < SpisokFileKarantin->Count ; st++)
				  {
					 //TStringDynArray arr(SplitString(SpisokFileKarantin->Strings[st], "|"));
					 if(MyMd5 == ((DuplicatListFile*)SpisokFileKarantin->Items[st])->MD5)
					 {
						if(name != ((DuplicatListFile*)SpisokFileKarantin->Items[st])->FilePatch)
						{
						   UnicodeString fileOut;
						   fileOut = RenameFileName(ExtractFilePath(FileName) + ExtractFileName(name) + ".vir");

						   if(RenameFile(FileName,fileOut))
						   {
							  postFileArchiv = true;
							  String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
							  ZanoshuVListWiew3PriOshibki(fileOut, sizefile, result , MyMd5, Sha256, false);
							  break;
						   }
						   else
						   {
							  ErrorLog("Ошибка переименовывания файла " + FileName + "\n в " +fileOut +"\n");
						   }

						}
						else
						{
						   postFileArchiv = true;
						   break;
						}
					 }
				  }
				  if(!postFileArchiv)
				  {
					 UnicodeString nameFy = RenameFileName(ExtractFilePath(FileName) + ExtractFileName(name) + ".vir");

					 if(RenameFile(FileName,nameFy))
					 {
						//FileSpisokProwerki->Add(IntToStr(rescan)+";" + nameFy+";" +IntToStr(sizefile) +";"+MyMd5);
						SpisokFileName->Add(new StructFileList(1,rescan,nameFy, sizefile,MyMd5,Sha256));
						SpisokFileKarantin->Add(new DuplicatListFile(name, MyMd5));
					 }
					 else
						ErrorLog("Ошибка переименовывания файла " + FileName + "\n в " +nameFy +"\n");

				  }
				  // из списка удаляю две последние записи. так как там будет файл "object.ini"
				  //закоментировал. надо будет переправить на использоание TList
				  //if(FileSpisokProwerki->IndexOf(ExtractFilePath(FileName) + "object.ini") != -1)
					 //FileSpisokProwerki->Delete(FileSpisokProwerki->IndexOf(ExtractFilePath(FileName)+ "Object.ini"));

					 /*if(SpisokFileArchiv->IndexOf(ExtractFilePath(SpisokFileArchiv->Strings[i]) + "object.ini") != -1)
					 {
						SpisokFileArchiv->Delete(SpisokFileArchiv->IndexOf(ExtractFilePath(SpisokFileArchiv->Strings[i])+ "Object.ini"));
						--i;
					 } */

					 // этот участок протестировать
					 //------------------Begin.

				  if(SpisokFileArchiv->IndexOf(ExtractFilePath(ExtractFileDir(FileName)) + "object.ini") != -1)
				  {
					 SpisokFileArchiv->Delete(SpisokFileArchiv->IndexOf(ExtractFilePath(ExtractFileDir(FileName))+ "Object.ini"));
					 --i;
				  }
					 //------------------End
					//закоментировал. надо будет переправить на использоание TList
				  //if(FileSpisokProwerki->IndexOf(ExtractFilePath(ExtractFileDir(FileName)) + "object.ini") != -1)
					 //FileSpisokProwerki->Delete(FileSpisokProwerki->IndexOf(ExtractFilePath(ExtractFileDir(FileName)) + "object.ini"));
			   }//End когда нет поля name
			}//if(!boolextName)
		 }//if( (MyMd5 !="") && (MyMd5) != "nou")
	  }

	  else
	  {
		 OtWetErrorSizeFile(FileName, "", sizefile, MyMd5);//otwet oshibkaSize
	  }
   }

}

//---------------------------------------------------------------------
void __fastcall TForm3::ParsingArchiv(UnicodeString DirPatchArchiv, int rescan)
{
   TStringList * SpisokFileArchiv = new TStringList(NULL);
   TList *ListFileKarantin = new TList();

   SpisokFileArchiv->Text = MyFiledDirArchiv(DirPatchArchiv);

   stat1->Panels->Items[0]->Text = L"Парсинг архива";

   if( SpisokFileArchiv->Text !="")
   {

		 for (int i = 0; i < SpisokFileArchiv->Count; i++)
		 {

			 //++++++++Begin AVZ .dta++++++++
			 if(ExtractFileExt(SpisokFileArchiv->Strings[i]) ==".dta"&&
				ExtractFileName(SpisokFileArchiv->Strings[i]).LowerCase().Pos("avz")!=0)
				  ParsingArchivAVZ_dta(SpisokFileArchiv->Strings[i], ListFileKarantin, rescan);


			 else if(ExtractFileExt(SpisokFileArchiv->Strings[i]).LowerCase() ==".ini" &&
					 ExtractFileName(SpisokFileArchiv->Strings[i]).LowerCase().Pos("avz")!=0)
			 {
				  // Делаю так по тому, что если не прописать это, то файлы .ini карантинов занесутся на проверку.
				;
			 }
			 ////++++++++ EndAVZ .dta++++++++

			 ////++++++++Begin AVZ .dat++++++++
			 else if(ExtractFileExt(SpisokFileArchiv->Strings[i]).LowerCase() == ".dat" &&
					 ExtractFileName(SpisokFileArchiv->Strings[i]).LowerCase().Pos("bcqr") !=0)
				 ParsingArchivAVZ_dat(SpisokFileArchiv->Strings[i], ListFileKarantin, rescan);

			 else if(ExtractFileExt(SpisokFileArchiv->Strings[i]).LowerCase() == ".ini" &&
					 ExtractFileName(SpisokFileArchiv->Strings[i]).LowerCase().Pos("bcqr")!=0)
			 {
				;
			 }
			 ////++++++++end AVZ .dat++++++++

			 //  Плохая проверка парсинка карантина UVS где расширение файла
			 // имеет хеш сумм SHA1 пример ADSNDS.DLL._BAF0F1105A8247832657DBDB4924B88D384C7E9A

			 else
			 if(ExtractFileExt(SpisokFileArchiv->Strings[i]).Length() == 42)
				ParsingArchivUVS(SpisokFileArchiv->Strings[i],ListFileKarantin,rescan);

			  // провожу проверку файлов "txt". Если второе расширение имеет 42 символа,
			  // то это описание файла из карантина UVS
			  //пример ADSNDS.DLL._BAF0F1105A8247832657DBDB4924B88D384C7E9A.txt
			  //При этом этот файл не заносится в список.
			 else if(ExtractFileExt(SpisokFileArchiv->Strings[i]).LowerCase() == ".txt")
			 {
				 if(ExtractFileExt(ChangeFileExt(SpisokFileArchiv->Strings[i],"")).Length() !=42)
					 //FileSpisokProwerki->Add(IntToStr(rescan) +";" + SpisokFileArchiv->Strings[i]);
					 SpisokFileName->Add(new StructFileList(0,rescan,SpisokFileArchiv->Strings[i], 0,UnicodeString(""),UnicodeString("")));
			 }

			 // парсинг карантина tdskillera
			 else if(ExtractFileExt(SpisokFileArchiv->Strings[i]) ==".dta"&&
					 ExtractFileName(SpisokFileArchiv->Strings[i]).LowerCase().Pos("tsk")!=0)
				 ParsingArchivTDSciller(SpisokFileArchiv->Strings[i],ListFileKarantin,  SpisokFileArchiv, i, rescan);

			 else if(ExtractFileExt(SpisokFileArchiv->Strings[i]).LowerCase() == ".ini" &&
					 ExtractFileName(SpisokFileArchiv->Strings[i]).LowerCase().Pos("tsk")!=0)
			 {
				;
			 }
			 // end парсинг карантина tdskillera
			 else
				//FileSpisokProwerki->Add(IntToStr(rescan) +";"+ SpisokFileArchiv->Strings[i]);
				SpisokFileName->Add(new StructFileList(0,rescan,SpisokFileArchiv->Strings[i], 0,UnicodeString(""),UnicodeString("")));

			 Application->ProcessMessages();
		 }
		 stat1->Panels->Items[0]->Text = L"Парсинг окончен";
		 stat1->Panels->Items[0]->Text = L"";
   }
   else
   {
	  stat1->Panels->Items[0]->Text = L"Список пуст, похоже архив пустой.";
	  stat1->Panels->Items[0]->Text = L"";

	  ErrorLog("Список пуст, похоже архив пустой.\n" +DirPatchArchiv);
   }
	delete SpisokFileArchiv;
	//delete SpisokFileKarantin;

	for (int i = 0; i < ListFileKarantin->Count; i++) {
		delete (DuplicatListFile*)ListFileKarantin->Items[i];
		ListFileKarantin->Delete(i);

	}
	delete  ListFileKarantin;
}
//+++++++++++++++++++++++++++++++++++++
void __fastcall TForm3::ExtArchiv(UnicodeString ArcName, int rescan)
{
	// Создаю по каждой директории отдельно. А то ошибка при распаковке, если нет
	// директории.
	UnicodeString NameArchiv = ArcName;
	//путь DirPatchArchiv который я передаю в функцию ParsingArchiv
	//путь Dir который я передаю в функцию extract2

	UnicodeString Dir = "", head="", DirPatchArchiv = "";

  stat1->Panels->Items[0]->Text = L"Начало распаковки архива";


  if(ArcName.LowerCase().Pos("aitotaltmp\\archiw")==0)
  {
	UnicodeString dateAT = "_" + FormatDateTime("YYYY.MM.DD",Date())+ "_" + FormatDateTime("HH-NN-SS",Time());

	if(!TDirectory::Exists(ExtractFilePath(Application->ExeName) + "AitotalTMP"))
	   TDirectory::CreateDirectoryW(ExtractFilePath(Application->ExeName) + "AitotalTMP");

	Dir = ExtractFilePath(Application->ExeName) + "AitotalTMP\\archiw\\";
	TDirectory::CreateDirectoryW(Dir);
	Dir += ExtractFileName(ArcName+dateAT); // Если, что здесь добавить +"\\" ,
	TDirectory::CreateDirectoryW(Dir);
	DirPatchArchiv = Dir;
	ArcName =String(L"\\\\?\\")+ArcName;
	Dir = String(L"\\\\?\\")+Dir;
	SpisokArchivFolder->Add(Dir);
  }

   unsigned int GuidArchiv;

   if(Read_head_file_New(ArcName, head))
   {
	  //if(Parse_Head_Guid(head, GuidArchiv))
	  if(Parse_Head_Guid_New(head, GuidArchiv))
	  {
	

			 std::auto_ptr<TStringList>Password(new TStringList(NULL));

			 Password->Add("virus");
			 Password->Add("infected");
			 Password->Add("malware");
			 Password->Add("AntiSMS.com");

			 if(TFile::Exists(PatchProgramma + "tools\\password.txt"))
			 {
				 std::auto_ptr<TStringList>PasswordFile(new TStringList(NULL));


				 PasswordFile->LoadFromFile(PatchProgramma + "tools\\password.txt");

				 // maxPass ограничение на количество пароля не более 10.
				 for(int i=0, maxPass=0; i<PasswordFile->Count; ++i)
				 {
					 if(maxPass<10)
					 {
						 if(PasswordFile->Strings[i] !="")
						 {
							 Password->Add(PasswordFile->Strings[i]);
							 ++maxPass;
						 }
					 }
				 }
			 }
			 Mapasswor = false;

			 for(int i = 0; i < Password->Count; i++)
			 {
				 switch(extArchivToPassword(ArcName.w_str(), Dir.w_str(), Password->Strings[i].w_str(), GuidArchiv))
				 {
					 case EX_OK:
						 ParsingArchiv(DirPatchArchiv, rescan);
						 return;
					 case errorlibdll:
						 if(AtOptions.ErrorArchiv)
							 ShowMessage(L"Ошибка распаковки архива.\n" + NameArchiv);
						 ErrorLog(L"Ошибка распаковки архива. " + head +L"\n"+ NameArchiv);
						 return;

					 case erroropenarchiv:
						 if(AtOptions.ErrorArchiv)
							 ShowMessage(L"Ошибка распаковки архива.\n" + NameArchiv);
						 ErrorLog(L"Ошибка распаковки архива. " + head +L"\n"+ NameArchiv);
						 return;
					 case errorHead:
						 if(AtOptions.ErrorArchiv)
							 ShowMessage(L"Ошибка распаковки архива.\n" + NameArchiv);
						 ErrorLog(L"Ошибка распаковки архива. " + head +L"\n"+ NameArchiv);
						 return;

					 case errorDataPass:
						 continue;
						 //break;  //Закоментировал 03.06.2021 Что бы убрать варнинг

					 /*default:
						 if(AtOptions.ErrorArchiv)
							 ShowMessage(L"Ошибка распаковки архива.\n" + NameArchiv);
						 ErrorLog(L"Ошибка распаковки архива. " + head +L"\n"+ NameArchiv);
						 return;*/
				 }
			 }
			 FParoll = new TFParoll(this);
			 FParoll->NameArchiv = ArcName;
			 FParoll->Dir = Dir;
			 FParoll->GuidArchiv = GuidArchiv;
			 FParoll->ShowModal();


			 if(Mapasswor == true)
				ParsingArchiv(DirPatchArchiv, rescan);

		   //-----------End-----------
		 //}
	  }
	  //else
		 //ShowMessage(L"Ошибка распаковки архива.-2");
   }
   //else
	  //ShowMessage(L"Ошибка распаковки архива.-1");

  stat1->Panels->Items[0]->Text = L"";
}

// +++++++++++++++++++++++++++++++++++++++++++++++++
UnicodeString TForm3::ExstractFileNameBezExt(UnicodeString NameFile)
{
   return ChangeFileExt(NameFile,"");
}
//---------------------------------------------------------------------------

void __fastcall TForm3::MyOption(TObject *Sender)
{
   MyOptionsForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::OptionReadIni()
{
	std::auto_ptr<TStringStream>s(new TStringStream());
	IniOptions = new TIniFile(ExtractFilePath(Application->ExeName) + "tools\\options.ini");
	AtOptions.ProxyVT.ProxiChecked = IniOptions->ReadBool("Network","ProxiChecked" , false);

	if(AtOptions.ProxyVT.ProxiChecked != false)
	{
	   AtOptions.ProxyVT.Proxy = IniOptions->ReadString("Network","Proxi"," ");
	   AtOptions.ProxyVT.IpPort = IniOptions->ReadInteger("Network","IpPort",0);
	   AtOptions.ProxyVT.OptProxiLogin = IniOptions->ReadString("Network","ProxiLogin"," ");
	   AtOptions.ProxyVT.Socket = IniOptions->ReadInteger("Network","Socket",0);
	   IniOptions->ReadBinaryStream("Network","ProxiPassword",s.get());
	   AtOptions.ProxyVT.OptProxiPassword = MyCryptDeCrypt(s->DataString,true);

	}
	AtOptions.Exit_Trei = IniOptions->ReadBool("Tools", "Exit_Tray", false);
	//Если труе ведем лог работы в потоке сканирования.
	logirovanie = IniOptions->ReadBool("test", "logirovanie", false);
	AtOptions.BallonHint = IniOptions->ReadBool("Tools", "BallonHint", false);
	AtOptions.FileCount = IniOptions->ReadInteger("Tools", "FileCount",5);
	AtOptions.Thread =  IniOptions->ReadInteger("Tools", "ThreadCount",20);
	AtOptions.ErrorArchiv = IniOptions->ReadBool("Tools","MessageErrorArchive", false);
	ApikeyCount = IniOptions->ReadInteger("Tools", "ApikeyCount",0);

	if(ApikeyCount == 0)
	{
	   ApikeyCount == 1;
	   AtOptions.Apikey.reserve(1);
	   AtOptions.Apikey.insert(AtOptions.Apikey.end(),IniOptions->ReadString("Tools", "apikey", "ae3b8044123881cd4f04fa92a709ed6132413ea79bf2f09917b04df5abe47ef9"));
	}
	else
	{
	   AtOptions.Apikey.reserve(ApikeyCount);

	   for(int i=0; i < ApikeyCount;i++)
	   {
		 if(i == 0)
		 {
			UnicodeString key = IniOptions->ReadString("Tools", "apikey", "3c04a612f2bf23e46dc857ffa0655544ea3a9d0d3c25b007057908eb7c8ca7b1");
			AtOptions.Apikey.insert(AtOptions.Apikey.end(),key);
		 }
		 else
		 {
			UnicodeString num = L"apikey"+IntToStr(i);
			UnicodeString key = IniOptions->ReadString("Tools", num, "3c04a612f2bf23e46dc857ffa0655544ea3a9d0d3c25b007057908eb7c8ca7b1");
			AtOptions.Apikey.insert(AtOptions.Apikey.end(),key);
		 }
	   }
	}
	Application->ProcessMessages();
	delete IniOptions;

	//Добавил, что бы данные сразу вступили в силу.
	if(!StopThread)
			StartThreadVT();
}
//******************************************************
void __fastcall TForm3::MyFiledDir(UnicodeString Dir, int rescan)
{
	TStringDynArray Files;
	String Mask= "*"; //  "*.txt"
	try
	{
	   Files = TDirectory::GetFiles(Dir, Mask ,TSearchOption::soAllDirectories );

	   for(int i=0; i<Files.Length; i++)
	   {
		  if(ExtractFileExt(Files[i].LowerCase()) == ".rar" ||
			 ExtractFileExt(Files[i].LowerCase()) == ".zip" ||
			 ExtractFileExt(Files[i].LowerCase()) == ".7z"
			 )
		  {
			 ExtArchiv(Files[i],rescan);
		  }
		  else
			 SpisokFileName->Add(new StructFileList(0,rescan,Files[i], 0,UnicodeString(""),UnicodeString("")));
	   }
	}
	catch (EDirectoryNotFoundException  &d)
	{
	   if(ExtractFileExt(Dir.LowerCase()) == ".rar" ||
		  ExtractFileExt(Dir.LowerCase()) == ".zip" ||
		  ExtractFileExt(Dir.LowerCase()) == ".7z"
		  )
	   {
		  ExtArchiv(Dir,rescan);
	   }
	   else
		 SpisokFileName->Add(new StructFileList(0,rescan,Dir, 0,UnicodeString(""),UnicodeString("")));
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::MyTools(TObject *Sender)
{
   Application->CreateForm(__classid(TMyOptionsForm), &MyOptionsForm);
   MyOptionsForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::PauseT(AnsiString pause_str)
{
   TDateTime t,tpause;
   bool stop=false;
   tpause=StrToDateTime(pause_str);
   t=Now();
   while(!stop)
   {
	  stop=(Now()-t)>tpause;
	  //что бы не грузился процессор, а то грузит на 25%
	  //Sleep(1);
	  ///Application->ProcessMessages();
   }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//---------------------------------------------------------------------------
void __fastcall TForm3::PauseProba(AnsiString pause_str)
{
   TDateTime t,tpause;
   bool stop=false;
   tpause=StrToDateTime(pause_str);
   t=Now();
   while(!stop)
   {
	  stop=(Now()-t)>tpause;
	  //что бы не грузился процессор, а то грузит на 25%
	  Sleep(1000);
	  Application->ProcessMessages();
   }
}
//++++++++++++++++++
void __fastcall TForm3::InsertToListView(int numberListView, Strukt FileItem, int FileNumber)
{
   if(numberListView == 1)
   {
	  ListItem = Form3->ListView1->Items->Add();
	  ListItem->Caption = ExtractFileName(FileItem.MyPatch);
	  ListItem->SubItems->Add(FileItem.MyPatch);
	  ListItem->SubItems->Add(FileItem.SizeFile);
	  ListItem->SubItems->Add(FileItem.Md5File);
	  ListItem->SubItems->Add(FileItem.SHA256);
	  ListItem->Data = (void*) new numchek(FileNumber,FileItem.Cheked);
   }
   if(numberListView == 3)
   {
	  ListItem = Form3->ListView3->Items->Add();
	  ListItem->Caption = ExtractFileName(FileItem.MyPatch);
	  ListItem->SubItems->Add(FileItem.MyPatch);
	  ListItem->SubItems->Add("");
	  ListItem->SubItems->Add(FileItem.SizeFile);
	  ListItem->SubItems->Add(FileItem.Md5File);
	  ListItem->SubItems->Add(FileItem.SHA256);
	  ListItem->SubItems->Add("");
	  ListItem->SubItems->Add("");
   }
}

//++++++++++++++++++++++++++
void __fastcall TForm3::CopyTListToListViewofStream()
{
	//std::auto_ptr<TStringList> SpisokFileDublicats (new TStringList(NULL) );
	TList* ListFileDublicats =  new TList();

	if(SpisokFileName->Count != 0)
	{
		Strukt FileItem;
		FileItem.SHA256 = "";
		for (int i = 0; i < SpisokFileName->Count; i++)
		{
			stat1->Panels->Items[0]->Text = L"Разбор списка файлов. Обработано " + String(i*100/ SpisokFileName->Count) +L"%";

			FileItem.Cheked = ((StructFileList*)SpisokFileName->Items[i])->rescan;
			FileItem.MyPatch = ((StructFileList*)SpisokFileName->Items[i])->filename;

			if(!((StructFileList*)SpisokFileName->Items[i])->parsing)
			{

				try
				{
					TFileStream *fs;
					if(FileItem.MyPatch.Length()>253)// Проверять длину пути до файла, если он превышает 253 символа то в переди ставлю "\\\\?\\"
						fs = new TFileStream(String("\\\\?\\"+FileItem.MyPatch), fmOpenRead | fmShareDenyNone);
					else
						fs = new TFileStream(FileItem.MyPatch, fmOpenRead | fmShareDenyNone);

					FileItem.SizeFile = fs->Size;

					if((FileItem.SizeFile <=0)  || (FileItem.SizeFile > 134271701))
						OtWetErrorSizeFile(FileItem.MyPatch,"", FileItem.SizeFile, "");
					else
					{
                        try
						{
							FileItem.SHA256 = THashSHA2::GetHashString(fs, THashSHA2::TSHA2Version::SHA256);
						}
						catch(Exception &E)
						{
							stat1->Panels->Items[1]->Text  = "Error SHA256.1";
							ErrorLog("Ошибка расчета Sha256.1  CopyTListToListViewofStream()\n" + E.ToString());
						}

						FileItem.Md5File = CalksStreamMD5(fs).LowerCase();



						if(FileItem.Md5File == "")
						{
							ZanoshuVListWiew3PriOshibki(FileItem.MyPatch,
												  FileItem.SizeFile,
												  "Ошибка определения хеш суммы. Возможно файл не доступен.",
												  "",
												  "",
												  false);
							ErrorLog(FileItem.MyPatch +"\nОшибка определения MD5");
							continue;
						}
						else
						{
							bool dublicat = false;

							for(int st = 0; st<ListFileDublicats->Count;st++)
							{
								//Проверяю если МД5 совпадает и Путь до файла совпадают, то это дубликат.
								if(FileItem.Md5File == ((DuplicatListFile*)ListFileDublicats->Items[st])->MD5 &&
									ExtractFilePath(FileItem.MyPatch).LowerCase() ==  ExtractFilePath(((DuplicatListFile*)ListFileDublicats->Items[st])->FilePatch.LowerCase()))
								{
									dublicat = true;
									break;
								}
							}

							if(!dublicat)
							{
								InsertToListView(1, FileItem, FileNumber);
								ListFileDublicats->Add(new DuplicatListFile(FileItem.MyPatch,FileItem.Md5File));
							}
							else
								InsertToListView(3, FileItem, FileNumber);
						}
					}
					delete fs;
				}
				catch(Exception &E)
				{
					ShowMessage("Ошибка = " + FileItem.MyPatch +  "\n Не удалось открыть файл.\n" + E.ToString());
					ErrorLog("Ошибка = " + FileItem.MyPatch +  "\n Не удалось открыть файл. CopyTListToListViewofStream(). \n" + E.ToString());
				}
			}
			else
			{
				FileItem.SizeFile = ((StructFileList*)SpisokFileName->Items[i])->SizeFile;
				FileItem.Md5File = ((StructFileList*)SpisokFileName->Items[i])->Md5File;
				FileItem.SHA256 =  ((StructFileList*)SpisokFileName->Items[i])->SHA256;
				InsertToListView(1, FileItem, FileNumber);
			}
			FileNumber++;
			Application->ProcessMessages();
		}

		stat1->Panels->Items[0]->Text = "";
		ClearSpisokFileName();
	}
	//очищаю список дубликатов.
	for (int i = 0; i < ListFileDublicats->Count; i++)
	{
		//DuplicatListFile *pMyObj = (DuplicatListFile*)ListFileDublicats->Items[i];
		delete (DuplicatListFile*)ListFileDublicats->Items[i];
		ListFileDublicats->Delete(i);
	}
	ListFileDublicats->Clear();
	delete  ListFileDublicats;
}

//+++++++++++++++++++++++
void TForm3::ScanFile()
{
   StopThread = false;
   StopThreadPopupMenu = false;
   StopInet= false;
   CopyTListToListViewofStream();

   if(ProgressBar1->Max <=0)
	  ProgressBar1->Position = 0;

   ProgressBar1->Max = ListView1->Items->Count;
   StartThreadVT();
}
//+++++++++++++++++++++++++++++++++++++++++++++++
void TForm3::StartThreadVT()
{
	bool stop = false;

	while(stop == false)
	{
	   if(ListView1->Items->Count ==0)
		 break;

	   if(StopThread)
				break;

	   if(!GetStatusConnect())
	   {
			 if(!StopInet)
			 {
				StopInet = true;
				ShowMessage("Интернет не подключен");
			 }
			 break;
	   }

		for(int q=0;q <= AtOptions.Thread;q++)
		  {
			 if(StopThread)
				break;

			 if(q == AtOptions.Thread)
			 {
				stop = true;
				break;
			 }

			 if(VTIndy[q] ==NULL )
			 {
				if(ListView1->Items->Count ==0)
				   break;

				filenumber=q;

				VTIndy[filenumber] = new ScanVTIndy(true);

				//Увеличиваю время на 10 мин.
				//Prob = Prob +10./24/60;
				TDateTime Prob = IncMinute(Now(), 10); //Увеличиваю время на 10 мин.
				VTIndy[filenumber]->TimeBreak = FormatDateTime("YYYY.MM.DD HH:NN:SS",Prob);
				VTIndy[filenumber]->VtBase.BasePatchFileName = ListView1->Items->Item[0]->SubItems->Strings[0];
				VTIndy[filenumber]->VtBase.BaseMD5 = ListView1->Items->Item[0]->SubItems->Strings[2];
				VTIndy[filenumber]->VtBase.BaseSizeFile = ListView1->Items->Item[0]->SubItems->Strings[1].ToInt();
				//Устанавливаем вести логирование или нет. Читается в ини файле.
				VTIndy[filenumber]->logirovanie =  logirovanie;
				VTIndy[filenumber]->VtBase.BaseSHA256 = ListView1->Items->Item[0]->SubItems->Strings[3];
				VTIndy[filenumber]->VtBase.BasePredScanData = FormatDateTime("YYYY.MM.DD HH:NN:SS",Now());
				VTIndy[filenumber]->PotokNumber = filenumber;
				VTIndy[filenumber]->FileNumber = ((numchek*)ListView1->Items->Item[0]->Data)->NumBer;
				VTIndy[filenumber]->VtBase.FileNumber = ((numchek*)ListView1->Items->Item[0]->Data)->NumBer;
				VTIndy[filenumber]->OnTerminate = &AtThreadTerminated;

				VTIndy[filenumber]->ScanIndyVT.Proverka = false;
				//проверяем если в коммандной строке мы отправили 1 то смотрим ещё на ChReSca->Checked;
				// если 0 то это norescan и мы сразу передаём 0
				if(((numchek*)ListView1->Items->Item[0]->Data)->rescan)
				   VTIndy[filenumber]->ScanIndyVT.Rescan = ChReSca->Checked;
				else
				   VTIndy[filenumber]->ScanIndyVT.Rescan = false;


				if(AtOptions.ProxyVT.ProxiChecked)
				{
				   VTIndy[filenumber]->ScanIndyVT.ProxyVT = AtOptions.ProxyVT;
				}

				if(ApikeyNumber >= ApikeyCount)
				{
				   ApikeyNumber = 0;
				}

				VTIndy[filenumber]->ScanIndyVT.ApiKey = AtOptions.Apikey[ApikeyNumber];
				VTIndy[filenumber]->Start();
				ListView1->Items->Item[0]->Delete();
				ListView1->Repaint();
				filenumber++;
				ApikeyNumber++;
				ProgressBar1->StepBy(1);
				Application->ProcessMessages();
			 }
		  }
	}

}

void __fastcall TForm3::CreateStartVT(Base VT,bool Proverka, int ApikeyNumber)
{

   int i = 0;

   for(int q=50;q < 100;q++)
   {
	  if(q == AtOptions.Thread)
	  {
		 q=50;
		 PauseT("00:00:01");
	  }

	  if(VTIndy[q] ==NULL)
	  {
		 i=q;
		 break;
	  }
   }

   if(!GetStatusConnect())
   {
	  if(!StopInet)
	  {
		 StopInet = true;
		 ShowMessage("Интернет не подключен");
	  }
	  return;
   }


   VTIndy[i] = new ScanVTIndy(true);
   VTIndy[i]->VtBase = VT;
   //Заношу номер потока для последующей проверки есть поток или нет.
   // В коде потока значение удаляется.
   SpisokNamePotok->Add(i);
   VTIndy[i]->PotokNumber = i;
   VTIndy[i]->ScanIndyVT.Proverka = Proverka;
   VTIndy[i]->OnTerminate = &AtThreadTerminated;
   VTIndy[i]->ScanIndyVT.Rescan = ChReSca->Checked;
   VTIndy[i]->FileNumber = VT.FileNumber;

   if(AtOptions.ProxyVT.ProxiChecked)
   {
	  VTIndy[i]->ScanIndyVT.ProxyVT = AtOptions.ProxyVT;
   }

   VTIndy[i]->ScanIndyVT.ApiKey = AtOptions.Apikey[ApikeyNumber];
   VTIndy[i]->Start();

}

//+++++++++++++++++++++++++++++++++++++++++
void __fastcall TForm3::AtThreadTerminated(TObject *Sender)
{
   for(int i= 0; i<100; i++)
   {
	  if(VTIndy[i] == Sender)
	  {
		 /* PlaySound("ZVUK1",0, SND_RESOURCE | SND_ASYNC);
		 //Закоментировал звук при завершение потока
			PauseT("00:00:04");
			TrayIcon1->BeforeDestruction();
		 */

		 VTIndy[i] = NULL;
		 delete VTIndy[i];
		 if(!StopThread)
			StartThreadVT();
		 break;
	  }
   }
}
//++++++++++++++++++++++++++++++++++++++++
void __fastcall TForm3::Pm3Delete(TObject *Sender)
{
	if (ListView3->Selected != NULL)
	{
		ListView3->DeleteSelected();
		ListView3->Repaint();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm1Delete(TObject *Sender)
{
   if (ListView1->Selected != NULL)
   {
		ListView1->DeleteSelected();
		ListView1->Repaint();
   }
	ProgressBar1->Max = ListView1->Items->Count;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm3CopyNameFile(TObject *Sender)
{
	if (ListView3->Selected != NULL)
	{
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView3->Selected->Caption;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm3CopyPatchFiele(TObject *Sender)
{
   if (ListView3->Selected != NULL)
	{
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ExtractFilePath(ListView3->Selected->SubItems->Strings[0]);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm3CopyPatchNameFile(TObject *Sender)
{
   if (ListView3->Selected != NULL)
	{
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView3->Selected->SubItems->Strings[0];
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm3CopyHeshMd5(TObject *Sender)
{
	if (ListView3->Selected != NULL)
	{
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView3->Selected->SubItems->Strings[3];
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm3CopyAdres(TObject *Sender)
{
	if (ListView3->Selected != NULL)
	{
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView3->Selected->SubItems->Strings[6];
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm3OpenDirFile(TObject *Sender)
{
   if (ListView3->Selected != NULL)
   {
	  UnicodeString patch = "/select, " + ListView3->Selected->SubItems->Strings[0];
	  ShellExecuteW(Handle, NULL, L"explorer", patch.w_str(), NULL, SW_SHOWNORMAL);

   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::TreyIc(TObject *Sender)
{
   if(Form3->Showing)
   {
	  Form3->Hide();
	  TrayIcon1->Visible = true;
   }
   else
   {
	  WindowState = wsNormal;
	  Form3->Show();
	  TrayIcon1->Visible = true;
	  Application->BringToFront();
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if(CanClose)
	{
		if(AtOptions.Exit_Trei)
		{
		   TrayIcon1->Visible = false;
		   Button1Click(Sender);
		   CanClose = true;
		}
		else
		{
			CanClose = false;
			Hide();
			WindowState = wsMinimized;
			TrayIcon1->Visible = true;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm2Delete(TObject *Sender)
{
	 StopThreaSelected();
}
//+++++++++++++++++++++++++
void TForm3::StopThreaCheked()
{
   ListView2->Items->BeginUpdate();//что бы не мерцал икран.
   for(int totalList = 0; totalList < ListView2->Items->Count ; totalList++)
   {
	  if (ListView2->Items->Item[totalList]->Checked == true)
	  {
		 if(ListView2->Items->Item[totalList]->SubItems->Strings[0] != "")
		 {
			for(int qw = 0; qw<55; qw++)
			{
			   if(VTIndy[qw] != NULL)
			   {
				  if(VTIndy[qw]->FileNumber == (int)ListView2->Items->Item[totalList]->Data)
				  {
					 VTIndy[qw]->Terminate();
					 break;
				  }
			   }
			}
			 ListView2->Items->Item[totalList]->Delete();
			 ListView2->Repaint();
			 --totalList;
		 }
	  }
   }
   ListView2->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void TForm3::StopThreaSelected()
{
   /*for(int q=0; q<ListView2->Items->Count; q++)
   {
	   if(ListView2->Items->Item[q]->Selected == true)
	   {
		  if(ListView2->Items->Item[q]->SubItems->Strings[0] !="")
		  {
			 for(int z=0; z<99; z++)
			 {
				if(VTIndy[z] != NULL)
				{
				   if(VTIndy[z]->FileNumber == (int)ListView2->Items->Item[q]->Data)
				   {
					  VTIndy[z]->Terminate();
					  Sleep(1);
					  break;
				   }
				}
			 }
		  }
		  ListView2->Items->Item[q]->Delete();
		  q--;
		  ListView2->Repaint();

	   }
	   //Application->ProcessMessages();
   } */
	if(ListView2->Selected !=NULL)
	{
		ListView2->DeleteSelected();
		ListView2->Repaint();
	}
}
//+++++++++++++++++++++++++
void __fastcall TForm3::Pm3OpenReZultat(TObject *Sender)
{
   for(int q=0; q<ListView3->Items->Count; q++)
   {
	   if(ListView3->Items->Item[q]->Selected == true)
	   {
		  if(ListView3->Items->Item[q]->SubItems->Strings[6] == "")
		  {
			 ShowMessage("Результата нет");
			 return;
		  }
		  ShellExecuteW(Handle, NULL,ListView3->Items->Item[q]->SubItems->Strings[6].w_str(),0,0,SW_SHOWNORMAL);
	   }
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm3DeleteCheked(TObject *Sender)
{
	ListView3->Items->BeginUpdate();
   for (int i = 0; i < ListView3->Items->Count; i++)
   {
	  if(ListView3->Items->Item[i]->Checked == true)
	  {
		 ListView3->Items->Item[i]->Delete();
		 --i;
		 ListView3->Repaint();
	  }

   }
   ListView3->Items->EndUpdate();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TForm3::Edit1KeyPress(TObject *Sender, System::WideChar &Key)
{
	if((Key >='0') && (Key <='9') || (Key >='A') && (Key <='F')
	   || (Key >='a') && (Key <='f')|| (Key ==VK_BACK) || (Edit1->Text.Length()<=32))

	   return;

	else
	{
	   ShowMessage("Символ "+AnsiString(Key) + " Не допустимый символ MD5.");
	   Key =0;
	}
}
//---------------------------------------------------------------------------

AnsiString __fastcall TForm3::MyCryptDeCrypt(AnsiString Paroll, bool EncryptDecrypt)
{
	HCRYPTPROV hProv;
	HCRYPTHASH hash;
	HCRYPTKEY key;
	AnsiString psw = "vj;tn,snm";
	UnicodeString buf;
	DWORD len;

	CryptAcquireContextW(&hProv,0,0,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT);
	CryptCreateHash(hProv,CALG_SHA, 0,0,&hash);
	CryptHashData(hash,psw.c_str(),psw.Length(),0);
	CryptDeriveKey(hProv, CALG_RC4, hash, 0, &key);
	CryptDestroyHash(hash);
	len = Paroll.Length();

	if( EncryptDecrypt == true)
	   CryptEncrypt(key,0,1,0, Paroll.c_str(),&len,len);

	else
	   CryptDecrypt(key,0,1,0, Paroll.c_str(),&len);
	CryptReleaseContext(hProv,0);
	return Paroll;
}

//---------------------------------------------------------------------------
AnsiString __fastcall TForm3::CryptDeCryptFileError(AnsiString Paroll, bool EncryptDecrypt)
{
	HCRYPTPROV hProv;
	HCRYPTHASH hash;
	HCRYPTKEY key;
	AnsiString psw = "dsafgewr52343227sgdfsgdfg";
	AnsiString buf;
	DWORD len;

	CryptAcquireContextA(&hProv,0,0,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT);
	CryptCreateHash(hProv,CALG_SHA, 0,0,&hash);
	CryptHashData(hash,psw.c_str(),psw.Length(),0);
	CryptDeriveKey(hProv, CALG_RC4, hash, 0, &key);
	CryptDestroyHash(hash);
	len = Paroll.Length();

	if( EncryptDecrypt == true)
	   CryptEncrypt(key,0,1,0, Paroll.c_str(),&len,len);

	else
	   CryptDecrypt(key,0,1,0, Paroll.c_str(),&len);
	//RichEdit1->Lines->Add(Paroll);
	CryptReleaseContext(hProv,0);
	return Paroll;
}

//---------------------------------------------------------------------------
void __fastcall TForm3::ListView3ColumnClick(TObject *Sender, TListColumn *Column)

{
	if(nSortColumn == Column->Index) bAscend = !bAscend;
	else
	{
		nSortColumn = Column->Index;
		bAscend = true;
	}
	ListView3->CustomSort((PFNLVCOMPARE)SortProc, Column->Index); // <---

}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm1DeleteChecked(TObject *Sender)
{
	ListView1->Items->BeginUpdate();
	for (int i = 0; i < ListView1->Items->Count; i++)
	{
		if(ListView1->Items->Item[i]->Checked == true)
		{
		   ListView1->Items->Item[i]->Delete();
		   --i;
		   ListView1->Repaint();
		   Application->ProcessMessages();
		}
	}
	ListView1->Items->EndUpdate();
	ProgressBar1->Max = ListView1->Items->Count;

}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm2DeleteChecedListView(TObject *Sender)
{
   StopThreaCheked();
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm2VidelitVse(TObject *Sender)
{
   for (int i = 0; i < ListView2->Items->Count; i++)
   {
		ListView2->Items->Item[i]->Checked = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm2OtmenaVideleviya(TObject *Sender)
{
   for (int i = 0; i < ListView2->Items->Count; i++)
   {
		ListView2->Items->Item[i]->Checked = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm3FileInfo(TObject *Sender)
{
   if (ListView3->Selected != NULL)
   {
	  if(ListView3->Selected->SubItems->Strings[0] != "")
	  {
		 SHELLEXECUTEINFOW SEI;
		 ZeroMemory(&SEI, sizeof(SHELLEXECUTEINFOW));
		 SEI.cbSize= sizeof(SHELLEXECUTEINFOW);
		 SEI.lpVerb = L"properties";
		 SEI.lpFile= ListView3->Selected->SubItems->Strings[0].c_str();
		 SEI.nShow = SW_SHOW;
		 SEI.fMask= SEE_MASK_INVOKEIDLIST;
		 ShellExecuteExW(&SEI);
	  }
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm1OpenDirFile(TObject *Sender)
{
   if (ListView1->Selected != NULL)
   {
	  UnicodeString patch = "/select, " + ListView1->Selected->SubItems->Strings[0];
	  ShellExecuteW(Handle, NULL, L"explorer", patch.w_str(), NULL, SW_SHOWNORMAL);
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm2OpenDirFile(TObject *Sender)
{
   if (ListView2->Selected != NULL)
   {
	  UnicodeString patch ="/select, " + ListView2->Selected->SubItems->Strings[0];
	  ShellExecuteW(Handle, NULL, L"explorer", patch.w_str(), NULL, SW_SHOWNORMAL);
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm3VidelitVse(TObject *Sender)
{
   for (int i = 0; i < ListView3->Items->Count; i++)
   {
	  ListView3->Items->Item[i]->Checked = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm3OtmenaVidelit(TObject *Sender)
{
   for (int i = 0; i < ListView3->Items->Count; i++)
   {
	  ListView3->Items->Item[i]->Checked = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm1OtmenaVideleniya(TObject *Sender)
{
   for (int i = 0; i < ListView1->Items->Count; i++)
   {
	  ListView1->Items->Item[i]->Checked = false;
   }
}

void __fastcall TForm3::Button3Click(TObject *Sender)
{
	if(Edit1->Text !="")
	{
	   if(GetStatusConnect())
	   {
		  Button3->Enabled =false;
		  if(ReportVT(Edit1->Text))
		  {
			 Application->ProcessMessages();
			 if (http_response_code == 200) {

				try{
				   std::auto_ptr<TJSONObject> ResRescan(static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(BaseJesson)));

				   if(ResRescan->Get("response_code")->JsonValue->Value().Trim().ToInt() == 1){

					  TListItem *ListItem;
					  ListItem = Form3->ListView3->Items->Add();
					  ListItem->Checked = false;
					  ListItem->Caption = ExtractFileName("CalcMD5"); // Имя файла
					  ListItem->SubItems->Add("CalcMD5");// пулный путь.
					  ListItem->SubItems->Add(ResRescan->Get("positives")->JsonValue->Value() + "/" + ResRescan->Get("total")->JsonValue->Value());// результат сканирования
					  ListItem->SubItems->Add(0);// размер
					  ListItem->SubItems->Add(Edit1->Text);// хеш сумма
					  ListItem->SubItems->Add(ResRescan->Get("sha256")->JsonValue->Value());//Хеш SHA256
					  ListItem->SubItems->Add(ResRescan->Get("scan_date")->JsonValue->Value());//Дата проверки
					  ListItem->SubItems->Add(ResRescan->Get("permalink")->JsonValue->Value());// ссылка на результат.
					  ListItem->Data = new String(BaseJesson);
				   }
				   else
				   {
					  TListItem *ListItem;
					  ListItem = Form3->ListView3->Items->Add();
					  ListItem->Checked = false;
					  ListItem->Caption = ExtractFileName("CalcMD5"); // Имя файла
					  ListItem->SubItems->Add("CalcMD5");// пулный путь.
					  ListItem->SubItems->Add("0/0");// результат сканирования
					  ListItem->SubItems->Add(0);// размер
					  ListItem->SubItems->Add(Edit1->Text);// хеш сумма
					  ListItem->SubItems->Add("");//Хеш SHA256
					  ListItem->SubItems->Add("");//Дата проверки
					  ListItem->SubItems->Add("");// ссылка на результат.
				   }
				}
				catch(Exception & e)
				{
				   e.Message;
				}
			 }
			 else(Button3Click(Sender));
		  }
		  Button3->Enabled =true;
	   }
	   else
		  ShowMessage("Интернет не подключен");

	}
}
//---------------------------------------------------------------------------
void __fastcall TForm3::MmExit(TObject *Sender)
{
	Button1Click(Sender);
   /*if(SpisokArchivFolder->Count > 0)
   {
	   for(int i=0; i< SpisokArchivFolder->Count; i++)
	   {
		   if(TDirectory::Exists(SpisokArchivFolder->Strings[i]))
		   {
			   TDirectory::Delete(SpisokArchivFolder->Strings[i],true);
			   //ShowMessage("as");
		   }
	   }
   } */

	AtOptions.Exit_Trei = true;
	Form3->Close();
}
//+++++++++++++++++++++++
void __fastcall TForm3::FormDestroy(TObject *Sender)
{
	DeleteDirectoryTempArchiv();
	//SpisokArchivFolder->SaveToFile("eks.log");
	Button1Click(Sender);
	TrayIcon1->Visible = false;
	//UnicodeString a = ExtractFilePath(ParamStr(0)) +"AitotalTMP\\archiw"; //Remove_dirW(a.w_str()); // Удаляю директорию распакованных архивов
	delete SpisokNamePotok;
	DeleteDirectoryTempArchiv();//Удаляю два раза, на верняка.
	ClearSpisokFileName();
	delete  SpisokFileName;
	delete SpisokArchivFolder;
    // ну, и по привычке, чистим за собой перед выходом
	DragAcceptFiles(this->Handle, false);

}
//---------------------------------------------------------------------------

void __fastcall TForm3::ListView2CustomDrawSubItem(TCustomListView *Sender, TListItem *Item,
		  int SubItem, TCustomDrawState State, bool &DefaultDraw)
{
   int asd = Item->Index;
   if((asd%2) ==0)
   {
	   Sender->Canvas->Brush->Color = (TColor)RGB(240,240,240);
   }
   else
   {
	   Sender->Canvas->Brush->Color = clWindow;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::ListView2CustomDrawItem(TCustomListView *Sender, TListItem *Item,
		  TCustomDrawState State, bool &DefaultDraw)
{
   if((Item->Index%2) ==0)
   {
	   Sender->Canvas->Brush->Color = (TColor)RGB(240,240,240);
   }
   else
   {
	   Sender->Canvas->Brush->Color = clWindow;
   }
}
//---------------------------------------------------------------------------


void __fastcall TForm3::ListView3CustomDrawItem(TCustomListView *Sender, TListItem *Item,
		  TCustomDrawState State, bool &DefaultDraw)
{
   if((Item->Index%2) ==0)
   {
	   Sender->Canvas->Brush->Color = (TColor)RGB(235,235,235);
   }
   else
   {
	   Sender->Canvas->Brush->Color = clWindow;
   }
   //если есть детект перекрашиваю в бледно розовый.
   if(Item->SubItems->Strings[1] !="")
   {
	   TStringDynArray arr(SplitString(Item->SubItems->Strings[1], "/"));

	   if(arr[0].ToInt() >0)
		   Sender->Canvas->Brush->Color = (TColor)RGB(255,204,229);
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::ListView1CustomDrawItem(TCustomListView *Sender, TListItem *Item,
		  TCustomDrawState State, bool &DefaultDraw)
{
	if((Item->Index%2) ==0)
   {
	   Sender->Canvas->Brush->Color = (TColor)RGB(235,235,235);
   }

   else
   {
	   Sender->Canvas->Brush->Color = clWindow;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::ListView1CustomDrawSubItem(TCustomListView *Sender, TListItem *Item,
		  int SubItem, TCustomDrawState State, bool &DefaultDraw)
{
   if((Item->Index%2) ==0)
   {
	   Sender->Canvas->Brush->Color = (TColor)RGB(235,235,235);
   }

   else
   {
	   Sender->Canvas->Brush->Color = clWindow;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::ListView3CustomDrawSubItem(TCustomListView *Sender, TListItem *Item,
		  int SubItem, TCustomDrawState State, bool &DefaultDraw)
{
	 if((Item->Index%2) ==0)
   {
	   Sender->Canvas->Brush->Color = (TColor)RGB(235,235,235);
	   Sender->Update();
   }
   else
   {
	   Sender->Canvas->Brush->Color = clWindow;
	   Sender->Update();
   }

   //если есть детект перекрашиваю в бледно розовый.
   if(Item->SubItems->Strings[1] !="")
   {
	   TStringDynArray arr(SplitString(Item->SubItems->Strings[1], "/"));

	   if(arr[0].ToInt() >0)
		   Sender->Canvas->Brush->Color = (TColor)RGB(255,204,229);

	   Sender->Update();
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::ListView2ColumnClick(TObject *Sender, TListColumn *Column)

{
   if(nSortColumn == Column->Index) bAscend = !bAscend;
	else
	{
		nSortColumn = Column->Index;
		bAscend = true;
	}
	ListView2->CustomSort((PFNLVCOMPARE)SortProcListViewe2, Column->Index); // <---
}
//---------------------------------------------------------------------------

void __fastcall TForm3::ListView1ColumnClick(TObject *Sender, TListColumn *Column)

{
   if(nSortColumn == Column->Index) bAscend = !bAscend;
	else
	{
		nSortColumn = Column->Index;
		bAscend = true;
	}
	ListView1->CustomSort((PFNLVCOMPARE)SortProcListViewe1, Column->Index); // <---
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LV3SaveRezult(TObject *Sender)
{
   std::auto_ptr<TStringList> SpisokLV3 (new TStringList(NULL));
   UnicodeString Stroka = "";

   SpisokLV3->Add("Имя файла;Путь к файлу;Результат;Размер;MD5;SHA256;Дата;ССылка");
   for(int i=0; i < ListView3->Items->Count; i++)
   {
	  Stroka = ListView3->Items->Item[i]->Caption;
	  for(int j=0;j<ListView3->Items->Item[i]->SubItems->Count;++j)
	  {
		 Stroka += + ";" +ListView3->Items->Item[i]->SubItems->Strings[j];
	  }
	  SpisokLV3->Add(Stroka);
   }
   SaveDialog1->InitialDir = ExtractFilePath(Application->ExeName) + "AitotalTMP\\";

   if(SaveDialog1->Execute())
	  SpisokLV3->SaveToFile(SaveDialog1->FileName);
}

//---------------------------------------------------------------------------

void __fastcall TForm3::LV3PokaztRelt(TObject *Sender)
{
   if (ListView3->Selected != NULL)
   {
	  if(ListView3->Selected->Data != NULL)
	  {
		 Application->CreateForm(__classid(TFormResultScan), &FormResultScan);
		 Base VTBase;
		 VTBase.BaseFileName = ListView3->Selected->Caption;
		 VTBase.BasePatchFileName = ListView3->Selected->SubItems->Strings[0];
		 VTBase.BaseDetect =  ListView3->Selected->SubItems->Strings[1];
		 VTBase.BaseMD5 = ListView3->Selected->SubItems->Strings[2];
		 VTBase.BaseSHA256 = ListView3->Selected->SubItems->Strings[3];
		 VTBase.BaseSizeFile = ListView3->Selected->SubItems->Strings[4];
		 VTBase.BaseDataProverki = ListView3->Selected->SubItems->Strings[5];
		 VTBase.BaseAdress =  ListView3->Selected->SubItems->Strings[6];
		 VTBase.BaseJesson = *(String*) ListView3->Selected->Data;
		 FormResultScan->VTBase = VTBase;
		 FormResultScan->Tompost = false;
		 FormResultScan->ShowModal();
	  }
	  else
		 ShowMessage("Нет результата.");
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LV3Clear(TObject *Sender)
{
   ListView3->Clear();
   TLabelCountProwereno->Caption ="";
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LWProverit(TObject *Sender)
{
   for(int q=0; q<ListView2->Items->Count; q++)
   {
	   if(ListView2->Items->Item[q]->Selected == true)
	   {
		  Base VTbase;
		  VTbase.BasePatchFileName = ListView2->Items->Item[q]->SubItems->Strings[0];
		  VTbase.BaseSizeFile = ListView2->Items->Item[q]->SubItems->Strings[1];
		  VTbase.BaseMD5 =  ListView2->Items->Item[q]->SubItems->Strings[2];
		  VTbase.BaseSHA256 = ListView2->Items->Item[q]->SubItems->Strings[3];
		  VTbase.FileNumber = (int)ListView2->Items->Item[q]->Data;
		  CreateStartVT(VTbase, true, 0);
		  ListView2->Repaint();
	   }
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm2ScanCheked(TObject *Sender)
{
   StopThreadPopupMenu = false;
   unsigned int ApikeyNumber = 0;

   for(int i = 0; i<ListView2->Items->Count; i++)
   {
	  if(ListView2->Items->Item[i]->Checked == true)
	  {
		 if(!StopThreadPopupMenu )
		 {
			if( SpisokNamePotok->Count < AtOptions.FileCount)
			{
			   if(!GetStatusConnect())
			   {
				  ShowMessage("Интернет не подключен");
				  break;
			   }
				  if(StopThreadPopupMenu)
					 break;

				  Base VTbase;
				  VTbase.BasePatchFileName = ListView2->Items->Item[i]->SubItems->Strings[0];
				  VTbase.BaseMD5 = ListView2->Items->Item[i]->SubItems->Strings[2];
				  VTbase.BaseSizeFile = ListView2->Items->Item[i]->SubItems->Strings[1];
				  VTbase.BasePredScanData = ListView2->Items->Item[i]->SubItems->Strings[3];
				  VTbase.FileNumber = (int)ListView2->Items->Item[i]->Data;

				  if(ApikeyNumber >= AtOptions.Apikey.size())
				  {
					 ApikeyNumber = 0;
				  }

				  CreateStartVT(VTbase,true, ApikeyNumber);
				  ApikeyNumber++;
			}
		 }
	  }
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm3OpenReZultCheked(TObject *Sender)
{
   if (ListView3->Items->Count != 0)
   {
	   bool pausePervoiVkladki = true;
	   for(int i = 0, q=0; i < ListView3->Items->Count; i++,q++)
	   {
		  if(ListView3->Items->Item[i]->Checked == true)
		  {
			 if(ListView3->Items->Item[i]->SubItems->Strings[6] == "")
			 {
				MessageBoxA(0,"Результата нет", 0,MB_OK);
			 }
			 if(q<10)
			 {
				if(pausePervoiVkladki)
				{
				   ShellExecuteW(Handle, NULL, ListView3->Items->Item[i]->SubItems->Strings[6].w_str(),0,0,SW_SHOWNORMAL);
				   PauseT("00:00:02");
				   pausePervoiVkladki = false;
				}
				else
				   ShellExecuteW(Handle, NULL,ListView3->Items->Item[i]->SubItems->Strings[6].w_str(),0,0,SW_SHOWNORMAL);
			 }
			 else
			 {
				q=0;
				PauseT("00:00:05");
			 }
		  }
		  Application->ProcessMessages();
	   }
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LW2CopiNameFile(TObject *Sender)
{
   if (ListView2->Selected != NULL)
   {
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView2->Selected->Caption;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LW2CopiPath(TObject *Sender)
{
   if (ListView2->Selected != NULL)
	{
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ExtractFilePath(ListView2->Selected->SubItems->Strings[0]);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LW2CopiMD5(TObject *Sender)
{
   if (ListView2->Selected != NULL)
	{
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView2->Selected->SubItems->Strings[2];
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm3::LV2CopySHA256(TObject *Sender)
{
   if (ListView2->Selected != NULL)
	{
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView2->Selected->SubItems->Strings[3];
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm3::LW2CopiSizeFile(TObject *Sender)
{
	if (ListView2->Selected != NULL)
	{
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView2->Selected->SubItems->Strings[1];
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LW2CopiNameFilePath(TObject *Sender)
{
   if (ListView2->Selected != NULL)
   {
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView2->Selected->SubItems->Strings[0];
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LW1CopiNameFile(TObject *Sender)
{
   if (ListView1->Selected != NULL)
   {
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView1->Selected->Caption;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LW1CopiPath(TObject *Sender)
{
   if (ListView1->Selected != NULL)
	{
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ExtractFilePath(ListView1->Selected->SubItems->Strings[0]);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LW1CopiFileNamePatch(TObject *Sender)
{
   if (ListView1->Selected != NULL)
   {
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView1->Selected->SubItems->Strings[0];
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LW1CopiMD5(TObject *Sender)
{
   if (ListView1->Selected != NULL)
	{
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView1->Selected->SubItems->Strings[2];
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm3::LV1CopySHA256(TObject *Sender)
{
if (ListView1->Selected != NULL)
	{
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView1->Selected->SubItems->Strings[3];
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm3::LW1CopiFileSize(TObject *Sender)
{
 if (ListView1->Selected != NULL)
	{
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView1->Selected->SubItems->Strings[1];
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::About(TObject *Sender)
{
  AboutBox->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LV1Diblklic(TObject *Sender)
{
   if(ListView1->Selected)
   {
	   if(ListView1->Selected->Checked == true)
		   ListView1->Selected->Checked = false;
	   else
		   ListView1->Selected->Checked = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LV2Dblklic(TObject *Sender)
{
	if(ListView2->Selected)
	{
		if(ListView2->Selected->Checked == true)
			ListView2->Selected->Checked = false;
		else
			ListView2->Selected->Checked = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm3::ErrorLog(UnicodeString ErrorMessage)
{
   try
   {
	  if(!FileExists(PatchProgramma + "AitotalTMP\\Error.log"))
	  {
		 int FileCr;
		 FileCr = FileCreate(PatchProgramma + "AitotalTMP\\Error.log");
		 FileClose(FileCr);

	  }
	   //Закоментировал, так как программа падает от переполнение стека. Когда проверяемый файл занят другим процессом
	  /*if(MyFileSize2(PatchProgramma + "AitotalTMP\\Error.log") > 1048576)
	  {
		 TFile::Delete(PatchProgramma + "AitotalTMP\\Error.log");
		 int FileCr;
		 FileCr = FileCreate(PatchProgramma + "AitotalTMP\\Error.log");
		 FileClose(FileCr);

	  } */
	  std::auto_ptr<TStringList> FileError (new TStringList(NULL));
	  FileError->LoadFromFile(PatchProgramma + "AitotalTMP\\Error.log");
	  FileError->Add("+++"+ TOSVersion::Name +"+++"+TOSVersion::Major + "+++"+ TOSVersion::Minor +"+++"+ TOSVersion::Build +"+++" +TOSVersion::ServicePackMajor+ "+++" + "+++" +String(MY_VERSION)+ "++++" + String(MY_DATE)+ " +++");
	  FileError->Add(FormatDateTime("YYYY.MM.DD",Date())+ " " + FormatDateTime("HH:NN:SS",Time()));
	  FileError->Add(ErrorMessage);
	  FileError->SaveToFile(PatchProgramma + "AitotalTMP\\Error.log", TEncoding::UTF8);
   }
   catch(EFOpenError &e)
   {
	  ShowMessage("No open file Error.log!");
   }

}
//---------------------------------------------------------------------------

void __fastcall TForm3::SHA2561Click(TObject *Sender)
{
   if (ListView3->Selected != NULL)
   {
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView3->Selected->SubItems->Strings[4];
   }
}
//---------------------------------------------------------------------------


void __fastcall TForm3::Pm2FileInfo(TObject *Sender)
{
   if (ListView2->Selected != NULL)
   {
	  if(ListView2->Selected->SubItems->Strings[0] != "")
	  {
		 SHELLEXECUTEINFOW SEI;
		 ZeroMemory(&SEI, sizeof(SHELLEXECUTEINFOW));
		 SEI.cbSize= sizeof(SHELLEXECUTEINFOW);
		 SEI.lpVerb = L"properties";
		 SEI.lpFile= ListView2->Selected->SubItems->Strings[0].c_str();
		 SEI.nShow = SW_SHOW;
		 SEI.fMask= SEE_MASK_INVOKEIDLIST;
		 ShellExecuteExW(&SEI);
	  }
   }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TForm3::LV2CopySsilka(TObject *Sender)
{
   if (ListView2->Selected != NULL)
   {
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView2->Selected->SubItems->Strings[5];
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::ApplicationEvents1Minimize(TObject *Sender)
{
   //Hide();
   WindowState = wsMinimized;
   TrayIcon1->Visible = true;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TForm3::TrayIcon1BalloonClick(TObject *Sender)
{
   UnicodeString Patch = Form3->TrayIcon1->BalloonHint.SubString(1,Form3->TrayIcon1->BalloonHint.Pos(" : ")-1);
   UnicodeString detect =  Form3->TrayIcon1->BalloonHint.SubString(Form3->TrayIcon1->BalloonHint.Pos(" : ")+ 3, 5);

   for(int i=0; i < Form3->ListView3->Items->Count; i++)
   {
	   if(AnsiCompareStr(Form3->ListView3->Items->Item[i]->SubItems->Strings[0],Patch)==0
		  && AnsiCompareStr(Form3->ListView3->Items->Item[i]->SubItems->Strings[1],detect)==0)
	   {
		   if(Form3->ListView3->Items->Item[i]->Data != NULL)
		   {
			  Application->CreateForm(__classid(TFormResultScan), &FormResultScan);
			  Base VTBase;
			  VTBase.BaseFileName = Form3->ListView3->Items->Item[i]->Caption;
			  VTBase.BasePatchFileName = Form3->ListView3->Items->Item[i]->SubItems->Strings[0];
			  VTBase.BaseDetect =  Form3->ListView3->Items->Item[i]->SubItems->Strings[1];
			  VTBase.BaseMD5 = Form3->ListView3->Items->Item[i]->SubItems->Strings[2];
			  VTBase.BaseSHA256 = Form3->ListView3->Items->Item[i]->SubItems->Strings[3];
			  VTBase.BaseSizeFile = Form3->ListView3->Items->Item[i]->SubItems->Strings[4];
			  VTBase.BaseDataProverki = Form3->ListView3->Items->Item[i]->SubItems->Strings[5];
			  VTBase.BaseAdress =  Form3->ListView3->Items->Item[i]->SubItems->Strings[6];
			  VTBase.BaseJesson = *(String*) Form3->ListView3->Items->Item[i]->Data;

			  FormResultScan->VTBase = VTBase;
			  FormResultScan->Tompost = true;
			  FormResultScan->ShowModal();
		   }
		   else
			  ShowMessage("Нет результата.");
	   }
   }

}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm2StopSelected(TObject *Sender)
{
	for(int q=0; q<ListView2->Items->Count; q++)
	{
	   if(ListView2->Items->Item[q]->Selected == true)
	   {
		  if(ListView2->Items->Item[q]->SubItems->Strings[0] !="")
		  {
			 for(int qw = 0; qw<55; qw++)
			 {
				if(VTIndy[qw] != NULL)
				{
				   if(VTIndy[qw]->VtBase.FileNumber == (int)ListView2->Items->Item[q]->Data)
				   {
					  VTIndy[qw]->Terminate();
					  Sleep(100);
					  break;
				   }
				}
			 }
		  }
	   }
	   Application->ProcessMessages();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm2StopCHeked(TObject *Sender)
{
	for(int totalList = 0; totalList < ListView2->Items->Count ; totalList++)
   {
	  if (ListView2->Items->Item[totalList]->Checked == true)
	  {
		 if(ListView2->Items->Item[totalList]->SubItems->Strings[0] != "")
		 {
			for(int qw = 0; qw<55; qw++)
			{
			   if(VTIndy[qw] != NULL)
			   {
				  if(VTIndy[qw]->VtBase.FileNumber == (int)ListView2->Items->Item[totalList]->Data)
				  {
					 VTIndy[qw]->Terminate();
					 Sleep(100);
					 break;
				  }
			   }
			}
		 }
	  }
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm1FileInfo(TObject *Sender)
{
   if (ListView1->Selected != NULL)
   {
	  if(ListView1->Selected->SubItems->Strings[0] != "")
	  {
		 SHELLEXECUTEINFOW SEI;
		 ZeroMemory(&SEI, sizeof(SHELLEXECUTEINFOW));
		 SEI.cbSize= sizeof(SHELLEXECUTEINFOW);
		 SEI.lpVerb = L"properties";
		 SEI.lpFile= ListView1->Selected->SubItems->Strings[0].c_str();
		 SEI.nShow = SW_SHOW;
		 SEI.fMask= SEE_MASK_INVOKEIDLIST;
		 ShellExecuteExW(&SEI);
	  }
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm1SelectedToCheked(TObject *Sender)
{
	for(int q=0; q<ListView1->Items->Count; q++)
	{
	   if(ListView1->Items->Item[q]->Selected == true)
	   {
		  ListView1->Items->Item[q]->Checked = true;
		  ListView1->Items->Item[q]->Selected = false;
	   }
	   ListView1->Repaint();
	   Application->ProcessMessages();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm2SelectedToCheked(TObject *Sender)
{
	for(int q=0; q<ListView2->Items->Count; q++)
	{
	   if(ListView2->Items->Item[q]->Selected == true)
	   {
		  ListView2->Items->Item[q]->Checked = true;
		  ListView2->Items->Item[q]->Selected = false;
	   }
	   ListView2->Repaint();
	   Application->ProcessMessages();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Pm3SelectedToCheked(TObject *Sender)
{
	for(int q=0; q<ListView3->Items->Count; q++)
	{
	   if(ListView3->Items->Item[q]->Selected == true)
	   {
		  ListView3->Items->Item[q]->Checked = true;
		  ListView3->Items->Item[q]->Selected = false;
	   }
	   ListView3->Repaint();
	   Application->ProcessMessages();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::MM1ScanFolderArchiv(TObject *Sender)
{
	if (GetStatusConnect())
	{
		BROWSEINFOW info;
		wchar_t szDir[MAX_PATH];
		wchar_t szDisplayName[MAX_PATH];
		LPITEMIDLIST pidl;
		LPMALLOC pShellMalloc;

		if (SHGetMalloc(&pShellMalloc) == NO_ERROR)
		{
			memset(&info, 0x00, sizeof(info));
			info.hwndOwner = 0;
			info.pidlRoot = NULL;
			info.pszDisplayName = szDisplayName;
			info.lpszTitle = L"Выбор папки с карантином";
			info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_USENEWUI;

			pidl = SHBrowseForFolderW(&info);

			if (pidl)
			{
				if (SHGetPathFromIDListW(pidl, szDir))
				{
					ParsingArchiv(szDir,1);
				}
				pShellMalloc->Free(pidl);
				pShellMalloc->Release();
			}
			ScanFile();
		}
	}
	else
		MessageBoxW(0, L"Нет соединение с интернетом", L"Внимание ошибка",
		MB_OK + MB_ICONSTOP);
}
//---------------------------------------------------------------------------
void __fastcall TForm3::InsertToReestrMenu(TObject *Sender)
{
   if(TOSVersion::Major >=6)
   {
	  TRegistry *reg = new TRegistry(KEY_ALL_ACCESS|KEY_WOW64_64KEY);

	  if (reg)
	  {
		  reg->RootKey=HKEY_CLASSES_ROOT;
		  if(reg->OpenKey("AllFilesystemObjects\\shell\\Aitotal",true))

		  {
			 reg->WriteString("MUIVerb","Aitotal");
			 reg->WriteString("SubCommands","AiTotal1;AiTotal2");
			 reg->WriteString("Icon",ExtractFilePath(ParamStr(0))+"Aitotal.exe");
			 reg->CloseKey();
		  }
		  else
			 ShowMessage("Ошибка открытия реестра AllFilesystemObjects\\shell\\Aitotal");

		  reg->RootKey=HKEY_CURRENT_USER;
		  if(reg->OpenKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\AiTotal1",true))
		  {
			 reg->WriteString("","Scan with AiTotal");
			 reg->WriteString("Icon",ExtractFilePath(ParamStr(0))+"Aitotal.exe");

			 reg->OpenKey("command",true);
			 reg->WriteString("", "\"" + ExtractFilePath(ParamStr(0))+ "Aitotal.exe\" \"%1\"");
			 reg->CloseKey();   //Закрываем ключ
		  }
		  else
			 ShowMessage("Ошибка открытия реестра shell\\AiTotal1");

		  if(reg->OpenKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\AiTotal2",true))
		  {
			 reg->WriteString("","Scan with AiTotal (no ReScan)");
			 reg->WriteString("Icon",ExtractFilePath(ParamStr(0))+"Aitotal.exe");

			 reg->OpenKey("command",true);
			 reg->WriteString("", "\"" + ExtractFilePath(ParamStr(0))+ "Aitotal.exe\" /norescan \"%1\"");
			 reg->CloseKey();
		  }
		  else
			 ShowMessage("Ошибка открытия реестра shell\\AiTotal2");
	  }
	  delete reg;
   }
   else
	  ShowMessage("Извините. Данная опция работает только с версии Vista и выше");


}
//---------------------------------------------------------------------------

void __fastcall TForm3::DeleteReestr(TObject *Sender)
{
   TRegistry *reg=new TRegistry(KEY_ALL_ACCESS|KEY_WOW64_64KEY); //Создали обьект reg класса TRegistry
   reg->RootKey=HKEY_CLASSES_ROOT;  //Как и при записи указываем раздел с которым будем работать
   //Здесь окрывать ключ не надо, так как мы его не сможем удалить перед того, как к нему обратились, для этого его нужно будет закрыть
   if(reg->KeyExists("AllFilesystemObjects\\shell\\Aitotal"))
   {
	  if(!reg->DeleteKey("AllFilesystemObjects\\shell\\Aitotal"))
		 ShowMessage("Ошибка удаления ключа реестра");  //Удаляем ключ
   }

   reg->RootKey=HKEY_LOCAL_MACHINE;
   if(reg->KeyExists("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\AiTotal1"))
   {
	  if(!reg->DeleteKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\AiTotal1"))
		 ShowMessage("Ошибка удаления ключа реестра AiTotal1");
   }

   if(reg->KeyExists("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\AiTotal12"))
   {
	  if(!reg->DeleteKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\AiTotal2"))
		 ShowMessage("Ошибка удаления ключа реестра AiTotal2");
   }
   delete reg;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LW1OnDeletion(TObject *Sender, TListItem *Item)
{
	delete (numchek*)Item->Data;

	int q =   ListView1->Items->Count-1;
	if(q==0)
		TLabelItogo->Caption = "";
	else
	   TLabelItogo->Caption = "На очереди = " + IntToStr(q);
}
//---------------------------------------------------------------------------
void __fastcall TForm3::LW1OnInsert(TObject *Sender, TListItem *Item)
{
	TLabelItogo->Caption = "На очереди = " + IntToStr(ListView1->Items->Count);
}
//---------------------------------------------------------------------------


void __fastcall TForm3::LW2OnDeletion(TObject *Sender, TListItem *Item)
{
	if(Item->SubItems->Strings[0] !="" )
	{
		for(int z=0; z<99; z++)
		{
			if(VTIndy[z] != NULL)
			{
				if(VTIndy[z]->FileNumber == (int)Item->Data)
				{
					VTIndy[z]->Terminate();
					break;
				}
			}
		}
	}

	int q = ListView2->Items->Count - 1;
	if(q==0)
		TLabelCountProwerki->Caption = "";
	else
		TLabelCountProwerki->Caption = "Проверяется файлов = " + IntToStr(q);
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LE2OnInsert(TObject *Sender, TListItem *Item)
{
   TLabelCountProwerki->Caption = "Проверяется файлов = " +  IntToStr(ListView2->Items->Count);
}
//---------------------------------------------------------------------------


void __fastcall TForm3::LW3OnDeletions(TObject *Sender, TListItem *Item)

{
   int q =  ListView3->Items->Count-1;
   if(q!=0)
	  TLabelCountProwereno->Caption = "Проверено файлов = " + IntToStr(q);
   else
	  TLabelCountProwereno->Caption ="";

   delete (String*)Item->Data;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::LW3OnInsert(TObject *Sender, TListItem *Item)
{
   TLabelCountProwereno->Caption = "Проверено файлов = " + IntToStr(ListView3->Items->Count);
}
//---------------------------------------------------------------------------
void __fastcall TForm3::SpisokFileTList()
{
  ;
  //StructFileList *pMyObj = new StructFileList;

SpisokFileName->Add(new StructFileList(0,1,UnicodeString("sfjshfj"), 54456,UnicodeString("wre"),UnicodeString("swq")));

}
//++++++++++++++

TDateTime UTCToLocalTime(TDateTime AValue)
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
void __fastcall TForm3::Button2Click(TObject *Sender)
{
	if(TDirectory::Exists(L"\\\\?\\" +ExtractFilePath(ParamStr(0)) +"AitotalTMP\\archiw\\"))
	{
	   try
	   {
			TDirectory::Delete(L"\\\\?\\" +ExtractFilePath(ParamStr(0)) +"AitotalTMP\\archiw\\", true);
	   }
	   catch(Exception &E)
	   {
		   ErrorLog("Ошибка удаления AitotalTMP\\archiw\\ "+ E.Message + E.ClassName());
	   }
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::DeleteDirectoryTempArchiv()
{
	for(int i=0; i < SpisokArchivFolder->Count; i++)
	{
		if(TDirectory::Exists(SpisokArchivFolder->Strings[i]))
		{
			try
			{
				TDirectory::Delete(SpisokArchivFolder->Strings[i], true);
			}
			catch(Exception &E)
			{
				ErrorLog("Ошибка удаления рапсакованного архива SpisokArchivFolder->Strings[i] "+ E.Message + E.ClassName());
			}
		}
	}
}
//++++++++++++++++++++++++++++++++++++

void TForm3::ClearSpisokFileName()
{
   for (int i = 0; i < SpisokFileName->Count; i++)
  {
	 //StructFileList *asd = (StructFileList*)SpisokFileName->Items[i];
	 delete (StructFileList*)SpisokFileName->Items[i];
  }
  SpisokFileName->Clear();
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void __fastcall TForm3::Pm2SaveLog(TObject *Sender)
{
   std::auto_ptr<TStringList> SpisokLV2 (new TStringList(NULL));
   UnicodeString Stroka = "";

   SpisokLV2->Add("Имя файла;Путь к файлу;Размер;MD5;SHA256;%Загрузки/проверки;ССылка");
   for(int i=0; i < ListView2->Items->Count; i++)
   {
	  Stroka = ListView2->Items->Item[i]->Caption;
	  for(int j=0;j<ListView2->Items->Item[i]->SubItems->Count;++j)
	  {
		 Stroka += + ";" +ListView2->Items->Item[i]->SubItems->Strings[j];
	  }
	  SpisokLV2->Add(Stroka);
   }
   if(!TDirectory::Exists(ExtractFilePath(PatchProgramma) + "LogTMP",false))
	  TDirectory::CreateDirectory(ExtractFilePath(PatchProgramma) + "LogTMP");
	  SpisokLV2->SaveToFile(ExtractFilePath(PatchProgramma) + "LogTMP\\LogLV2.csv");
}
//---------------------------------------------------------------------------

//Проверка на занятость файлаа.
//true файл занят.
bool __fastcall TForm3::ProverkaNaIspolFila(UnicodeString filename)
{
   try
   {  //begin
	  TFileStream *fs = new TFileStream(filename, fmOpenRead | fmShareDenyNone);
	  delete fs;
	  return false;
   }
   catch(EFOpenError &E)
   {
	   ErrorLog(filename + "\n Проверка на занятость файлаа. EFOpenError\n" + E.Message);
	   return true;
   }
   catch(Exception &E)
   {
	   ErrorLog(filename + "\n Проверка на занятость файлаа. \n" + E.Message);
	   return true;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TForm3::ReportVT (UnicodeString chesch)
{
   std::auto_ptr<TIdHTTP> IndyVT (new TIdHTTP(NULL) );
   TIdSSLIOHandlerSocketOpenSSL *ssl = new TIdSSLIOHandlerSocketOpenSSL(NULL);
   TIdSocksInfo *soketInfo = new TIdSocksInfo(NULL);
   bool resultat= false;
   IndyVT->HandleRedirects = 1;
   IndyVT->Request->UserAgent = "Mozilla/5.0 (Windows; U; Windows NT 5.1;en-US; rv:1.8.1.1) Gecko/20061204 Firefox/2.0.0.1";

   if(AtOptions.ProxyVT.ProxiChecked)
   {
	  switch(AtOptions.ProxyVT.Socket)
	  {
		 case 0:
		 {
			if(AtOptions.ProxyVT.Proxy !=" " || AtOptions.ProxyVT.Proxy.Length() !=0)
			{
			   IndyVT->ProxyParams->ProxyServer = AtOptions.ProxyVT.Proxy;
			}

			if(AtOptions.ProxyVT.IpPort !=0)
			   IndyVT->ProxyParams->ProxyPort = AtOptions.ProxyVT.IpPort;

			if(AtOptions.ProxyVT.OptProxiLogin !=" " || AtOptions.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   IndyVT->ProxyParams->ProxyUsername = AtOptions.ProxyVT.OptProxiLogin;

			}

			if(AtOptions.ProxyVT.OptProxiPassword !=" " || AtOptions.ProxyVT.OptProxiPassword.Length() != 0)
			{
			   IndyVT->ProxyParams->ProxyPassword = AtOptions.ProxyVT.OptProxiPassword;
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

			if(AtOptions.ProxyVT.Proxy !=" " || AtOptions.ProxyVT.Proxy.Length() !=0)
			{
				  //IndyVT->ProxyParams->ProxyServer = "";
			   soketInfo->Host = AtOptions.ProxyVT.Proxy;
			}
			if(AtOptions.ProxyVT.IpPort !=0)
			{
			   IndyVT->ProxyParams->ProxyPort = 0;
			   soketInfo->Port = AtOptions.ProxyVT.IpPort;
			}

			if(AtOptions.ProxyVT.OptProxiLogin !=" " || AtOptions.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Username = AtOptions.ProxyVT.OptProxiLogin;
			}


			if(AtOptions.ProxyVT.OptProxiPassword !=" " || AtOptions.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Password = AtOptions.ProxyVT.OptProxiLogin;
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

			if(AtOptions.ProxyVT.Proxy !=" " || AtOptions.ProxyVT.Proxy.Length() !=0)
			{
			   soketInfo->Host = AtOptions.ProxyVT.Proxy;
			}
			if(AtOptions.ProxyVT.IpPort !=0)
			{
			   IndyVT->ProxyParams->ProxyPort = 0;
			   soketInfo->Port = AtOptions.ProxyVT.IpPort;
			}

			if(AtOptions.ProxyVT.OptProxiLogin !=" " || AtOptions.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Username = AtOptions.ProxyVT.OptProxiLogin;
			}


			if(AtOptions.ProxyVT.OptProxiPassword !=" " || AtOptions.ProxyVT.OptProxiLogin.Length() != 0)
			{
			   soketInfo->Password = AtOptions.ProxyVT.OptProxiLogin;
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

   IndyVT->Request->Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";


   std::auto_ptr<TIdMultiPartFormDataStream>
					   PostData(new TIdMultiPartFormDataStream);

	UnicodeString Url= "https://www.virustotal.com/api/v3/files/" + Edit1->Text + "/analyse";
	//UnicodeString Url= "https://www.virustotal.com/api/v3/files/"+ Edit1->Text;

	try
	{
	   IndyVT->Request->Host = "http://www.virustotal.com";
	   IndyVT->Request->ContentType = "application/x-www-form-urlencoded";
	   IndyVT->Request->CustomHeaders->AddValue("x-apikey", AtOptions.Apikey[0]);
	   IndyVT->Post(Url,PostData.get());
	   http_response_code = IndyVT->ResponseCode;
	   /*std::auto_ptr<TStringList> SpisokLV2 (new TStringList(NULL));
	   SpisokLV2->Text =   BaseJesson;
	   SpisokLV2->SaveToFile("BaseJesson");  */

	   resultat = true;
	}
	/*catch(EIdHTTPProtocolException &E)
	{
	   AtOptions.http_response_code = E.ErrorCode;
	   ErrorMessage = "Ошибка: try (In) Rep" + IntToStr(E.ErrorCode) + "Класс ошибки = " +E.ClassName();
	   Synchronize(&ErrorLog);
	   resultat = false;
	   ScanVTIndy::Terminate();
	}*/
	catch(EIdHTTPProtocolException &E)
	{
	   ;//resultat = false;
	}

	catch(EIdOSSLConnectError &E)
	{
	   ;//resultat = false;
	}
	 catch (EIdSocketError &E)
	{
	   ;//resultat = false;
	}
	catch (EIdOpenSSLAPISSLError &E)
	{
	   ;//resultat = false;
	}
	catch (EIdConnClosedGracefully &E)
	{
	   ;//resultat = false;
	}

	catch(EIdException &E)// Другие исключения Indy
	{
		  //ErrorMessage = "Ошибка: try (DrIn)Rep\""+E.Message+"\"" + "Класс ошибки = " +E.ClassName();
		  ErrorLog("CalcMD5 = " + chesch +  "\n Ошибка EIdException TForm3::ReportVT.\n" + E.ToString());
	}

	catch(Exception &E)   // Другие НЕ Indy исключения
	{
		ErrorLog("CalcMD5 = " + chesch +  "\n Ошибка Exception TForm3::ReportVT.\n" + E.ToString());
	}

	return resultat;
}
//---------------------------------------------------------------------------


void __fastcall TForm3::ScanRevers(TObject *Sender)
{
	if((ListView1->Items->Count !=0) || (ListView2->Items->Count !=0) || (ListView3->Items->Count !=0))
	{
		int msgboxID = MessageBox(
			NULL,
			L"При восстановление сессии результаты текущей проверки будут утеряны.\nВы хотите продолжить восстановление сессии? ",
			L"Восстановление сессии",
			MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2

		);

		switch(msgboxID)
		{
			case IDYES:
				JsesonToListViewe();
				break;

			case IDNO:
				break;
		}
	}
	else
		JsesonToListViewe();

	 //
}
//---------------------------------------------------------------------------

void __fastcall TForm3::SaveSessionKak(TObject *Sender)
{
	Form3->SaveJeson->InitialDir = Form3->PatchProgramma + "AitotalTMP\\";
	Form3->SaveJeson->FileName = "jeson_" + FormatDateTime("YYYY.MM.DD",Date()) + "_" + FormatDateTime("HH-NN-SS",Time());

	if(Form3->SaveJeson->Execute())
		LVItemsToTextINJssonProg(Form3->SaveJeson->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TForm3::SaveSession(TObject *Sender)
{
   LVItemsToTextINJssonProg(Form3->PatchProgramma + "AitotalTMP\\jeson_" + FormatDateTime("YYYY.MM.DD",Date()) + "_" + FormatDateTime("HH-NN-SS",Time()));
}
//---------------------------------------------------------------------------

