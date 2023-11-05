#include <windows.h>

#include <stdint.h>
#include <EBObject.hpp>
#include <profile/EBLogger.hpp>

#include "profilab.hpp"

#define START_ID 99999

std::map<int, EBObjectPointer<DLL_CLASS>> instances;
int lastInstanceId = -1;

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
    return 1;
}


EBObjectPointer<DLL_CLASS> getInstance(double *puser)
{
    int i = puser[100];

    EBCpp::EBString instanceId = EBCpp::EBUtils::intToStr(i);
    lastInstanceId = i;

    auto p = instances.find(i);
    if (p == instances.end())
    {
        auto dllInstance = DLL_CLASS::createObject<DLL_CLASS>();
        instances.insert(std::pair(i, dllInstance));
        dllInstance->setPUser(puser);
        dllInstance->initFromDll();
        return dllInstance;
    }

    auto res = p->second;
    return res;
}

//-----------------------------------------------------------------------------

// ProfiLab reads the number of outputs
DLL_EXPORT uint8_t CNumOutputsEx(double *puser)
{
    getInstance(puser)->setPUser(puser);
    return getInstance(puser)->getOutputCount();
}

// ProfiLab reads the number of inputs
DLL_EXPORT uint8_t CNumInputsEx(double *puser)
{
    getInstance(puser)->setPUser(puser);
    return getInstance(puser)->getInputCount();
}

//-----------------------------------------------------------------------------

// ProfiLab reads the input names
DLL_EXPORT void GetInputName(uint8_t channel, char *name)
{
    double puser[101];
    puser[100] = lastInstanceId;    
    auto port = getInstance(puser)->getInput(channel);
    if (port != nullptr)
    {
        strcpy(name, port->getPortName().dataPtr());
    }
    else
    {
        strcpy(name, "UNDEF_IN");
    }
}

//-----------------------------------------------------------------------------

// ProfiLab reads the output names
DLL_EXPORT void GetOutputName(uint8_t channel, char *name)
{
    double puser[101];
    puser[100] = lastInstanceId;
    auto port = getInstance(puser)->getOutput(channel);
    if (port != nullptr)
    {
        strcpy(name, port->getPortName().dataPtr());
    }
    else
    {
        strcpy(name, "UNDEF_OUT");
    }
}
//-----------------------------------------------------------------------------

// ProfiLab simulation start call
DLL_EXPORT void CSimStart(double *pin, double *pout, double *puser)
{
    getInstance(puser)->startFromDll(pin, pout, puser);
}
//-----------------------------------------------------------------------------

// ProfiLab call for every simulation step
DLL_EXPORT void CCalculate(double *pin, double *pout, double *puser)
{
    getInstance(puser)->updateFromDll(pin, pout, puser);
}
//-----------------------------------------------------------------------------

// ProfiLab simulation end call
DLL_EXPORT void CSimStop(double *pin, double *pout, double *puser)
{
    getInstance(puser)->stopFromDll(pin, pout, puser);
}
//-----------------------------------------------------------------------------

// ProfiLab configuration call
DLL_EXPORT void CConfigure(double *puser)
{
    getInstance(puser)->configureFromDll(puser);    
}
//-----------------------------------------------------------------------------