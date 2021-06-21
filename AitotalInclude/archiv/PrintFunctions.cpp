﻿// ---------------------------------------------------------------------------
#pragma hdrstop
#include "PrintFunctions.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
extern void toExtractingS(const wchar_t* nameFile, const __int64 line);
extern void toLog(const wchar_t* line); // ф-ция логирования из файла формы
extern void toExtracting(const wchar_t* line);
extern void toLogA(const char* line);

// ---------------------------------------------------------------------------
void PrintString(const UString& s)
{
	//toLog((LPCSTR)GetOemString(s));
	toLog(s);
}
void PrintStringExtracting(const UString& s)
{
	toExtracting(s);
}

void PrintStringExtractingS(const UString &s, const UInt6464 &ci)
{
	toExtractingS(s,ci);
}

// ---------------------------------------------------------------------------
void PrintString(const char *s) {
	toLogA((LPCSTR)s);
}

// ---------------------------------------------------------------------------
void PrintNewLine() {
}

// ---------------------------------------------------------------------------
void PrintStringLn(const UString& s) {
	toLog(s);
}

// ---------------------------------------------------------------------------
void PrintError(const UString& s) {
	PrintString(s);
}
// ---------------------------------------------------------------------------
