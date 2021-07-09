//---------------------------------------------------------------------------

#include <vcl.h>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>

#pragma hdrstop

#include "IniFiles.hpp"
#include <memory>    //std::auto_prt<  >
#include "Aitotal1.h"
#include "AitotalInclude/InternetConnected.h"
#include "Options1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#define VT_API_BASE_URL "https://www.virustotal.com/vtapi/v2/"

#pragma resource "*.dfm"
TMyOptionsForm *MyOptionsForm;

TIniFile * Ini;
//---------------------------------------------------------------------------
__fastcall TMyOptionsForm::TMyOptionsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
static size_t write_data(char *ptr, size_t size, size_t nmemb, UnicodeString* data)
{
	if (data)
	{
		 (*data)+=  AnsiString(ptr, size*nmemb);

		 return size*nmemb;
	}
	else return 0;  // будет ошибка
}
//++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++

void __fastcall TMyOptionsForm::OptionSave(TObject *Sender)
{
	std::auto_ptr<TStringStream>s(new TStringStream());
	// проверка использовать прокси или нет.
	if(ProxyChecked->Checked)
	{
	   if(OptProxi->Text !="")
		  Ini->WriteString("Network","Proxi",OptProxi->Text);
	   if(OptIpPort->Text !="")
		  Ini->WriteString("Network","IpPort",OptIpPort->Text);
	   if(OptProxiLogin->Text !="")
		  Ini->WriteString("Network","ProxiLogin",OptProxiLogin->Text);

	   Ini->WriteInteger("Network", "Socket", ComboBox1->ItemIndex);
	   //AnsiString password = XOR_Crypt(MyOptProxiPassword->Text,"Aitotal12");
	   if(OptProxiPassword->Text !="")
	   {
		  UnicodeString password = Form3->MyCryptDeCrypt(OptProxiPassword->Text,true);
		  s->WriteString(password);
		  s->Position = 0;
		  Ini->WriteBinaryStream("Network","ProxiPassword",s.get());
	   }
	}

	Ini->WriteBool("Network","ProxiChecked" , ProxyChecked->Checked);

	if(OptApikey->Text !="")
		Ini->WriteString("Tools", "apikey", OptApikey->Text);

	else
		Ini->WriteString("Tools", "apikey", "3c04a612f2bf23e46dc857ffa0655544ea3a9d0d3c25b007057908eb7c8ca7b1");

	//ставлю ограничение на 19 файлов.
	if(OpTionFileCount->Text.ToInt() <=0 || OpTionFileCount->Text.ToInt() >20)
	{
		//OpTionFileCount->Text = 5;
		MessageBoxA(0, "Количество файлов при загрузке  не должно быть 0 или более 20", 0, MB_OK);
		return;
	}
	else
	   Ini->WriteInteger("Tools", "FileCount", OpTionFileCount->Text.ToInt());

	if(OptionThreadCount->Text.ToInt() <=0 || OptionThreadCount->Text.ToInt() >50)
	{
	   //OptionThreadCount->Text = 20;
	   MessageBoxA(0, "Общее количество проверяемых файлов не должно быть 0 или более 50", 0, MB_OK);
	   return;
	}
	else
	   Ini->WriteInteger("Tools", "ThreadCount", OptionThreadCount->Text.ToInt());

	//если труе мы закрываем программы, елзе - сворачиваем в трей
	Ini->WriteBool("Tools", "Exit_Tray", RBExit->Checked);
	//если труе то выводим сообщение в баллонхинт у трейикона.
	Ini->WriteBool("Tools", "BallonHint", CBBallonhint->Checked);
	//Если труе выводим уведомление если произошла ошибка при распаковке архива.
	Ini->WriteBool("Tools","MessageErrorArchive", ErrorArchiv->Checked);
	//"RunDuplicateApplication" Если труе, то можно запускать копии программы, если фалс то нет.
	Ini->WriteBool("Tools","RunDuplicateApplication", RunAplicatDubl->Checked);
	
	Form3->OptionReadIni();

	MyOptionsForm->Close();
}
//---------------------------------------------------------------------------


