﻿// ---------------------------------------------------------------------------
#ifndef CallbackClassesExtractH
#define CallbackClassesExtractH
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
// Archive Extracting callback class
// ---------------------------------------------------------------------------
class CArchiveExtractCallback:
  public IArchiveExtractCallback,
  public ICryptoGetTextPassword,
  public CMyUnknownImp
{
public:
  MY_UNKNOWN_IMP1(ICryptoGetTextPassword)

  // IProgress
  STDMETHOD(SetTotal)(UInt6464 size);
  STDMETHOD(SetCompleted)(const UInt6464 *completeValue);

  // IArchiveExtractCallback
  STDMETHOD(GetStream)(UInt3232 index, ISequentialOutStream **outStream, Int3232 askExtractMode);
  STDMETHOD(PrepareOperation)(Int3232 askExtractMode);
  STDMETHOD(SetOperationResult)(Int3232 resultEOperationResult);

  // ICryptoGetTextPassword
  STDMETHOD(CryptoGetTextPassword)(BSTR *aPassword);

private:
  CMyComPtr<IInArchive> _archiveHandler;
  FString _directoryPath;  // Output directory
  UString _filePath;       // name inside arcvhive
  FString _diskFilePath;   // full path to file on disk
  bool _extractMode;
  UInt6464 sizeFil;
  struct CProcessedFileInfo
  {
    FILETIME MTime;
	UInt3232 Attrib;
    bool isDir;
    bool AttribDefined;
    bool MTimeDefined;
  } _processedFileInfo;

  COutFileStream *_outFileStreamSpec;
  CMyComPtr<ISequentialOutStream> _outFileStream;

public:
  void Init(IInArchive *archiveHandler, const FString &directoryPath);

  UInt6464 NumErrors;
  bool PasswordIsDefined;
  UString Password;

  CArchiveExtractCallback() : PasswordIsDefined(false) {}

};
// -----------------------------------------------------------------------------
#endif
