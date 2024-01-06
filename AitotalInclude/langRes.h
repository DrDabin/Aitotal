﻿// ---------------------------------------------------------------------------

#ifndef __langResH
#define __langResH
// --------------------------------------------------------------------------

#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
struct TLanguageRec {
  UnicodeString id;
  PUnicodeString var;
};
UnicodeString LnMesUnpakArchiv = "Распаковка архива";
UnicodeString LnMesUnpak = "Процент распаковки = ";
UnicodeString LnMesNoCMDParam = "\n Нет такого параметра коммандной строки.\n";
UnicodeString LnMesErrorToFunc = "Ошибка в функции";
UnicodeString LnMesNoInterConect = "Нет соединение с интернетом";
UnicodeString LnMesChoosingFolder = "Выбор папки"  ;
UnicodeString LnMesAttentionError = "Внимание ошибка" ;
UnicodeString LnMesResNotFound = "Ресурс не найден"  ;
UnicodeString LnMesFileSize = "Размер файла"      ;
UnicodeString LnMesFileSizeMore = "Размер файла больше Мб" ;
UnicodeString LnMesErrorSizeFile = "Ошибка определения размера файла" ;
UnicodeString LnMesErrorCalcSHA256 = "Ошибка расчета SHA256" ;
UnicodeString LnMesErrorRenamFile = "Ошибка переименовывания файла" ;
UnicodeString LnMesAVZArchivParsing = "Парсинг архива AVZ. Файл" ;
UnicodeString LnMesUVSArchivParsing ="Парсинг архива UVS. Файл";
UnicodeString LnMesTDScillerArchivParsing = "Парсинг архива TDSciller. Файл"  ;
UnicodeString LnMesArchivParsing = "Парсинг архива";
UnicodeString LnMesParsingComplette = "Парсинг окончен ";
UnicodeString LnMesListEmti = "Список пуст, похоже архив пустой" ;
UnicodeString LnMesArchivStartUnpache = "Начало распаковки архива" ;
UnicodeString LnMesArchivErrorUnpacking = "Ошибка распаковки архива.\n ";
UnicodeString LnMesProcessParsingList = "Разбор списка файлов. Обработано" ;
UnicodeString LnMesErrorHechSumm = "Ошибка определения хеш суммы. Возможно файл не доступен.";
UnicodeString LnMesErroDetectMD5 = "Ошибка определения MD5" ;
UnicodeString LnMesEror = "Ошибка";
UnicodeString LnMesUnableOpenFile = "\n Не удалось открыть файл.\n";
UnicodeString LnMesInternetNotConnect = "Интернет не подключен";
UnicodeString LnMesNoresult = "Результата нет";
UnicodeString LnMesInvalidCharacter = "Не допустимый символ MD5.";
UnicodeString LnMesCharacter = "Символ ";
UnicodeString LnMesFileName = "Имя файла" ;
UnicodeString LnMesFilePatch = "Путь к файлу";
UnicodeString LnMesResult = "Результат ";
UnicodeString LnMesSize = "Размер" ;
UnicodeString LnMesDate = "Дата"  ;
UnicodeString LnMesLink = "Ссылка" ;
UnicodeString LnMesChekFolderKarantin = "Выбор папки с карантином ";
UnicodeString LnMesErrorOpenRegistr = "Ошибка открытия реестра ";
UnicodeString LnMesWarnVistaVer ="Sorry. This option only works with Vista version or greater";
UnicodeString LnMesErrorDelRegistry ="Ошибка удаления ключа реестра";
UnicodeString LnMesInQueue = "На очереди = ";
UnicodeString LnMesFileToBeChek = "Проверяется файлов";
UnicodeString LnMesFilesChek = "Проверено файлов ";
UnicodeString LnMesErrorDeletion = "Ошибка удаления ";
UnicodeString LnMesErrorDelUnpackArchiv = "Ошибка удаления распакованного архива";
UnicodeString LnMesUploadCheking = "%Загрузки/проверки";
UnicodeString LnMesFileChekBusy = "Проверка на занятость файла.";
UnicodeString LnMesSessionRestor = "Восстановление сессии";
UnicodeString LnMesIfSesionIsRestor = "При восстановление сессии результаты текущей проверки будут утеряны.\nВы хотите продолжить восстановление сессии?";
UnicodeString LnMesMaxFileUpload = "Количество файлов при загрузке  не должно быть 0 или более 20";
UnicodeString LnMesMaxFileScaning = "Общее количество проверяемых файлов не должно быть 0 или более 50" ;
UnicodeString LnMesErrorsizeFileMin0 = "максимальный размер файла <= 0";
UnicodeString LnMesIncorectPassword ="Неверный пароль";
UnicodeString LnMesErrorIncPassword = "Ошибка. Неверный пароль.\n Введите новый пароль или нажмите отмена" ;
UnicodeString LnMesErrorOpenArchiv = "Ошибка. Архив поврежден." ;
UnicodeString LnMesErrorReportParsing = "Ошибка. Архив поврежден." ;
UnicodeString LnMesErrorfileReportNou = "Ошибка. Отчёт по файлу не готов";
UnicodeString LnMesResultSaved = "Результат сохранён" ;
UnicodeString LnMesErorCalcSizeFileOpe = "/nОшибка определения размера файла. Ошибка при открытие файла =";
UnicodeString LnMesErorCalcSizeFileCalc = "/nОшибка определения размера файла. Ошибка при расчете размера = ";
UnicodeString LnMesErroeReadSignArchiv = "\n Ошибка. Чтения сигнатуры архива.\n";
UnicodeString LnMesErrorArchivCorupted = "Возможно битый архив";
UnicodeString LnMesErrorNouSign = "\n Ошибка. Нет совпадение сигнатур архива";
UnicodeString LnMesErrorNouArchivSign = "\n Ошибка. Не известный архив его";
UnicodeString LnMesErrorNouTablSaveScaning = "Таблицы пустые. Файл сканирования не сохранён.";
UnicodeString LnMesStartOfScaning = "Начало сканирование";
UnicodeString LnMesScaningIsStoped = "Проверка остановлена" ;
UnicodeString LnMesUplodCompl = "Загрузка завершена в";
UnicodeString LnMesPerenosVTablScan = "Переношу всё в вкладку сканирование";
UnicodeString LnMesErrorNetwork = "Ошибка сети.";
UnicodeString LnMesSearchHessshBaseVT = "Ищем хеш в базе VT";
UnicodeString LnMesDisplToTablScan = "Вывожу в таблицу о начале сканирования";
UnicodeString LnMesScanStopped = "Проверка остановлена";
UnicodeString LnMesNouLimitApiKey = "Превышен лимит";
UnicodeString LnMesUploadFile = "Загрузка файла";
UnicodeString LnMesFileMax32MB = "айл более 32МБ";
UnicodeString LnMesErrorUploadFile = "Ошибка в загрузки файла.";
UnicodeString LnMesAnalizFile = "Идет анализ файла";
UnicodeString LnMesObrabotResult = "Обработка результата" ;
UnicodeString LnMesNoSovpadSHA256 = "Не совпадает sha256: у меня =";
UnicodeString LnMesToVT =  "На сайте =";
UnicodeString LnMesFileVerified = "Файл проверен";
UnicodeString LnMesOtwetNarescan = "Ждём ответа на повторный анализ";
TLanguageRec lr[82];
lr[0].var = &LnMesUnpakArchiv;  // this address to frMain->sButton1Msg
lr[0].var = &LnMesUnpakArchiv;  // this address to frMain->sButton1Msg
lr[0].id = "LnMesUnpakArchiv";  // id in *lng.ini
lr[1].var = &LnMesUnpak;  // this address to frMain->sButton1Msg
lr[1].id = "LnMesUnpak";  // id in *lng.ini
lr[2].var = &LnMesNoCMDParam;  // this address to frMain->sButton1Msg
lr[2].id = "LnMesNoCMDParam";  // id in *lng.ini
lr[3].var = &LnMesErrorToFunc;  // this address to frMain->sButton1Msg
lr[3].id = "LnMesErrorToFunc";  // id in *lng.ini
lr[4].var = &LnMesNoInterConect;  // this address to frMain->sButton1Msg
lr[4].id = "LnMesNoInterConect";  // id in *lng.ini
lr[5].var = &LnMesChoosingFolder;  // this address to frMain->sButton1Msg
lr[5].id = "LnMesChoosingFolder";  // id in *lng.ini
lr[6].var = &LnMesAttentionError;  // this address to frMain->sButton1Msg
lr[6].id = "LnMesAttentionError";  // id in *lng.ini
lr[7].var = &LnMesResNotFound;  // this address to frMain->sButton1Msg
lr[7].id = "LnMesResNotFound";  // id in *lng.ini
lr[8].var = &LnMesFileSize;  // this address to frMain->sButton1Msg
lr[8].id = "LnMesFileSize";  // id in *lng.ini
lr[9].var = &LnMesFileSizeMore;  // this address to frMain->sButton1Msg
lr[9].id = "LnMesFileSizeMore";  // id in *lng.ini
lr[10].var = &LnMesErrorSizeFile;  // this address to frMain->sButton1Msg
lr[10].id = "LnMesErrorSizeFile";  // id in *lng.ini
lr[11].var = &LnMesErrorCalcSHA256;  // this address to frMain->sButton1Msg
lr[11].id = "LnMesErrorCalcSHA256";  // id in *lng.ini
lr[12].var = &LnMesErrorRenamFile;  // this address to frMain->sButton1Msg
lr[12].id = "LnMesErrorRenamFile";  // id in *lng.ini
lr[13].var = &LnMesAVZArchivParsing;  // this address to frMain->sButton1Msg
lr[13].id = "LnMesAVZArchivParsing";  // id in *lng.ini
lr[14].var = &LnMesUVSArchivParsing;  // this address to frMain->sButton1Msg
lr[14].id = "LnMesUVSArchivParsing";  // id in *lng.ini
lr[15].var = &LnMesTDScillerArchivParsing;  // this address to frMain->sButton1Msg
lr[15].id = "LnMesTDScillerArchivParsing";  // id in *lng.ini
lr[16].var = &LnMesArchivParsing;  // this address to frMain->sButton1Msg
lr[16].id = "LnMesArchivParsing";  // id in *lng.ini
lr[17].var = &LnMesParsingComplette;  // this address to frMain->sButton1Msg
lr[17].id = "LnMesParsingComplette";  // id in *lng.ini
lr[18].var = &LnMesListEmti;  // this address to frMain->sButton1Msg
lr[18].id = "LnMesListEmti";  // id in *lng.ini
lr[19].var = &LnMesArchivStartUnpache;  // this address to frMain->sButton1Msg
lr[19].id = "LnMesArchivStartUnpache";  // id in *lng.ini
lr[20].var = &LnMesArchivErrorUnpacking;  // this address to frMain->sButton1Msg
lr[20].id = "LnMesArchivErrorUnpacking";
lr[21].var = &LnMesProcessParsingList;  // this address to frMain->sButton1Msg
lr[21].id = "LnMesProcessParsingList";  // id in *lng.ini
lr[22].var = &LnMesErroDetectMD5;  // this address to frMain->sButton1Msg
lr[22].id = "LnMesErroDetectMD5";  // id in *lng.ini
lr[23].var = &LnMesEror;  // this address to frMain->sButton1Msg
lr[23].id = "LnMesEror";  // id in *lng.ini
lr[24].var = &LnMesUnableOpenFile;  // this address to frMain->sButton1Msg
lr[24].id = "LnMesUnableOpenFile";  // id in *lng.ini
lr[25].var = &LnMesInternetNotConnect;  // this address to frMain->sButton1Msg
lr[25].id = "LnMesInternetNotConnect";  // id in *lng.ini
lr[26].var = &LnMesNoresult;  // this address to frMain->sButton1Msg
lr[26].id = "LnMesNoresult";  // id in *lng.ini
lr[27].var = &LnMesInvalidCharacter;  // this address to frMain->sButton1Msg
lr[27].id = "LnMesInvalidCharacter";  // id in *lng.ini
lr[28].var = &LnMesCharacter;  // this address to frMain->sButton1Msg
lr[28].id = "LnMesCharacter";  // id in *lng.ini
lr[29].var = &LnMesFileName;  // this address to frMain->sButton1Msg
lr[29].id = "LnMesFileName";  // id in *lng.ini
lr[30].var = &LnMesFilePatch;  // this address to frMain->sButton1Msg
lr[30].id = "LnMesFilePatch"
lr[31].var = &LnMesResult;  // this address to frMain->sButton1Msg
lr[31].id = "LnMesResult";  // id in *lng.ini
lr[32].var = &LnMesSize;  // this address to frMain->sButton1Msg
lr[32].id = "LnMesSize";  // id in *lng.ini
lr[33].var = &LnMesDate;  // this address to frMain->sButton1Msg
lr[33].id = "LnMesDate";  // id in *lng.ini
lr[34].var = &LnMesLink;  // this address to frMain->sButton1Msg
lr[34].id = "LnMesLink";  // id in *lng.ini
lr[35].var = &LnMesChekFolderKarantin;  // this address to frMain->sButton1Msg
lr[35].id = "LnMesChekFolderKarantin";  // id in *lng.ini
lr[36].var = &LnMesErrorOpenRegistr;  // this address to frMain->sButton1Msg
lr[36].id = "LnMesErrorOpenRegistr";  // id in *lng.ini
lr[37].var = &LnMesWarnVistaVer;  // this address to frMain->sButton1Msg
lr[37].id = "LnMesWarnVistaVer";  // id in *lng.ini
lr[38].var = &LnMesErrorDelRegistry;  // this address to frMain->sButton1Msg
lr[38].id = "LnMesErrorDelRegistry";  // id in *lng.ini
lr[39].var = &LnMesInQueue;  // this address to frMain->sButton1Msg
lr[39].id = "LnMesInQueue";  // id in *lng.ini
lr[40].var = &LnMesFileToBeChek;  //
lr[40].id = "LnMesFileToBeChek";
lr[41].var = &LnMesFilesChek;  //
lr[41].id = "LnMesFilesChek";
lr[42].var = &LnMesErrorDeletion;  //
lr[42].id = "LnMesErrorDeletion";
lr[43].var = &LnMesErrorDelUnpackArchiv;  //
lr[43].id = "LnMesErrorDelUnpackArchiv";
lr[44].var = &LnMesUploadCheking;  //
lr[44].id = "LnMesUploadCheking";
lr[45].var = &LnMesFileChekBusy;  //
lr[45].id = "LnMesFileChekBusy";
lr[46].var = &LnMesSessionRestor;  //
lr[46].id = "LnMesSessionRestor";
lr[47].var = &LnMesIfSesionIsRestor;  //
lr[47].id = "LnMesIfSesionIsRestor";
lr[48].var = &LnMesMaxFileUpload;  //
lr[48].id = "LnMesMaxFileUpload";
lr[49].var = &LnMesMaxFileScaning;  //
lr[49].id = "LnMesMaxFileScaning";
lr[50].var = &LnMesErrorsizeFileMin0;  //
lr[50].id = "LnMesErrorsizeFileMin0";
lr[51].var = &LnMesIncorectPassword;  //
lr[51].id = "LnMesIncorectPassword";
lr[52].var = &LnMesErrorIncPassword;  //
lr[52].id = "LnMesErrorIncPassword";
lr[53].var = &LnMesErrorOpenArchiv;  //
lr[53].id = "LnMesErrorOpenArchiv";
lr[54].var = &LnMesErrorReportParsing;  //
lr[54].id = "LnMesErrorReportParsing";
lr[55].var = &LnMesErrorfileReportNou;  //
lr[55].id = "LnMesErrorfileReportNou";
lr[56].var = &LnMesResultSaved;  //
lr[56].id = "LnMesResultSaved";
lr[57].var = &LnMesErorCalcSizeFileOpe;  //
lr[57].id = "LnMesErorCalcSizeFileOpe";
lr[58].var = &LnMesErorCalcSizeFileCalc;  //
lr[58].id = "LnMesErorCalcSizeFileCalc";
lr[59].var = &LnMesErroeReadSignArchiv;  //
lr[59].id = "LnMesErroeReadSignArchiv";
lr[60].var = &LnMesErrorArchivCorupted;  //
lr[60].id = "LnMesErrorArchivCorupted";
lr[61].var = &LnMesErrorNouSign;  //
lr[61].id = "LnMesErrorNouSign";
lr[62].var = &LnMesErrorNouArchivSign;  //
lr[62].id = "LnMesErrorNouArchivSign";
lr[63].var = &LnMesErrorNouTablSaveScaning;  //
lr[63].id = "LnMesErrorNouTablSaveScaning";
lr[64].var = &LnMesStartOfScaning;  //
lr[64].id = "LnMesStartOfScaning";
lr[65].var = &LnMesScaningIsStoped;  //
lr[65].id = "LnMesScaningIsStoped";
lr[66].var = &LnMesUplodCompl;  //
lr[66].id = "LnMesUplodCompl";
lr[67].var = &LnMesPerenosVTablScan;  //
lr[67].id = "LnMesPerenosVTablScan";
lr[68].var = &LnMesErrorNetwork;  //
lr[68].id = "LnMesErrorNetwork";
lr[69].var = &LnMesSearchHessshBaseVT;  //
lr[69].id = "LnMesSearchHessshBaseVT";
lr[70].var = &LnMesDisplToTablScan;  //
lr[70].id = "LnMesDisplToTablScan";
lr[71].var = &LnMesScanStopped;  //
lr[71].id = "LnMesScanStopped";
lr[72].var = &LnMesNouLimitApiKey;  //
lr[72].id = "LnMesNouLimitApiKey";
lr[73].var = &LnMesUploadFile;  //
lr[73].id = "LnMesUploadFile";
lr[74].var = &LnMesFileMax32MB;  //
lr[74].id = "LnMesFileMax32MB";
lr[75].var = &LnMesErrorUploadFile;  //
lr[75].id = "LnMesErrorUploadFile";
lr[76].var = &LnMesAnalizFile;  //
lr[76].id = "LnMesAnalizFile";
lr[77].var = &LnMesObrabotResult;  //
lr[77].id = "LnMesObrabotResult";
lr[78].var = &LnMesNoSovpadSHA256;  //
lr[78].id = "LnMesNoSovpadSHA256";
lr[79].var = &LnMesToVT;  //
lr[79].id = "LnMesToVT";
lr[80].var = &LnMesFileVerified;  //
lr[80].id = "LnMesFileVerified";
lr[81].var = &LnMesOtwetNarescan;  //
lr[81].id = "LnMesOtwetNarescan";




// ---------------------------------------------------------------------------
#endif