void __fastcall TMyOptionsForm::FormCreate(TObject *Sender)
{
	KeyPreview = True;

	MyOptionsPage->ActivePage =  MyOptionsGlobal;
	if(FileExists(ExtractFilePath(Application->ExeName) + "tools\\options.ini"))
	{
	   Ini = new TIniFile(ExtractFilePath(Application->ExeName) + "tools\\options.ini");  // создаем объект
	   OptionReadIni(Sender);
	}

	else
	{
	   int FileCr;
	   FileCr = FileCreate(ExtractFilePath(Application->ExeName) + "tools\\options.ini");
	   FileClose(FileCr);
	   Ini = new TIniFile(ExtractFilePath(Application->ExeName) + "tools\\options.ini");  // создаем объект
	   OptionReadIni(Sender);
	}
}
//---------------------------------------------------------------------------

void __fastcall TMyOptionsForm::OptionReadIni(TObject *Sender)
{
	ProxyChecked->Checked = Ini->ReadBool("Network","ProxiChecked" , false);
	std::auto_ptr<TStringStream>s(new TStringStream());
	// проверка использовать прокси или нет.
	if(ProxyChecked->Checked)
	{
		OptProxi->Text = Ini->ReadString("Network","Proxi","");
		OptIpPort->Text = Ini->ReadString("Network","IpPort","");
		OptProxiLogin->Text = Ini->ReadString("Network","ProxiLogin","");
		Ini->ReadBinaryStream("Network","ProxiPassword",s.get());
		OptProxiPassword->Text = Form3->MyCryptDeCrypt(s->DataString,false);
		ComboBox1->ItemIndex = Ini->ReadInteger("Network", "Socket", 0);
	}
	else
	{
		OptProxi->Enabled =false;
		OptIpPort->Enabled =false;
		OptProxiLogin->Enabled =false;
		OptProxiPassword->Enabled =false;
		ComboBox1->Enabled = false;
		TypSocket->Enabled = false;
		BtTestProxi->Visible = false;
    }

	if(Ini->ReadInteger("Tools", "FileCount",0) <= 0 || Ini->ReadInteger("Tools", "FileCount",0) >20)
	   OpTionFileCount->Text = 5;

	else
	   OpTionFileCount->Text = Ini->ReadInteger("Tools", "FileCount",5);

	if(Ini->ReadInteger("Tools", "ThreadCount",0) <= 0 || Ini->ReadInteger("Tools", "ThreadCount",0) >50)
	   OptionThreadCount->Text = 20;

	else
	   OptionThreadCount->Text = Ini->ReadInteger("Tools", "ThreadCount",5);

	if(Ini->ReadBool("Tools", "Exit_Tray", false) == true)
	   RBExit->Checked = true;
	else
	   RBTrei->Checked = true;

	UnicodeString ApikeyPot = Ini->ReadString("Tools", "apikey", "3c04a612f2bf23e46dc857ffa0655544ea3a9d0d3c25b007057908eb7c8ca7b1");
	if(ApikeyPot =="" || ApikeyPot =="3c04a612f2bf23e46dc857ffa0655544ea3a9d0d3c25b007057908eb7c8ca7b1")
	  OptApikey->Text = "";
	else   OptApikey->Text = ApikeyPot;

	if(Ini->ReadBool("Tools","MessageErrorArchive", true) == true)
	  ErrorArchiv->Checked = true;
	else ErrorArchiv->Checked = false;

	//если труе то выводим сообщение в баллонхинт у трейикона.
	if(Ini->ReadBool("Tools", "BallonHint", false)== true)
	  CBBallonhint->Checked = true;
	else
	   CBBallonhint->Checked = false;

	if(Ini->ReadBool("Tools", "RunDuplicateApplication", false) == true)
	   RunAplicatDubl->Checked = true;
	else
	   RunAplicatDubl->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TMyOptionsForm::OpTionFileCountKeyPress(TObject *Sender, System::WideChar &Key)

{
	if((Key >='0') && (Key <='9') || (Key ==VK_BACK))
	{

	   return;
	}
	Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMyOptionsForm::ProxyCheckedClick(TObject *Sender)
{
   // проверка использовать прокси или нет.
   if(ProxyChecked->Checked)
   {
	  OptProxi->Enabled = true;
	  OptIpPort->Enabled = true;
	  OptProxiLogin->Enabled = true;
	  OptProxiPassword->Enabled = true;
	  ComboBox1->Enabled = true;
	  TypSocket->Enabled = true;
	  BtTestProxi->Visible = true;
   }
   else
   {
	  OptProxi->Enabled = false;
	  OptIpPort->Enabled = false;
	  OptProxiLogin->Enabled = false;
	  OptProxiPassword->Enabled = false;
	  ComboBox1->Enabled = false;
	  TypSocket->Enabled = false;
	  BtTestProxi->Visible = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TMyOptionsForm::OpTionFileCountChange(TObject *Sender)
{
   if(OpTionFileCount->Text !="")
   {
	   UnicodeString number = OpTionFileCount->Text;
	   int num ;
	   try
	   {
		  num = number.ToInt() ;
	   }
	   catch(...)
	   {
		   OptionThreadCount->Text ="";
		   return;
	   }
	   if(num<1 || num>20)
	   {
		  //MessageBoxW(0, L"Количество файлов при загрузке не должно быть 0 или более 20", 0, MB_OK);
		  ShowMessage("Количество файлов при загрузке не должно быть 0 или более 20");
		  OpTionFileCount->Text = Form3->AtOptions.FileCount;
	   }
   }
}
//---------------------------------------------------------------------------
void __fastcall TMyOptionsForm::OpTionCountThreadChange(TObject *Sender)
{
   if(OpTionFileCount->Text !="")
   {
	  UnicodeString number = OptionThreadCount->Text;
	  int num ;
	  try
	  {
		 num = number.ToInt() ;
	  }
	  catch(...)
	  {
		  OptionThreadCount->Text ="";
		  return;

	  }
	  if(num<1 || num>50)
	  {
		 //MessageBoxW(0, L"Общее количество проверяемых файлов не должно быть 0 или более 50", 0, MB_OK);
		 ShowMessage("Общее количество проверяемых файлов не должно быть 0 или более 50");
		 OptionThreadCount->Text = Form3->AtOptions.Thread;
		 //MyOptionsForm->Activate();
	  }
   }
}
//---------------------------------------------------------------------------

void __fastcall TMyOptionsForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
   if(Key ==VK_RETURN)
   {
	  OptionSave(Sender);
   }
   if(Key == VK_ESCAPE)
      MyOptionsForm->Close();
}
//---------------------------------------------------------------------------

void __fastcall TMyOptionsForm::TestProxi(TObject *Sender)
{
   if (GetStatusConnect())
   {
	  std::auto_ptr<TIdHTTP>IndyVT(new TIdHTTP(NULL));
	  TIdSSLIOHandlerSocketOpenSSL *ssl = new TIdSSLIOHandlerSocketOpenSSL(NULL);
	  TIdSocksInfo *soketInfo = new TIdSocksInfo(NULL);

	  IndyVT->HandleRedirects = 1;
	  IndyVT->Request->ContentType = L"application/x-msdownload";
	  IndyVT->Request->UserAgent = "Mozilla/5.0 (Windows; U; Windows NT 5.1;en-US; rv:1.8.1.1) Gecko/20061204 Firefox/2.0.0.1";
	  //IndyVT->Request->Accept= "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
	  IndyVT->Request->CustomHeaders->AddValue("x-apikey", "3c04a612f2bf23e46dc857ffa0655544ea3a9d0d3c25b007057908eb7c8ca7b1");
	  if(ProxyChecked)
	  {
		 switch(ComboBox1->ItemIndex)
		 {
			case 0:
			{
			   IndyVT->ProxyParams->BasicAuthentication = false;

			   if(OptProxi->Text !=" " || OptProxi->Text.Length() !=0)
			   {
				  IndyVT->ProxyParams->ProxyServer = OptProxi->Text;
			   }

			   if(OptIpPort->Text.ToInt() !=0)
				  IndyVT->ProxyParams->ProxyPort = OptIpPort->Text.ToInt();

			   if(OptProxiLogin->Text !=" " || OptProxiLogin->Text.Length() != 0)
			   {
				  IndyVT->ProxyParams->ProxyUsername = OptProxiLogin->Text;
				  IndyVT->ProxyParams->BasicAuthentication = true;
			   }

			   if(OptProxiPassword->Text !=" " || OptProxiPassword->Text.Length() != 0)
			   {
				  IndyVT->ProxyParams->ProxyPassword = OptProxiPassword->Text;

			   }
			   IndyVT->IOHandler = ssl;
			   break;
			}

			case 1:
			{
			   soketInfo->Version = svSocks4;

			   if(OptProxi->Text !=" " || OptProxi->Text.Length() !=0)
			   {
				  //IndyVT->ProxyParams->ProxyServer = "";
				  soketInfo->Host = OptProxi->Text;

			   }
			   if(OptIpPort->Text.ToInt() !=0)
			   {
				  IndyVT->ProxyParams->ProxyPort = 0;
				  soketInfo->Port = OptIpPort->Text.ToInt();
			   }

			   if(OptProxiLogin->Text !=" " || OptProxiLogin->Text.Length() != 0)
			   {
				  soketInfo->Username = OptProxiLogin->Text;

			   }


			   if(OptProxiPassword->Text !=" " || OptProxiPassword->Text.Length() != 0)
			   {
				  soketInfo->Password = OptProxiPassword->Text;
				  soketInfo->Authentication = saUsernamePassword;
			   }
			   else
			   {
				   soketInfo->Authentication = saNoAuthentication;
			   }
			   IndyVT->IOHandler = ssl;
			   ssl->TransparentProxy = soketInfo;
			   break;
			}
			case 2:
			{
			   soketInfo->Version = svSocks5;

			   if(OptProxi->Text !=" " || OptProxi->Text.Length() !=0)
			   {
				  //IndyVT->ProxyParams->ProxyServer = "";
				  soketInfo->Host = OptProxi->Text;

			   }
			   if(OptIpPort->Text.ToInt() !=0)
			   {
				  IndyVT->ProxyParams->ProxyPort = 0;
				  soketInfo->Port = OptIpPort->Text.ToInt();
			   }

			   if(OptProxiLogin->Text !=" " || OptProxiLogin->Text.Length() != 0)
			   {
				  soketInfo->Username = OptProxiLogin->Text;

			   }


			   if(OptProxiPassword->Text !=" " || OptProxiPassword->Text.Length() != 0)
			   {
				  soketInfo->Password = OptProxiPassword->Text;
				  soketInfo->Authentication = saUsernamePassword;
			   }
			   else
			   {
				   soketInfo->Authentication = saNoAuthentication;
			   }
			   IndyVT->IOHandler = ssl;
			   ssl->TransparentProxy = soketInfo;
			   break;
			}
		 }
	  }

	  UnicodeString Url= "https://www.virustotal.com/api/v3/files/EEE79D5C8E221C38DB7616B64237859E";

	  __try
	  {
		 try
		 {
			IndyVT->Get(Url);
			IndyVT->ResponseCode;
			ShowMessage("Ok");
		 }
		 catch(EIdHTTPProtocolException &E)
		 {
			ShowMessage("Error = "+IntToStr(E.ErrorCode));
		 }
		 catch(EIdException &E)// Другие исключения Indy
		 {
			ShowMessage("Error In= " + E.Message);
		 }
		 catch(Exception &E)   // Другие НЕ Indy исключения
		 {
			ShowMessage("Error nouIn = " + E.Message);
		 }
	  }
	  __finally
	  {
		 delete ssl;
		 delete soketInfo;
	  }
   }
}
//---------------------------------------------------------------------------

void __fastcall TMyOptionsForm::OptRest(TObject *Sender)
{
   Ini->WriteString("Tools", "apikey", "3c04a612f2bf23e46dc857ffa0655544ea3a9d0d3c25b007057908eb7c8ca7b1");
   Ini->WriteInteger("Tools", "FileCount",5);
   Ini->WriteInteger("Tools", "ThreadCount",20);
   Ini->WriteBool("Tools", "RunDuplicateApplication", false);
   //если труе мы закрываем программу, елзе - сворачиваем в трей
   Ini->WriteBool("Tools", "Exit_Tray", false);

   //Если труе выводим уведомление если произошла ошибка при распаковке архива.
   Ini->WriteBool("Tools","MessageErrorArchive", true);
   Form3->OptionReadIni();
   OptionReadIni(Sender);
}
//---------------------------------------------------------------------------



