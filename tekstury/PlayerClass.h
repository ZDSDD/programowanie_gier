#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Player {
public:
    glm::vec3 position;
    glm::vec3 color;
    float speed;
    float jump_speed;
    bool jump_active;

    glm::vec3 boundingBoxMin;
    glm::vec3 boundingBoxMax;

    Player() {
        jump_active = false;
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        color = glm::vec3(0.0f, 0.0f, 1.0f);
        speed = 0.5f;
        jump_speed = 2.4f;
        UpdateBoundingBox();
    }

    void FarAway() {
        if (glm::abs(position.x) > 17.0f || glm::abs(position.z) > 45.0f) color = glm::vec3(1.0f, 0.0f, 0.0f);
        else {
            color = glm::vec3(0.0f, 0.0f, 1.0f);
        }
    }

    void UpdateBoundingBox() {
        // Update bounding box position based on player's position
        boundingBoxMin = position - glm::vec3(0.4f, 0.4f, 0.4f); // Assuming player size is 1x1x1
        boundingBoxMax = position + glm::vec3(0.4f, 0.4f, 0.4f);
    }

    void Move(glm::vec3 change) {
        position = position + speed * change;
        UpdateBoundingBox();
    }

    bool CheckCollision(const glm::vec3 &otherObjectMin, const glm::vec3 &otherObjectMax) {
        // Check if player's bounding box intersects with other object's bounding box
        return (boundingBoxMin.x <= otherObjectMax.x && boundingBoxMax.x >= otherObjectMin.x) &&
               (boundingBoxMin.y <= otherObjectMax.y && boundingBoxMax.y >= otherObjectMin.y) &&
               (boundingBoxMin.z <= otherObjectMax.z && boundingBoxMax.z >= otherObjectMin.z);
    }

    void CheckJump(float t, float g, float yPos) {
        if (jump_active == true) {
            position.y += jump_speed * t + 0.5f * g * t * t;
            UpdateBoundingBox();
            if (position.y < yPos) {
                jump_active = false;
                position.y = yPos;
            }
        }
    }
};