﻿//---------------------------------------------------------------------------
#include <vcl.h>
#include <System.IOUtils.hpp>
#include <StrUtils.hpp>

#include <sys/types.h>
#include <sys\stat.h>
#include <errno.h>


#pragma hdrstop
 #include "Aitotal1.h"
#include "RabotaSfilam.h"
#include "AitotalInclude/Struct.h"//Структуры. Перенесу туда все мои структуры. И надо будет их обьединить.
#include "AitotalInclude/cheshMD5/md5.hpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
UnicodeString LnMesErorCalcSizeFileOpe = "/nОшибка определения размера файла. Ошибка при открытие файла =";
UnicodeString LnMesErorCalcSizeFileCalc = "/nОшибка определения размера файла. Ошибка при расчете размера = ";

struct stati64 statbuf;

// иЗМЕНЯЕМ ИМЯ ФАЙЛА ЕСЛИ ОНО ЕСТЬ В ДАННОЙ ДИРЕКТОРИИ. прибавляем (i)
UnicodeString RenameFileName(UnicodeString fileNime)
{
   if(TFile::Exists(fileNime))
   {
			for(int i = 2; ; i++)
			{
				UnicodeString name = ChangeFileExt(fileNime,"") + "(" + i + ")" + ExtractFileExt(fileNime);

				if(!TFile::Exists(name))
				{
				   return name;
                }
			}
   }
   return fileNime;
}

__int64 MyFileSize2(UnicodeString Path)
{
	HANDLE file;
	Path = L"\\\\?\\" + Path;

	file = CreateFileW(Path.w_str(), GENERIC_READ,FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

	if(file == INVALID_HANDLE_VALUE)
	{
		CloseHandle(file);
		//ShowMessage(SysErrorMessage(GetLastError()));
		Form3->ErrorLog(Path + LnMesErorCalcSizeFileOpe   +   SysErrorMessage(GetLastError())  );
		return -1;
	}
	   //ShowMessage("Non fatal error - File cannot be opened because this file is in use.");

	else
	{
	  __int64 dwSize;
	  dwSize = GetFileSize(file, NULL);
	  CloseHandle(file);

	  if (dwSize == INVALID_FILE_SIZE)
	  {
		 Form3->ErrorLog(Path + LnMesErorCalcSizeFileCalc   +   SysErrorMessage(GetLastError())  );
		 // Получим код ошибки.
		 return -2;
	  }
	  else
	  {
		 return dwSize;
	  }

	}
}
//++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++
__int64 FileSizeStatic(UnicodeString Path)
{


   try{

	Path = L"\\\\?\\" + Path;
   int rezult = _wstati64(Path.w_str(), &statbuf);

   if(rezult == 0)
   {
	  return statbuf.st_size;
   }
   else
   {
	  switch (errno)
	  {
		 case EPERM:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Операция не разрешена");
		   break;

		 case ENOENT:
		  Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Отсутствует файл или каталог");
           break;

		 case ESRCH:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Отсутствует такой процесс");
		   break;

		 case EINTR:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Прерванная функция");
		   break;

		 case EIO:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Ошибка ввода вывода");
		   break;

		 case ENXIO:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Отсутствует устройство или адресс");
		   break;

		 case E2BIG:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Список аргументов слишком длинный");
		   break;

		 case ENOEXEC:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Ошибка формата exec");
		   break;

		 case EBADF:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Неверный номер файла");
		   break;

		 case ECHILD:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Нет порожденных процессов");
		   break;

		 case EAGAIN:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Больше процессов нет или недостаточно памяти, или достигнут максимальный уровень вложенности");
		   break;

		 case ENOMEM:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Недостаточно памяти");
		   break;

		 case EACCES:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  В разрешении отказано");
		   break;

		 case EFAULT:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Неверный адрес");
		   break;

		 case EBUSY:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Устройство или ресурс заняты");
		   break;

		 case EEXIST:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Файл существует");
		   break;

		 case EXDEV:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Ссылка другого устройства");
		   break;

		 case ENODEV:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Нет такого устройства");
		   break;

		 case ENOTDIR:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Не является каталогом");
		   break;

		 case EISDIR:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Является каталогом");
		   break;

		 case EINVAL:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Недопустимый аргумент");
		   break;

		 case ENFILE:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Слишком много файлов открыто в системе");
		   break;

		 case EMFILE:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Слишком много открытых файлов");
		   break;

		 case ENOTTY:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Неподходящая операция управления вводом-выводом");
		   break;

		 case EFBIG:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Файл слишком велик");
		   break;

		 case ENOSPC:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  На устройстве не осталось места");
		   break;

		 case ESPIPE:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Недопустимый поиск");
		   break;

		 case EROFS:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Файловая система доступна только для чтения");
		   break;

		 case EMLINK:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Слишком много ссылок");
		   break;

		 case EPIPE:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Канал нарушен");
		   break;

		 case EDOM:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Неверный адрес");
		   break;

		 case ERANGE:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Результат слишком большой");
		   break;

		 case EDEADLOCK:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Аналогично EDEADLK для обеспечения совместимости с предыдущими версиями Microsoft C");
		   break;

		 case ENAMETOOLONG:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Слишком длинное имя файла");
		   break;

		 case ENOTEMPTY:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Каталог не пуст");
		   break;

		 case EILSEQ:
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Недопустимая последовательность байтов");
		   break;

		 default:
		   // Should never be reached.
		   Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Ошибка не известна 1");
	  }
   }
   }
   catch(...)
   {
		  Form3->ErrorLog(Path + "/nОшибка определения размера файла. =  Ошибка не известна 2");
   }
   return -1;
}
