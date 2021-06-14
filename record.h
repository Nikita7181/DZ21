#pragma once
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "utils.h"

class record 
{
    int id;
    std::string name;
    std::string chair;
    int semester;
public:
    int getid() const
    {return id;}
    void setid(int id_n)
    {id = id_n;}
    std::string getname() const
    {return name;}
    void setname(std::string name_n)
    {name = name_n;}
    std::string getchair() const
    {return chair;}
    void setchair(std::string n_chair)
    {chair = n_chair;}
    int getsemester() const
    {return semester;}
    void setsemestr(int n_semestr)
    {semester = n_semestr;}
    void setId (int id)
    {
        this->id = id;
    }
    virtual void print() const = 0;
    virtual void add_discipline () = 0;
    virtual void edit_discipline () = 0;
    virtual void del(int indx) = 0;
    virtual std::string serialize () = 0;
    virtual void deserialize (std::string str) =0;
    virtual bool isNegative () = 0;
};