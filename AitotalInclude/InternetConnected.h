
#include <wininet.h>
typedef BOOL(WINAPI * INETCHECKPROC)(LPDWORD lpdwFlags, DWORD dwReserved);

bool GetStatusConnect()
{
    // ���������� 1, ���� ���� ��������� � ���������, � ��������� ������ -0
	bool bRetVal;
	HINSTANCE hLib;// = NULL;
	INETCHECKPROC pfnInternetGetConnectedState;

	// ������ (� ������ ���������) ���������� wininet.dll
	hLib = LoadLibraryA("wininet.dll");
	if (!hLib) // ���� �� ���������� ��������� ����������, ��
			return false; // ���������� 0

	// �������� ����� ������� InternetGetConnectedState
	pfnInternetGetConnectedState = (INETCHECKPROC)GetProcAddress(hLib,
		"InternetGetConnectedState");

	// ���� �� ������� �������� ����� �������
	if (!pfnInternetGetConnectedState)
		return false; // ���������� 0

	DWORD lpdwFlags;

	// ��������� ����������
	bRetVal = pfnInternetGetConnectedState(&lpdwFlags, 0) != 0;

	FreeLibrary(hLib); // ��������� ����������
	return bRetVal; // ���������� ������ �����������
}
