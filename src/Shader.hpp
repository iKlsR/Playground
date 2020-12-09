#ifndef SHADER_HPP
#define SHADER_HPP

#include "../extern/gl3w/include/GL/gl3w.h"
#include <QOpenGLShaderProgram>
#include <QFile>

class Shader
{
public:
    static QSharedPointer<Shader> load(const QString&, const QString&);
    const QOpenGLShaderProgram &program() const;

    void use();
    void stop();

    void setUniform(const char *name, const QMatrix4x4& value);
    void setUniform(const char *name, const QVector3D& value);
    void setUniform(const char *name, float value);
    void setUniform(const char *name, bool value);
    void setUniform(const char *name, int value);

private:
    Shader(const QString&, const QString&);
    QOpenGLShaderProgram shaderProgram;
};

#endif // SHADER_HPP
