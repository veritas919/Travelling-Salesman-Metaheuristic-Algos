// Abstract class

#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <string>

class Algorithm
{

public:
    virtual void load(const std::string &) =0; 
    virtual void execute(int) = 0;
    virtual void stats() = 0;
    virtual void select(const std::string & ) = 0;
    virtual void save(const std::string & ) = 0;

};

#endif