//
// Created by kamil on 13.05.2024.
//

#ifndef PROJEKT1_SPRITERENDERER_H
#define PROJEKT1_SPRITERENDERER_H


#include "Shader.h"
#include "glm/vec3.hpp"
#include "Texture2D.h"

class SpriteRenderer {
public:
    explicit SpriteRenderer(Shader &shader);

    ~SpriteRenderer();

    void DrawSprite(Texture2D &texture, glm::vec2 position,
                    glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
                    glm::vec3 color = glm::vec3(1.0f));

private:
    Shader shader;
    unsigned int quadVAO;

    void initRenderData();
};


#endif //PROJEKT1_SPRITERENDERER_H
