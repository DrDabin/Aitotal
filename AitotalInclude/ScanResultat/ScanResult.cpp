//---------------------------------------------------------------------------

#include <vcl.h>
#include <clipbrd.hpp>
#include <StrUtils.hpp>
#include <DateUtils.hpp>
#include <windows.h>
#include <DBXJSON.hpp>
#include <memory>    //std::auto_prt<>
#include <IniFiles.hpp>     // <--  TIniFile
#include <System.IOUtils.hpp>

#pragma hdrstop

#include "Aitotal1.h"
#include "ScanResult.h"
#include "AitotalInclude/Options/Options1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormResultScan *FormResultScan;
UnicodeString LnMesErrorReportParsing = "Ошибка парсинга отчёта" ;
UnicodeString LnMesErrorfileReportNou = "Ошибка. Отчёт по файлу не готов";
UnicodeString LnMesResultSaved = "Результат сохранён" ;
UnicodeString LnMesNouInternet = "Нет интернета";

extern UnicodeString LnMesUploadFile ;
extern UnicodeString LnMesUnpakArchiv;
extern UnicodeString LnMesUnpak;
extern UnicodeString LnMesNoCMDParam  ;
extern UnicodeString LnMesErrorToFunc  ;
extern UnicodeString LnMesNoInterConect  ;
extern UnicodeString LnMesChoosingFolder  ;
extern UnicodeString LnMesAttentionError ;
extern UnicodeString LnMesResNotFound  ;
extern UnicodeString LnMesFileSize     ;
extern UnicodeString LnMesFileSizeMore ;
extern UnicodeString LnMesErrorSizeFile ;
extern UnicodeString LnMesErrorCalcSHA256 ;
extern UnicodeString LnMesErrorRenamFile  ;
extern UnicodeString LnMesAVZArchivParsing ;
extern UnicodeString LnMesUVSArchivParsing   ;
extern UnicodeString LnMesTDScillerArchivParsing  ;
extern UnicodeString LnMesArchivParsing    ;
extern UnicodeString LnMesParsingComplette ;
extern UnicodeString LnMesListEmti    ;
extern UnicodeString LnMesArchivStartUnpache   ;
extern UnicodeString LnMesArchivErrorUnpacking  ;
extern UnicodeString LnMesProcessParsingList ;
extern UnicodeString LnMesErrorHechSumm  ;
extern UnicodeString LnMesErroDetectMD5  ;
extern UnicodeString LnMesEror          ;
extern UnicodeString LnMesUnableOpenFile ;
extern UnicodeString LnMesInternetNotConnect;
extern UnicodeString LnMesNoresult   ;
extern UnicodeString LnMesInvalidCharacter ;
extern UnicodeString LnMesCharacter  ;
extern UnicodeString LnMesFileName ;
extern UnicodeString LnMesFilePatch            ;
extern UnicodeString LnMesResult              ;
extern UnicodeString LnMesSize                ;
extern UnicodeString LnMesDate                ;
extern UnicodeString LnMesLink                ;
extern UnicodeString LnMesChekFolderKarantin  ;
extern UnicodeString LnMesErrorOpenRegistr   ;
extern UnicodeString LnMesWarnVistaVer      ;
extern UnicodeString LnMesErrorDelRegistry   ;
extern UnicodeString LnMesInQueue        ;
extern UnicodeString LnMesFileToBeChek  ;
extern UnicodeString LnMesFilesChek     ;
extern UnicodeString LnMesErrorDeletion ;
extern UnicodeString LnMesErrorDelUnpackArchiv ;
extern UnicodeString LnMesUploadCheking  ;
extern UnicodeString LnMesFileChekBusy  ;
extern UnicodeString LnMesSessionRestor  ;
extern UnicodeString LnMesIfSesionIsRestor  ;
extern UnicodeString LnMesMaxFileUpload  ;
extern UnicodeString LnMesMaxFileScaning  ;
extern UnicodeString LnMesErrorsizeFileMin0  ;
extern UnicodeString LnMesIncorectPassword  ;
extern UnicodeString LnMesErrorIncPassword  ;
extern UnicodeString LnMesErrorOpenArchiv    ;
extern UnicodeString LnMesErrorReportParsing  ;
extern UnicodeString LnMesErrorfileReportNou ;
extern UnicodeString LnMesResultSaved         ;
extern UnicodeString LnMesErorCalcSizeFileOpe ;
extern UnicodeString LnMesErorCalcSizeFileCalc ;
extern UnicodeString LnMesErroeReadSignArchiv ;
extern UnicodeString LnMesErrorArchivCorupted ;
extern UnicodeString LnMesErrorNouSign      ;
extern UnicodeString LnMesErrorNouArchivSign ;
extern UnicodeString LnMesErrorNouTablSaveScaning;
extern UnicodeString LnMesStartOfScaning    ;
extern UnicodeString LnMesScaningIsStoped   ;
extern UnicodeString LnMesUplodCompl       ;
extern UnicodeString LnMesPerenosVTablScan   ;
extern UnicodeString LnMesErrorNetwork  ;
extern UnicodeString LnMesSearchHessshBaseVT ;
extern UnicodeString LnMesDisplToTablScan ;
extern UnicodeString LnMesScanStopped   ;
extern UnicodeString LnMesNouLimitApiKey  ;

extern UnicodeString LnMesFileMax32MB  ;
extern UnicodeString LnMesErrorUploadFile ;
extern UnicodeString LnMesAnalizFile  ;
extern UnicodeString LnMesObrabotResult ;
extern UnicodeString LnMesNoSovpadSHA256  ;
extern UnicodeString LnMesToVT          ;
extern UnicodeString LnMesFileVerified;
extern UnicodeString LnMesOtwetNarescan  ;
extern UnicodeString LnMesNouInternet;

int LV1nSortColumn=0;
bool LV1bAscend = false;


int Mycompare (int a, int b)
{
	if (a>b)
		 return 1;

	if (a<b)
		 return -1;

	return 0;
 }

