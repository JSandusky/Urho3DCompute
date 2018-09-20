#include "../../Precompiled.h"

#include "../../Compute/OpenCL/CLErrorCodes.h"
#include "../../Compute/OpenCL/OpenCLComputeBuffer.h"
#include "../../Compute/OpenCL/OpenCLComputeDevice.h"
#include "../../Compute/OpenCL/OpenCLComputeShader.h"
#include "../../Compute/OpenCL/OpenCLComputeKernel.h"

#include "../../IO/Log.h"

namespace Urho3D
{

OpenCLComputeShader::OpenCLComputeShader(const String& name, OpenCLComputeDevice* device) :
    ComputeShader(name, device)
{
    isCompiled_ = false;
}

OpenCLComputeShader::~OpenCLComputeShader()
{
    if (program_)
        delete program_;
}

bool OpenCLComputeShader::CompileShader(const Vector<String>& sources, const String& defines)
{
    isCompiled_ = false;

    // Destroy anything we've already got
    if (program_)
        delete program_;
    program_ = 0x0;

    // Attempt to create the shader and kernel
    cl_int errCode = CL_SUCCESS;

    OpenCLComputeDevice* device = (OpenCLComputeDevice*)device_;
    cl::Program::Sources srcs;
    srcs.resize(sources.Size());
    for (unsigned i = 0; i < sources.Size(); ++i)
        srcs[i] = std::make_pair(sources[i].CString(), sources[i].Length());

    program_ = new cl::Program(device->GetContext(), srcs);
    if (!defines.Empty())
    {
        String interimDef = String("-cl-std=CL2.0 ") + defines;
        errCode = program_->build(device->GetDeviceVector(), interimDef.CString());
    }
    else
        errCode = program_->build(device->GetDeviceVector(), "-cl-std=CL2.0");
    if (errCode != CL_SUCCESS)
    {
        URHO3D_LOGERRORF("%s: Failed to generate OpenCL shader program: %s", TranslateOpenCLErrorCode(errCode), name_);
        if (errCode == CL_BUILD_PROGRAM_FAILURE)
        {
            cl_build_status status = program_->getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device->GetDevice());
            if (status == CL_BUILD_ERROR)
            {
                std::string name = cl::Device::getDefault().getInfo<CL_DEVICE_NAME>();
                std::string buildlog = program_->getBuildInfo<CL_PROGRAM_BUILD_LOG>(device->GetDevice());
                URHO3D_LOGERRORF("Build log: %s\n%s", name, buildlog);
            }
        }
        delete program_;
        program_ = 0x0;
        return false;
    }

    isCompiled_ = true;
    return true;
}

bool OpenCLComputeShader::CompileShader(const String& source, const String& defines)
{
    Vector<String> srcs;
    srcs.Push(source);
    return CompileShader(srcs, defines);
}

ComputeKernel* OpenCLComputeShader::GetKernel(const String& name)
{
    if (!IsCompiled())
    {
        URHO3D_LOGERRORF("Attempting to get kernel for uncompiled compute shader: %s", name);
        return 0x0;
    }

    ComputeKernel* kernel = new OpenCLComputeKernel(name, this, (OpenCLComputeDevice*)device_);
    if (kernel->IsExecutable())
        return kernel;
    delete kernel;
    URHO3D_LOGERRORF("Kernel for compute shader: %s is not executable", name);
    return 0x0;
}

}