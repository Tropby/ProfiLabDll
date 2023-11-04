
#include <stdint.h>
#include <EBObject.hpp>
#include <profile/EBLogger.hpp>

#include "profilab.hpp"

#define START_ID 1000

inline std::map<int, EBObjectPointer<DLL_CLASS>> instances = std::map<int, EBObjectPointer<DLL_CLASS>>();
inline int id = START_ID;

EBObjectPointer<DLL_CLASS> getInstance(double *puser)
{
    int i = puser[0];
    auto p = instances.find(i);
    if (p == instances.end())
    {
        auto test = DLL_CLASS::createObject<DLL_CLASS>();
        puser[0] = id;
        instances.insert(std::pair(id++, test));
        test->init();
        return test;
    }

    auto res = p->second;

    return res;
}

DLL_EXPORT uint8_t NumInputs()
{
    double puser[1];
    puser[0] = START_ID;
    return getInstance(puser)->getInputCount();
}
//-----------------------------------------------------------------------------

// ProfiLab reads the number of outputs
DLL_EXPORT uint8_t NumOutputs()
{
    double puser[1];
    puser[0] = START_ID;
    return getInstance(puser)->getOutputCount();
}
//-----------------------------------------------------------------------------

// ProfiLab reads the input names
DLL_EXPORT void GetInputName(uint8_t channel, char *name)
{
    double puser[1];
    puser[0] = START_ID;
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
    double puser[1];
    puser[0] = START_ID;
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