#pragma once 

enum class CollisionType
{
    STATIC,
    DYNAMIC,
    KINETIC // Moves but is not affected by physics (characters controlled by player input) wont use this for now
};

enum class CollisionResponse
{
    BLOCK,
    OVERLAP,
    //KINETIC, Usually used for characters
    IGNORE
};

struct CollisionProperties
{
    CollisionType mType{ CollisionType::STATIC };
    CollisionResponse mResponse{ CollisionResponse::BLOCK };

    // Type checks
    bool IsStatic() const { return mType == CollisionType::STATIC; }
    bool IsDynamic() const { return mType == CollisionType::DYNAMIC; }
    bool IsKinetic() const { return mType == CollisionType::KINETIC; }

    // Response checks
    bool IsBlockResponse() const { return mResponse == CollisionResponse::BLOCK; }
    bool IsOverlapResponse() const { return mResponse == CollisionResponse::OVERLAP; }
    bool IsIgnoreResponse() const { return mResponse == CollisionResponse::IGNORE; }
};
