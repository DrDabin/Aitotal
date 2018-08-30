// Common/IntToString.h

#ifndef __COMMON_INT_TO_STRING_H
#define __COMMON_INT_TO_STRING_H

#include "MyTypes.h"

void ConvertUInt32ToString(UInt3232 value, char *s) throw();
void ConvertUInt64ToString(UInt6464 value, char *s) throw();

void ConvertUInt32ToString(UInt3232 value, wchar_t *s) throw();
void ConvertUInt64ToString(UInt6464 value, wchar_t *s) throw();

void ConvertUInt64ToOct(UInt6464 value, char *s) throw();

void ConvertUInt32ToHex(UInt3232 value, char *s) throw();
void ConvertUInt64ToHex(UInt6464 value, char *s) throw();
void ConvertUInt32ToHex8Digits(UInt3232 value, char *s) throw();
// void ConvertUInt32ToHex8Digits(UInt3232 value, wchar_t *s) throw();

void ConvertInt64ToString(Int6464 value, char *s) throw();
void ConvertInt64ToString(Int6464 value, wchar_t *s) throw();

#endif
