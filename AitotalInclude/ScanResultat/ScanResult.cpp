﻿//---------------------------------------------------------------------------

#include <vcl.h>
#include <clipbrd.hpp>
#include <StrUtils.hpp>
#include <DateUtils.hpp>
#include <windows.h>
#include <DBXJSON.hpp>
#include <memory>    //std::auto_prt<>
#include <System.IOUtils.hpp>

#pragma hdrstop

#include "Aitotal1.h"
#include "ScanResult.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormResultScan *FormResultScan;

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
		ShowMessage("Ошибка парсинга отчёта.\n" + E.Message);
		Form3->ErrorLog("Ошибка парсинга отчёта. Форма результата.\n" +VTBase.BaseJesson);
	 }
  }
  else
	 ShowMessage("Ошибка. Отчёт по файлу не готов");

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
   if (permalink != "")
   {
	   ShellExecuteW(Handle, NULL,permalink.w_str(),0,0,SW_SHOWNORMAL);
   }
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
			 ShowMessage("Результат сохранён");
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

