//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "ScanThread.h"
#include "Aitotal1.h"
#include "AitotalInclude/ScanVTIndy/ScanVTIndy1.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall ThreadScan::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall ThreadScan::ThreadScan(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall ThreadScan::Execute()
{
	//---- Place thread code here ----

	Form3->StopThread = false;
   //CopyTListToListView();
   Form3->CopyTListToListViewofStream();
	   //if(ProgressBar1->Max <=0)
		  //ProgressBar1->Position = 0;

	   //ProgressBar1->Max = ListView1->Items->Count;

	   int i = 0;
	   int ApikeyNumber =0;

	   while(/*!StopThread &&*/ Form3->ListView1->Items->Count)
	   {


		  // —помощью цикла провер€ю есть ли поток с номером i (ThUpload[i])
		  // ≈сли все значени€ от 0 до 50 зан€ты, то идет ожидание по таймеру.
		  for(int q=0;q <= Form3->AtOptions.Thread;q++)
		  {
			 if(Form3->StopThread)
				break;

			 if(q == Form3->AtOptions.Thread)
			 {
				q=0;
				Form3->PauseT("00:00:05");
			 }

			 if(VTIndy[q] ==NULL)
			 {
				i=q;
				break;
			 }
		  }

		  if(Form3->ListView1->Items->Count == 0)
			 break;

		  Base MyTestBase;

		  if(Form3->StopThread)
			 break;


		  if(Form3->StopThread)
			 break;

		  VTIndy[i] = new ScanVTIndy(true);
		  //”величиваю врем€ на 15 мин.
		  TDateTime Prob = Time();

		  Prob = Prob+15./24/60;
		  VTIndy[i]->TimeBreak = FormatDateTime("YYYY.MM.DD",Date())+ " " + FormatDateTime("HH:NN:SS",Prob);
		  VTIndy[i]->VtBase.BasePatchFileName = Form3->ListView1->Items->Item[0]->SubItems->Strings[0];
		  VTIndy[i]->VtBase.BaseMD5 = Form3->ListView1->Items->Item[0]->SubItems->Strings[2];
		  VTIndy[i]->VtBase.BaseSizeFile = Form3->ListView1->Items->Item[0]->SubItems->Strings[1].ToInt();
		  //”станавливаем вести логирование или нет. „итаетс€ в ини файле.
		  VTIndy[i]->logirovanie =  Form3->logirovanie;
		  //VTIndy[i]->VtBase.BaseSHA256 = MyTestBase.BaseSHA256 = ListView1->Items->Item[0]->SubItems->Strings[3];
		  VTIndy[i]->VtBase.BaseSHA256 = Form3->ListView1->Items->Item[0]->SubItems->Strings[3];
		  VTIndy[i]->VtBase.BasePredScanData = FormatDateTime("YYYY.MM.DD",Date())+ " " + FormatDateTime("HH:NN:SS",Time());

		  VTIndy[i]->PotokNumber = i;
		  VTIndy[i]->FileNumber = ((numchek*)Form3->ListView1->Items->Item[0]->Data)->NumBer;
		  VTIndy[i]->VtBase.FileNumber = ((numchek*)Form3->ListView1->Items->Item[0]->Data)->NumBer;
		  VTIndy[i]->OnTerminate = &Form3->AtThreadTerminated;

		  VTIndy[i]->ScanIndyVT.Proverka = false;
		  //провер€ем если в коммандной строке мы отправили 1 то смотрим ещЄ на ChReSca->Checked;
		  // если 0 то это norescan и мы сразу передаЄм 0
		  if(((numchek*)Form3->ListView1->Items->Item[0]->Data)->rescan)
			VTIndy[i]->ScanIndyVT.Rescan = Form3->ChReSca->Checked;
		  else
			VTIndy[i]->ScanIndyVT.Rescan = false;


		  if(Form3->AtOptions.ProxyVT.ProxiChecked)
		  {
			 VTIndy[i]->ScanIndyVT.ProxyVT = Form3->AtOptions.ProxyVT;
		  }

		  if(ApikeyNumber >= Form3->ApikeyCount)
		  {
			 ApikeyNumber = 0;
		  }

		  VTIndy[i]->ScanIndyVT.ApiKey = Form3->AtOptions.Apikey[ApikeyNumber];
		  VTIndy[i]->Start();
		  // удал€ю строку
		  Form3->ListView1->Items->Item[0]->Delete();

		  Form3->ListView1->Repaint();
		  i++;
		  ApikeyNumber++;
		  //countSpisokNamePotok--;
		  //PauseT("00:00:01");
		  //ProgressBar1->StepBy(1);
		  //Application->ProcessMessages();
	   }



}
//---------------------------------------------------------------------------
