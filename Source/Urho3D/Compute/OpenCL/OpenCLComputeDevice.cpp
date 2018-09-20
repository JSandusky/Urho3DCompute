#include "../../Compute/OpenCL/CLErrorCodes.h"
#include "../../IO/Log.h"
#include "../../Compute/OpenCL/OpenCLComputeBuffer.h"
#include "../../Compute/OpenCL/OpenCLComputeDevice.h"
#include "../../Compute/OpenCL/OpenCLComputeShader.h"


#include <CL/cl.hpp>
#include <CL/cl.h>

namespace Urho3D
{

OpenCLComputeDevice::OpenCLComputeDevice()
{

}
    
OpenCLComputeDevice::~OpenCLComputeDevice()
{
    if (commandQueue_)
        delete commandQueue_;
    if (context_)
        delete context_;
    commandQueue_ = 0x0;
    context_ = 0x0;
}

bool OpenCLComputeDevice::Initialize()
{
    URHO3D_LOGINFO("Initializing OpenCL");
    cl_int errCode = CL_SUCCESS;

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::Platform plat;
    std::string chosenPlat;
    for (auto &p : platforms)
    {
        std::string platver = p.getInfo<CL_PLATFORM_VERSION>();
        URHO3D_LOGDEBUG("Available OpenCL platform: %s", platver);
    }

    for (auto &p : platforms) {
        std::string platver = p.getInfo<CL_PLATFORM_VERSION>();
        if (platver.find("OpenCL 2.") != std::string::npos) {
            plat = p;
            chosenPlat = platver;
            URHO3D_LOGDEBUG("Selecting %s for OpenCL platform", platver);
        }
    }

    if (plat() == 0 && platforms.size() > 0)
    {
        plat = platforms.front();
        std::string platver = plat.getInfo<CL_PLATFORM_VERSION>();
        URHO3D_LOGDEBUG("Falling back to %s for OpenCL platform", platver);
    }
        
    if (plat() == 0)  {
        URHO3D_LOGERROR("No OpenCL platform found.");
        return false;
    }

    std::vector<cl::Device> devices;
    plat.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    if (devices.empty())
    {
        plat.getDevices(CL_DEVICE_TYPE_CPU, &devices);
        URHO3D_LOGWARNINGF("No GPU devices, falling back to CPU mode: %s", chosenPlat);
    }
    if (devices.empty())
    {
        URHO3D_LOGERRORF("Could not acquire an OpenCL device for: %s", chosenPlat);
        return false;
    }
    //cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)plat(), 0 };

    context_ = new cl::Context(devices.front());

    devices_ = context_->getInfo<CL_CONTEXT_DEVICES>();
    if (devices_.size() == 0)
    {
        URHO3D_LOGERROR("Unable to acquire any OpenCL devices");
        return false;
    }
        
    device_ = devices_.front();
    commandQueue_ = new cl::CommandQueue(*context_, devices_[0], 0, &errCode);
    if (errCode != CL_SUCCESS)
    {
        URHO3D_LOGERRORF("%s: Unable to construct OpenCL command queue", TranslateOpenCLErrorCode(errCode));
        return false;
    }

    std::string platformName = plat.getInfo<CL_PLATFORM_NAME>();
    std::string platformVer = plat.getInfo<CL_PLATFORM_VERSION>();
    std::string deviceName = device_.getInfo<CL_DEVICE_NAME>();
    //SPRUE_LOG_INFO(FString("%3 initialized: %1, %2", platformName, deviceName, platformVer).str());

    return true;
}

void OpenCLComputeDevice::Finish()
{
    if (commandQueue_->finish() != CL_SUCCESS)
        URHO3D_LOGERROR("Failed to 'finish' OpenCL command queue");
}

void OpenCLComputeDevice::Barrier()
{
    if (::clEnqueueBarrier((*commandQueue_)()) != CL_SUCCESS)
        URHO3D_LOGERROR("Failed to encqueue OpenCL barrier");
}

ComputeBuffer* OpenCLComputeDevice::CreateBuffer(const String& name, unsigned size, unsigned bufferType)
{
    return new OpenCLComputeBuffer(name, this, size, bufferType);
}

ComputeBuffer* OpenCLComputeDevice::CreateBuffer(const String& name, unsigned width, unsigned height, unsigned bufferType)
{
    return new OpenCLComputeBuffer(name, this, width, height, bufferType);
}

ComputeBuffer* OpenCLComputeDevice::CreateBuffer(const String& name, unsigned width, unsigned height, unsigned depth, unsigned bufferType)
{
    return new OpenCLComputeBuffer(name, this, width, height, depth, bufferType);
}

ComputeShader* OpenCLComputeDevice::CreateShader(const String& name)
{
    return new OpenCLComputeShader(name, this);
}

}