#pragma once

// export sign for gcc
#define DLL_EXPORT extern "C" _stdcall

// Value for low state
#define PL_LOW (0.0)

// Value for high state
#define PL_HIGH (5.0)

#include <EBEventLoop.hpp>
#include <map>

using namespace EBCpp;

typedef struct
{
    uint8_t inputs;
    uint8_t outputs;

} ProfilabConfig;

class ProfilabPort : public EBObject<ProfilabPort>
{
public:
    void setPortName(EBString name)
    {
        this->name = name;
    }

    EBString getPortName()
    {
        return name;
    }

    void setValue(double value)
    {
        this->value = value;
    }

    double getValue()
    {
        return value;
    }

private:
    double value;
    EBString name;
};

void pltestfunction(){

}

class Profilab : public EBObject<Profilab>
{
public:
    Profilab() : inited(false)
    {
    }

    EBString dllPath()
    {
        char path[MAX_PATH];
        HMODULE hm = NULL;

        if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                  GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                              (LPCSTR)&pltestfunction, &hm) == 0)
        {
            int ret = GetLastError();
            fprintf(stderr, "GetModuleHandle failed, error = %d\n", ret);
            return "";
        }
        if (GetModuleFileName(hm, path, sizeof(path)) == 0)
        {
            int ret = GetLastError();
            fprintf(stderr, "GetModuleFileName failed, error = %d\n", ret);
            return "";
        }

        return path;
    }

    uint8_t getInputCount()
    {
        return inputs.getSize();
    }

    uint8_t getOutputCount()
    {    
        return outputs.getSize();
    }

    EBObjectPointer<ProfilabPort> getInput(uint8_t channel)
    {
        if (channel < inputs.getSize())
            return inputs.get(channel);
        else
            return nullptr;
    }

    EBObjectPointer<ProfilabPort> getOutput(uint8_t channel)
    {
        if (channel < outputs.getSize())
            return outputs.get(channel);
        else
            return nullptr;
    }

    void addInput(EBString name)
    {
        EBObjectPointer<ProfilabPort> port = createObject<ProfilabPort>();
        port->setPortName(name);
        inputs.append(port);
    }

    void addOutput(EBString name)
    {
        EBObjectPointer<ProfilabPort> port = createObject<ProfilabPort>();
        port->setPortName(name);
        outputs.append(port);
    }

    virtual void init(){};
    virtual void start(){};
    virtual void update(){};
    virtual void stop(){};
    virtual void configure(){};

    void setPUser(double* puser)
    {
        this->puser = puser;
    }

    void updateFromDll(double *pin, double *pout, double *puser)
    {
        this->puser = puser;
        EBEventLoop::getInstance()->processEvents();

        int i = 0;
        for (auto &in : inputs)
        {
            in.get()->setValue(pin[i++]);
        }

        update();

        int o = 0;
        for (auto &out : outputs)
        {
            pout[o++] = out.get()->getValue();
        }
    }

    void configureFromDll(double *puser)
    {
        this->puser = puser;
        configure();
        initFromDll();
    }

    void startFromDll(double *pin, double *pout, double *puser)
    {
        this->puser = puser;
        int i = 0;
        for (auto &in : inputs)
        {
            in.get()->setValue(pin[i++]);
        }

        start();

        int o = 0;
        for (auto &out : outputs)
        {
            pout[o++] = out.get()->getValue();
        }
    }

    void stopFromDll(double *pin, double *pout, double *puser)
    {
        this->puser = puser;
        stop();
    }

    void initFromDll()
    {
        inputs.clear();
        outputs.clear();
        init();
    }

protected:
    void setConfigValue(int index, double value)
    {
        puser[index] = value;
    }

    double getConfigValue(int index)
    {
        return puser[index];
    }

   

private:
    bool inited;

    double *puser;

    EBList<EBObjectPointer<ProfilabPort>> inputs, outputs;
};