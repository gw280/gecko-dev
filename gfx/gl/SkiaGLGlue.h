/* -*- Mode: c++; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 40; -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/RefPtr.h"
#include "skia/GrGLInterface.h"
#include "skia/GrContext.h"
#include "GLContext.h"
#include "GLContextSkia.h"

namespace mozilla {
namespace gl {

class SkiaGLGlue : public GenericAtomicRefCounted
{
public:
    SkiaGLGlue(GLContext* context)
        : mGLContext(context)
    {
        SkAutoTUnref<GrGLInterface> i(CreateGrGLInterfaceFromGLContext(mGLContext));
        i->fCallbackData = reinterpret_cast<GrGLInterfaceCallbackData>(this);
        mGrGLInterface = i;
        SkAutoTUnref<GrContext> gr(GrContext::Create(kOpenGL_GrBackend, (GrBackendContext)mGrGLInterface.get()));

        mGrContext = gr;
    }

    GLContext* GetGLContext() const { return mGLContext.get(); }
    GrContext* GetGrContext() const { return mGrContext.get(); }
private:
    /*
     * These members have inter-dependencies, but do not keep each other alive, so
     * destruction order is very important here: mGrContext uses mGrGLInterface, and
     * through it, uses mGLContext, so it is important that they be declared in the
     * present order.
     */
    RefPtr<GLContext> mGLContext;
    SkRefPtr<GrGLInterface> mGrGLInterface;
    SkRefPtr<GrContext> mGrContext;
};

}
}
