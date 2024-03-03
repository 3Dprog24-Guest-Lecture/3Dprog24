#pragma once

class IRender
{
public:
    virtual void Draw(const class Shader* shader = nullptr) const = 0;
};

// A render class can be added later if necessary