int __stdcall SortProcMy(TListItem *Item1, TListItem *Item2, long Para)
{
   char sep;
   int I=0;

   switch(LV1nSortColumn)
   {
	  case 0:
		 I = AnsiCompareText(((TListItem *)Item1)->Caption, ((TListItem *)Item2)->Caption);
		 break;
	  case 1:
		 I = AnsiCompareText(((TListItem *)Item1)->SubItems->Strings[0], ((TListItem *)Item2)->SubItems->Strings[0]);
		 break;
	  case 2:
		 I = AnsiCompareText(((TListItem *)Item1)->SubItems->Strings[1].Trim(), ((TListItem *)Item2)->SubItems->Strings[1].Trim());
		 break;

	  case 3:
		 I = AnsiCompareText(((TListItem *)Item1)->SubItems->Strings[2].Trim(), ((TListItem *)Item2)->SubItems->Strings[2].Trim());
		 break;
   }

   if(!LV1bAscend)
	  I = -I;
   return I;// ...
}

//++++++++++++++++++++++++


//---------------------------------------------------------------------------
__fastcall TFormResultScan::TFormResultScan(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormResultScan::FormActivate(TObject *Sender)
{
   LBVTFileName->Caption = VTBase.BaseFileName;
   LBVtPathFile->Caption = ExtractFileDir(VTBase.BasePatchFileName);
   LBVtDataScan->Caption = VTBase.BaseDataProverki;
   LbResult->Caption = VTBase.BaseDetect;

  if(VTBase.BaseDetect != "")
   {
	   TStringDynArray arr(SplitString(VTBase.BaseDetect, "/"));

	   if(arr[0].ToInt() > 0)
	       LbResult->Font->Color = (TColor)RGB(251,38,182);

	   else
		   LbResult->Font->Color = (TColor)RGB(66,2182,41);
   }
   ListView1->Clear();

   if(VTBase.BaseJesson !="")
   {
	  // делаю для того, что бы окно появлялось поверх всех окон.
	  if(Tompost)
		 SetWindowPos(Handle, HWND_TOPMOST, Left, Top, Width, Height, SWP_NOACTIVATE || SWP_NOMOVE || SWP_NOSIZE);

	  std::auto_ptr<TJSONObject> ResRescan(static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(VTBase.BaseJesson)));

	  //что бы после активации   окно было не верхнем урвне, а то будет поверх всех окон.
	 SetWindowPos(Handle, HWND_NOTOPMOST, Left, Top, Width, Height, /*SWP_NOACTIVATE ||*/ SWP_NOMOVE || SWP_NOSIZE);

	  try
	  {
		 //permalink = ResRescan->Get("permalink")->JsonValue->Value();
		 //Закоментировал, так как сразу из ответа джейсона удалил лишнее, оставил только детект антивирусов()
		 // Всё из объекта scan. но без scan {Avast{}, drweb{} и т.д.}
		 //TJSONObject *obj = static_cast<TJSONObject*>(ResRescan->Get("scans")->JsonValue);

		 for( int i = 0; i<ResRescan->Count; i++)
		 {
			TJSONObject *Jarr = static_cast<TJSONObject*>(ResRescan->Pairs[i]->JsonValue);


			ListItem = ListView1->Items->Add();
			ListItem->Caption = ResRescan->Pairs[i]->JsonString->Value();

			if(Jarr->Get("category")->JsonValue->Value() != "malicious")
			{
			   ListItem->SubItems->Add("");
			}
			else
			{
			   ListItem->SubItems->Add(Jarr->Get("result")->JsonValue->Value());
			}

			ListItem->SubItems->Add(Jarr->Get("engine_version")->JsonValue->Value());
			update = Jarr->Get("engine_update")->JsonValue->Value();

			if(update.Length() ==8)
			{
			   update = update.Insert(".",5);
			   update = update.Insert(".",8);
			}
			ListItem->SubItems->Add(update);

		 }
		 SortProgramming(0);
		 SortProgramming(1);
		 SortProgramming(1);
		 //что бы после активации   окно было не верхнем урвне, а то будет поверх всех окон.
		 //SetWindowPos(Handle, HWND_NOTOPMOST, Left, Top, Width, Height, /*SWP_NOACTIVATE ||*/ SWP_NOMOVE || SWP_NOSIZE);
	 }
	 catch(Exception &E)
	 {
		ShowMessage(LnMesErrorReportParsing + ".\n" + E.Message);
		Form3->ErrorLog(LnMesErrorReportParsing +". Форма результата.\n" +VTBase.BaseJesson);
	 }
  }
  else
	 ShowMessage(LnMesErrorfileReportNou);

}
//---------------------------------------------------------------------------
void __fastcall TFormResultScan::ListView1ColumnClick(TObject *Sender, TListColumn *Column)

