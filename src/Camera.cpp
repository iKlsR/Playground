#include <cmath>
#include "Input.hpp"
#include "Camera.hpp"

Camera::Camera()
{
    position    = QVector3D(0, 0, 7.f);
    front       = QVector3D(0, 0, -1);
    up          = QVector3D(0, 1,  0);

    speed = 7.37f * 3.3;

    QQuaternion::fromEulerAngles(0, 0, 0).getEulerAngles(&pitch, &yaw, &roll);

    updateCameraRot();
    resetMouseButtons();
}

// bool Camera::intersect(const Ray &r, Sphere &s, float t_min, float t_max, hit_record &rec) const {
// }

bool Camera::intersect(const Ray &r, Sphere &s, float t_min, float t_max, hit_record &rec) const {
    QVector3D origin_to_center = r.origin() - s.center;
    float a = QVector3D::dotProduct(r.direction(), r.direction());
    float b = QVector3D::dotProduct(origin_to_center, r.direction());
    float c = QVector3D::dotProduct(origin_to_center, origin_to_center) - s.radius * s.radius;
    float discriminant = b * b - a * c;

    // if we have at least one hit, pass the details to our hit_record
    if (discriminant > 0) {
        // get the point where we intersect the sphere first
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            // solve for t
            rec.intersect_scale = temp;
            // get the precise hit point
            rec.hit_point = r.point_at_parameter(rec.intersect_scale);
            // calculate sphere normal for shading
            rec.normal = (rec.hit_point - s.center) / s.radius;
            return true;
        }

        // get the point where the ray exits the sphere
        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.intersect_scale = temp;
            rec.hit_point = r.point_at_parameter(rec.intersect_scale);
            rec.normal = (rec.hit_point - s.center) / s.radius;
            return true;
        }
    }

    return false;
}

// http://antongerdelan.net/opengl/raycasting.html
QVector3D Camera::calculateMouseRay(const QPointF& pos)
{
    float x = pos.x();
    float y = pos.y();

    // viewport -> NDC
    float mousex = (2.0f * x) / viewport.width - 1.0f;
    float mousey = (2.0f * y) / viewport.height - 1.0f;
    QVector2D NDC = QVector2D(mousex, -mousey);

    // NDC -> HCC
    QVector4D HCC = QVector4D(NDC, -1.0f, 1.0f);

    // HCC -> View Space
    QMatrix4x4 projection_matrix_inverse = projectionMatrix.inverted();
    QVector4D eye_coords = projection_matrix_inverse * HCC;
    QVector4D ray_eye = QVector4D(eye_coords.x(), eye_coords.y(), -1.0f, 0.0f);

    // View Space -> World Space
    QMatrix4x4 view_matrix_inverse = viewMatrix.inverted();
    QVector4D world_coords = view_matrix_inverse * ray_eye;
    QVector3D final_ray_coords = QVector3D(world_coords);

    return final_ray_coords.normalized();
}

void Camera::onMouseMove(int x, int y)
{
    if (rightMouseDown) {
        yaw     += x / 10.f;
        pitch   += y / 10.f;
    }

    if (middleMouseDown) {
        //translate camera
        float dragSpeed = 0.01f;
        auto dir = rotation.rotatedVector(QVector3D(x * dragSpeed, -y * dragSpeed, 0));
        position = position + dir;
        updateCameraMatrices();
    }

    updateCameraRot();
}

void Camera::updateCameraRot()
{
    rotation = QQuaternion::fromEulerAngles(pitch, yaw, 0);
    this->update(0);
}

void Camera::updateCameraTransform()
{
    transform.setToIdentity();
    transform.translate(position);
    transform.rotate(rotation);
}

void Camera::updateCameraMatrices()
{
    updateCameraTransform();

    viewMatrix.setToIdentity();
    auto pos = transform.column(3).toVector3D();
    auto dir = (transform * QVector4D(front, 1)).toVector3D();
    auto _up = (transform * QVector4D(up, 0)).toVector3D();
    viewMatrix.lookAt(pos, dir, _up);

}

void Camera::updateProjectionMatrix(int width, int height)
{
    viewport.width = width;
    viewport.height = height;

    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, width / float(height), .1f, 512.f);
}

void Camera::resetMouseButtons()
{
    leftMouseDown = middleMouseDown = rightMouseDown = false;
}

void Camera::onMouseDown(Qt::MouseButton button)
{
    switch (button) {
        case Qt::LeftButton: {
            leftMouseDown = true;
            break;
        }
        case Qt::MiddleButton:
            middleMouseDown = true;
        break;
        case Qt::RightButton:
            rightMouseDown = true;
        break;
        default: break;
    }
}

void Camera::onMouseUp(Qt::MouseButton button)
{
    switch (button) {
        case Qt::LeftButton:
            leftMouseDown = false;
        break;
        case Qt::MiddleButton:
            middleMouseDown = false;
        break;
        case Qt::RightButton:
            rightMouseDown = false;
        break;
        default: break;
    }
}

void Camera::onMouseWheel(int delta)
{
    auto zoomSpeed = 0.0135f;
    auto forward = rotation.rotatedVector(QVector3D(0, 0, -1));
    auto movement = position + forward * zoomSpeed * delta;
    position = movement;
}

void Camera::update(float dt)
{
    auto linearSpeed = speed * dt;

    const QVector3D upVector(0, 1, 0);
    auto forwardVector = rotation.rotatedVector(QVector3D(0, 0, -1));
    auto x = QVector3D::crossProduct(forwardVector, upVector).normalized();
    auto z = QVector3D::crossProduct(upVector, x).normalized();

    if (Input::keyPressed(Qt::Key_W)) {
        position += linearSpeed * z;
    }

    if (Input::keyPressed(Qt::Key_S)) {
        position -= linearSpeed * z;
    }

    if (Input::keyPressed(Qt::Key_A)) {
        position -= linearSpeed * x;
    }

    if (Input::keyPressed(Qt::Key_D)) {
        position += linearSpeed * x;
    }

    updateCameraMatrices();
}
