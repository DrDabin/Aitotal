//---------------------------------------------------------------------------

#ifndef sha256H
#define sha256H

UnicodeString Mysha256(UnicodeString filename,UnicodeString &error);
//
UnicodeString Mysha256ver2(UnicodeString filename);
// ������ ����� ����� ������� �����.
UnicodeString Streamsha256(TStream *filename);
//---------------------------------------------------------------------------
#endif
