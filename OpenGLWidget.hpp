#ifndef OPENGLWIDGET_HPP
#define OPENGLWIDGET_HPP

#include <queue>

#include <QThread>
#include <QWindow>

#include "Playground.hpp"

class Renderer;
class Updater;

/*
 * This class is an intermediary to get access to a QSurface to draw to
 * It will be cast? and used as a regular QWidget belonging to a parent
 *
 */

class OpenGLWidget : public QWindow
{
    Q_OBJECT

public:
    explicit OpenGLWidget();
    ~OpenGLWidget();

private:
    Renderer *renderer;
    QThread *rendererThread;

    Updater *updater;
    QThread *updateThread;
};

#endif // OPENGLWIDGET_HPP
