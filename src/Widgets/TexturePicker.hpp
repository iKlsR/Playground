#ifndef PL_TEXTUREPICKER_HPP
#define PL_TEXTUREPICKER_HPP

#include <QWidget>

class QLabel;
class QPixmap;
class QVBoxLayout;
class QHBoxLayout;

class Button;
class Label;
class Model;

class QDragEnterEvent;
class QDropEvent;

class TexturePicker : public QWidget
{
    Q_OBJECT

public:
    explicit TexturePicker(const QString &caption = QString(), QWidget *parent = Q_NULLPTR);
    bool setPreview(const QString &path);
    ~TexturePicker();

    void dragEnterEvent(QDragEnterEvent*) override;
    void dropEvent(QDropEvent*) override;

signals:
    void swapTexture(const QString&);
// slots:
//     void updatePreview(Model*);

private:
    // QPixmap *_preview;
    Label   *_label;
    QLabel  *_preview_pane;
    Button  *_clear_button;
    Button  *_rotate_cw_button;
    Button  *_flip_button;

    QWidget *_button_container;
    QVBoxLayout *_button_container_layout;

    QHBoxLayout *_layout;
};

#endif // PL_TEXTUREPICKER_HPP
