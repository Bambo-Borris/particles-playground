#include "App.hpp"

// Some reason under testing using the SFML GPUPreference header didn't work
// so we'll explicitly do this ourselves!
#ifdef _MSC_VER
extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
extern "C" __declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 1;
#endif 

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    App app;
    app.run();
}
