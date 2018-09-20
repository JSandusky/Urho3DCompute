#pragma once

#include "../Core/Variant.h"

namespace Urho3D
{
    class ComputeDevice;
    class ComputeBuffer;

    class URHO3D_API ComputeKernel
    {
    public:
        ComputeKernel(const String& name, ComputeDevice* device) :
            name_(name), device_(device)
        {

        }

        virtual ~ComputeKernel() { }

        ComputeKernel(const ComputeKernel&) = delete;
        void operator=(const ComputeKernel&) = delete;

        virtual void Bind(ComputeBuffer* buffer, unsigned index) = 0;
        virtual void Execute(unsigned x, unsigned y, unsigned z) = 0;
        virtual void SetArg(unsigned index, void* value, unsigned sz) = 0;
        virtual bool IsExecutable() const = 0;

        template<typename T>
        void SetArg(unsigned index, const T& value) { SetArg(index, (void*)&value, sizeof(T)); }

        const String& GetName() const { return name_; }

    protected:
        ComputeDevice* device_;
        String name_;
    };

}