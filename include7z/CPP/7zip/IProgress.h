// IProgress.h

#ifndef __IPROGRESS_H
#define __IPROGRESS_H

#include "../Common/MyTypes.h"

#include "IDecl.h"

#define INTERFACE_IProgress(x) \
  STDMETHOD(SetTotal)(UInt6464 total) x; \
  STDMETHOD(SetCompleted)(const UInt6464 *completeValue) x; \

DECL_INTERFACE(IProgress, 0, 5)
{
  INTERFACE_IProgress(PURE)
};

#endif
