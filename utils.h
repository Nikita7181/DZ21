#pragma once
#include <iostream>
#include <string>

template <typename T>
std::string toString (const T val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

struct discipline
{
    std::string name;
    int mark;
    discipline (): name(""), mark(0)  {};
    discipline (std::string name, int mark) : mark(mark), name(name) {};
    void print ()
    {
        std::cout << "\t\tdiscipline: " << name << " - " << mark << std::endl;
    }
};