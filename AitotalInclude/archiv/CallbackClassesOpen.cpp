﻿// ---------------------------------------------------------------------------
#include "CallbackClassesOpen.h"
#include "PrintFunctions.h"

// ---------------------------------------------------------------------------
#include "Windows/NtCheck.h"

// Archive Open callback class
// ---------------------------------------------------------------------------
STDMETHODIMP CArchiveOpenCallback::SetTotal(const UInt6464 * /* files */, const UInt6464 * /* bytes */)
{
  return S_OK;
}
// ---------------------------------------------------------------------------
STDMETHODIMP CArchiveOpenCallback::SetCompleted(const UInt6464 * /* files */, const UInt6464 * /* bytes */)
{
  return S_OK;
}
// ---------------------------------------------------------------------------
STDMETHODIMP CArchiveOpenCallback::CryptoGetTextPassword(BSTR *password)
{
  if (!PasswordIsDefined)
  {
    // You can ask real password here from user
    // Password = GetPassword(OutStream);
    // PasswordIsDefined = true;
		PrintError(L"Password is not defined");
    return E_ABORT;
	}
  return StringToBstr(Password, password);
}