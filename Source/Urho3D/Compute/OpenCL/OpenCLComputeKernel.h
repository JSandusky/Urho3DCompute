#pragma once

#include "../../Compute/ComputeKernel.h"

namespace cl
{
    class Kernel;
}

namespace Urho3D
{
    class OpenCLComputeDevice;
    class OpenCLComputeShader;

    /// Kernel object from an OpenCL program.
    class URHO3D_API OpenCLComputeKernel : public ComputeKernel
    {
        OpenCLComputeKernel(const OpenCLComputeKernel&) = delete;
        void operator=(const OpenCLComputeKernel&) = delete;
    public:
        OpenCLComputeKernel(const String& name, OpenCLComputeShader* shader, OpenCLComputeDevice* device);
        virtual ~OpenCLComputeKernel();

        cl::Kernel* GetKernel() { return kernel_; }

        virtual void Bind(ComputeBuffer* buffer, unsigned index) override;
        virtual void Execute(unsigned x, unsigned y, unsigned z) override;
        virtual void SetArg(unsigned index, void* value, unsigned sz) override;
        virtual bool IsExecutable() const override { return kernel_ != 0x0; }

    protected:
        cl::Kernel* kernel_ = 0x0;
    };

}