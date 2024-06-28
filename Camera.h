//
// Created by remo on 6/19/24.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct GLFWwindow;

class Camera {
protected:
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 rotation = glm::vec3(0, 0, 0);

    glm::vec3 destinationPos;
    glm::vec3 destinationRot;

public:
    Camera();
    ~Camera();

    virtual glm::mat4 getViewMatrix();
    virtual void update(float deltaTime);

    void moveTo(glm::vec3 pos, glm::vec3 rot);
    void animateTo(glm::vec3 pos, glm::vec3 rot);
    float distanceToTarget(glm::vec3 origin, glm::vec3 target);
    float normalizeAngle(float angle);
    glm::vec3 rotationDir(glm::vec3 target);
    glm::vec3 directionToTarget(glm::vec3 target);
    glm::vec3 getPos();

    bool animatingMovement;
    bool animatingRotation;
    float rotationSpeed;
    float animationSpeed;
};

#endif //CAMERA_H
