#include "../../Compute/OpenCL/CLErrorCodes.h"
#include "../../Compute/ComputeBuffer.h"
#include "../../IO/Log.h"
#include "../../Compute/OpenCL/OpenCLComputeDevice.h"
#include "../../Compute/OpenCL/OpenCLComputeKernel.h"
#include "../../Compute/OpenCL/OpenCLComputeShader.h"

#include <CL/cl.hpp>

namespace Urho3D
{

    OpenCLComputeKernel::OpenCLComputeKernel(const String& name, OpenCLComputeShader* shader, OpenCLComputeDevice* device) :
        ComputeKernel(name, device)
    {
        int errCode = 0;
        kernel_ = new cl::Kernel(*(shader->program_), name_.CString(), &errCode);
        if (errCode != CL_SUCCESS)
        {
            URHO3D_LOGERRORF("%s: Failed to construct OpenCL kernel: %s", TranslateOpenCLErrorCode(errCode), name_);
            delete kernel_;
            kernel_ = 0x0;
        }
    }

    OpenCLComputeKernel::~OpenCLComputeKernel()
    {
        if (kernel_)
            delete kernel_;
    }

    void OpenCLComputeKernel::Bind(ComputeBuffer* buffer, unsigned index)
    {
        buffer->Bind(this, index);
    }

    void OpenCLComputeKernel::Execute(unsigned x, unsigned y, unsigned z)
    {
        if (kernel_)
        {
            OpenCLComputeDevice* device = (OpenCLComputeDevice*)device_;
            cl_int errCode = device->GetQueue().enqueueNDRangeKernel(*kernel_, cl::NDRange(), cl::NDRange(x, y, z));
            if (errCode != CL_SUCCESS)
                URHO3D_LOGERRORF("%s: Failed to queue/execute OpenCL kernel: %s", TranslateOpenCLErrorCode(errCode), name_);
        }
        else
            URHO3D_LOGERRORF("Attempted to execute uncompiled OpenCL shader: %s", name_);
    }

    void OpenCLComputeKernel::SetArg(unsigned index, void* value, unsigned sz)
    {
        if (kernel_)
            kernel_->setArg(index, sz, value);
    }
}