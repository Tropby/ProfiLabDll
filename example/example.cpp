#define DLL_CLASS Example

#include <http/EBHttpClient.hpp>

#include <profilab.hpp>

class DLL_CLASS : public Profilab
{
public:
    Example()
    {
    }

    virtual void init()
    {
        addInput("IN1");
        addOutput("OUT1");
    };

    virtual void start()
    {

    };

    virtual void update()
    {
        getOutput(0)->setValue(getInput(0)->getValue());
    };

    virtual void stop()
    {

    };

    virtual void configure()
    {

    };

    EB_SLOT(httpClientFinished)
    {

    }

private:
    
};

#include <profilabDll.hpp>