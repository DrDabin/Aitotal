//---------------------------------------------------------------------------
#include <DBXJSON.hpp>

#include <memory>    //std::auto_prt<>
#include <StrUtils.hpp>

#pragma hdrstop

#include "Aitotal1.h"
#include "AitotalInclude/Struct.h"//Структуры. Перенесу туда все мои структуры. И надо будет их обьединить.

//---------------------------------------------------------------------------
#pragma package(smart_init)


void __fastcall LVItemsToTextINJssonProg(UnicodeString NameFileJson)
{
   std::auto_ptr<TStringList>ListJesson(new TStringList(NULL));
   TJSONObject * LisViewGlavn = new TJSONObject();

   LisViewGlavn->AddPair("filenumber", String(Form3->FileNumber));

   TJSONObject *LVItems1;
   TJSONObject *LVItems2;
   TJSONObject *LVItems3;

   TJSONObject *obj;

   TJSONArray *LVArray1 = new TJSONArray();
   TJSONArray *LVArray2 = new TJSONArray();
   TJSONArray *LVArray3 = new TJSONArray();

   __try
   {
	   //Первая таблица
	   for(int I=0; I < Form3->ListView1->Items->Count; I++)
	   {
		  LVItems1 = new TJSONObject();

		  LVItems1->AddPair("namefile",Form3->ListView1->Items->Item[I]->SubItems->Strings[0]);
		  LVItems1->AddPair("filesize",Form3->ListView1->Items->Item[I]->SubItems->Strings[1]);
		  LVItems1->AddPair("md5",Form3->ListView1->Items->Item[I]->SubItems->Strings[2]);
		  LVItems1->AddPair("sha256",Form3->ListView1->Items->Item[I]->SubItems->Strings[3]);

		  if(Form3->ListView1->Items->Item[I]->Data != NULL)
		  {
			  if(((numchek*)Form3->ListView1->Items->Item[I]->Data)->rescan)
				 LVItems1->AddPair("cheked","true");
			  else
				 LVItems1->AddPair("cheked","false");

			  LVItems1->AddPair("filenumber", String(((numchek*)Form3->ListView1->Items->Item[I]->Data)->NumBer));
		  }
		  LVArray1->AddElement(LVItems1);
	   }

	   LisViewGlavn->AddPair("tablica1",LVArray1);
	   //+++

	   // Вторая таблица

	   for(int I=0; I < Form3->ListView2->Items->Count; I++)
	   {
		  LVItems2 = new TJSONObject();

		  LVItems2->AddPair("namefile",Form3->ListView2->Items->Item[I]->SubItems->Strings[0]);
		  LVItems2->AddPair("filesize",Form3->ListView2->Items->Item[I]->SubItems->Strings[1]);
		  LVItems2->AddPair("md5",Form3->ListView2->Items->Item[I]->SubItems->Strings[2]);
		  LVItems2->AddPair("sha256",Form3->ListView2->Items->Item[I]->SubItems->Strings[3]);
		  LVItems2->AddPair("zagproverka",Form3->ListView2->Items->Item[I]->SubItems->Strings[5]);
		  LVItems2->AddPair("urlscan",Form3->ListView2->Items->Item[I]->SubItems->Strings[5]);

		  if( Form3->ListView2->Items->Item[I]->Data != NULL)

			 LVItems2->AddPair("filenumber",String((int)Form3->ListView2->Items->Item[I]->Data));

		  LVArray2->AddElement(LVItems2);
	   }

	   LisViewGlavn->AddPair("tablica2",LVArray2);
	   //+++

	   //Третья таблица
	   for(int I=0; I < Form3->ListView3->Items->Count; I++)
	   {
		   LVItems3 = new TJSONObject();

		 LVItems3->AddPair("namefile",Form3->ListView3->Items->Item[I]->SubItems->Strings[0]);
		 LVItems3->AddPair("rezult",Form3->ListView3->Items->Item[I]->SubItems->Strings[1]);
		 LVItems3->AddPair("filesize",Form3->ListView3->Items->Item[I]->SubItems->Strings[2]);
		 LVItems3->AddPair("md5",Form3->ListView3->Items->Item[I]->SubItems->Strings[3]);
		 LVItems3->AddPair("sha256",Form3->ListView3->Items->Item[I]->SubItems->Strings[4]);
		 LVItems3->AddPair("datascan",Form3->ListView3->Items->Item[I]->SubItems->Strings[5]);
		 LVItems3->AddPair("urlscan",Form3->ListView3->Items->Item[I]->SubItems->Strings[6]);

		 if( Form3->ListView3->Items->Item[I]->Data !=NULL)
		 {
			try{
				TJSONObject * ResRescan =(TJSONObject*) TJSONObject::ParseJSONValue(*(String*) Form3->ListView3->Items->Item[I]->Data);
				LVItems3->AddPair("jsonscan", ResRescan);
			}
			catch(...)
			{
			   ShowMessage("jsonscan");
			}
		 }
		 LVArray3->AddElement(LVItems3);
	   }
	   LisViewGlavn->AddPair("tablica3",LVArray3);
	   //+++
	   ListJesson->Add(LisViewGlavn->ToJSON());

	   if(ListJesson->Text !="")
	      ListJesson->SaveToFile(NameFileJson + ".json");
	   else
		  ShowMessage("Таблицы пустые. Файл сканирования не сохранён.");
   }
   __finally
   {
	   delete LisViewGlavn;
   }

 }

