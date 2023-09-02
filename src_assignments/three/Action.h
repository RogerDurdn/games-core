#pragma once

#include <iostream>

class Action {
    std::string m_name, m_type;

public:
    Action();

    Action(const std::string name, const std::string type);

    const std::string &name() const;

    const std::string &type() const;
};