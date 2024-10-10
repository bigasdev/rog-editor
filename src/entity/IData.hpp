#pragma once 

class IData{
public:
    ~IData() = default;

    virtual void update() = 0;
    virtual void dispose() = 0;
};
