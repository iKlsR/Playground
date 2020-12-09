#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSplitter;
class QKeyEvent;
class QMenuBar;
class Renderer;

#include "Scene.hpp"

class ColorPicker;

#include <QItemDelegate>

class RowHeightDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
        return QSize(1, 36); // the row height is now 40
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    void createMenus();

    // bool eventFilter(QObject*, QEvent*) override;

    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;

    // void mousePressEvent(QMouseEvent*);
    // void mouseReleaseEvent(QMouseEvent*);
    // void mouseMoveEvent(QMouseEvent*);
    // void mouseWheelEvent(QWheelEvent*);

private:
    Renderer    *_renderer;
    QWidget     *_propertyWindow;
    QSplitter   *_windowSplit;
    QMenuBar    *_menuBar;

    ColorPicker *_colorPicker;
};

#endif // MAINWINDOW_H
