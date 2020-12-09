#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QDesktopWidget>

#include "GlobalFilter.hpp"
#include "MainWindow.hpp"

// Hints that a dedicated GPU should be used whenever possible
// https://stackoverflow.com/a/39047129/991834
#ifdef Q_OS_WIN
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

int main(int argc, char *argv[])
{

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication app(argc, argv);
    GlobalFilter filter;
    QApplication::instance()->installEventFilter(&filter);
    QString workingDir = QApplication::applicationDirPath();
    app.setWindowIcon(QIcon(workingDir + "/icons/pg.ico"));

    QDesktopWidget desktop;
    QRect windowSize(desktop.screenGeometry(desktop.primaryScreen()));
    windowSize.adjust(200, 200, -200, -200);

// Let's have a nicer font on Windows and *Nix
#if defined(Q_OS_WIN) || defined(Q_OS_UNIX)
    int id = QFontDatabase::addApplicationFont(workingDir + "/DroidSans.ttf");
    if (id != -1) {
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont monospace(family);
        monospace.setStyleStrategy(QFont::PreferAntialias);
        QApplication::setFont(monospace);
    }
#endif

    QFile qss(workingDir + "/Playground.qss");
    qss.open(QFile::ReadOnly | QFile::Text);
    QString qssStyleSheet(qss.readAll());
    qss.close();
    app.setStyleSheet(qssStyleSheet);

    MainWindow mainWindow;
    mainWindow.setWindowTitle("Playground");
    mainWindow.setGeometry(windowSize);
    mainWindow.show();
    // mainWindow.showMaximized();

    return app.exec();
}
