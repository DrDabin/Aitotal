﻿// ---------------------------------------------------------------------------

#ifndef Aitotal1H
#define Aitotal1H
// --------------------------------------------------------------------------
#include <System.IOUtils.hpp>
#include <IdBaseComponent.hpp>
#include <Menus.hpp>
#include "ScanVTIndy1.h"
#include <Vcl.XPMan.hpp>
#include <IdComponent.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>

//#include <System.Hash.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Dialogs.hpp>
#include <System.Classes.hpp>

 UINT MY_MESSAGE();
// ---------------------------------------------------------------------------
class TForm3 : public TForm {
__published: // IDE-managed Components
	TMainMenu *MainMenu1;
	TPopupMenu *PopupMenu1;
	TPopupMenu *PopupMenu2;
	TOpenDialog *OpenDialog1;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TMenuItem *N6;
	TMenuItem *N7;
	TMenuItem *N12;
	TMenuItem *N13;
	TPopupMenu *PopupMenu3;
	TMenuItem *N14;
	TMenuItem *N15;
	TMenuItem *N16;
	TMenuItem *N17;
	TMenuItem *N18;
	TMenuItem *N19;
	TMenuItem *Hesh1;
	TMenuItem *N20;
	TMenuItem *N21;
	TTrayIcon *TrayIcon1;
	TPopupMenu *PopupTrayIcon;
	TMenuItem *N22;
	TMenuItem *N9;
	TMenuItem *N23;
	TMenuItem *N25;
	TPanel *Panel1;
	TCheckBox *ChReSca;
	TButton *Button1;
	TProgressBar *ProgressBar1;
	TEdit *Edit1;
	TButton *Button3;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TListView *ListView2;
	TTabSheet *TabSheet3;
	TListView *ListView3;
	TMenuItem *N8;
	TMenuItem *SHA2561;
	TMenuItem *N10;
	TMenuItem *N11;
	TMenuItem *N26;
	TMenuItem *N27;
	TPanel *Panel2;

	TLabel *TLabelItogo;

	TLabel *TLabelCountProwerki;

	TLabel *TLabelCountProwereno;
	TMenuItem *N30;
	TMenuItem *N31;
	TMenuItem *N32;
	TMenuItem *Pm3OtmeVidelit1;
	TMenuItem *N24;
	TMenuItem *N33;
	TMenuItem *N34;
	TSaveDialog *SaveDialog1;
	TMenuItem *N35;
	TMenuItem *N36;
	TMenuItem *N37;
	TMenuItem *N38;
	TMenuItem *N39;
	TMenuItem *N40;
	TMenuItem *MD51;
	TMenuItem *N41;
	TMenuItem *N28;
	TMenuItem *N42;
	TMenuItem *N43;
	TMenuItem *N44;
	TMenuItem *N45;
	TMenuItem *MD52;
	TMenuItem *N46;
	TMenuItem *N47;
	TLabel *Label1;
	TButton *Button2;
	TMenuItem *N29;
	TMenuItem *SHA2562;
	TMenuItem *SHA2563;
	TMenuItem *N48;
	TStatusBar *stat1;
	TMenuItem *N49;
	TMenuItem *N50;
	TMenuItem *N51;
	TMenuItem *N52;
	TMenuItem *N53;
	TMenuItem *N54;
	TMenuItem *N55;
	TIdSSLIOHandlerSocketOpenSSL *IdSSLIOHandlerSocketOpenSSL1;
	TMenuItem *N56;
	TMenuItem *N57;
	TListView *ListView1;
	TMenuItem *N58;
	TMenuItem *N59;
	TMenuItem *N60;
	TSaveDialog *SaveJeson;
	TOpenDialog *OpenJeson;
	TMenuItem *N61;
	TFileOpenDialog *FileOpenDialog1;

