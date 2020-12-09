#include <QDebug>
#include <QApplication>
#include <QTimer>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include "Renderer.hpp"
#include "Scene.hpp"

// float clamp(float value, float lower, float upper) {
//     return std::min(std::max(value, lower), upper);
// }

Renderer::Renderer(QWidget *parent) : QOpenGLWidget(parent)
{
    // similar to GLFW's window hints, we select a profile and set
    // properties on the default surface that opengl will draw to
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setVersion(3, 3);
    format.setSamples(4);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapInterval(0);
    setFormat(format);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000.f / 60.f);
   
    setAcceptDrops(true);

    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

Renderer::~Renderer()
{
}

void Renderer::update()
{
    // Schedule a redraw
    QOpenGLWidget::update();
}

void Renderer::initializeGL()
{
    if (gl3wInit()) {
        qDebug() << "Failed to initialize OpenGL.\n";
        return;
    }

    if (!gl3wIsSupported(3, 3)) {
        qDebug() << "OpenGL 3.3 not supported\n";
        return;
    }

#ifdef QT_DEBUG
    printf(
        "OpenGL %s\nGLSL %s\nVENDOR %s\nRENDERER %s\n",
        glGetString(GL_VERSION),
        glGetString(GL_SHADING_LANGUAGE_VERSION),
        glGetString(GL_VENDOR),
        glGetString(GL_RENDERER)
    );
#endif

    _scene = QSharedPointer<Scene>(new Scene());
    _scene->init(this->width() * devicePixelRatio(), this->height() * devicePixelRatio());
}

void Renderer::paintGL()
{
    float deltaTime = _elapsedTimer.nsecsElapsed() / (1000.f * 1000.f * 1000.f);
    _elapsedTimer.restart();
    _scene->render(deltaTime, this->width() * devicePixelRatio(), this->height() * devicePixelRatio());   // temp, use struct
}

void Renderer::resizeGL(int w, int h)
{
    _scene->resize(w * devicePixelRatio(), h * devicePixelRatio());
}

void Renderer::focusOutEvent(QFocusEvent* event)
{
    _scene->focusOutEvent(event);
}

void Renderer::mousePressEvent(QMouseEvent *event)
{
    _scene->mousePressEvent(event);
}

void Renderer::mouseReleaseEvent(QMouseEvent *event)
{
    _scene->mouseReleaseEvent(event);
}

void Renderer::mouseMoveEvent(QMouseEvent *event)
{
    _scene->mouseMoveEvent(event);
}

void Renderer::wheelEvent(QWheelEvent *event)
{
    _scene->wheelEvent(event);
}

void Renderer::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void Renderer::dropEvent(QDropEvent *event)
{
    QList<QUrl> droppedUrls = event->mimeData()->urls();
    QStringList list;
    for (auto url : droppedUrls) {
        auto fileInfo = QFileInfo(url.toLocalFile());
        list << fileInfo.absoluteFilePath();
    }

    QApplication::processEvents();
    // what the fuck????
    this->makeCurrent();
    _scene->addModel(list.first());

    event->acceptProposedAction();
}