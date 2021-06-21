﻿// ---------------------------------------------------------------------------

#ifndef PrintFunctionsH
#define PrintFunctionsH
// ---------------------------------------------------------------------------
#include <stdio.h>
#include "Common/IntToString.h"
#include "Common/MyInitGuid.h"
#include "Common/StringConvert.h"
// ---------------------------------------------------------------------------
void PrintString(const UString& s);
void PrintString(const char *s);
void PrintNewLine();
void PrintStringLn(const AString& s);
void PrintStringExtracting(const UString& s);
void PrintError(const UString& s);
void PrintStringExtractingS(const UString& s, const UInt6464& ci);
// ---------------------------------------------------------------------------
#endif
