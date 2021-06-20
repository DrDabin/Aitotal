﻿#ifndef StructH
#define StructH
#include <vector>
struct Base
{
	UnicodeString BaseFileName;
	UnicodeString BasePatchFileName;
	UnicodeString BaseMD5;
	UnicodeString BaseSHA256;
	UnicodeString BaseSizeFile;
	UnicodeString BaseDataProverki;
	UnicodeString BaseDetect;
	UnicodeString BaseAdress;
	UnicodeString BasePatchFileArchiv;
	UnicodeString BaseJesson;
	UnicodeString BasePredScanData;
   	int FileNumber;
};
struct Proxy
{
	bool ProxiChecked;// true читаем\записываем настройки прокси.
	int IpPort;
	int Socket;
	UnicodeString Proxy;
	UnicodeString OptProxiLogin;
	UnicodeString OptProxiPassword;
};

struct numchek
{
	int NumBer;
	bool rescan;
	numchek (int y , bool c){NumBer=y; rescan=c;};
};


struct Options
{
	Proxy ProxyVT;
	int FileCount;
	int Thread;
	bool Exit_Trei;//если труе мы закрываем программы, елзе - сворачиваем в трей
	bool ErrorArchiv;// Определяем выводить уведомление при ошибки распаковки архивов.
	bool BallonHint;
	std::vector<UnicodeString> Apikey;
};

struct Strukt
{
	UnicodeString MyPatch;
	__int64 SizeFile;
	bool Cheked;
	UnicodeString SHA256;
	String Md5File;
};
 // Стркутура для заполнения списка TList *SpisokStructFile;
struct StructFileList
{
	public:
	// если труе то значения все заполнены( если нет ошибок), фалсе то всего два значения рескан и имя файла
	bool parsing;
	UnicodeString filename;
	bool rescan; //труе проводим рескан.
	__int64 SizeFile;
	UnicodeString SHA256;
	UnicodeString Md5File;

	StructFileList (bool par, bool res, UnicodeString nam, __int64 size, UnicodeString md5, UnicodeString sha)
	{
		parsing = par;
		rescan =  res;
		filename = nam;
		SizeFile = size;
		Md5File = md5;
		SHA256 = sha;
	}
};

struct DuplicatListFile
{
	UnicodeString FilePatch;
	UnicodeString MD5;
	DuplicatListFile (UnicodeString filepatch ,UnicodeString md5){FilePatch=filepatch; MD5=md5;};
};

#endif
