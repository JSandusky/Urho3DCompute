#pragma once

#include "../../Compute/ComputeShader.h"

namespace cl
{
    class Kernel;
    class Program;
}

namespace Urho3D
{
    class OpenCLComputeDevice;
    class ComputeKernel;

    /// OpenCL compute program. Potentially contains many kernels.
    class URHO3D_API OpenCLComputeShader : public ComputeShader
    {
        friend class OpenCLComputeKernel;

        OpenCLComputeShader(const OpenCLComputeShader&) = delete;
        void operator=(const OpenCLComputeShader&) = delete;
    public:
        /// Construct for the given compute device with an identifying name.
        OpenCLComputeShader(const String& name, OpenCLComputeDevice*);
        /// Destruct and free.
        virtual ~OpenCLComputeShader();

        /// Compiles the shader from the given sources (concatenated) and preprocessor definitions.
        virtual bool CompileShader(const Vector<String>& sources, const String& defines = String()) override;
        /// Compiles the shader from the given source code and preprocessor definitions.
        virtual bool CompileShader(const String& source, const String& defines = String()) override;
        /// Gets a kernel (if it exists) by name from the compiled shader.
        virtual ComputeKernel* GetKernel(const String& name) override;

        /// Returns the underlying OpenCL program object.
        cl::Program* GetProgram() { return program_; }

    private:
        /// OpenCL compute program that is wrapped.
        cl::Program* program_ = 0x0;
    };

}