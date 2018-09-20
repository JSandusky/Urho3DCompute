#pragma once

#include "../Container/Str.h"
#include "../Container/Vector.h"

namespace Urho3D
{
    class ComputeBuffer;
    class ComputeDevice;
    class ComputeKernel;

    class URHO3D_API ComputeShader
    {
    public:
        ComputeShader(const String& name, ComputeDevice* device) : 
            device_(device), name_(name) 
        { 
        }

        virtual ~ComputeShader() { }

        ComputeShader(const ComputeShader&) = delete;
        void operator=(const ComputeShader&) = delete;

        virtual bool CompileShader(const Vector<String>& sources, const String& defines = String()) = 0;
        virtual bool CompileShader(const String& source, const String& defines = String()) = 0;
        virtual ComputeKernel* GetKernel(const String& name) = 0;
        virtual bool IsCompiled() const { return isCompiled_; }

        const String& GetName() const { return name_; }

    protected:
        bool isCompiled_;
        String name_;
        ComputeDevice* device_;
    };

}