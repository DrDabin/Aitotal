//---------------------------------------------------------------------------

#ifndef ScanVTIndy1H
#define ScanVTIndy1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Hash.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>

#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>
#include "AitotalInclude/Struct.h"
//---------------------------------------------------------------------------
class ScanVTIndy : public TThread
{
private:
   void __fastcall ScanVTIndy::InOnWorkBegin(TObject *ASender, TWorkMode AWorkMode,__int64 AWorkCountMax);
   void __fastcall ScanVTIndy::InWork(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCount);
   void __fastcall ScanVTIndy::InWorkEnd(TObject *Sender, TWorkMode AWorkMode);
   // Вывожу типо прогресса, что делается или какая ошибка
   void __fastcall ScanVTIndy::ScanProgres();
   // Переношу всё во вторую вкладку при начале проверки "Сканирование"
   void __fastcall ScanVTIndy::AtScanBegin();
   // Удаляем значение из списка номера потока, что бы было ограничение на количество загрузок а не проверок.
   void __fastcall ScanVTIndy::DelSpisokNamePotok();
   // Функция действия при ошибке
   void __fastcall ScanVTIndy::OtwetOshibka();
   //Функция ответа. Переношу всё в фкладку Результат.
   //void __fastcall ScanVTIndy::Otwet();
   // Проверяю количество загружаемых файлов.
   //если их меньше чем в настройках то заношу в список номер PotokNumber и FileUpload = true.
   // если одинаково то FileUpload = false
   void __fastcall ScanVTIndy::FileUploadBool();
   TDateTime ScanVTIndy::UTCToLocalTime(TDateTime AValue);
   // Вывожу прогресс при загрузки файла.
   void __fastcall ScanVTIndy::UploadProgress();
   // Заношу ошибки в список и сразуже сохраняю.
   void __fastcall ScanVTIndy::ErrorLog();
   // Переношу в третью вкладку когда размер файла более 32Мб но менее 128Мб и если он до этого не проверялся.
   void __fastcall ScanVTIndy::OtwetErrorSizeFile();
   __int64 VTFileSize;//размер файла для загрузки
   int VtZagruzki; // процент загрузки файла.
   UnicodeString NameLogError; //Путь до файла с ошибкой
   UnicodeString ErrorMessage; // текст ошибки.
   UnicodeString LogMessage; // текст логирования.
   UnicodeString Progress;// Содержит, что делается
   bool FileUpload;

   /* Новые функциии для нового изменения на VT.
	  Поменялась версия на v3.
   */
   // Делаю запрос есть такой файл или нет.
   //Если есть выводит всю информацию о нем.
   // Если данного хеш нет, то выдает код ответа 404.
   bool __fastcall ScanVTIndy::VTFilesID (UnicodeString chesch);
   //загрузка файла.
   bool __fastcall ScanVTIndy::VTFiles(UnicodeString FileName);
   //Общая функция сканирования/проверки файла.
   // Используется в Execute.
   void __fastcall ScanVTIndy::ScanFiles();
   // Функция поиска хеша на VT
   // Использует функцию VTFilesID
   //Вывод false/true
   bool __fastcall ScanVTIndy::SearchHesh(UnicodeString Hesh);
   //Обрабатывается когда Не нужен реанализ проверенного файла.
   void __fastcall ScanVTIndy::ReScanFalse();
   void __fastcall ScanVTIndy::VTJSONParseDetect();
   void __fastcall ScanVTIndy::VIOtwet();
   //Проводит реанализ файла, и ждет отчета новой провеки?
   void __fastcall ScanVTIndy::ReScanVT();
   //Реанализ файла.
   bool __fastcall ScanVTIndy::VTAnalyse (UnicodeString chesch);
   //Получаем результат по ID после VTAnalyse
   bool __fastcall ScanVTIndy::VTAnalysasID ();
   //Извлекаю из ответа ID для Get analyses id
   bool __fastcall ScanVTIndy::JSONParseAnalysisID();
   //Парсинг после Analyses для вывода отчета.
   void __fastcall ScanVTIndy::VTJSONParseDetectAnalysasID();
   // Извлекаем статус проверки, в VTAnalysasID()
   // статусы
   // "completed" - завершен.
   // "queued" в ожидание.
   // "in-progress" в процессе.
   bool __fastcall ScanVTIndy::JSONParseAnalysisStatus();
protected:
	void __fastcall Execute();
public:
	__fastcall ScanVTIndy(bool CreateSuspended);
	//Номер потока, удаляется изсписка после загрузки файла, или начало
	// рескана
	int PotokNumber;
	// Номер файла по порядку., что бы не было совпадиний.
	// Меняетя в MyNewScan, ExtArchiv.
	//Одинаковых номеров не должно быть.
	int FileNumber;
	//труе - логируем всё, что происходит в ScanVTIndy и сохраняем в файл
	// имя файла лога == имя проверяемого файла
	bool logirovanie;
	//Функция логирования.
	void __fastcall ScanVTIndy::Logirovanie();
	Base VtBase;

	UnicodeString TimeBreak;
	// Переменная хранить дату и время завержение ожидания проверки файла.
	// использую при добавление к файлу логирования
	UnicodeString TimeLogirovanie;
	struct ScanVT
	{
	   Proxy ProxyVT;
	   AnsiString ApiKey;
	   bool Rescan;
	   bool Proverka; // false проводится первичная проверка. true повторная проверка.
	   int http_response_code;// ответ сервера 200, 204. ƒругие не обробатываютс¤
	   int VT_response_code;// значение jsson  response_code
	   //использую в UploadFileVT и LoadingFile.
	   //Если 1 то это исключение инди которые обробатываются. 0 это не обрабатываются исключения сразу выход.
	   int CodeErrorUpload;
	}ScanIndyVT;
};
//---------------------------------------------------------------------------
#endif

