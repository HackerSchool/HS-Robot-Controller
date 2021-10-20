#include "behaviour.hpp"

using namespace hs::behaviour;

Behaviour::Behaviour(robot::Robot& robot) : robot(robot)
{
    this->reset();
}

void Behaviour::reset()
{
    // Do nothing
}

void Behaviour::update()
{
    // Do nothing
}
