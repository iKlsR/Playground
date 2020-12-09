#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <QMatrix4x4>
#include <QVector3D>

class Transform {
public:
    Transform();
    Transform& translate(const QVector3D&);
    Transform& rotate(const float&, const QVector3D&);
    Transform& scale(const QVector3D&);
    Transform& scale(const float&);
    const QMatrix4x4 &transform() const;

private:
    QMatrix4x4 transformation;
};

#endif // TRANSFORM_HPP
