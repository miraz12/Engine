#pragma once 
#include "core/app.h"

namespace Example
{
    class Application;
}
class UserInterface
{
public:
    /*TODO: This is not very nice.. Make a super class that example app can derive from instead
    **TODO: or even better, create some kind of ui settings file.
    */
    UserInterface::UserInterface(Example::Application* app);
    void Run();

private:
    Example::Application* app;
};