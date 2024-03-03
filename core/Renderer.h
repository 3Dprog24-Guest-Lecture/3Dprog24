#pragma once

/**
 * @interface IRender
 * @brief Interface for renderable objects.
 *
 * Defines a common interface for objects that can be rendered.
 * This ensures that all renderable objects implement a Draw method,
 * allowing for consistent rendering across different types of objects.
 */
class IRender
{
public:
    virtual void Draw(const class Shader* shader = nullptr) const = 0;
};

// A render class can be added later if necessary