	void __fastcall FormActivate(TObject *Sender);
	void __fastcall MmExit(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall MmOpenFile(TObject *Sender);
	void __fastcall MmOpenDir(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Pm1Select(TObject *Sender);
	void __fastcall Pm1Scan(TObject *Sender);
	void __fastcall Pm1Reset(TObject *Sender);
	void __fastcall MyOption(TObject *Sender);
	void __fastcall MyTools(TObject *Sender);
	void __fastcall Pm3Delete(TObject *Sender);
	void __fastcall Pm1Delete(TObject *Sender);
	void __fastcall Pm3CopyNameFile(TObject *Sender);
	void __fastcall Pm3CopyPatchFiele(TObject *Sender);
	void __fastcall Pm3CopyPatchNameFile(TObject *Sender);
	void __fastcall Pm3CopyHeshMd5(TObject *Sender);
	void __fastcall Pm3CopyAdres(TObject *Sender);
	void __fastcall Pm3OpenDirFile(TObject *Sender);
	void __fastcall TreyIc(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall Pm2Delete(TObject *Sender);
	void __fastcall Pm3OpenReZultat(TObject *Sender);
	void __fastcall Pm3DeleteCheked(TObject *Sender);
	void __fastcall Edit1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall ListView3ColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall Pm1DeleteChecked(TObject *Sender);
	void __fastcall Pm2DeleteChecedListView(TObject *Sender);
	void __fastcall Pm2VidelitVse(TObject *Sender);
	void __fastcall Pm2OtmenaVideleviya(TObject *Sender);
	void __fastcall Pm3FileInfo(TObject *Sender);
	void __fastcall Pm1OpenDirFile(TObject *Sender);
	void __fastcall Pm2OpenDirFile(TObject *Sender);
	void __fastcall Pm3VidelitVse(TObject *Sender);
	void __fastcall Pm3OtmenaVidelit(TObject *Sender);
	void __fastcall Pm1OtmenaVideleniya(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall ListView2CustomDrawSubItem(TCustomListView *Sender, TListItem *Item,
          int SubItem, TCustomDrawState State, bool &DefaultDraw);
	void __fastcall ListView2CustomDrawItem(TCustomListView *Sender, TListItem *Item,
          TCustomDrawState State, bool &DefaultDraw);
	void __fastcall ListView3CustomDrawItem(TCustomListView *Sender, TListItem *Item,
          TCustomDrawState State, bool &DefaultDraw);
	void __fastcall ListView1CustomDrawItem(TCustomListView *Sender, TListItem *Item,
          TCustomDrawState State, bool &DefaultDraw);
	void __fastcall ListView1CustomDrawSubItem(TCustomListView *Sender, TListItem *Item,
          int SubItem, TCustomDrawState State, bool &DefaultDraw);
	void __fastcall ListView3CustomDrawSubItem(TCustomListView *Sender, TListItem *Item,
          int SubItem, TCustomDrawState State, bool &DefaultDraw);
	void __fastcall ListView2ColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall ListView1ColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall LV3SaveRezult(TObject *Sender);
	void __fastcall LV3PokaztRelt(TObject *Sender);
	void __fastcall LV3Clear(TObject *Sender);
	void __fastcall LWProverit(TObject *Sender);
	void __fastcall Pm2ScanCheked(TObject *Sender);
	void __fastcall Pm3OpenReZultCheked(TObject *Sender);
	void __fastcall LW2CopiNameFile(TObject *Sender);
	void __fastcall LW2CopiPath(TObject *Sender);
	void __fastcall LW2CopiMD5(TObject *Sender);
	void __fastcall LW2CopiSizeFile(TObject *Sender);
	void __fastcall LW2CopiNameFilePath(TObject *Sender);
	void __fastcall LW1CopiNameFile(TObject *Sender);
	void __fastcall LW1CopiPath(TObject *Sender);
	void __fastcall LW1CopiFileNamePatch(TObject *Sender);
	void __fastcall LW1CopiMD5(TObject *Sender);
	void __fastcall LW1CopiFileSize(TObject *Sender);
	void __fastcall About(TObject *Sender);
	void __fastcall LV1Diblklic(TObject *Sender);
	void __fastcall LV2Dblklic(TObject *Sender);
	void __fastcall SHA2561Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Pm2FileInfo(TObject *Sender);
	void __fastcall LV1CopySHA256(TObject *Sender);
	void __fastcall LV2CopySHA256(TObject *Sender);
	void __fastcall LV2CopySsilka(TObject *Sender);
	void __fastcall ApplicationEvents1Minimize(TObject *Sender);
	void __fastcall TrayIcon1BalloonClick(TObject *Sender);
	void __fastcall Pm2StopSelected(TObject *Sender);
	void __fastcall Pm2StopCHeked(TObject *Sender);
	void __fastcall Pm1FileInfo(TObject *Sender);
	void __fastcall Pm1SelectedToCheked(TObject *Sender);
	void __fastcall Pm2SelectedToCheked(TObject *Sender);
	void __fastcall Pm3SelectedToCheked(TObject *Sender);
	void __fastcall MM1ScanFolderArchiv(TObject *Sender);
	void __fastcall InsertToReestrMenu(TObject *Sender);
	void __fastcall DeleteReestr(TObject *Sender);
	void __fastcall LW1OnDeletion(TObject *Sender, TListItem *Item);
	void __fastcall LW2OnDeletion(TObject *Sender, TListItem *Item);
	void __fastcall LE2OnInsert(TObject *Sender, TListItem *Item);
	void __fastcall LW1OnInsert(TObject *Sender, TListItem *Item);
	void __fastcall LW3OnDeletions(TObject *Sender, TListItem *Item);
	void __fastcall LW3OnInsert(TObject *Sender, TListItem *Item);
	void __fastcall Pm2SaveLog(TObject *Sender);
	void __fastcall ScanRevers(TObject *Sender);
	void __fastcall SaveSessionKak(TObject *Sender);
	void __fastcall SaveSession(TObject *Sender);

private: // User declarations
	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_COPYDATA, TWMCopyData, GetMessage)
		MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, Response)
		MESSAGE_HANDLER (WM_QUERYENDSESSION, TMessage, WM_QueryEndSession)
	END_MESSAGE_MAP(TForm)
	// сообщение при разворачивание программы из трея.
	virtual void __fastcall WndProc(Winapi::Messages::TMessage &Message);
	void __fastcall TForm3::WM_QueryEndSession (TMessage &msg);
	void __fastcall TForm3::GetMessage(TWMCopyData &msg);
	//Выводит ответ при размере файла более 128 мб или 0 байт или -1байт.
	void __fastcall TForm3::OtWetErrorSizeFile(UnicodeString FileNime, UnicodeString MoveFileName,  __int64 sizefile, UnicodeString Md5);
	void __fastcall TForm3::ZanoshuVListWiew3PriOshibki(UnicodeString ArcName, __int64 MysizaFile , String rezult, String Heh, UnicodeString Sha256,bool date);
	int ApikeyCount;

public:// User declarations

     // Содержит порядковый номер файла. Что бы в базе не путаться  и в среней и последней вкладке
	// повторений не должно быть.
	// изменяю в ZanoshuVListWiew3PriOshibki,
	int FileNumber;
   __fastcall TForm3(TComponent* Owner);
	bool __fastcall TForm3::ReportVT (UnicodeString chesch);// Получаем отчет для хеш суммы с вирустотал
	 void __fastcall TForm3::ErrorLog(UnicodeString ErrorMessage);
	 void __fastcall Response(TWMDropFiles &Message);
	 void __fastcall TForm3::DeleteDirectoryTempArchiv(); //Удаляем папку с распакованными архивами.
	UnicodeString TForm3::MyFiledDirArchiv(UnicodeString Dir);
	UnicodeString TForm3::ExstractFileNameBezExt(UnicodeString NameFile);
	bool __fastcall TForm3::Kill(char * procname);
	void __fastcall TForm3::ListFiles(String path);
	void __fastcall TForm3::MiList(UnicodeString Put);
	AnsiString __fastcall TForm3::XOR_Crypt(AnsiString in, AnsiString pass);
	void TForm3::StartThreadVT();
	void __fastcall TForm3::PauseT(AnsiString pause_str);// делает паузу.
	void __fastcall TForm3::PauseProba(AnsiString pause_str);
	bool StopThread;
	bool StopThreadPopupMenu;// Если фолс то проводим проверку в PopupMene2. True не проводим проверку.
	//труе - логируем всё, что происходит в ScanVTIndy и сохраняем в файл
	// имя файла лога == имя проверяемого файла
	bool logirovanie;
	bool Mapasswor;
	bool StopInet; //

	//извлекаем файлы из ресурсов  применяется в IzvlechenieResursToFile()
	// NameResource имя ресурса. пример для 7z.dll  это 7zdll
	// NameFile имя в которое переименовываем файл изресурса.
	bool TForm3::ExtractResource(UnicodeString NameResource, UnicodeString NameFile);
	//Ф-я проверки на занятость файла.
	bool __fastcall TForm3::ProverkaNaIspolFila(UnicodeString filename);
	//извлекаем все файлы из ресурса.
	void TForm3::IzvlechenieResursToFile();
	ScanVTIndy *VTIndy[101];
	// Структура хранения настроек
	Options AtOptions;
	//Распаковываем архив
	void __fastcall TForm3::ExtArchiv(UnicodeString ArcName, int rescan);
	// Парсируем распакованный архив
	void __fastcall TForm3::ParsingArchiv(UnicodeString DirPatchArchiv, int rescan);
	void __fastcall TForm3::ParsingArchivAVZ_dta(UnicodeString FileName, TList *SpisokFileKarantin, int rescan);
	void __fastcall TForm3::ParsingArchivAVZ_dat(UnicodeString FileName, TList *SpisokFileKarantin, int rescan);
	void __fastcall TForm3::ParsingArchivUVS(UnicodeString FileName, TList *SpisokFileKarantin, int rescan );
	void __fastcall TForm3::ParsingArchivTDSciller(UnicodeString FileName, TList *SpisokFileKarantin, TStringList *SpisokFileArchiv, int &i, int rescan );
	//Заношу в TList структуру StructFileList в которой содержится значение о файле(рескмн, имя файла, размер и хеш суммы)
	void __fastcall TForm3::SpisokFileTList();
	 //bool exit; //false работаем,  true закрываем.
	TList *SpisokFileName;
	//новая версия. В основном для расчёта использую поток Stream
	void __fastcall TForm3::CopyTListToListViewofStream();
	//Общая ункция занесения данных в таблицы.
	//int numberListView номер ListViewe,  int FileNumber  номер файла по порядку.
	void __fastcall TForm3::InsertToListView(int numberListView, Strukt FileItem, int FileNumber);
	 //Разделяю ф-ю ScanFile на две.
	 //Это первая. Вней происходит парсинг файлов из  FileSpisok и перенос в ListView
	//bool __fastcall TForm3::FileSpisokToListWiew();
	//Очищаем список SpisikFileName
	void TForm3::ClearSpisokFileName();
	//останавливаем поток выбранный в ListWive2
	void TForm3::StopThreaSelected();
	//Удаляем потоки из листвайва у которых Cheked = true
	void TForm3::StopThreaCheked();
	void __fastcall TForm3::OptionReadIni();
	bool Scan;
	void __fastcall TForm3::MyFiledDir(UnicodeString Dir, int rescan);
	//Действие перед уничтожением потока.
	void __fastcall TForm3::AtThreadTerminated(TObject *Sender);
	void TForm3::ScanFile();
	AnsiString __fastcall TForm3::MyCryptDeCrypt(AnsiString Paroll, bool EncryptDecrypt);//функция шифрования.
    // шифрует файл ошибок Error
	AnsiString __fastcall TForm3::CryptDeCryptFileError(AnsiString Paroll, bool EncryptDecrypt);
	UnicodeString PatchProgramma;// содержит путь до программы.
	UnicodeString BaseJesson; //Заполняю в bool __fastcall TForm3::ReportVT (UnicodeString chesch)
	int http_response_code; //Код ответа сервера. Заполняю в bool __fastcall TForm3::ReportVT (UnicodeString chesch)
	//Создает поток и запускает его на и испонение.
	//Стараюсь все перенести из MyNewScan() всё, что связано с созданием и запуском потока.
	void __fastcall TForm3::CreateStartVT(Base VT,bool Proverka, int ApikeyNumber);

	TListItem *ListItem;
	TStringList *SpisokNamePotok;//(new TStringList(NULL));
	// Содержит список распакованных архивов, а именно AitotalTMP/archiv/имя архива
	//Это надо для того, что бы удалять только те распакованные архивы которые распаковывала данная копия.
	//Удаление происходит в FormDestroy()
	TStringList *SpisokArchivFolder;



};

// ---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
// ---------------------------------------------------------------------------
#endif

