#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>
#include <QFileInfo>

#include "TexturePicker.hpp"
#include "Button.hpp"
#include "Label.hpp"

TexturePicker::TexturePicker(const QString &caption, QWidget *parent) : QWidget(parent)
{
    // It's pretty much expected that accessible names are styled in the stylesheet
    _clear_button = new Button("c");
    _clear_button->setObjectName("clear_button");
    _clear_button->setAccessibleName("texture_button");
    _clear_button->setFixedSize(QSize(36, 36));

    _rotate_cw_button = new Button("r");
    _rotate_cw_button->setObjectName("rotate_button");
    _rotate_cw_button->setAccessibleName("texture_button");
    _rotate_cw_button->setFixedSize(QSize(36, 36));

    _flip_button = new Button("f");
    _flip_button->setObjectName("flip_button");
    _flip_button->setAccessibleName("texture_button");
    _flip_button->setFixedSize(QSize(36, 36));

    _label = new Label(caption, this);

    setObjectName(QStringLiteral("TexturePicker"));

    _preview_pane = new QLabel;
    _preview_pane->setObjectName("TexturePane");
    _preview_pane->setScaledContents(true);
    _preview_pane->setFixedSize(QSize(108, 108));
    _preview_pane->setPixmap(QPixmap(QApplication::applicationDirPath() + "/icons/checker.png"));

    // Setup our texture picker buttons
    _button_container = new QWidget;
    _button_container_layout = new QVBoxLayout;
    _button_container_layout->setMargin(0);
    _button_container_layout->setSpacing(0);

    _button_container_layout->addWidget(_clear_button);
    _button_container_layout->addWidget(_rotate_cw_button);
    _button_container_layout->addWidget(_flip_button);
    _button_container->setLayout(_button_container_layout);

    _layout = new QHBoxLayout;
    _layout->setMargin(0);
    _layout->setSpacing(0);
    _layout->addWidget(_label);
    _layout->addStretch();
    _layout->addWidget(_preview_pane);
    _layout->addWidget(_button_container);

    // dnd
    setAcceptDrops(true);

    setLayout(_layout);
}

bool TexturePicker::setPreview(const QString &path)
{
    _preview_pane->setPixmap(QPixmap(path));
    return true;
}

void TexturePicker::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void TexturePicker::dropEvent(QDropEvent *event)
{
    QList<QUrl> droppedUrls = event->mimeData()->urls();
    QStringList list;
    for (auto url : droppedUrls) {
        auto fileInfo = QFileInfo(url.toLocalFile());
        list << fileInfo.absoluteFilePath();
    }

    setPreview(list.first());
    emit swapTexture(list.first());

    event->acceptProposedAction();
}

TexturePicker::~TexturePicker()
{
}
