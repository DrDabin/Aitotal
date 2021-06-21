﻿// ---------------------------------------------------------------------------
#ifndef CallbackClassesOpenH
#define CallbackClassesOpenH
// ---------------------------------------------------------------------------
#include "Common/IntToString.h"
#include "Common/StringConvert.h"

#include "Windows/DLL.h"
#include "Windows/FileDir.h"
#include "Windows/FileFind.h"
#include "Windows/FileName.h"

#include "Windows/PropVariant.h"
#include "Windows/PropVariantConv.h"

#include "Common/MyInitGuid.h"
#include "../Common/FileStreams.h"
#include "../Archive/IArchive.h"

#include "../IPassword.h"
//#include "../MyVersion.h"
// ---------------------------------------------------------------------------
//using namespace NWindows;
// ---------------------------------------------------------------------------
// Archive Open callback class

class CArchiveOpenCallback:
  public IArchiveOpenCallback,
  public ICryptoGetTextPassword,
  public CMyUnknownImp
{
public:
  MY_UNKNOWN_IMP1(ICryptoGetTextPassword)

  STDMETHOD(SetTotal)(const UInt6464 *files, const UInt6464 *bytes);
  STDMETHOD(SetCompleted)(const UInt6464 *files, const UInt6464 *bytes);

  STDMETHOD(CryptoGetTextPassword)(BSTR *password);

  bool PasswordIsDefined;
  UString Password;

  CArchiveOpenCallback() : PasswordIsDefined(false) {}

};
// ---------------------------------------------------------------------------
#endif
