//---------------------------------------------------------------------------

#include <System.hpp>
#include <memory>
#include <StrUtils.hpp>
#include <IoUtils.hpp>
#include <IniFiles.hpp>

#pragma hdrstop

#include "Aitotal1.h"
#include "AitotalInclude/SizeFile.h"
#include "AitotalInclude/BaseParser.h"
#include "AitotalInclude/RabotaSfilam.h"
#include "md5.hpp"
#include "AitotalInclude/extract.h"
 #include "AitotalInclude/MyStruct.h" //Структуры. Перенесу туда все мои структуры.

#include "ParsingArchiv.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall ParsingArchiv::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall ParsingArchiv::ParsingArchiv(bool CreateSuspended)
	: TThread(CreateSuspended)
{
   FreeOnTerminate = true;
}
//---------------------------------------------------------------------------
void __fastcall ParsingArchiv::Execute()
{
	   NameLogError = ExtractFilePath(ParamStr(0)) + "AitotalTMP\\Error.log";

}
//---------------------------------------------------------------------------
void __fastcall ParsingArchiv::ErrorLog()
{
   try
   {
	  std::auto_ptr<TStringList> FileError (new TStringList(NULL));

	  if(!FileExists(NameLogError))
	  {
		 int FileCr;
		 FileCr = FileCreate(NameLogError);
		 FileClose(FileCr);
		 Sleep(100);
	  }
	  if(MyFileSize(NameLogError) > 4000000)
	  {
		 TFile::Delete(NameLogError);
		 int FileCr;
		 FileCr = FileCreate(NameLogError);
		 FileClose(FileCr);
		 Sleep(100);
	  }

	  FileError->LoadFromFile(NameLogError);


	  FileError->Add("++++++++++++++++++++++++++++++++++++++++++++++++");
	  FileError->Add(FormatDateTime("YYYY.MM.DD",Date())+ " " + FormatDateTime("HH:NN:SS",Time()));
	  FileError->Add(VtBase.BasePatchFileName);
	  FileError->Add(ErrorMessage);
	  FileError->SaveToFile(NameLogError, TEncoding::UTF8);
   }
   catch(EFOpenError &e)
   {
	  ShowMessage("No open file!");
   }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++
void __fastcall ParsingArchiv::ExtArchiv(UnicodeString ArcName)
{
    // Создаю по каждой директории отдельно. А то ошибка при распаковке, если нет
	// директории.
	UnicodeString NameArchiv = ArcName;
	UnicodeString DirPatchArchiv = "";
	UnicodeString Dir = "";
	std::auto_ptr<TStringList> SpisokFileArchiv (new TStringList(NULL) );
	std::auto_ptr<TStringList> SpisokFileKarantin (new TStringList(NULL) );
	std::auto_ptr<TStringList> FileSpisokProwerki (new TStringList(NULL));

   if(ArcName.LowerCase().Pos("aitotaltmp\\archiw")==0)
   {
	  UnicodeString dateAT = "_" + FormatDateTime("YYYY.MM.DD",Date())+ "_" + FormatDateTime("HH-NN-SS",Time());

	  if(!TDirectory::Exists(ExtractFilePath(Application->ExeName) + "AitotalTMP"))
		 TDirectory::CreateDirectoryA(ExtractFilePath(Application->ExeName) + "AitotalTMP");

	  Dir = ExtractFilePath(Application->ExeName) + "AitotalTMP\\archiw\\";
	  //CreateDirectoryW(Dir.c_str(), NULL);
	  TDirectory::CreateDirectoryA(Dir);
	  Dir += ExtractFileName(ArcName+dateAT); // Если, что здесь добавить +"\\"
	  //CreateDirectoryW(Dir.c_str(), NULL);
	  TDirectory::CreateDirectoryA(Dir);
	  DirPatchArchiv = ExtractFilePath(Application->ExeName) + "AitotalTMP\\archiw\\";
	  DirPatchArchiv += ExtractFileName(ArcName+dateAT);

	  ArcName =String(L"\\\\?\\")+ArcName;
	  Dir = String(L"\\\\?\\")+Dir;
   }
   //если в архиве находится архив...
   //закоментировал так как убрал распаковку вложенных файлов.
   /*else
   {
	  Dir = L"\\\\?\\" + ExtractFilePath(ArcName) + ExstractFileNameBezExt(ExtractFileName(ArcName));

	  TDirectory::CreateDirectoryA(Dir);


	  DirPatchArchiv = ExtractFilePath(ArcName) + ExstractFileNameBezExt(ExtractFileName(ArcName));

	  ArcName =String(L"\\\\?\\")+ArcName;
   } */


	if (extract(ArcName.w_str(), Dir.w_str()))
	{

		SpisokFileArchiv->Text = MyFiledDirArchiv(DirPatchArchiv);
		if( SpisokFileArchiv->Text !="")
		{

			for (int i = 0; i < SpisokFileArchiv->Count; i++)
			{
						   //++++++++Begin AVZ .dta++++++++
			   if(ExtractFileExt(SpisokFileArchiv->Strings[i]) ==".dta"&&
					ExtractFileName(SpisokFileArchiv->Strings[i]).LowerCase().Pos("avz")!=0)
			   {
				   try
				   {
					  std::auto_ptr<TIniFile>  Ini  (new TIniFile(ExtractFilePath(SpisokFileArchiv->Strings[i]) + ChangeFileExt(ExtractFileName(SpisokFileArchiv->Strings[i]),"") + ".ini"));
					  AnsiString name = Ini->ReadString("InfectedFile","Src","").LowerCase();
					  __int64 sizefile = MyFileSize(SpisokFileArchiv->Strings[i]);
					  AnsiString MyMd5 = "";
					  if(FileExists(SpisokFileArchiv->Strings[i]))
					  {
						  if((sizefile >0) && (sizefile < 134271701))
						  {
							 MyMd5 = CalkFileMD5(AnsiString(SpisokFileArchiv->Strings[i])).LowerCase();

							 if( (MyMd5 !="") && (MyMd5) != "nou")
							 {
								 bool postFileArchiv = false;
								 for( int st = 0; st < SpisokFileKarantin->Count ; st++)
								 {
									 TStringDynArray arr(SplitString(SpisokFileKarantin->Strings[st], "|"));
									 if(MyMd5 == arr[0])
									 {
										 if(name != arr[1])
										 {
											UnicodeString fileOut;
											fileOut = RenameFileName(ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir");

											if(RenameFile(SpisokFileArchiv->Strings[i],fileOut))
											{
											   postFileArchiv = true;
											   String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
											   ZanoshuVListWiew3PriOshibki(fileOut, sizefile, result , MyMd5, false);
											   // Добавить перенос файлов в третью в кладку. Отдельную функцию написать.
											   // Когда хеш одинаковый но файлы разные. При этом просто прописать ссылку.
											   break;
											}
											else
											{
											   ErrorMessage = "Ошибка переименовывания файла " + SpisokFileArchiv->Strings[i] + "\n в " +fileOut +"\n";
											   Synchronize(&ErrorLog);
											}
										 }
										 else
										 {
											postFileArchiv = true;
											break;
										 }
									 }
								 }
								 if(!postFileArchiv)
								 {

										UnicodeString nameFy = RenameFileName(ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir");

										if(RenameFile(SpisokFileArchiv->Strings[i],nameFy))
										{
										   FileSpisokProwerki->Add(nameFy);
										   SpisokFileKarantin->Add(MyMd5 + "|" +name);
										}
										else
										{
										   ErrorMessage = "Ошибка переименовывания файла " + SpisokFileArchiv->Strings[i] + "\n в " +nameFy +"\n";
										   Synchronize(&ErrorLog);
										}
								 }

							 }
						  }
						  else
						  {
							  OtWetErrorSizeFile(SpisokFileArchiv->Strings[i], ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir", sizefile, MyMd5);

						  }
					  }
				   }
				   catch(...)
				   {
					   ;
				   }
			   }
			   else
				  if(ExtractFileExt(SpisokFileArchiv->Strings[i]).LowerCase() ==".ini" &&
						  ExtractFileName(SpisokFileArchiv->Strings[i]).LowerCase().Pos("avz")!=0)
			   {
					// Делаю так по тому, что если не прописать здесь это то файлы .ini карантинов занесутся на проверку.
				   ;
			   }
			   ////++++++++End AVZ .dta++++++++

			   ////++++++++Begin AVZ .dat++++++++
			   else
				  if(ExtractFileExt(SpisokFileArchiv->Strings[i]).LowerCase() == ".dat" &&
					   ExtractFileName(SpisokFileArchiv->Strings[i]).LowerCase().Pos("bcqr") !=0)
			   {
				   try
				   {
					  std::auto_ptr<TIniFile>  Ini  (new TIniFile(ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExstractFileNameBezExt(ExtractFileName(SpisokFileArchiv->Strings[i])) + ".ini"));
					  AnsiString name = Ini->ReadString("InfectedFile","Src","").LowerCase();
					  __int64 sizefile = MyFileSize(SpisokFileArchiv->Strings[i]);
					  AnsiString MyMd5 = "";
					  if(FileExists(SpisokFileArchiv->Strings[i]))
					  {
						  if((sizefile >0) && (sizefile < 134271701))
						  {
							 MyMd5 = CalkFileMD5(AnsiString(SpisokFileArchiv->Strings[i])).LowerCase();


							 if( (MyMd5 !=""))
							 {
								 bool postFileArchiv = false;
								 for( int st = 0; st < SpisokFileKarantin->Count ; st++)
								 {
									 TStringDynArray arr(SplitString(SpisokFileKarantin->Strings[st], "|"));
									 if(MyMd5 == arr[0])
									 {
										 if(name != arr[1])
										 {
											UnicodeString fileOut;
											fileOut = RenameFileName(ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir");

											if(RenameFile(SpisokFileArchiv->Strings[i],fileOut))
											{
											   postFileArchiv = true;
											   String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
											   ZanoshuVListWiew3PriOshibki(fileOut, sizefile, result , MyMd5,false);
											   // Добавить перенос файлов в третью в кладку. Отдельную функцию написать.
											   // Когда хеш одинаковый но файлы разные. При этом просто прописать ссылку.
											   break;
											}
											else
											{
											   ErrorMessage = "Ошибка переименовывания файла " + SpisokFileArchiv->Strings[i] + "\n в " +fileOut +"\n";
											   Synchronize(&ErrorLog);
											}
										 }
										 else
										 {

											postFileArchiv = true;
											break;
										 }
									 }
								 }
								 if(!postFileArchiv)
								 {
									 UnicodeString nameFy = RenameFileName(ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir");

									 if(RenameFile(SpisokFileArchiv->Strings[i],nameFy))
									 {
										FileSpisokProwerki->Add(nameFy);
										SpisokFileKarantin->Add(MyMd5 + "|" +name);
									 }
									 else
									 {
										ErrorMessage = "Ошибка переименовывания файла " + SpisokFileArchiv->Strings[i] + "\n в " +nameFy +"\n";
										Synchronize(&ErrorLog);
									 }

								 }

							 }
						  }
						  else
						  {
							  OtWetErrorSizeFile(SpisokFileArchiv->Strings[i], ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir", sizefile, MyMd5);
						  }
					  }
				   }
				   catch(...)
				   {
					   ;
				   }
			   }

			   else
				  if(ExtractFileExt(SpisokFileArchiv->Strings[i]).LowerCase() == ".ini" &&
						  ExtractFileName(SpisokFileArchiv->Strings[i]).LowerCase().Pos("bcqr")!=0)
			   {
				   ;
			   }
			   ////++++++++end AVZ .dat++++++++
			   else
				  //  Плохая проверка парсинка карантина UVS где расширение файла
				  // имеет хеш сумм SHA1 пример ADSNDS.DLL._BAF0F1105A8247832657DBDB4924B88D384C7E9A
				  if(ExtractFileExt(SpisokFileArchiv->Strings[i]).Length() == 42)
				  {
					  AnsiString name = "";
					  __int64 sizefile = MyFileSize(SpisokFileArchiv->Strings[i]);
					  AnsiString MyMd5 = "";

					  if(FileExists(SpisokFileArchiv->Strings[i] + ".txt"))
					  {
						 //SpisokFileKarantin->Add(SpisokFileArchiv->Strings[i] + ".txt");
						 std::auto_ptr<TStringList> Zapros (new TStringList(NULL) );
						 Zapros->LoadFromFile( SpisokFileArchiv->Strings[i] + ".txt");

						 for(int total = 0; total <Zapros->Count ; total++)
						 {
							AnsiString stroka = Zapros->Strings[total];
							int qwer = stroka.Pos("Полное имя");

							if( qwer > 0)
							{
							   name = stroka.SubString(qwer+11, stroka.Length()-(qwer+10)).Trim();
							   break;
							}
						 }

						 if(name !="")
						 {

							if((sizefile >0) && (sizefile < 134271701))
							{
								MyMd5 = CalkFileMD5(SpisokFileArchiv->Strings[i]).LowerCase();

								if(MyMd5 !="")
								{
								   bool postFileArchiv = false;

								   for( int st = 0; st < SpisokFileKarantin->Count ; st++)
								   {
									  TStringDynArray arr(SplitString(SpisokFileKarantin->Strings[st], "|"));

									  if(MyMd5 == arr[0])
									  {
										 if(name != arr[1])
										 {
											postFileArchiv = true;
											String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
											ZanoshuVListWiew3PriOshibki(SpisokFileArchiv->Strings[i], sizefile, result , MyMd5, false);
											// Добавить перенос файлов в третью в кладку. Отдельную функцию написать.
											// Когда хеш одинаковый но файлы разные. При этом просто прописать ссылку.
											break;
										 }
										 else
										 {
											postFileArchiv = true;
											break;
										 }
									  }
								   }
								   if(!postFileArchiv)
								   {
									  try
									  {
										 SpisokFileKarantin->Add(MyMd5 + "|" + name);
										 FileSpisokProwerki->Add(SpisokFileArchiv->Strings[i]);
									  }
									  catch(...)
									  {
										 ;
									  }
								   }
								}
							}
							else
							{
							   OtWetErrorSizeFile(SpisokFileArchiv->Strings[i], ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir", sizefile, MyMd5);
							}
						 }
						 else
						 {
							 if((sizefile >0) && (sizefile < 134271701))
							 {
								MyMd5 = CalkFileMD5(SpisokFileArchiv->Strings[i]).LowerCase();

								if(MyMd5 !="")
								{
								   bool postFileArchiv = false;

								   for( int st = 0; st < SpisokFileKarantin->Count ; st++)
								   {
									  TStringDynArray arr(SplitString(SpisokFileKarantin->Strings[st], "|"));

									  if(MyMd5 == arr[0])
									  {
										 if(name != arr[1])
										 {
											postFileArchiv = true;
											String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
											ZanoshuVListWiew3PriOshibki(SpisokFileArchiv->Strings[i], sizefile, result , MyMd5, false);
											// Добавить перенос файлов в третью в кладку. Отдельную функцию написать.
											// Когда хеш одинаковый но файлы разные. При этом просто прописать ссылку.
											break;
										 }
										 else
										 {
											postFileArchiv = true;
											break;
										 }
									  }
								   }
								   if(!postFileArchiv)
								   {
									  try
									  {
										 SpisokFileKarantin->Add(MyMd5 + "|" + name);
										 FileSpisokProwerki->Add(SpisokFileArchiv->Strings[i]);
									  }
									  catch(...)
									  {
										 ;
									  }
								   }
								}
							}
							else
							{
							   OtWetErrorSizeFile(SpisokFileArchiv->Strings[i], ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir", sizefile, MyMd5);
							}
						 }
					  }
					  else
					  {
                         if((sizefile >0) && (sizefile < 134271701))
						 {
                            MyMd5 = CalkFileMD5(SpisokFileArchiv->Strings[i]).LowerCase();

							if(MyMd5 !="")
							{
							   bool postFileArchiv = false;

							   for( int st = 0; st < SpisokFileKarantin->Count ; st++)
							   {
									  TStringDynArray arr(SplitString(SpisokFileKarantin->Strings[st], "|"));

									  if(MyMd5 == arr[0])
									  {
										 if(name != arr[1])
										 {
											postFileArchiv = true;
											String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
											ZanoshuVListWiew3PriOshibki(SpisokFileArchiv->Strings[i], sizefile, result , MyMd5, false);
											// Добавить перенос файлов в третью в кладку. Отдельную функцию написать.
											// Когда хеш одинаковый но файлы разные. При этом просто прописать ссылку.
											break;
										 }
										 else
										 {
											postFileArchiv = true;
											break;
										 }
									  }
							   }
							   if(!postFileArchiv)
							   {
								  SpisokFileKarantin->Add(MyMd5 + "|" + name);
								  FileSpisokProwerki->Add(SpisokFileArchiv->Strings[i]);
							   }

							}
						 }
						 else
						 {
							OtWetErrorSizeFile(SpisokFileArchiv->Strings[i], ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir", sizefile, MyMd5);
						 }
					  }

				  }
			   else// провожу проверку файлов "txt". Если второе расширение имеет 42 символа,
				   //  то это описание файла из карантина UVS
				   //пример ADSNDS.DLL._BAF0F1105A8247832657DBDB4924B88D384C7E9A.txt
				   //При этом этот файл не заносится в список.
			   if(ExtractFileExt(SpisokFileArchiv->Strings[i]).LowerCase() == ".txt")
			   {
				  if(ExtractFileExt(ChangeFileExt(SpisokFileArchiv->Strings[i],"")).Length() !=42)
					 FileSpisokProwerki->Add(SpisokFileArchiv->Strings[i]);
			   }

			   // парсинг карантина tdskillera
			   else
				  if(ExtractFileExt(SpisokFileArchiv->Strings[i]) ==".dta"&&
					ExtractFileName(SpisokFileArchiv->Strings[i]).LowerCase().Pos("tsk")!=0)
			   {
				  // проверяю присутствует файл ини или нет.
				  if(FileExists(ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExstractFileNameBezExt(ExtractFileName(SpisokFileArchiv->Strings[i])) + ".ini") ==false)
				  {


					 if(FileExists(SpisokFileArchiv->Strings[i]))
					 {
						__int64 sizefile = MyFileSize(SpisokFileArchiv->Strings[i]);
						AnsiString MyMd5 = "";
						if((sizefile >0) && (sizefile < 134271701))
						{
						   MyMd5 = CalkFileMD5(AnsiString(SpisokFileArchiv->Strings[i])).LowerCase();


						   if( (MyMd5 !=""))
						   {
							  bool postFileArchiv = false;

							  for( int st = 0; st < SpisokFileKarantin->Count ; st++)
							  {
								 TStringDynArray arr(SplitString(SpisokFileKarantin->Strings[st], "|"));
								 if(MyMd5 == arr[0])
								 {
									if(SpisokFileArchiv->Strings[i] != arr[1])
									{
									   postFileArchiv = true;
									   String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
									   ZanoshuVListWiew3PriOshibki(SpisokFileArchiv->Strings[i], sizefile, result , MyMd5, false);
									   // Добавить перенос файлов в третью в кладку. Отдельную функцию написать.
									   // Когда хеш одинаковый но файлы разные. При этом просто прописать ссылку.
									   break;
									}
									else
									{

									   postFileArchiv = true;
									   break;
									}
								 }
									 }
									 if(!postFileArchiv)
									 {
									   try
									   {
										  //RenameFile(SpisokFileArchiv->Strings[i],ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir");
										  SpisokFileKarantin->Add(MyMd5 + "|" +SpisokFileArchiv->Strings[i]);
										  FileSpisokProwerki->Add(SpisokFileArchiv->Strings[i]);
									   }
									   catch(...)
									   {
										  ;
									   }
									 }
								   }
							  }
							  else
							  {
								 OtWetErrorSizeFile(SpisokFileArchiv->Strings[i], "", sizefile, MyMd5);

							  }
						   }
				  }

				  std::auto_ptr<TStringList> Zapros (new TStringList(NULL) );
                  AnsiString name = "";
				  bool boolextName = false;

				  try
				  {
					 Zapros->LoadFromFile(ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExstractFileNameBezExt(ExtractFileName(SpisokFileArchiv->Strings[i])) + ".ini");
				  }
				  catch(...)
				  {
					  ;
                  }


				  for(int q=0; q<Zapros->Count;q++)
				  {
					  if(Zapros->Strings[q].Pos("Src") !=0)
					  {
						 name = Zapros->Strings[q].SubString(Zapros->Strings[q].Pos("Src")+4, Zapros->Strings[q].Length()-(Zapros->Strings[q].Pos("Src")+3));
						 if(name !="")
						 {
							boolextName = true;
							__int64 sizefile = MyFileSize(SpisokFileArchiv->Strings[i]);
							AnsiString MyMd5 = "";

							if(FileExists(SpisokFileArchiv->Strings[i]))
							{
							   if((sizefile >0) && (sizefile < 134271701))
							   {
								  MyMd5 = CalkFileMD5(AnsiString(SpisokFileArchiv->Strings[i])).LowerCase();


								  if( (MyMd5 !=""))
								  {
									 bool postFileArchiv = false;

									 for( int st = 0; st < SpisokFileKarantin->Count ; st++)
									 {
										 TStringDynArray arr(SplitString(SpisokFileKarantin->Strings[st], "|"));
										 if(MyMd5 == arr[0])
										 {
											if(name != arr[1])
											{
											   UnicodeString fileOut;
											   fileOut = RenameFileName(ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir");

											   if(RenameFile(SpisokFileArchiv->Strings[i],fileOut))
											   {
												  postFileArchiv = true;
												  String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
												  ZanoshuVListWiew3PriOshibki(fileOut, sizefile, result , MyMd5, false);
												  break;
											   }
											   else
											   {
												  ErrorMessage = "Ошибка переименовывания файла " + SpisokFileArchiv->Strings[i] + "\n в " +fileOut +"\n";
												  Synchronize(&ErrorLog);
											   }
											}
											else
											{

											   postFileArchiv = true;
											   break;
											}
										 }
									 }
									 if(!postFileArchiv)
									 {
										UnicodeString nameFy = RenameFileName(ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir");

										if(RenameFile(SpisokFileArchiv->Strings[i],nameFy))
										{
										   FileSpisokProwerki->Add(nameFy);
										   SpisokFileKarantin->Add(MyMd5 + "|" +name);
										}
										else
										{
										   ErrorMessage = "Ошибка переименовывания файла " + SpisokFileArchiv->Strings[i] + "\n в " +nameFy +"\n";
										   Synchronize(&ErrorLog);
										}
									 }

										 if(FileSpisokProwerki->IndexOf(ExtractFilePath(SpisokFileArchiv->Strings[i]) + "object.ini") != -1)
											FileSpisokProwerki->Delete(FileSpisokProwerki->IndexOf(ExtractFilePath(SpisokFileArchiv->Strings[i])+ "Object.ini"));

										 /*if(SpisokFileArchiv->IndexOf(ExtractFilePath(SpisokFileArchiv->Strings[i]) + "object.ini") != -1)
										 {
											SpisokFileArchiv->Delete(SpisokFileArchiv->IndexOf(ExtractFilePath(SpisokFileArchiv->Strings[i])+ "Object.ini"));
											--i;
										 }*/

										 if(SpisokFileArchiv->IndexOf(ExtractFilePath(ExtractFileDir(SpisokFileArchiv->Strings[i])) + "object.ini") != -1)
										 {
											SpisokFileArchiv->Delete(SpisokFileArchiv->IndexOf(ExtractFilePath(ExtractFileDir(SpisokFileArchiv->Strings[i]))+ "Object.ini"));
											--i;
										 }

										 if(FileSpisokProwerki->IndexOf(ExtractFilePath(ExtractFileDir(SpisokFileArchiv->Strings[i])) + "object.ini") != -1)
											FileSpisokProwerki->Delete(FileSpisokProwerki->IndexOf(ExtractFilePath(ExtractFileDir(SpisokFileArchiv->Strings[i])) + "object.ini"));
								 }
							  }
							  else
							  {
								 OtWetErrorSizeFile(SpisokFileArchiv->Strings[i], ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir", sizefile, MyMd5);
							  }
							}
						 }
					  }
					  // Добавить условие если в ини нет Src. То есть пути к файлу.
					  // Надо будет обробатывать "object.ini"
				  }

				  if(!boolextName)
				  {
					 UnicodeString NameIni = "";
					 UnicodeString PatchIni = "";

					 for(int q=0; q<Zapros->Count;q++)
					 {
						int post = Zapros->Strings[q].Pos("Name:");

						if( post !=0)
						{
							NameIni = Zapros->Strings[q].SubString(post + 5, Zapros->Strings[q].Length() - (post+4));
						}
					 }

					 if(NameIni =="")
					 {
						for(int q=0; q<Zapros->Count;q++)
						{
						   int post = Zapros->Strings[q].Pos("Type:");

						   if( post !=0)
						   {
							  NameIni = Zapros->Strings[q].SubString(post + 5, Zapros->Strings[q].Length() - (post+4));
						   }
						}
					 }


					 Zapros->Clear();

					 try
					 {
						Zapros->LoadFromFile(ExtractFilePath(SpisokFileArchiv->Strings[i]) + "object.ini");
					 }
					 catch(...)
					 {
						 ;
					 }

					 for(int q=0; q<Zapros->Count; q++)
					 {
						 int post = Zapros->Strings[q].Pos("Name:");

						if( post !=0)
						{
							PatchIni = Zapros->Strings[q].SubString(post + 5, Zapros->Strings[q].Length() - (post+4));
						}
					 }
					 //begi когда нет поля name
					 if(NameIni !="" && PatchIni !="")
					 {
						name = PatchIni + "\\" + NameIni;
						__int64 sizefile = MyFileSize(SpisokFileArchiv->Strings[i]);
							AnsiString MyMd5 = "";

							if(FileExists(SpisokFileArchiv->Strings[i]))
							{
							   if((sizefile >0) && (sizefile < 134271701))
							   {
								  MyMd5 = CalkFileMD5(AnsiString(SpisokFileArchiv->Strings[i])).LowerCase();


								  if( (MyMd5 !=""))
								  {
									 bool postFileArchiv = false;

									 for( int st = 0; st < SpisokFileKarantin->Count ; st++)
									 {
										 TStringDynArray arr(SplitString(SpisokFileKarantin->Strings[st], "|"));
										 if(MyMd5 == arr[0])
										 {
											if(name != arr[1])
											{
											   UnicodeString fileOut;
											   fileOut = RenameFileName(ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir");

											   if(RenameFile(SpisokFileArchiv->Strings[i],fileOut))
											   {
												  postFileArchiv = true;
												  String result = "https://www.virustotal.com/en/search/?query=" + MyMd5;
												  ZanoshuVListWiew3PriOshibki(fileOut, sizefile, result , MyMd5,false);
												  break;
											   }
											   else
											   {
												  ErrorMessage = "Ошибка переименовывания файла " + SpisokFileArchiv->Strings[i] + "\n в " +fileOut +"\n";
												  Synchronize(&ErrorLog);
											   }
											}
											else
											{
											   postFileArchiv = true;
											   break;
											}
										 }
									 }
									 if(!postFileArchiv)
									 {
										UnicodeString nameFy = RenameFileName(ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir");

										if(RenameFile(SpisokFileArchiv->Strings[i],nameFy))
										{
										   FileSpisokProwerki->Add(nameFy);
										   SpisokFileKarantin->Add(MyMd5 + "|" +name);
										}
										else
										{
										   ErrorMessage = "Ошибка переименовывания файла " + SpisokFileArchiv->Strings[i] + "\n в " +nameFy +"\n";
										   Synchronize(&ErrorLog);
										}
									 }
									 // из списка удаляю две последние записи. так как там будет файл "object.ini"

									 if(FileSpisokProwerki->IndexOf(ExtractFilePath(SpisokFileArchiv->Strings[i]) + "object.ini") != -1)
											FileSpisokProwerki->Delete(FileSpisokProwerki->IndexOf(ExtractFilePath(SpisokFileArchiv->Strings[i])+ "Object.ini"));

										 /*if(SpisokFileArchiv->IndexOf(ExtractFilePath(SpisokFileArchiv->Strings[i]) + "object.ini") != -1)
										 {
											SpisokFileArchiv->Delete(SpisokFileArchiv->IndexOf(ExtractFilePath(SpisokFileArchiv->Strings[i])+ "Object.ini"));
											--i;
										 } */

										 if(SpisokFileArchiv->IndexOf(ExtractFilePath(ExtractFileDir(SpisokFileArchiv->Strings[i])) + "object.ini") != -1)
										 {
											SpisokFileArchiv->Delete(SpisokFileArchiv->IndexOf(ExtractFilePath(ExtractFileDir(SpisokFileArchiv->Strings[i]))+ "Object.ini"));
											--i;
										 }

										 if(FileSpisokProwerki->IndexOf(ExtractFilePath(ExtractFileDir(SpisokFileArchiv->Strings[i])) + "object.ini") != -1)
											FileSpisokProwerki->Delete(FileSpisokProwerki->IndexOf(ExtractFilePath(ExtractFileDir(SpisokFileArchiv->Strings[i])) + "object.ini"));

								 }
							  }
							  else
							  {
								 OtWetErrorSizeFile(SpisokFileArchiv->Strings[i], ExtractFilePath(SpisokFileArchiv->Strings[i]) + ExtractFileName(name) + ".vir", sizefile, MyMd5);
							  // Добавить функцию перено во втретью в кладку ели размер больше 128мб или меньше 0
							  }
							}
					 }
					 //End когда нет поля name
				  }


			   }
			   else
				  if(ExtractFileExt(SpisokFileArchiv->Strings[i]).LowerCase() == ".ini" &&
						  ExtractFileName(SpisokFileArchiv->Strings[i]).LowerCase().Pos("tsk")!=0)
			   {
				   ;
			   }
			   // end парсинг карантина tdskillera

			   else
				   FileSpisokProwerki->Add(SpisokFileArchiv->Strings[i]);
			}

		}
		//MyFiledDir(Dir);
	}
	else
	{
	   if(AtOptions.ErrorArchiv)
		  ShowMessage(L"Ошибка распаковки архива.\n" + NameArchiv);

	   ErrorMessage = L"Ошибка распаковки архива.\n" + NameArchiv;
	}
	//FileSpisokProwerki->SaveToFile(ExtractFilePath(Application->ExeName) + "AitotalTMP\\asd");

}
//+++++++++++++++++++++++++++++++++++++++
AnsiString ParsingArchiv::MyFiledDirArchiv(UnicodeString Dir)
{
	TStringDynArray Files;
	AnsiString Spisok;
	String Mask= "*"; //  "*.txt"
	std::auto_ptr<TStringList> SpisokFileArchiv (new TStringList(NULL) );

	try
	{
	   Files = TDirectory::GetFiles(Dir, Mask ,TSearchOption::soAllDirectories );

	   for(int i=0; i<Files.Length; i++)
	   {
		  AnsiString qwe = Files[i];
		  //закоментировал так как вложенный архив не распаковываем.
		  /*if(ExtractFileExt(qwe.LowerCase()) == ".rar" ||
			 ExtractFileExt(qwe.LowerCase()) == ".zip" ||
			 ExtractFileExt(qwe.LowerCase()) == ".7z")
		  {
			 ExtArchiv(qwe);
		  }
		  else*/
			 SpisokFileArchiv->Add(qwe.LowerCase());
	   }
	}
	catch (EDirectoryNotFoundException  &d)
	{
	   /*if(ExtractFileExt(Dir.LowerCase()) == ".rar" ||
		  ExtractFileExt(Dir.LowerCase()) == ".zip" ||
		  ExtractFileExt(Dir.LowerCase()) == ".7z")
	   {
		  ExtArchiv(Dir);
	   }
	   else*/
		  SpisokFileArchiv->Add(Dir.LowerCase());
	}

	if(SpisokFileArchiv->Text !=" ")
	{
		Spisok = SpisokFileArchiv->Text;
	}
	else
	   Spisok="";

	return Spisok;
}
//++++++++++++++++++++++++++++++++++
void __fastcall ParsingArchiv::ZanoshuVListWiew3PriOshibki(String ArcName, __int64 MysizaFile , String rezult, String Heh, bool date)
{
	ListItem = Form3->ListView3->Items->Add();
	ListItem->Caption =ExtractFileName(ArcName);
	ListItem->Checked = false;
	ListItem->SubItems->Add(ArcName);
	ListItem->SubItems->Add(MysizaFile);
	if(Heh =="")
	{
	   ListItem->SubItems->Add("");
	   ListItem->SubItems->Add("");
	}
	else
	{
	   ListItem->SubItems->Add(Heh);
	   ListItem->SubItems->Add("");
	   //ListItem->SubItems->Add(MySha256(ArcName));
	}

	if(date)
	   ListItem->SubItems->Add(FormatDateTime("YYYY.MM.DD",Date())+ " " + FormatDateTime("HH:NN:SS",Time()));
	else
	   ListItem->SubItems->Add("");
	ListItem->SubItems->Add("");
	ListItem->SubItems->Add(rezult);
	FileNumber++;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++
void __fastcall ParsingArchiv::OtWetErrorSizeFile(UnicodeString FileNime, UnicodeString MoveFileName,  __int64 sizefile, UnicodeString Md5)
{
   if(MoveFileName !="")
   {
	  UnicodeString nameFy = RenameFileName(MoveFileName);
	  MoveFileName = nameFy;
	  RenameFile(FileNime,MoveFileName);
   }
   else
	 MoveFileName = ExtractFilePath(FileNime);


   if(sizefile == 0)
   {

	  String resultat = "Размер файла = 0 байт";
	 // ErrorLog(MoveFileName + "\n Размер файла = 0 байт");
	  ZanoshuVListWiew3PriOshibki(MoveFileName, sizefile ,  resultat , Md5, true);
   }
   if(sizefile > 134271701)
   {

	  String resultat = "Размер файла больше 128 Мб";

	  ErrorMessage= MoveFileName + "\n Размер файла больше 128мб";
	  Synchronize(&ErrorLog);
	  ZanoshuVListWiew3PriOshibki(MoveFileName, sizefile , resultat , Md5, true );
   }
   if(sizefile < 0)
   {
	  String resultat = "Ошибка определения размера файла";
	  ErrorMessage= MoveFileName + "\n Ошибка определения размера файла";
	  Synchronize(&ErrorLog);
	  ZanoshuVListWiew3PriOshibki(MoveFileName, sizefile , resultat , Md5, true );
   }
}
//---------------------------------------------------------------------------
AnsiString ParsingArchiv::ExstractFileNameBezExt(UnicodeString NameFile)
{
   return ChangeFileExt(NameFile,"");
}
