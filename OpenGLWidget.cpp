#include "OpenGLWidget.hpp"
#include "Renderer.hpp"
#include "Updater.hpp"

#include <QWaitCondition>
#include <QMutex>

OpenGLWidget::OpenGLWidget() : QWindow()
{
    setFlags(Qt::Widget);
    setSurfaceType(QSurface::OpenGLSurface);

    // Don't think this format needs to be in sync with that of the context
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setVersion(3, 3);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    // format.setSwapInterval(0); // run as fast as possible for whatever reason

    setFormat(format);
    create();

    std::condition_variable job_ready;
    std::mutex mtx;

    Swapper *swapper = new Swapper;

    // create our Renderer and move it to a dedicated thread
    updater = new Updater(&job_ready, &mtx, swapper);
    updateThread = new QThread;

    renderer = new Renderer(this, &job_ready, &mtx, swapper);
    rendererThread = new QThread;

    updater->moveToThread(updateThread);
    renderer->moveToThread(rendererThread);

    // Use this to start the renderer in a specific function
    connect(updateThread, &QThread::started, updater, &Updater::start);
    connect(updateThread, &QThread::finished, updateThread, &QObject::deleteLater);

    connect(rendererThread, &QThread::started, renderer, &Renderer::start);
    connect(rendererThread, &QThread::finished, rendererThread, &QObject::deleteLater);

    // this should wait until the update thread (currently this signals it to start work)
    updateThread->start();
    rendererThread->start();

//    rendererThread->wait();
//    updateThread->wait();

    // ============================================================================== //
    //                                  D A N G E R                                     //
    // ============================================================================== //
    // do the management here

    // We will need some swapping mechanism
}

OpenGLWidget::~OpenGLWidget()
{

}
