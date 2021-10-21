#include "manager.hpp"

using namespace hs;
using namespace hs::behaviour;

Manager::Manager(robot::Robot& robot) : robot(robot)
{
    // Register empty behaviour type
    this->registerType<Behaviour>();
    this->set<Behaviour>();
}

Manager::~Manager()
{
    for (auto& b : this->behaviours)
        delete b.second;
}

void Manager::reset()
{
    this->behaviour->reset();
}

void Manager::update()
{
    this->behaviour->update();
}
