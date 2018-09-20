#pragma once

#include "../Container/Str.h"
#include "../Container/Ptr.h"
#include "../Container/Vector.h"

namespace Urho3D
{

    class Texture;
    class ComputeBuffer;
    class ComputeShader;

    class URHO3D_API ComputeDevice
    {
    public:
        ComputeDevice() { }
        virtual ~ComputeDevice() { }

        ComputeDevice(const ComputeDevice&) = delete;
        void operator=(const ComputeDevice&) = delete;

        /// Device must be initialized after construction, will return false if initialization failed.
        virtual bool Initialize() = 0;
        virtual bool IsValid() const = 0;
        virtual void Finish() = 0;
        virtual void Barrier() = 0;

        /// Construct an arbitrary data buffer.
        virtual ComputeBuffer* CreateBuffer(const String& name, unsigned size, unsigned bufferType) = 0;
        /// Construct an Image2D.
        virtual ComputeBuffer* CreateBuffer(const String& name, unsigned width, unsigned height, unsigned bufferType) = 0;
        /// Construct an Image3D.
        virtual ComputeBuffer* CreateBuffer(const String& name, unsigned width, unsigned height, unsigned depth, unsigned bufferType) = 0;
        /// Construct a shader (not prepared or compiled).
        virtual ComputeShader* CreateShader(const String& name) = 0;
        /// Construct a shared view for an existing GPU resource.
        virtual ComputeBuffer* CreateSharedView(const String& name, const SharedPtr<Texture>& forTexture) = 0;
    };

}