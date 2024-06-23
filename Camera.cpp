//
// Created by remo on 6/19/24.
//
#include "Camera.h"

Camera::Camera() {
    animatingMovement = false;
    animatingMovement = false;
    rotationSpeed = 0.05f;
    animationSpeed = 13.0f;
}

Camera::~Camera() {
}

glm::mat4 Camera::getViewMatrix() {
    glm::mat4 viewMatrix(1.0f);
    viewMatrix = glm::rotate(viewMatrix, rotation.x, glm::vec3(1, 0, 0));
    viewMatrix = glm::rotate(viewMatrix, rotation.y, glm::vec3(0, 1, 0));
    viewMatrix = glm::rotate(viewMatrix, rotation.z, glm::vec3(0, 0, 1));
    viewMatrix = glm::translate(viewMatrix, position);
    return viewMatrix;
}

void Camera::update(float deltaTime) {
    // Animate movement
    if (animatingMovement) {
        position += directionToTarget(destinationPos) * deltaTime * animationSpeed;
        if (distanceToTarget(position, destinationPos) < 0.2f) {
            position = destinationPos;
            animatingMovement = false;
        }
    }

    // Animate Rotation
    if (animatingRotation) {
        rotation = rotationDir(destinationRot);
        if (rotation == destinationRot) {
            animatingRotation = false;
        }
    }
}

glm::vec3 Camera::getPos() {
    return position;
}


void Camera::moveTo(glm::vec3 pos, glm::vec3 rot) {
    position = pos;
    rotation = glm::vec3(glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z));
}

void Camera::animateTo(glm::vec3 pos, glm::vec3 rot) {
    destinationPos = pos;
    destinationRot = glm::vec3(glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z));
    animatingMovement = true;
    animatingRotation = true;
}

glm::vec3 Camera::directionToTarget(glm::vec3 target) {
    // Calculate difference between position and target
    glm::vec3 diff = glm::vec3(
        target.x - position.x,
        target.y - position.y,
        target.z - position.z
    );

    // Normalize result and return
    return glm::normalize(diff);
}

glm::vec3 Camera::rotationDir(glm::vec3 target) {
    // Calculate the difference and normalize it
    glm::vec3 diff = destinationRot - rotation;
    diff.x = normalizeAngle(diff.x);
    diff.y = normalizeAngle(diff.y);
    diff.z = normalizeAngle(diff.z);

    // Calculate the step to move in this frame
    glm::vec3 stepRot = glm::normalize(diff) * rotationSpeed;

    // Prevent overshooting by clamping the step
    if (glm::length(diff) < rotationSpeed) {
        return destinationRot; // We're close enough to the destination
    }
    return rotation + stepRot;
}

// Normalize angle to the range [-pi, pi]
float Camera::normalizeAngle(float angle) {
    while (angle > glm::pi<float>()) angle -= glm::two_pi<float>();
    while (angle < -glm::pi<float>()) angle += glm::two_pi<float>();
    return angle;
}

float Camera::distanceToTarget(glm::vec3 origin, glm::vec3 target) {
    // Calculate difference between position and target
    glm::vec3 diff = glm::vec3(
        origin.x - target.x,
        origin.y - target.y,
        origin.z - target.z
    );

    // Calculate the magnitude of the difference vector
    float distance = glm::sqrt(
        glm::pow(diff.x, 2.0f) +
        glm::pow(diff.y, 2.0f) +
        glm::pow(diff.z, 2.0f)
    );
    return distance;
}


