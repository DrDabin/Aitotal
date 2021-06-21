//---------------------------------------------------------------------------

#ifndef paroll1H
#define paroll1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFParoll : public TForm
{
__published:	// IDE-managed Components
	TEdit *EdParoll;
	TButton *BtOk;
	TCheckBox *CHPassword;
	TStatusBar *StatusBar1;
	TButton *Button1;
	void __fastcall BtOkClick(TObject *Sender);
	void __fastcall CHPasswordClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    UnicodeString NameArchiv;
	int GuidArchiv;
	UnicodeString Dir;
	__fastcall TFParoll(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFParoll *FParoll;
//---------------------------------------------------------------------------
#endif
