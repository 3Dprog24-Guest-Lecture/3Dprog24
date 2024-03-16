#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>

struct Vertex 
{
    glm::vec3 mPosition;
    glm::vec3 mNormal;
    glm::vec2 mTexCoords;

    Vertex() {};
    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords)
        : mPosition(position), mNormal(normal), mTexCoords(texCoords) {}
    
    static void SetupAttributes() 
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mPosition));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mTexCoords));
        glEnableVertexAttribArray(2);
    }
};