void __fastcall JsesonToListViewe()
 {
	if(Form3->OpenJeson->Execute())
	{

	if(TFile::Exists(Form3->OpenJeson->FileName))
   {
	  std::auto_ptr<TStringList>ListJesson1(new TStringList(NULL));
	  ListJesson1->LoadFromFile(Form3->OpenJeson->FileName);


	  TJSONObject *jObj = static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(ListJesson1->Text));

	  if(jObj !=NULL)
	  {
		 TJSONValue *num = jObj->GetValue("filenumber");
		 if(num!=NULL)
		 {
			Form3->FileNumber = num->Value().ToInt();

			Form3->Button1Click(Form3);
			Form3->LV3Clear(Form3);
			Form3->ListView1->Clear();
			Form3->ListView2->Clear();

			//Парсинг третьей таблицы;
			try
			{
			   TJSONArray *Jarr = static_cast<TJSONArray*>(jObj->GetValue("tablica3"));

			   for(int i=0; i< Jarr->Count; i++)
			   {
				  try
				  {
					 TJSONObject *book = (TJSONObject*) Jarr->Items[i];
					 TJSONValue* Pair;
					 TListItem *ListItem;
					 ListItem = Form3->ListView3->Items->Add();
					 ListItem->Caption = ExtractFileName(book->GetValue("namefile")->Value()); // Имя файла
					 ListItem->SubItems->Add(book->GetValue("namefile")->Value());// пулный путь.

					 Pair =book->GetValue("rezult");
					 if(Pair !=NULL)
						ListItem->SubItems->Add(Pair->Value());
					 else
						ListItem->SubItems->Add("");// результат сканирования

					 Pair =book->GetValue("filesize");

					 if(Pair !=NULL)
						ListItem->SubItems->Add(Pair->Value()); // размер
					 else
						ListItem->SubItems->Add("");//// размер

					 Pair =book->GetValue("md5");
					 if(Pair !=NULL)
						ListItem->SubItems->Add(Pair->Value()); // хеш сумма
					 else
						ListItem->SubItems->Add("");// хеш сумма

					 Pair =book->GetValue("sha256");
					 if(Pair !=NULL)
						ListItem->SubItems->Add(Pair->Value()); //Хеш SHA256
					 else
						ListItem->SubItems->Add("");//Хеш SHA256

					 Pair =book->GetValue("datascan");
					 if(Pair !=NULL)
						ListItem->SubItems->Add(Pair->Value()); //Дата проверки
					 else
						ListItem->SubItems->Add("");//Дата проверки

					 Pair =book->GetValue("urlscan");
					 if(Pair !=NULL)
						ListItem->SubItems->Add(Pair->Value());// ссылка на результат.
					 else
						ListItem->SubItems->Add("");// ссылка на результат.

					 Pair =book->GetValue("jsonscan");
					 if(Pair !=NULL)
					 {
					   ListItem->Data = new String(Pair->ToString());
					   //ListJesson->Text =  Pair->ToJSON();
					   ///ListJesson->SaveToFile("lgf");
					 }

						 //ListItem->Data = new String(book->GetValue("namefile")->Value());
				  }
				  catch(...)
				  {
					 ShowMessage("try book3");
				  }

			   }

			}
			catch(Exception &e)
			{
			   ShowMessage("tablica3");;
			}

			//парсинг второй таблицы
			try
			{
			   TJSONArray *Jarr = static_cast<TJSONArray*>(jObj->GetValue("tablica2"));

			   for(int i=0; i< Jarr->Count; i++)
			   {
				  try
				  {
					 TJSONObject *book = (TJSONObject*) Jarr->Items[i];
					 TJSONValue* Pair;
					 TListItem *ListItem;
					 ListItem = Form3->ListView1->Items->Add();
					 ListItem->Caption = ExtractFileName(book->GetValue("namefile")->Value()); // Имя файла
					 ListItem->SubItems->Add(book->GetValue("namefile")->Value());// пулный путь.

					 Pair =book->GetValue("filesize");

					 if(Pair !=NULL)
						ListItem->SubItems->Add(Pair->Value()); // размер
					 else
						ListItem->SubItems->Add("");//// размер

					 Pair =book->GetValue("md5");
					 if(Pair !=NULL)
						ListItem->SubItems->Add(Pair->Value()); // хеш сумма
					 else
						ListItem->SubItems->Add("");// хеш сумма

					 Pair =book->GetValue("sha256");
					 if(Pair !=NULL)
						ListItem->SubItems->Add(Pair->Value()); //Хеш SHA256
					 else
						ListItem->SubItems->Add("");//Хеш SHA256

					 /*bool chek = false;
					 Pair =book->GetValue("cheked");
					 if(Pair !=NULL)
					 {
						if(Pair->Value() == "true"); //рескан
						   chek = true;
					 }*/
					 int filenum = 0;
					 Pair =book->GetValue("filenumber");
					 if(Pair !=NULL)
						filenum = Pair->Value().ToInt();// номер файла

					 ListItem->Data = (void*) new numchek(filenum,false);
					 //ListJesson->Text =  Pair->ToJSON();
					   ///ListJesson->SaveToFile("lgf");

				  }
				  catch(...)
				  {
					 ShowMessage("try book2");
				  }

			   }
			}
			catch(Exception &x)
			{
			   ShowMessage("number2"+ x.Message) ;
			}

			//парсинг первой таблицы
			try
			{
			   TJSONArray *Jarr = static_cast<TJSONArray*>(jObj->GetValue("tablica1"));

			   for(int i=0; i< Jarr->Count; i++)
			   {
				  try
				  {
					 TJSONObject *book = (TJSONObject*) Jarr->Items[i];
					 TJSONValue* Pair;
					 TListItem *ListItem;
					 ListItem = Form3->ListView1->Items->Add();
					 ListItem->Caption = ExtractFileName(book->GetValue("namefile")->Value()); // Имя файла
					 ListItem->SubItems->Add(book->GetValue("namefile")->Value());// пулный путь.

					 Pair =book->GetValue("filesize");

					 if(Pair !=NULL)
						ListItem->SubItems->Add(Pair->Value()); // размер
					 else
						ListItem->SubItems->Add("");//// размер

					 Pair =book->GetValue("md5");
					 if(Pair !=NULL)
						ListItem->SubItems->Add(Pair->Value()); // хеш сумма
					 else
						ListItem->SubItems->Add("");// хеш сумма

					 Pair =book->GetValue("sha256");
					 if(Pair !=NULL)
						ListItem->SubItems->Add(Pair->Value()); //Хеш SHA256
					 else
						ListItem->SubItems->Add("");//Хеш SHA256

					 bool chek = false;
					 Pair =book->GetValue("cheked");
					 if(Pair !=NULL)
					 {
						if(Pair->Value() == "true"); //рескан
						   chek = true;
					 }
					 int filenum = 0;
					 Pair =book->GetValue("filenumber");
					 if(Pair !=NULL)
						filenum = Pair->Value().ToInt();// номер файла

					 ListItem->Data = (void*) new numchek(filenum,chek);
					 //ListJesson->Text =  Pair->ToJSON();
					   ///ListJesson->SaveToFile("lgf");

				  }
				  catch(...)
				  {
					 ShowMessage("try book1");
				  }

			   }
			}
			catch(Exception &x)
			{
			   ShowMessage("number1"+ x.Message) ;
			}

			Form3->ScanFile();

		 }
		 else
			ShowMessage("NouNumber");

		 delete jObj;
	  }
	  else
		 ShowMessage("NULL jObj");//Ошибка парсинга jsesson

   }
   else
	  ShowMessage("Нет файла");
   }
}
