#include "app.hpp"

using namespace hs;
using namespace hs::app;

App::App(const robot::Robot& robot, behaviour::Manager& manager) : robot(robot), manager(manager)
{
}