{
   if(LV1nSortColumn == Column->Index) LV1bAscend = !LV1bAscend;
	else
	{
		LV1nSortColumn = Column->Index;
		LV1bAscend = true;
	}
	ListView1->CustomSort((PFNLVCOMPARE)SortProcMy, Column->Index);
}
//---------------------------------------------------------------------------
void __fastcall TFormResultScan::SortProgramming(int i)
{
   if(LV1nSortColumn == i) LV1bAscend = !LV1bAscend;
	else
	{
		LV1nSortColumn = i;
		LV1bAscend = true;
	}
	ListView1->CustomSort((PFNLVCOMPARE)SortProcMy, i);
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void __fastcall TFormResultScan::ListView1CustomDrawItem(TCustomListView *Sender,
          TListItem *Item, TCustomDrawState State, bool &DefaultDraw)

{
   if((Item->Index%2) ==0)
   {
	   Sender->Canvas->Brush->Color = (TColor)RGB(235,235,235);
   }

}
//---------------------------------------------------------------------------

void __fastcall TFormResultScan::ListView1CustomDrawSubItem(TCustomListView *Sender,
          TListItem *Item, int SubItem, TCustomDrawState State, bool &DefaultDraw)

{
   if((Item->Index%2) ==0)
   {
	   Sender->Canvas->Brush->Color = (TColor)RGB(235,235,235);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormResultScan::LBVTFileNameMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
   LBVTFileName->Font->Style = LBVTFileName->Font->Style << fsUnderline << fsBold;
   LBVTFileName->Font->Color = (TColor)RGB(51,115,232);
}
//---------------------------------------------------------------------------

void __fastcall TFormResultScan::LBVTFileNameMouseLeave(TObject *Sender)
{
  LBVTFileName->Font->Style =  LBVTFileName->Font->Style >> fsUnderline >>fsBold;
  LBVTFileName->Font->Color = clBlack;
}
//---------------------------------------------------------------------------

void __fastcall TFormResultScan::LBSvoistvoFila(TObject *Sender)
{
   if(LBVTFileName->Caption != "" && LBVtPathFile->Caption !="")
	  {
		 SHELLEXECUTEINFOW SEI;
		 ZeroMemory(&SEI, sizeof(SHELLEXECUTEINFOW));
		 SEI.cbSize= sizeof(SHELLEXECUTEINFOW);
		 SEI.lpVerb = L"properties";
		 SEI.lpFile= UnicodeString(LBVtPathFile->Caption + "\\" + LBVTFileName->Caption).c_str();
		 SEI.nShow = SW_SHOW;
		 SEI.fMask= SEE_MASK_INVOKEIDLIST;
		 ShellExecuteExW(&SEI);
	  }
}
//---------------------------------------------------------------------------

void __fastcall TFormResultScan::OpenBrauzer(TObject *Sender)
{
   //изменил так как сейчас нет ссылки на результат.
   /*if (permalink != "")
   {
	   ShellExecuteW(Handle, NULL,permalink.w_str(),0,0,SW_SHOWNORMAL);
   }*/
   ShellExecuteW(Handle, NULL,VTBase.BaseAdress.w_str(),0,0,SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TFormResultScan::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
   if(Key == VK_ESCAPE)
   {
	  FormResultScan->Close();
   }
   //Отлов двух клавишь Alt+V
   /*if((GetAsyncKeyState(VK_MENU)!=0)&&(GetAsyncKeyState((int) 'V')!=0))
	 {
		 ShowMessage("Ошибка. Отчёт по файлу не готов");
	 }
	*/
}
//---------------------------------------------------------------------------
void __fastcall TFormResultScan::PMCopyDetect(TObject *Sender)
{
   if (ListView1->Selected != NULL)
   {
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView1->Selected->SubItems->Strings[0];
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormResultScan::PMCopyAllStroka(TObject *Sender)
{
   if (ListView1->Selected != NULL)
   {
	  std::auto_ptr<TClipboard>clp(new TClipboard);
	  clp->AsText= ListView1->Selected->Caption +"  " +
				   ListView1->Selected->SubItems->Strings[0] +"  " +
				   ListView1->Selected->SubItems->Strings[1] +"  " +
				   ListView1->Selected->SubItems->Strings[2];
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormResultScan::PMSvoistvoFile(TObject *Sender)
{
	if(VTBase.BasePatchFileName != "")
	{
		SHELLEXECUTEINFOW SEI;
		ZeroMemory(&SEI, sizeof(SHELLEXECUTEINFOW));
		SEI.cbSize= sizeof(SHELLEXECUTEINFOW);
		SEI.lpVerb = L"properties";
		SEI.lpFile= VTBase.BasePatchFileName.w_str();
		SEI.nShow = SW_SHOW;
		SEI.fMask= SEE_MASK_INVOKEIDLIST;
		ShellExecuteExW(&SEI);
	}

}
//---------------------------------------------------------------------------

void __fastcall TFormResultScan::PmOpenFilePatch(TObject *Sender)
{
	if (VTBase.BasePatchFileName != "")
	{
	  UnicodeString patch ="/select, " + VTBase.BasePatchFileName;
	  ShellExecuteW(Handle, NULL, L"explorer", patch.w_str(), NULL, SW_SHOWNORMAL);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormResultScan::PmSaveToFile(TObject *Sender)
{
	 std::auto_ptr<TStringList> Zapros (new TStringList(NULL) );
	 for(int i = 0; i < ListView1->Items->Count; i++)
	 {
		 Zapros->Add(ListView1->Items->Item[i]->Caption +"  " +
					 ListView1->Items->Item[i]->SubItems->Strings[0] +"  " +
					 ListView1->Items->Item[i]->SubItems->Strings[1] +"  " +
					 ListView1->Items->Item[i]->SubItems->Strings[2]
					) ;


	 }
	 if(SaveDialog->Execute())
	 {
		try{
			 Zapros->SaveToFile(SaveDialog->FileName+".txt",TEncoding::Unicode);
			 ShowMessage(LnMesResultSaved);
		}
		catch(Exception &E)
		{
			ShowMessage(E.Message);
		}


	 }
}
//---------------------------------------------------------------------------


void __fastcall TFormResultScan::exitResult(TObject *Sender)
{
   FormResultScan->Close();
}
//---------------------------------------------------------------------------
// Закоментировал. так как ни где пока не используется.
/* void __fastcall TFormResultScan::Execute(UnicodeString Lng)
{
	//   open ini by name
	//TMemIniFile * prog = new TMemIniFile();
 TIniFile * ini = new TIniFile(ExtractFilePath(Application->ExeName) +"rulng.lng");
//    all forms
  for (int i = 0; i < Application->ComponentCount ; i++)
  {
	 TForm *fm = (TForm *)Application->Components[i];
	   //if (fm->ClassNameIs("TfrSwLng")) continue;  // do not localize this form

//   if file not exists returm the old Caption
	if(fm->ClassNameIs("")) continue;
	 ini->WriteString(fm->Name, "Caption", fm->Caption);

//   Components on form
//   only ComponentCount  - TMenuItem is TComponent
	 for (int n = 0; n < fm->ComponentCount; n++) {
//     we have TRadioButton, TButton, TBitBtn, TAction
	  TObject * o = fm->Components[n];

	   if (o->ClassNameIs("TRadioButton")) {
		  TRadioButton * r = dynamic_cast<TRadioButton*>(o);
			ini->WriteString(fm->Name, r->Name, r->Caption);
	   }
	   if (o->ClassNameIs("TButton")) {
		  TButton * b = dynamic_cast<TButton*>(o);
			ini->WriteString(fm->Name, b->Name, b->Caption);
	   }
	   if (o->ClassNameIs("TLabel")) {
		  TLabel * b = dynamic_cast<TLabel*>(o);
			ini->WriteString(fm->Name, b->Name, b->Caption);
	   }

	   if (o->ClassNameIs("TPageControl")) {
		  TPageControl * b = dynamic_cast<TPageControl*>(o);
		  for(int i=0; i < b->ControlCount;i++){
			ini->WriteString(fm->Name, b->Pages[i]->Name, b->Pages[i]->Caption); }
	   }
	   if (o->ClassNameIs("TListView")) {
		  TListView * b = dynamic_cast<TListView*>(o);
		  for(int i=0; i < b->Columns->Count;i++)
		  {
			ini->WriteString(fm->Name,b->Name + b->Columns->Items[i]->Index, b->Columns->Items[i]->Caption);
		  }
	   }

	   if (o->ClassNameIs("TMenuItem"))
	   {
		  TMenuItem * b = dynamic_cast<TMenuItem*>(o);
			ini->WriteString(fm->Name, b->Name, b->Caption);
	   }
	   if (o->ClassNameIs("TCheckBox")) {
		  TCheckBox * b = dynamic_cast<TCheckBox*>(o);
			ini->WriteString(fm->Name, b->Name, b->Caption);
	   }
	   if (o->ClassNameIs("TLabeledEdit")) {
		  TLabeledEdit * b = dynamic_cast<TLabeledEdit*>(o);
			ini->WriteString(fm->Name, b->Name, b->EditLabel->Caption );
	   }
	   if (o->ClassNameIs("TComboBox")) {
		  TComboBox * b = dynamic_cast<TComboBox*>(o);

		  for(int i=0; i< b->Items->Count; i++)
			ini->WriteString(fm->Name, b->Name + b->Items->Strings[i], b->Items->Strings[i] );
	   }
	 }
  }

	TLanguageRec lr[83];
	lr[0].var = &LnMesUnpakArchiv;  // this address to frMain->sButton1Msg
	lr[0].id = "LnMesUnpakArchiv";  // id in *lng.ini
	lr[1].var = &LnMesUnpak;  // this address to frMain->sButton1Msg
	lr[1].id = "LnMesUnpak";  // id in *lng.ini
	lr[2].var = &LnMesNoCMDParam;  // this address to frMain->sButton1Msg
	lr[2].id = "LnMesNoCMDParam";  // id in *lng.ini
	lr[3].var = &LnMesErrorToFunc;  // this address to frMain->sButton1Msg
	lr[3].id = "LnMesErrorToFunc";  // id in *lng.ini
	lr[4].var = &LnMesNoInterConect;  // this address to frMain->sButton1Msg
	lr[4].id = "LnMesNoInterConect";  // id in *lng.ini
	lr[5].var = &LnMesChoosingFolder;  // this address to frMain->sButton1Msg
	lr[5].id = "LnMesChoosingFolder";  // id in *lng.ini
	lr[6].var = &LnMesAttentionError;  // this address to frMain->sButton1Msg
	lr[6].id = "LnMesAttentionError";  // id in *lng.ini
	lr[7].var = &LnMesResNotFound;  // this address to frMain->sButton1Msg
	lr[7].id = "LnMesResNotFound";  // id in *lng.ini
	lr[8].var = &LnMesFileSize;  // this address to frMain->sButton1Msg
	lr[8].id = "LnMesFileSize";  // id in *lng.ini
	lr[9].var = &LnMesFileSizeMore;  // this address to frMain->sButton1Msg
	lr[9].id = "LnMesFileSizeMore";  // id in *lng.ini
	lr[10].var = &LnMesErrorSizeFile;  // this address to frMain->sButton1Msg
	lr[10].id = "LnMesErrorSizeFile";  // id in *lng.ini
	lr[11].var = &LnMesErrorCalcSHA256;  // this address to frMain->sButton1Msg
	lr[11].id = "LnMesErrorCalcSHA256";  // id in *lng.ini
	lr[12].var = &LnMesErrorRenamFile;  // this address to frMain->sButton1Msg
	lr[12].id = "LnMesErrorRenamFile";  // id in *lng.ini
	lr[13].var = &LnMesAVZArchivParsing;  // this address to frMain->sButton1Msg
	lr[13].id = "LnMesAVZArchivParsing";  // id in *lng.ini
	lr[14].var = &LnMesUVSArchivParsing;  // this address to frMain->sButton1Msg
	lr[14].id = "LnMesUVSArchivParsing";  // id in *lng.ini
	lr[15].var = &LnMesTDScillerArchivParsing;  // this address to frMain->sButton1Msg
	lr[15].id = "LnMesTDScillerArchivParsing";  // id in *lng.ini
	lr[16].var = &LnMesArchivParsing;  // this address to frMain->sButton1Msg
	lr[16].id = "LnMesArchivParsing";  // id in *lng.ini
	lr[17].var = &LnMesParsingComplette;  // this address to frMain->sButton1Msg
	lr[17].id = "LnMesParsingComplette";  // id in *lng.ini
	lr[18].var = &LnMesListEmti;  // this address to frMain->sButton1Msg
	lr[18].id = "LnMesListEmti";  // id in *lng.ini
	lr[19].var = &LnMesArchivStartUnpache;  // this address to frMain->sButton1Msg
	lr[19].id = "LnMesArchivStartUnpache";  // id in *lng.ini
	lr[20].var = &LnMesArchivErrorUnpacking;  // this address to frMain->sButton1Msg
	lr[20].id = "LnMesArchivErrorUnpacking";
	lr[21].var = &LnMesProcessParsingList;  // this address to frMain->sButton1Msg
	lr[21].id = "LnMesProcessParsingList";  // id in *lng.ini
	lr[22].var = &LnMesErroDetectMD5;  // this address to frMain->sButton1Msg
	lr[22].id = "LnMesErroDetectMD5";  // id in *lng.ini
	lr[23].var = &LnMesEror;  // this address to frMain->sButton1Msg
	lr[23].id = "LnMesEror";  // id in *lng.ini
	lr[24].var = &LnMesUnableOpenFile;  // this address to frMain->sButton1Msg
	lr[24].id = "LnMesUnableOpenFile";  // id in *lng.ini
	lr[25].var = &LnMesInternetNotConnect;  // this address to frMain->sButton1Msg
	lr[25].id = "LnMesInternetNotConnect";  // id in *lng.ini
	lr[26].var = &LnMesNoresult;  // this address to frMain->sButton1Msg
	lr[26].id = "LnMesNoresult";  // id in *lng.ini
	lr[27].var = &LnMesInvalidCharacter;  // this address to frMain->sButton1Msg
	lr[27].id = "LnMesInvalidCharacter";  // id in *lng.ini
	lr[28].var = &LnMesCharacter;  // this address to frMain->sButton1Msg
	lr[28].id = "LnMesCharacter";  // id in *lng.ini
	lr[29].var = &LnMesFileName;  // this address to frMain->sButton1Msg
	lr[29].id = "LnMesFileName";  // id in *lng.ini
	lr[30].var = &LnMesFilePatch;  // this address to frMain->sButton1Msg
	lr[30].id = "LnMesFilePatch";
	lr[31].var = &LnMesResult;  // this address to frMain->sButton1Msg
	lr[31].id = "LnMesResult";  // id in *lng.ini
	lr[32].var = &LnMesSize;  // this address to frMain->sButton1Msg
	lr[32].id = "LnMesSize";  // id in *lng.ini
	lr[33].var = &LnMesDate;  // this address to frMain->sButton1Msg
	lr[33].id = "LnMesDate";  // id in *lng.ini
	lr[34].var = &LnMesLink;  // this address to frMain->sButton1Msg
	lr[34].id = "LnMesLink";  // id in *lng.ini
	lr[35].var = &LnMesChekFolderKarantin;  // this address to frMain->sButton1Msg
	lr[35].id = "LnMesChekFolderKarantin";  // id in *lng.ini
	lr[36].var = &LnMesErrorOpenRegistr;  // this address to frMain->sButton1Msg
	lr[36].id = "LnMesErrorOpenRegistr";  // id in *lng.ini
	lr[37].var = &LnMesWarnVistaVer;  // this address to frMain->sButton1Msg
	lr[37].id = "LnMesWarnVistaVer";  // id in *lng.ini
	lr[38].var = &LnMesErrorDelRegistry;  // this address to frMain->sButton1Msg
	lr[38].id = "LnMesErrorDelRegistry";  // id in *lng.ini
	lr[39].var = &LnMesInQueue;  // this address to frMain->sButton1Msg
	lr[39].id = "LnMesInQueue";  // id in *lng.ini
	lr[40].var = &LnMesFileToBeChek;  //
	lr[40].id = "LnMesFileToBeChek";
	lr[41].var = &LnMesFilesChek;  //
	lr[41].id = "LnMesFilesChek";
	lr[42].var = &LnMesErrorDeletion;  //
	lr[42].id = "LnMesErrorDeletion";
	lr[43].var = &LnMesErrorDelUnpackArchiv;  //
	lr[43].id = "LnMesErrorDelUnpackArchiv";
	lr[44].var = &LnMesUploadCheking;  //
	lr[44].id = "LnMesUploadCheking";
	lr[45].var = &LnMesFileChekBusy;  //
	lr[45].id = "LnMesFileChekBusy";
	lr[46].var = &LnMesSessionRestor;  //
	lr[46].id = "LnMesSessionRestor";
	lr[47].var = &LnMesIfSesionIsRestor;  //
	lr[47].id = "LnMesIfSesionIsRestor";
	lr[48].var = &LnMesMaxFileUpload;  //
	lr[48].id = "LnMesMaxFileUpload";
	lr[49].var = &LnMesMaxFileScaning;  //
	lr[49].id = "LnMesMaxFileScaning";
	lr[50].var = &LnMesErrorsizeFileMin0;  //
	lr[50].id = "LnMesErrorsizeFileMin0";
	lr[51].var = &LnMesIncorectPassword;  //
	lr[51].id = "LnMesIncorectPassword";
	lr[52].var = &LnMesErrorIncPassword;  //
	lr[52].id = "LnMesErrorIncPassword";
	lr[53].var = &LnMesErrorOpenArchiv;  //
	lr[53].id = "LnMesErrorOpenArchiv";
	lr[54].var = &LnMesErrorReportParsing;  //
	lr[54].id = "LnMesErrorReportParsing";
	lr[55].var = &LnMesErrorfileReportNou;  //
	lr[55].id = "LnMesErrorfileReportNou";
	lr[56].var = &LnMesResultSaved;  //
	lr[56].id = "LnMesResultSaved";
	lr[57].var = &LnMesErorCalcSizeFileOpe;  //
	lr[57].id = "LnMesErorCalcSizeFileOpe";
	lr[58].var = &LnMesErorCalcSizeFileCalc;  //
	lr[58].id = "LnMesErorCalcSizeFileCalc";
	lr[59].var = &LnMesErroeReadSignArchiv;  //
	lr[59].id = "LnMesErroeReadSignArchiv";
	lr[60].var = &LnMesErrorArchivCorupted;  //
	lr[60].id = "LnMesErrorArchivCorupted";
	lr[61].var = &LnMesErrorNouSign;  //
	lr[61].id = "LnMesErrorNouSign";
	lr[62].var = &LnMesErrorNouArchivSign;  //
	lr[62].id = "LnMesErrorNouArchivSign";
	lr[63].var = &LnMesErrorNouTablSaveScaning;  //
	lr[63].id = "LnMesErrorNouTablSaveScaning";
	lr[64].var = &LnMesStartOfScaning;  //
	lr[64].id = "LnMesStartOfScaning";
	lr[65].var = &LnMesScaningIsStoped;  //
	lr[65].id = "LnMesScaningIsStoped";
	lr[66].var = &LnMesUplodCompl;  //
	lr[66].id = "LnMesUplodCompl";
	lr[67].var = &LnMesPerenosVTablScan;  //
	lr[67].id = "LnMesPerenosVTablScan";
	lr[68].var = &LnMesErrorNetwork;  //
	lr[68].id = "LnMesErrorNetwork";
	lr[69].var = &LnMesSearchHessshBaseVT;  //
	lr[69].id = "LnMesSearchHessshBaseVT";
	lr[70].var = &LnMesDisplToTablScan;  //
	lr[70].id = "LnMesDisplToTablScan";
	lr[71].var = &LnMesScanStopped;  //
	lr[71].id = "LnMesScanStopped";
	lr[72].var = &LnMesNouLimitApiKey;  //
	lr[72].id = "LnMesNouLimitApiKey";
	lr[73].var = &LnMesUploadFile;  //
	lr[73].id = "LnMesUploadFile";
	lr[74].var = &LnMesFileMax32MB;  //
	lr[74].id = "LnMesFileMax32MB";
	lr[75].var = &LnMesErrorUploadFile;  //
	lr[75].id = "LnMesErrorUploadFile";
	lr[76].var = &LnMesAnalizFile;  //
	lr[76].id = "LnMesAnalizFile";
	lr[77].var = &LnMesObrabotResult;  //
	lr[77].id = "LnMesObrabotResult";
	lr[78].var = &LnMesNoSovpadSHA256;  //
	lr[78].id = "LnMesNoSovpadSHA256";
	lr[79].var = &LnMesToVT;  //
	lr[79].id = "LnMesToVT";
	lr[80].var = &LnMesFileVerified;  //
	lr[80].id = "LnMesFileVerified";
	lr[81].var = &LnMesOtwetNarescan;  //
	lr[81].id = "LnMesOtwetNarescan";
	lr[82].var = &LnMesNouInternet;
	lr[82].id = "LnMesNouInternet";
	for (int i = 0; i < 83; i++)
   *(UnicodeString*)lr[i].var = ini->ReadString("Variables", lr[i].id, *(UnicodeString*)lr[i].var);
  delete ini;
} */

//+++++++++++++++++++++
void __fastcall TFormResultScan::Langua(UnicodeString Lng)
{
	//   open ini by name
	wchar_t *pchResType = L"BINARY";
   std::auto_ptr<TMemIniFile> ini (new TMemIniFile("") );
   HRSRC hSrsrc=FindResourceW(HInstance,ChangeFileExt(Lng,"").c_str(),pchResType);
   if(hSrsrc ==NULL)
   {
	  ShowMessage("ResursNout =" + ChangeFileExt(Lng,"") + "=" +Lng);
	  ;//ErrorLog("Ресурс не найденNameResource");
	  ;//return false;
   }

	else
	{
		HGLOBAL hGlobal=LoadResource(HInstance,hSrsrc);
		if(hGlobal!=NULL)
		{
			void *pvResPtr=LockResource(hGlobal);
			if(pvResPtr==NULL)
			{
				ShowMessage("ResursNout= NULL =" +ChangeFileExt(Lng,""));;//return false;
			}
			else
			{
			  std::auto_ptr<TMemoryStream> Zapros (new TMemoryStream() );
			  std::auto_ptr<TStringList> ZaprosList (new TStringList() );

			   //Размер ресурса
			  unsigned long  vulResourceSize=SizeofResource(NULL,hSrsrc);

			  Zapros->WriteBuffer(pvResPtr,vulResourceSize);
			  Zapros->Position = 0;
			  ZaprosList->LoadFromStream(Zapros.get()) ;
			  ini->SetStrings(ZaprosList.get());
			  FreeResource(hGlobal);
			  //return true;
			}
	   }
	   else
		ShowMessage("hGlobal==NULL");


		for (int i = 0; i < Application->ComponentCount ; i++)
		{
			TForm *fm = (TForm *)Application->Components[i];
			//if (fm->ClassNameIs("TfrSwLng")) continue;  // do not localize this form

			//   		if file not exists returm the old Caption
			if(fm->ClassNameIs("")) continue;
			//ini->WriteString(fm->Name, "Caption", fm->Caption);

			//   		Components on form
			//   only ComponentCount  - TMenuItem is TComponent
			for (int n = 0; n < fm->ComponentCount; n++)
			{
				//we have TRadioButton, TButton, TBitBtn, TAction
				TObject * o = fm->Components[n];

				if (o->ClassNameIs("TRadioButton")) {
					TRadioButton * r = dynamic_cast<TRadioButton*>(o);
					r->Caption = ini->ReadString(fm->Name, r->Name, r->Caption);
				}
				if (o->ClassNameIs("TButton")) {
					TButton * b = dynamic_cast<TButton*>(o);
					b->Caption = ini->ReadString(fm->Name, b->Name, b->Caption);
				}
				if (o->ClassNameIs("TLabel")) {
					TLabel * b = dynamic_cast<TLabel*>(o);
					b->Caption = ini->ReadString(fm->Name, b->Name, b->Caption);
				}

				if (o->ClassNameIs("TPageControl"))
				{
					TPageControl * b = dynamic_cast<TPageControl*>(o);
					for(int i=0; i < b->ControlCount;i++)
						b->Pages[i]->Caption = ini->ReadString(fm->Name, b->Pages[i]->Name, b->Pages[i]->Caption);
				}
				if (o->ClassNameIs("TListView"))
				{
					TListView * b = dynamic_cast<TListView*>(o);
					for(int i=0; i < b->Columns->Count;i++)
						b->Columns->Items[i]->Caption = ini->ReadString(fm->Name,b->Name + IntToStr(b->Columns->Items[i]->Index), b->Columns->Items[i]->Caption);
				}

				if (o->ClassNameIs("TMenuItem"))
				{
					TMenuItem * b = dynamic_cast<TMenuItem*>(o);
					//ini->WriteString(fm->Name, b->Name, b->Caption);
					b->Caption = ini->ReadString(fm->Name, b->Name, b->Caption);
				}
				if (o->ClassNameIs("TCheckBox")) {
					TCheckBox * b = dynamic_cast<TCheckBox*>(o);
					b->Caption = ini->ReadString(fm->Name, b->Name, b->Caption);
				}
				if (o->ClassNameIs("TLabeledEdit")) {
					TLabeledEdit * b = dynamic_cast<TLabeledEdit*>(o);
					//ini->WriteString(fm->Name, b->Name, b->EditLabel->Caption );
					b->EditLabel->Caption = ini->ReadString(fm->Name, b->Name, b->EditLabel->Caption);
				}
				if (o->ClassNameIs("TComboBox")) {
					TComboBox * b = dynamic_cast<TComboBox*>(o);

					for(int i=0; i< b->Items->Count; i++)
						b->Items->Strings[i] = ini->ReadString(fm->Name, b->Name + IntToStr(i), b->Items->Strings[i]);
				}
			}
		}
	}

	TLanguageRec lr[83];
	lr[0].var = &LnMesUnpakArchiv;  // this address to frMain->sButton1Msg
	lr[0].id = "LnMesUnpakArchiv";  // id in *lng.ini
	lr[1].var = &LnMesUnpak;  // this address to frMain->sButton1Msg
	lr[1].id = "LnMesUnpak";  // id in *lng.ini
	lr[2].var = &LnMesNoCMDParam;  // this address to frMain->sButton1Msg
	lr[2].id = "LnMesNoCMDParam";  // id in *lng.ini
	lr[3].var = &LnMesErrorToFunc;  // this address to frMain->sButton1Msg
	lr[3].id = "LnMesErrorToFunc";  // id in *lng.ini
	lr[4].var = &LnMesNoInterConect;  // this address to frMain->sButton1Msg
	lr[4].id = "LnMesNoInterConect";  // id in *lng.ini
	lr[5].var = &LnMesChoosingFolder;  // this address to frMain->sButton1Msg
	lr[5].id = "LnMesChoosingFolder";  // id in *lng.ini
	lr[6].var = &LnMesAttentionError;  // this address to frMain->sButton1Msg
	lr[6].id = "LnMesAttentionError";  // id in *lng.ini
	lr[7].var = &LnMesResNotFound;  // this address to frMain->sButton1Msg
	lr[7].id = "LnMesResNotFound";  // id in *lng.ini
	lr[8].var = &LnMesFileSize;  // this address to frMain->sButton1Msg
	lr[8].id = "LnMesFileSize";  // id in *lng.ini
	lr[9].var = &LnMesFileSizeMore;  // this address to frMain->sButton1Msg
	lr[9].id = "LnMesFileSizeMore";  // id in *lng.ini
	lr[10].var = &LnMesErrorSizeFile;  // this address to frMain->sButton1Msg
	lr[10].id = "LnMesErrorSizeFile";  // id in *lng.ini
	lr[11].var = &LnMesErrorCalcSHA256;  // this address to frMain->sButton1Msg
	lr[11].id = "LnMesErrorCalcSHA256";  // id in *lng.ini
	lr[12].var = &LnMesErrorRenamFile;  // this address to frMain->sButton1Msg
	lr[12].id = "LnMesErrorRenamFile";  // id in *lng.ini
	lr[13].var = &LnMesAVZArchivParsing;  // this address to frMain->sButton1Msg
	lr[13].id = "LnMesAVZArchivParsing";  // id in *lng.ini
	lr[14].var = &LnMesUVSArchivParsing;  // this address to frMain->sButton1Msg
	lr[14].id = "LnMesUVSArchivParsing";  // id in *lng.ini
	lr[15].var = &LnMesTDScillerArchivParsing;  // this address to frMain->sButton1Msg
	lr[15].id = "LnMesTDScillerArchivParsing";  // id in *lng.ini
	lr[16].var = &LnMesArchivParsing;  // this address to frMain->sButton1Msg
	lr[16].id = "LnMesArchivParsing";  // id in *lng.ini
	lr[17].var = &LnMesParsingComplette;  // this address to frMain->sButton1Msg
	lr[17].id = "LnMesParsingComplette";  // id in *lng.ini
	lr[18].var = &LnMesListEmti;  // this address to frMain->sButton1Msg
	lr[18].id = "LnMesListEmti";  // id in *lng.ini
	lr[19].var = &LnMesArchivStartUnpache;  // this address to frMain->sButton1Msg
	lr[19].id = "LnMesArchivStartUnpache";  // id in *lng.ini
	lr[20].var = &LnMesArchivErrorUnpacking;  // this address to frMain->sButton1Msg
	lr[20].id = "LnMesArchivErrorUnpacking";
	lr[21].var = &LnMesProcessParsingList;  // this address to frMain->sButton1Msg
	lr[21].id = "LnMesProcessParsingList";  // id in *lng.ini
	lr[22].var = &LnMesErroDetectMD5;  // this address to frMain->sButton1Msg
	lr[22].id = "LnMesErroDetectMD5";  // id in *lng.ini
	lr[23].var = &LnMesEror;  // this address to frMain->sButton1Msg
	lr[23].id = "LnMesEror";  // id in *lng.ini
	lr[24].var = &LnMesUnableOpenFile;  // this address to frMain->sButton1Msg
	lr[24].id = "LnMesUnableOpenFile";  // id in *lng.ini
	lr[25].var = &LnMesInternetNotConnect;  // this address to frMain->sButton1Msg
	lr[25].id = "LnMesInternetNotConnect";  // id in *lng.ini
	lr[26].var = &LnMesNoresult;  // this address to frMain->sButton1Msg
	lr[26].id = "LnMesNoresult";  // id in *lng.ini
	lr[27].var = &LnMesInvalidCharacter;  // this address to frMain->sButton1Msg
	lr[27].id = "LnMesInvalidCharacter";  // id in *lng.ini
	lr[28].var = &LnMesCharacter;  // this address to frMain->sButton1Msg
	lr[28].id = "LnMesCharacter";  // id in *lng.ini
	lr[29].var = &LnMesFileName;  // this address to frMain->sButton1Msg
	lr[29].id = "LnMesFileName";  // id in *lng.ini
	lr[30].var = &LnMesFilePatch;  // this address to frMain->sButton1Msg
	lr[30].id = "LnMesFilePatch";
	lr[31].var = &LnMesResult;  // this address to frMain->sButton1Msg
	lr[31].id = "LnMesResult";  // id in *lng.ini
	lr[32].var = &LnMesSize;  // this address to frMain->sButton1Msg
	lr[32].id = "LnMesSize";  // id in *lng.ini
	lr[33].var = &LnMesDate;  // this address to frMain->sButton1Msg
	lr[33].id = "LnMesDate";  // id in *lng.ini
	lr[34].var = &LnMesLink;  // this address to frMain->sButton1Msg
	lr[34].id = "LnMesLink";  // id in *lng.ini
	lr[35].var = &LnMesChekFolderKarantin;  // this address to frMain->sButton1Msg
	lr[35].id = "LnMesChekFolderKarantin";  // id in *lng.ini
	lr[36].var = &LnMesErrorOpenRegistr;  // this address to frMain->sButton1Msg
	lr[36].id = "LnMesErrorOpenRegistr";  // id in *lng.ini
	lr[37].var = &LnMesWarnVistaVer;  // this address to frMain->sButton1Msg
	lr[37].id = "LnMesWarnVistaVer";  // id in *lng.ini
	lr[38].var = &LnMesErrorDelRegistry;  // this address to frMain->sButton1Msg
	lr[38].id = "LnMesErrorDelRegistry";  // id in *lng.ini
	lr[39].var = &LnMesInQueue;  // this address to frMain->sButton1Msg
	lr[39].id = "LnMesInQueue";  // id in *lng.ini
	lr[40].var = &LnMesFileToBeChek;  //
	lr[40].id = "LnMesFileToBeChek";
	lr[41].var = &LnMesFilesChek;  //
	lr[41].id = "LnMesFilesChek";
	lr[42].var = &LnMesErrorDeletion;  //
	lr[42].id = "LnMesErrorDeletion";
	lr[43].var = &LnMesErrorDelUnpackArchiv;  //
	lr[43].id = "LnMesErrorDelUnpackArchiv";
	lr[44].var = &LnMesUploadCheking;  //
	lr[44].id = "LnMesUploadCheking";
	lr[45].var = &LnMesFileChekBusy;  //
	lr[45].id = "LnMesFileChekBusy";
	lr[46].var = &LnMesSessionRestor;  //
	lr[46].id = "LnMesSessionRestor";
	lr[47].var = &LnMesIfSesionIsRestor;  //
	lr[47].id = "LnMesIfSesionIsRestor";
	lr[48].var = &LnMesMaxFileUpload;  //
	lr[48].id = "LnMesMaxFileUpload";
	lr[49].var = &LnMesMaxFileScaning;  //
	lr[49].id = "LnMesMaxFileScaning";
	lr[50].var = &LnMesErrorsizeFileMin0;  //
	lr[50].id = "LnMesErrorsizeFileMin0";
	lr[51].var = &LnMesIncorectPassword;  //
	lr[51].id = "LnMesIncorectPassword";
	lr[52].var = &LnMesErrorIncPassword;  //
	lr[52].id = "LnMesErrorIncPassword";
	lr[53].var = &LnMesErrorOpenArchiv;  //
	lr[53].id = "LnMesErrorOpenArchiv";
	lr[54].var = &LnMesErrorReportParsing;  //
	lr[54].id = "LnMesErrorReportParsing";
	lr[55].var = &LnMesErrorfileReportNou;  //
	lr[55].id = "LnMesErrorfileReportNou";
	lr[56].var = &LnMesResultSaved;  //
	lr[56].id = "LnMesResultSaved";
	lr[57].var = &LnMesErorCalcSizeFileOpe;  //
	lr[57].id = "LnMesErorCalcSizeFileOpe";
	lr[58].var = &LnMesErorCalcSizeFileCalc;  //
	lr[58].id = "LnMesErorCalcSizeFileCalc";
	lr[59].var = &LnMesErroeReadSignArchiv;  //
	lr[59].id = "LnMesErroeReadSignArchiv";
	lr[60].var = &LnMesErrorArchivCorupted;  //
	lr[60].id = "LnMesErrorArchivCorupted";
	lr[61].var = &LnMesErrorNouSign;  //
	lr[61].id = "LnMesErrorNouSign";
	lr[62].var = &LnMesErrorNouArchivSign;  //
	lr[62].id = "LnMesErrorNouArchivSign";
	lr[63].var = &LnMesErrorNouTablSaveScaning;  //
	lr[63].id = "LnMesErrorNouTablSaveScaning";
	lr[64].var = &LnMesStartOfScaning;  //
	lr[64].id = "LnMesStartOfScaning";
	lr[65].var = &LnMesScaningIsStoped;  //
	lr[65].id = "LnMesScaningIsStoped";
	lr[66].var = &LnMesUplodCompl;  //
	lr[66].id = "LnMesUplodCompl";
	lr[67].var = &LnMesPerenosVTablScan;  //
	lr[67].id = "LnMesPerenosVTablScan";
	lr[68].var = &LnMesErrorNetwork;  //
	lr[68].id = "LnMesErrorNetwork";
	lr[69].var = &LnMesSearchHessshBaseVT;  //
	lr[69].id = "LnMesSearchHessshBaseVT";
	lr[70].var = &LnMesDisplToTablScan;  //
	lr[70].id = "LnMesDisplToTablScan";
	lr[71].var = &LnMesScanStopped;  //
	lr[71].id = "LnMesScanStopped";
	lr[72].var = &LnMesNouLimitApiKey;  //
	lr[72].id = "LnMesNouLimitApiKey";
	lr[73].var = &LnMesUploadFile;  //
	lr[73].id = "LnMesUploadFile";
	lr[74].var = &LnMesFileMax32MB;  //
	lr[74].id = "LnMesFileMax32MB";
	lr[75].var = &LnMesErrorUploadFile;  //
	lr[75].id = "LnMesErrorUploadFile";
	lr[76].var = &LnMesAnalizFile;  //
	lr[76].id = "LnMesAnalizFile";
	lr[77].var = &LnMesObrabotResult;  //
	lr[77].id = "LnMesObrabotResult";
	lr[78].var = &LnMesNoSovpadSHA256;  //
	lr[78].id = "LnMesNoSovpadSHA256";
	lr[79].var = &LnMesToVT;  //
	lr[79].id = "LnMesToVT";
	lr[80].var = &LnMesFileVerified;  //
	lr[80].id = "LnMesFileVerified";
	lr[81].var = &LnMesOtwetNarescan;  //
	lr[81].id = "LnMesOtwetNarescan";
	lr[82].var = &LnMesNouInternet;
	lr[82].id = "LnMesNouInternet";
	for (int i = 0; i < 83; i++)
		*(UnicodeString*)lr[i].var = ini->ReadString("Variables", lr[i].id, *(UnicodeString*)lr[i].var);
}

//+++++++++++++++++++
void __fastcall TFormResultScan::FormCreate(TObject *Sender)
{
	std::auto_ptr<TIniFile> ini (new TIniFile(ExtractFilePath(Application->ExeName) + "tools\\options.ini"));
	UnicodeString LangName = ini->ReadString("Tools","LanguageFile","");

	if(LangName !="")
	{

		if(LangName == "English")
		{

			Langua(LangName);
		}
		else
			Langua("Russian");
	}
	else
	{
		//switch(GetSystemDefaultUILanguage())
		switch(GetUserDefaultUILanguage()) //локаль пользователя, а не системы.
		{

			case 0x0409: Langua("English.lng");//Английский
						ini->WriteString("Tools","LanguageFile","English");
						break;
			case 0x0419:  ; //Русский
			case 0x422: ;  //Украинский
						Langua("Russian.lng");
						ini->WriteString("Tools","LanguageFile","Russian");
						break;
			default: Langua("English.lng");
						ini->WriteString("Tools","LanguageFile","English");

		}
    }


}
//---------------------------------------------------------------------------

