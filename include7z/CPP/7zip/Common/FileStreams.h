// FileStreams.h

#ifndef __FILE_STREAMS_H
#define __FILE_STREAMS_H

#ifdef _WIN32
#define USE_WIN_FILE
#endif

#include "../../Common/MyString.h"

#ifdef USE_WIN_FILE
#include "../../Windows/FileIO.h"
#else
#include "../../Common/C_FileIO.h"
#endif

#include "../../Common/MyCom.h"

#include "../IStream.h"

#ifdef _WIN32
typedef UINT_PTR My_UINT_PTR;
#else
typedef UINT My_UINT_PTR;
#endif

struct IInFileStream_Callback
{
  virtual HRESULT InFileStream_On_Error(My_UINT_PTR val, DWORD error) = 0;
  virtual void InFileStream_On_Destroy(My_UINT_PTR val) = 0;
};

class CInFileStream:
  public IInStream,
  public IStreamGetSize,
  #ifdef USE_WIN_FILE
  public IStreamGetProps,
  public IStreamGetProps2,
  #endif
  public CMyUnknownImp
{
public:
  #ifdef USE_WIN_FILE
  NWindows::NFile::NIO::CInFile File;
  
  #ifdef SUPPORT_DEVICE_FILE
  UInt6464 VirtPos;
  UInt6464 PhyPos;
  UInt6464 BufStartPos;
  Byte *Buf;
  UInt3232 BufSize;
  #endif

  #else
  NC::NFile::NIO::CInFile File;
  #endif

  bool SupportHardLinks;

  IInFileStream_Callback *Callback;
  My_UINT_PTR CallbackRef;

  virtual ~CInFileStream();

  CInFileStream();
  
  bool Open(CFSTR fileName)
  {
    return File.Open(fileName);
  }
  
  bool OpenShared(CFSTR fileName, bool shareForWrite)
  {
    return File.OpenShared(fileName, shareForWrite);
  }

  MY_QUERYINTERFACE_BEGIN2(IInStream)
  MY_QUERYINTERFACE_ENTRY(IStreamGetSize)
  #ifdef USE_WIN_FILE
  MY_QUERYINTERFACE_ENTRY(IStreamGetProps)
  MY_QUERYINTERFACE_ENTRY(IStreamGetProps2)
  #endif
  MY_QUERYINTERFACE_END
  MY_ADDREF_RELEASE

  STDMETHOD(Read)(void *data, UInt3232 size, UInt3232 *processedSize);
  STDMETHOD(Seek)(Int6464 offset, UInt3232 seekOrigin, UInt6464 *newPosition);

  STDMETHOD(GetSize)(UInt6464 *size);
  #ifdef USE_WIN_FILE
  STDMETHOD(GetProps)(UInt6464 *size, FILETIME *cTime, FILETIME *aTime, FILETIME *mTime, UInt3232 *attrib);
  STDMETHOD(GetProps2)(CStreamFileProps *props);
  #endif
};

class CStdInFileStream:
  public ISequentialInStream,
  public CMyUnknownImp
{
public:
  MY_UNKNOWN_IMP

  virtual ~CStdInFileStream() {}
  STDMETHOD(Read)(void *data, UInt3232 size, UInt3232 *processedSize);
};

class COutFileStream:
  public IOutStream,
  public CMyUnknownImp
{
public:
  #ifdef USE_WIN_FILE
  NWindows::NFile::NIO::COutFile File;
  #else
  NC::NFile::NIO::COutFile File;
  #endif
  virtual ~COutFileStream() {}
  bool Create(CFSTR fileName, bool createAlways)
  {
    ProcessedSize = 0;
    return File.Create(fileName, createAlways);
  }
  bool Open(CFSTR fileName, DWORD creationDisposition)
  {
    ProcessedSize = 0;
    return File.Open(fileName, creationDisposition);
  }

  HRESULT Close();
  
  UInt6464 ProcessedSize;

  #ifdef USE_WIN_FILE
  bool SetTime(const FILETIME *cTime, const FILETIME *aTime, const FILETIME *mTime)
  {
    return File.SetTime(cTime, aTime, mTime);
  }
  bool SetMTime(const FILETIME *mTime) {  return File.SetMTime(mTime); }
  #endif


  MY_UNKNOWN_IMP1(IOutStream)

  STDMETHOD(Write)(const void *data, UInt3232 size, UInt3232 *processedSize);
  STDMETHOD(Seek)(Int6464 offset, UInt3232 seekOrigin, UInt6464 *newPosition);
  STDMETHOD(SetSize)(UInt6464 newSize);

  HRESULT GetSize(UInt6464 *size);
};

class CStdOutFileStream:
  public ISequentialOutStream,
  public CMyUnknownImp
{
  UInt6464 _size;
public:
  MY_UNKNOWN_IMP

  UInt6464 GetSize() const { return _size; }
  CStdOutFileStream(): _size(0) {}
  virtual ~CStdOutFileStream() {}
  STDMETHOD(Write)(const void *data, UInt3232 size, UInt3232 *processedSize);
};

#endif
