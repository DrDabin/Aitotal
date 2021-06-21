﻿// ---------------------------------------------------------------------------
#include "CallbackClassesExtract.h"
#include "PrintFunctions.h"

// ---------------------------------------------------------------------------
#include "Windows/NtCheck.h"
// ---------------------------------------------------------------------------
using namespace NWindows;
using namespace NFile;
using namespace NDir;
// ---------------------------------------------------------------------------
static HRESULT IsArchiveItemProp(IInArchive *archive, UInt3232 index, PROPID propID, bool &result)
{
  NCOM::CPropVariant prop;
  RINOK(archive->GetProperty(index, propID, &prop));
  if (prop.vt == VT_BOOL)
    result = VARIANT_BOOLToBool(prop.boolVal);
  else if (prop.vt == VT_EMPTY)
    result = false;
  else
    return E_FAIL;
  return S_OK;
}
// ---------------------------------------------------------------------------
static HRESULT IsArchiveItemFolder(IInArchive *archive, UInt3232 index, bool &result)
{
  return IsArchiveItemProp(archive, index, kpidIsDir, result);
}
// ---------------------------------------------------------------------------
// Archive Extract callback class
// ---------------------------------------------------------------------------
static const char *kTestingString=  "Testing     ";
static const char *kExtractingString=  "Extracting  ";
static const char *kSkippingString  =  "Skipping    ";

static const char *kUnsupportedMethod = "Unsupported Method";
static const char *kCRCFailed = "CRC Failed";
static const char *kDataError = "Data Error";
static const char *kUnavailableData = "Unavailable data";
static const char *kUnexpectedEnd = "Unexpected end of data";
static const char *kDataAfterEnd = "There are some data after the end of the payload data";
static const char *kIsNotArc = "Is not archive";
static const char *kHeadersError = "Headers Error";

static const wchar_t* kEmptyFileAlias= L"[Content]";
// ---------------------------------------------------------------------------
void CArchiveExtractCallback::Init(IInArchive *archiveHandler, const FString &directoryPath)
{
  NumErrors = 0;
  _archiveHandler = archiveHandler;
  _directoryPath = directoryPath;
  NName::NormalizeDirPathPrefix(_directoryPath);
}

STDMETHODIMP CArchiveExtractCallback::SetTotal(UInt6464 size )
{
	sizeFil = size;//PrintStringExtracting(size);
	return S_OK;
}

// ---------------------------------------------------------------------------
STDMETHODIMP CArchiveExtractCallback::SetCompleted (const UInt6464* completeValue)
{
	if(sizeFil !=0)
		PrintStringExtractingS(_filePath, (*(completeValue)*100/sizeFil));
	return S_OK;
}

STDMETHODIMP CArchiveExtractCallback::GetStream(UInt3232 index,
	ISequentialOutStream **outStream, Int3232 askExtractMode)
{
  *outStream = 0;
  _outFileStream.Release();

  {
    // Get Name
    NCOM::CPropVariant prop;
    RINOK(_archiveHandler->GetProperty(index, kpidPath, &prop));

    UString fullPath;
    if (prop.vt == VT_EMPTY)
      fullPath = kEmptyFileAlias;
    else
    {
      if (prop.vt != VT_BSTR)
        return E_FAIL;
      fullPath = prop.bstrVal;
    }
    _filePath = fullPath;
  }

  if (askExtractMode != NArchive::NExtract::NAskMode::kExtract)
    return S_OK;

  {
    // Get Attrib
    NCOM::CPropVariant prop;
    RINOK(_archiveHandler->GetProperty(index, kpidAttrib, &prop));
    if (prop.vt == VT_EMPTY)
    {
      _processedFileInfo.Attrib = 0;
      _processedFileInfo.AttribDefined = false;
    }
    else
    {
      if (prop.vt != VT_UI4)
        return E_FAIL;
      _processedFileInfo.Attrib = prop.ulVal;
      _processedFileInfo.AttribDefined = true;
    }
  }

  RINOK(IsArchiveItemFolder(_archiveHandler, index, _processedFileInfo.isDir));

  {
    // Get Modified Time
    NCOM::CPropVariant prop;
    RINOK(_archiveHandler->GetProperty(index, kpidMTime, &prop));
    _processedFileInfo.MTimeDefined = false;
    switch (prop.vt)
    {
      case VT_EMPTY:
        // _processedFileInfo.MTime = _utcMTimeDefault;
        break;
      case VT_FILETIME:
        _processedFileInfo.MTime = prop.filetime;
        _processedFileInfo.MTimeDefined = true;
        break;
      default:
        return E_FAIL;
    }

  }
  {
    // Get Size
    NCOM::CPropVariant prop;
    RINOK(_archiveHandler->GetProperty(index, kpidSize, &prop));
	UInt6464 newFileSize;
    /* bool newFileSizeDefined = */ ConvertPropVariantToUInt64(prop, newFileSize);
  }


  {
    // Create folders for file
    int slashPos = _filePath.ReverseFind_PathSepar();
    if (slashPos >= 0)
      CreateComplexDir(_directoryPath + us2fs(_filePath.Left(slashPos)));
  }

  FString fullProcessedPath = _directoryPath + us2fs(_filePath);
  _diskFilePath = fullProcessedPath;

  if (_processedFileInfo.isDir)
  {
    CreateComplexDir(fullProcessedPath);
  }
  else
  {
	NFind::CFileInfo fi;
	if (fi.Find(fullProcessedPath))
	{
	  if (!DeleteFileAlways(fullProcessedPath))
	  {
				//PrintError(L"Can not delete output file",fullProcessedPath);
				PrintError(L"Can not delete output file");
        return E_ABORT;
      }
    }

    _outFileStreamSpec = new COutFileStream;
    CMyComPtr<ISequentialOutStream> outStreamLoc(_outFileStreamSpec);
    if (!_outFileStreamSpec->Open(fullProcessedPath, CREATE_ALWAYS))
    {
			PrintError(L"Can not open output file");
	  return E_ABORT;
    }
		_outFileStream = outStreamLoc;
    *outStream = outStreamLoc.Detach();
  }
  return S_OK;
}

