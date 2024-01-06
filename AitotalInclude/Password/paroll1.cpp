﻿//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "paroll1.h"
#include "Aitotal1.h"
#include "AitotalInclude/archiv/extract.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TFParoll *FParoll;

UnicodeString LnMesIncorectPassword ="Неверный пароль";
UnicodeString LnMesErrorIncPassword = "Ошибка. Неверный пароль.\n Введите новый пароль или нажмите отмена" ;
UnicodeString LnMesErrorOpenArchiv = "Ошибка. Архив поврежден." ;
//---------------------------------------------------------------------------
__fastcall TFParoll::TFParoll(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFParoll::BtOkClick(TObject *Sender)
{
	if(EdParoll->Text != "")
	{
		int a =   extArchivToPassword(NameArchiv.w_str(), Dir.w_str(), EdParoll->Text.w_str(), GuidArchiv);
		Form3->ErrorLog("\"PasswordError = " +IntToStr(a)+"\"" +NameArchiv);
		switch(a)
		{
			case EX_OK:
				Form3->Mapasswor = true;
				FParoll->Close();
				break;
			case errorDataPass:
				EdParoll->Text = "";
				StatusBar1->Panels->Items[0]->Text = String(LnMesIncorectPassword);
				ShowMessage(LnMesErrorIncPassword);
				break;

			case errorHead:
				ShowMessage(LnMesErrorOpenArchiv);
				FParoll->Close();
				break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFParoll::CHPasswordClick(TObject *Sender)
{
   if(CHPassword->Checked)
   {
	  EdParoll->PasswordChar = '*';
   }
   else
	  EdParoll->PasswordChar = '\0';
}
//---------------------------------------------------------------------------

void __fastcall TFParoll::Button1Click(TObject *Sender)
{
   FParoll->Close();
}
//---------------------------------------------------------------------------


