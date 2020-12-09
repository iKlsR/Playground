#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>

#include <QWindow>
#include <QElapsedTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>

#include <mutex>
#include <condition_variable>

class OpenGLWidget;

#include "Playground.hpp"

class Renderer : public QObject
{
    Q_OBJECT

public:
    explicit Renderer(OpenGLWidget *window,
                      std::condition_variable *job_ready,
                      std::mutex *mtx,
                      Swapper *swpr);
    ~Renderer();

    bool createContext();
    void start();
    void run();
//    void render(RenderJob *job);

private:
    QOpenGLContext *threadContext;
    OpenGLWidget *surfaceWindow;

    std::condition_variable *job_ready;
    std::mutex *mtx;
    Swapper *swapper;
    QHash<uint32_t, GLuint> vaoStore;
    QOpenGLShaderProgram *program;
};

#endif // RENDERER_HPP
