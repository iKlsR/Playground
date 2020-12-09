#include "MainWindow.hpp"
#include "OpenGLWidget.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    viewportSurface = new OpenGLWidget;

    setCentralWidget(QWidget::createWindowContainer(viewportSurface, this));
    resize(QSize(1920, 1080));
}

MainWindow::~MainWindow()
{

}
