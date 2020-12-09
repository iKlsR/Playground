#include "shader.hpp"

QSharedPointer<Shader> Shader::load(const QString &vertexPath, const QString &fragPath)
{
    return QSharedPointer<Shader>(new Shader(vertexPath, fragPath));
}

const QOpenGLShaderProgram &Shader::program() const
{
    return shaderProgram;
}

void Shader::use()
{
    shaderProgram.bind();
}

void Shader::stop()
{
    shaderProgram.release();
}

void Shader::setUniform(const char *name, const QMatrix4x4& value)
{
    shaderProgram.setUniformValue(name, value);
}

void Shader::setUniform(const char *name, const QVector3D &value)
{
    shaderProgram.setUniformValue(name, value);
}

void Shader::setUniform(const char *name, float value)
{
    shaderProgram.setUniformValue(name, value);
}

void Shader::setUniform(const char *name, bool value)
{
    shaderProgram.setUniformValue(name, value ? 1 : 0);
}

void Shader::setUniform(const char *name, int value)
{
    shaderProgram.setUniformValue(name, value);
}

Shader::Shader(const QString &vertexPath, const QString &fragPath)
{
    QFile vsFile(vertexPath);
    vsFile.open(QFile::ReadOnly | QFile::Text);
    QString vertexShader(vsFile.readAll());

    QFile fsFile(fragPath);
    fsFile.open(QFile::ReadOnly | QFile::Text);
    QString fragmentShader(fsFile.readAll());

    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex);
    vshader->compileSourceCode(vertexShader);

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment);
    fshader->compileSourceCode(fragmentShader);

    shaderProgram.addShader(vshader);
    shaderProgram.addShader(fshader);

    shaderProgram.link();
    shaderProgram.bind();
}