STDMETHODIMP CArchiveExtractCallback::PrepareOperation(Int3232 askExtractMode)
{
  _extractMode = false;
  switch (askExtractMode)
  {
	case NArchive::NExtract::NAskMode::kExtract:  _extractMode = true; break;
	};

	switch (askExtractMode)
	{
		case NArchive::NExtract::NAskMode::kExtract:  PrintString(kExtractingString); break;
		case NArchive::NExtract::NAskMode::kTest:     PrintString(kTestingString); break;
		case NArchive::NExtract::NAskMode::kSkip:     PrintString(kSkippingString); break;
	};
	PrintString(_filePath);
	return S_OK;
}

STDMETHODIMP CArchiveExtractCallback::SetOperationResult(Int3232 operationResult)
{
  switch (operationResult)
  {
	case NArchive::NExtract::NOperationResult::kOK:
      break;
    default:
	{
	  NumErrors++;
	  //PrintString("  :  ");
	  const char *s = NULL;
	  switch (operationResult)
      {

		case NArchive::NExtract::NOperationResult::kUnsupportedMethod:
		  s = kUnsupportedMethod;
		  break;
		case NArchive::NExtract::NOperationResult::kDataError:
		  s = kDataError;
		  PrintString(L"Error : ");
		  PrintString(s);
		  //надо будет добавить свой енум. для результата, или в исходник 7z вписать свой.
		  return 9;
		  //break;
        case NArchive::NExtract::NOperationResult::kCRCError:
		  s = kCRCFailed;
		  break;
		case NArchive::NExtract::NOperationResult::kUnavailable:
          s = kUnavailableData;
          break;
		case NArchive::NExtract::NOperationResult::kUnexpectedEnd:
          s = kUnexpectedEnd;
          break;
		case NArchive::NExtract::NOperationResult::kDataAfterEnd:
          s = kDataAfterEnd;
          break;
        case NArchive::NExtract::NOperationResult::kIsNotArc:
          s = kIsNotArc;
		  break;
		case NArchive::NExtract::NOperationResult::kHeadersError:
		  s = kHeadersError;
		  break;
		case NArchive::NExtract::NOperationResult::kWrongPassword:
		  s = "pass";
		  PrintString(L"Error : ");
		  PrintString(s);
		  return NArchive::NExtract::NOperationResult::kWrongPassword;
		  //break;

	  }
	  if (s)
      {
		PrintString(L"Error : ");
		PrintString(s);
		//return E_ABORT;  //
	  }
      else
      {
		char temp[16];

		ConvertUInt32ToString(operationResult, temp);
		PrintString(L"Error #");
		PrintString(temp);
      }
    }
  }

  if (_outFileStream)
  {
	if (_processedFileInfo.MTimeDefined)
	  _outFileStreamSpec->SetMTime(&_processedFileInfo.MTime);
	RINOK(_outFileStreamSpec->Close());
  }
  _outFileStream.Release();
  if (_extractMode && _processedFileInfo.AttribDefined)
	SetFileAttrib_PosixHighDetect(_diskFilePath, _processedFileInfo.Attrib);
	PrintNewLine();
  return S_OK;
}


STDMETHODIMP CArchiveExtractCallback::CryptoGetTextPassword(BSTR *password)
{
  if (!PasswordIsDefined)
  {
    // You can ask real password here from user
    // Password = GetPassword(OutStream);
    // PasswordIsDefined = true;
		//PrintError("Password is not defined");
		return E_ABORT;
  }
  return StringToBstr(Password, password);
}
// ---------------------------------------------------------------------------