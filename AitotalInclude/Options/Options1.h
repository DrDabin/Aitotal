﻿//---------------------------------------------------------------------------

#ifndef Options1H
#define Options1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include "Aitotal1.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdCustomTransparentProxy.hpp>
#include <IdSocks.hpp>
//---------------------------------------------------------------------------
class TMyOptionsForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPageControl *MyOptionsPage;
	TTabSheet *MyOptionsNetworc;
	TTabSheet *MyOptionsGlobal;
	TPanel *Panel2;
	TPanel *Panel3;
	TLabeledEdit *OptProxi;
	TLabeledEdit *OptIpPort;
	TActionList *ActionList1;
	TAction *MuOptionsSeve;
	TPanel *Panel4;
	TButton *Button1;
	TAction *MyOptionsReadIni;
	TLabel *Label1;
	TEdit *OpTionFileCount;
	TLabeledEdit *OptProxiPassword;
	TLabeledEdit *OptProxiLogin;
	TCheckBox *ProxyChecked;
	TLabel *Label2;
	TLabel *Label3;
	TRadioButton *RBExit;
	TRadioButton *RBTrei;
	TLabel *Label4;
	TLabel *Label5;
	TEdit *OptionThreadCount;
	TLabeledEdit *OptApikey;
	TCheckBox *ErrorArchiv;
	TButton *BtTestProxi;
	TButton *ToolsReset;
	TCheckBox *RunAplicatDubl;
	TLabel *TypSocket;
	TComboBox *ComboBox1;
	TCheckBox *CBBallonhint;
	void __fastcall OptionSave(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall OptionReadIni(TObject *Sender);
	void __fastcall OpTionFileCountKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall ProxyCheckedClick(TObject *Sender);
	void __fastcall OpTionFileCountChange(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TestProxi(TObject *Sender);
	void __fastcall OptRest(TObject *Sender);
	void __fastcall OpTionCountThreadChange(TObject *Sender);



private:	// User declarations
public:		// User declarations
	__fastcall TMyOptionsForm(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TMyOptionsForm *MyOptionsForm;
//---------------------------------------------------------------------------
#endif
