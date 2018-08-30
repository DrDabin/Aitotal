//---------------------------------------------------------------------------

#ifndef ScanResultH
#define ScanResultH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "AitotalInclude/Struct.h"
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormResultScan : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *LbNameFile;
	TLabel *LbPathFile;
	TLabel *LbScanDate;
	TLabel *LBVTFileName;
	TPanel *Panel2;
	TListView *ListView1;
	TPanel *Panel3;
	TLabel *LbResult;
	TLabel *LBVtPathFile;
	TLabel *LBVtDataScan;
	TButton *Button1;
	TPopupMenu *PopupMenu1;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TMenuItem *N6;
	TMenuItem *N7;
	TMenuItem *N8;
	TSaveDialog *SaveDialog;
	TButton *Button2;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall ListView1ColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall ListView1CustomDrawItem(TCustomListView *Sender, TListItem *Item,
		  TCustomDrawState State, bool &DefaultDraw);
	void __fastcall ListView1CustomDrawSubItem(TCustomListView *Sender, TListItem *Item,
          int SubItem, TCustomDrawState State, bool &DefaultDraw);
	void __fastcall LBVTFileNameMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall LBVTFileNameMouseLeave(TObject *Sender);
	void __fastcall LBSvoistvoFila(TObject *Sender);
	void __fastcall OpenBrauzer(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall PMCopyDetect(TObject *Sender);
	void __fastcall PMCopyAllStroka(TObject *Sender);
	void __fastcall PMSvoistvoFile(TObject *Sender);
	void __fastcall PmOpenFilePatch(TObject *Sender);
	void __fastcall PmSaveToFile(TObject *Sender);
	void __fastcall exitResult(TObject *Sender);
private:	// User declarations
   TListItem *ListItem;

public:		// User declarations
	UnicodeString update;
	UnicodeString permalink;
	Base VTBase;
	bool Tompost; // Если труето форму результат выводить поверхвсех окон.
	void __fastcall TFormResultScan::SortProgramming(int i);
	__fastcall TFormResultScan(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormResultScan *FormResultScan;
//---------------------------------------------------------------------------
#endif
