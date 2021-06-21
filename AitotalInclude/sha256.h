//---------------------------------------------------------------------------

#ifndef sha256H
#define sha256H

UnicodeString Mysha256(UnicodeString filename,UnicodeString &error);
//
UnicodeString Mysha256ver2(UnicodeString filename);
// Вместо имени файла передаю поток.
UnicodeString Streamsha256(TStream *filename);
//---------------------------------------------------------------------------
#endif
