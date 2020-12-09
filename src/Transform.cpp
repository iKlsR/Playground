#include "Transform.hpp"

Transform::Transform() {
    transformation = QMatrix4x4();
    transformation.setToIdentity();
}

Transform &Transform::translate(const QVector3D &value) {
    transformation.translate(value);
    return *this;
}

Transform &Transform::rotate(const float &angle, const QVector3D &value) {
    transformation.rotate(angle, value);
    return *this;
}

Transform &Transform::scale(const QVector3D &value) {
    transformation.scale(value);
    return *this;
}

Transform &Transform::scale(const float &value) {
    transformation.scale(value);
    return *this;
}

const QMatrix4x4 &Transform::transform() const {
    return transformation;
}
