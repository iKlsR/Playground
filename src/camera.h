#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QKeyEvent>

#include "input.hpp"

class Camera
{
public:
    Camera();

    virtual void onMouseDown(Qt::MouseButton button);
    virtual void onMouseUp(Qt::MouseButton button);
    virtual void onMouseMove(int x, int y);
    virtual void onMouseWheel(int val);

    virtual void update(float dt);

    void resetMouseStates();

    QVector3D position;
    QVector3D front;
    QVector3D up;
    QVector3D target;
    QVector3D direction;

    bool leftMouseDown;
    bool middleMouseDown;
    bool rightMouseDown;

    float lookSpeed;
    float linearSpeed;

    QQuaternion rotation;

    float yaw;
    float pitch;
    float roll;

    QVector3D pivot;
    float distFromPivot;

    QMatrix4x4 getView() {
        return viewMatrix;
    }

    QMatrix4x4 transform;

    void updateCameraRot();
    void updateCameraTransform();
    void updateCameraMatrices();

    void resetMouseButtons();

private:
    float fov;
    float aspectRatio;
    float nearClip;
    float farClip;

    float speed;

    QMatrix4x4 viewMatrix;
    QMatrix4x4 projMatrix;
};

#endif // CAMERA_H
