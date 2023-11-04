#define DLL_CLASS Example

#include <profilab.hpp>

class DLL_CLASS : public Profilab
{
public:
    Example()
    {
        addInput("IN1");
        addOutput("OUT1");
    }

    virtual void init()
    {

    };

    virtual void start()
    {

    };

    virtual void update()
    {
        
    };

    virtual void stop()
    {

    };

    virtual void configure()
    {

    };

private:

};

#include <profilabDll.hpp>