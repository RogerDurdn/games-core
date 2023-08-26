#pragma once

#include <iostream>

class Action {
    std::string name, type;

private:
    std::string getName() const;

    std::string getType() const;
};