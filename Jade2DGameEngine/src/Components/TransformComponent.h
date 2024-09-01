#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

struct TransformComponent {

    glm::vec2 position;
    glm::vec2 scale;
    double rotation;

    TransformComponent( glm::vec2 position = glm::vec2( 0, 0 ), glm::vec2 scale = glm::vec2( 1, 1 ), double rotation = 0.0 ) {

        position.x = position.x * Game::globalRenderScale;
        position.y = position.y * Game::globalRenderScale;
        this->position = position;
        scale.x = scale.x * Game::globalRenderScale;
        scale.y = scale.y * Game::globalRenderScale;
        this->scale = scale;
        this->rotation = rotation;
    }
};

#endif
