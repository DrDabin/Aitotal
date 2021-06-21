//---------------------------------------------------------------------------

#ifndef ScanThreadH
#define ScanThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "AitotalInclude/ScanVTIndy/ScanVTIndy1.h"
//---------------------------------------------------------------------------
class ThreadScan : public TThread
{
private:
protected:
	void __fastcall Execute();
public:
	__fastcall ThreadScan(bool CreateSuspended);
	ScanVTIndy *VTIndy[100];

};
//---------------------------------------------------------------------------
#endif
