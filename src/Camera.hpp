#ifndef PL_CAMERA_HPP
#define PL_CAMERA_HPP

#include <QMatrix4x4>
#include <QKeyEvent>

struct Viewport
{
    int width;
    int height;
    float dpi;
};

class Ray
{
public:
    Ray() = default;
    Ray(const QVector3D &start, const QVector3D &direction) : S(start), V(direction) {}

    QVector3D origin() const { return S; }
    QVector3D direction() const { return V; }
    QVector3D point_at_parameter(float t) const { return S + t * V; }

    QVector3D S;
    QVector3D V;
};

class Sphere
{
public:
    Sphere() = default;
    Sphere(QVector3D C, float R) : center(C), radius(R) {}

    QVector3D center;
    float radius;
};

struct hit_record {
    float intersect_scale;
    QVector3D hit_point;
    QVector3D normal;
};

class Camera
{
public:
    Camera();

    virtual void onMouseDown(Qt::MouseButton button);
    virtual void onMouseUp(Qt::MouseButton button);
    virtual void onMouseMove(int x, int y);
    virtual void onMouseWheel(int val);

    virtual void update(float dt);
    QVector3D calculateMouseRay(const QPointF& pos);
    bool intersect(const Ray &r, Sphere &s, float t_min, float t_max, hit_record &rec) const;

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

    QMatrix4x4 getProjectionMatrix() {
        return projectionMatrix;
    }

    QMatrix4x4 transform;

    void updateCameraRot();
    void updateCameraTransform();
    void updateCameraMatrices();
    void updateProjectionMatrix(int, int);

    void resetMouseButtons();

private:
    float fov;
    float aspectRatio;
    float nearClip;
    float farClip;

    float speed;

    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;
    Viewport viewport;
};

#endif // PL_CAMERA_HPP
