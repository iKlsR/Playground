#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../extern/gl3w/include/GL/gl3w.h"

#include <QOpenGLWidget>
#include <QDebug>
#include <QOpenGLTimerQuery>
#include <QElapsedTimer>

class QDragEnterEvent;
class QDropEvent;

#include "Scene.hpp"

#include "Model.hpp"

class Renderer : public QOpenGLWidget
{
    Q_OBJECT

public:
    Renderer(QWidget *parent = Q_NULLPTR);
    ~Renderer();

    
    void update();

    void initializeGL() override;
    void resizeGL(int, int) override;
    void paintGL() override;

    void focusOutEvent(QFocusEvent* event) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;

    void dragEnterEvent(QDragEnterEvent*) override;
    void dropEvent(QDropEvent*) override;

    QSharedPointer<Scene> _scene;
private:
    QElapsedTimer _elapsedTimer;
};

#endif
