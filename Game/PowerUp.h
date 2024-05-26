//
// Created by User on 26.05.2024.
//

#ifndef POWERUP_H
#define POWERUP_H
#include "GameObject.h"
const glm::vec2 SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f);

enum PowerUpType
{
    SPEED,
    STICKY,
    PASS_THROUGH,
    PAD_SIZE_INCREASE,
    CONFUSE,
    CHAOS
};

class PowerUp : public GameObject
{
public:
    // powerup state
    PowerUpType Type;
    float       Duration;
    bool        Activated;
    // constructor
    PowerUp(PowerUpType type, glm::vec3 color, float duration,
            glm::vec2 position, Texture2D texture)
        : GameObject(position, SIZE, texture, color, VELOCITY),
          Type(type), Duration(duration), Activated()
    { }
};
#endif //POWERUP_H
