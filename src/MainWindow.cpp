#include <QKeyEvent>
#include <QMenuBar>
#include <QSplitter>
#include <QKeySequence>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QAbstractItemView>
#include <QCheckBox>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QBrush>

#include "Input.hpp"
#include "MainWindow.hpp"
#include "Renderer.hpp"
#include "Model.hpp"

#include "Widgets/Button.hpp"
#include "Widgets/Slider.hpp"
#include "Widgets/ColorPicker.hpp"
#include "Widgets/TextInput.hpp"
#include "Widgets/CheckBox.hpp"
#include "Widgets/TexturePicker.hpp"

MainWindow::MainWindow()
{
    _windowSplit = new QSplitter(this);
    _windowSplit->setHandleWidth(1);

    _renderer = new Renderer;
    Q_ASSERT(_renderer);
    _propertyWindow = new QWidget;

    _colorPicker = new ColorPicker;
    _colorPicker->setObjectName(QStringLiteral("ColorPicker"));
    _colorPicker->setAccessibleName("colorpicker");

    // temp
    auto layout = new QVBoxLayout;
    auto dumBut = new QPushButton("OpenGL and Qt are a fucking mess!");
    dumBut->setAccessibleName("default");

    QTreeWidget *treeWidget = new QTreeWidget;
    treeWidget->setColumnCount(2);
    treeWidget->setHeaderHidden(true);
    treeWidget->header()->setMinimumSectionSize(0);
    treeWidget->header()->setStretchLastSection(false);
    treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    treeWidget->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    QTreeWidgetItem *rootItem = new QTreeWidgetItem;
    rootItem->setText(0, "Scene");
    rootItem->setText(1, QString());

    QTreeWidgetItem *treeItem = new QTreeWidgetItem;
    treeItem->setText(0, "Node");
    treeItem->setIcon(1, QIcon(QApplication::applicationDirPath() + "/icons/eye_open.png"));

    QTreeWidgetItem *treeItem2 = new QTreeWidgetItem;
    treeItem2->setText(0, "Node2");
    treeItem2->setIcon(1, QIcon(QApplication::applicationDirPath() + "/icons/eye_closed.png"));

    rootItem->addChild(treeItem);
    rootItem->addChild(treeItem2);
    treeWidget->addTopLevelItem(rootItem);
    treeWidget->expandItem(rootItem);

    // layout->addWidget(treeWidget);

    // auto ll = new QHBoxLayout;
    // ll->setMargin(0);
    // ll->setSpacing(4);
    // auto ww = new QWidget;
    // auto dumButGr = new Button("Make a mess!", ButtonType::PL_SUCCESS);
    // auto dumButRd = new Button("Clean the mess!", ButtonType::PL_ERROR);
    // ll->addWidget(dumButGr);
    // ll->addWidget(dumButRd);
    // layout->addWidget(dumBut);
    // ww->setLayout(ll);
    // layout->addWidget(ww);
    // layout->addWidget(new QLineEdit());
    // auto dumCb = new QComboBox;
    // dumCb->setItemDelegate(new RowHeightDelegate());
    // QSizePolicy p;
    // p.setVerticalPolicy(QSizePolicy::Maximum);
    // p.setHorizontalPolicy(QSizePolicy::Preferred);
    // dumCb->addItem(" Young");
    // dumCb->addItem(" Dumb");
    // dumCb->addItem(" Broke");
    // auto lbl = new QLabel("Boi if u don't-");
    // lbl->setAccessibleName("label_widget");
    // layout->addWidget(lbl);
    // layout->addWidget(dumCb);
    // layout->addWidget(_colorPicker);
    // layout->addWidget();
    // layout->addWidget(new QPushButton("Omae wa mou - shindeiru"));

    // auto dsb = new QWidget;
    // auto dl = new QHBoxLayout;
    // dl->setMargin(0);
    // dl->addWidget(new QDoubleSpinBox);
    // dl->addWidget(new QDoubleSpinBox);
    // dl->addWidget(new QDoubleSpinBox);
    // dsb->setLayout(dl);
    // layout->addWidget(dsb);

    layout->addWidget(new Button("This does nothing", ButtonType::PL_SUCCESS));
    layout->addWidget(new CheckBox("Show Bounding Box"));
    layout->addWidget(new TextInput);

    auto roughSlider = new Slider;
    auto metalSlider = new Slider;

    layout->addWidget(roughSlider);
    layout->addWidget(metalSlider);

    auto texturePickerWidget  = new TexturePicker("Albedo");
    auto texturePickerWidget2 = new TexturePicker("Metallic");
    auto texturePickerWidget3 = new TexturePicker("Roughness");
    auto texturePickerWidget4 = new TexturePicker("Normal");
    auto texturePickerWidget5 = new TexturePicker("AO");

    connect(_renderer->_scene.data(), &Scene::nodeSelected,
            [texturePickerWidget, texturePickerWidget2, texturePickerWidget3, texturePickerWidget4, texturePickerWidget5]
            (Model *selectedModel)
    {
        texturePickerWidget->setPreview(selectedModel->material->textures.value(TextureSlot::ALBEDO));
        texturePickerWidget2->setPreview(selectedModel->material->textures.value(TextureSlot::METALLIC));
        texturePickerWidget3->setPreview(selectedModel->material->textures.value(TextureSlot::ROUGHNESS));
        texturePickerWidget4->setPreview(selectedModel->material->textures.value(TextureSlot::NORMAL));
        texturePickerWidget5->setPreview(selectedModel->material->textures.value(TextureSlot::AO));
    });

    connect(roughSlider, &QSlider::valueChanged, [this](int val) {
        _renderer->_scene->roughFloat = (float) val / 10.f;
    });

    connect(metalSlider, &QSlider::valueChanged, [this](int val) {
        _renderer->_scene->metalFloat = (float) val / 10.f;
    });

    connect(texturePickerWidget, &TexturePicker::swapTexture, [this](const QString &tex) {
        _renderer->_scene->swapTexture(TextureSlot::ALBEDO, tex);
    });

    connect(texturePickerWidget2, &TexturePicker::swapTexture, [this](const QString &tex) {
        _renderer->_scene->swapTexture(TextureSlot::METALLIC, tex);
    });

    connect(texturePickerWidget3, &TexturePicker::swapTexture, [this](const QString &tex) {
        _renderer->_scene->swapTexture(TextureSlot::ROUGHNESS, tex);
    });

    connect(texturePickerWidget4, &TexturePicker::swapTexture, [this](const QString &tex) {
        _renderer->_scene->swapTexture(TextureSlot::NORMAL, tex);
    });

    connect(texturePickerWidget5, &TexturePicker::swapTexture, [this](const QString &tex) {
        _renderer->_scene->swapTexture(TextureSlot::AO, tex);
    });

    // auto desc = new QLabel("floor_albedo.png");
    // desc->setAccessibleName("label_widget");
    layout->addWidget(texturePickerWidget);
    layout->addWidget(texturePickerWidget2);
    layout->addWidget(texturePickerWidget3);
    layout->addWidget(texturePickerWidget4);
    layout->addWidget(texturePickerWidget5);
    layout->addStretch();
    _propertyWindow->setLayout(layout);
    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    _propertyWindow->setSizePolicy(policy);
    _windowSplit->addWidget(_propertyWindow);
    _windowSplit->addWidget(_renderer);
    _windowSplit->setStretchFactor(0, 1);
    _windowSplit->setStretchFactor(1, 2);

    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
    installEventFilter(this);
    createMenus();
    setCentralWidget(_windowSplit);
}

MainWindow::~MainWindow()
{
    // delete _renderer;
}

// bool MainWindow::eventFilter(QObject *object, QEvent *event)
// {
//     return QMainWindow::eventFilter(object, event);
// }

void MainWindow::createMenus()
{
    _menuBar = new QMenuBar;
    QMenu *fileMenu = new QMenu("File");
    QMenu *addMenu = new QMenu("Add");
    QMenu *helpMenu = new QMenu("Help");
    QAction *exit = new QAction("Exit the Sandbox");
    exit->setShortcut(QKeySequence("Ctrl+Q"));
    fileMenu->addAction(exit);

    _menuBar->addMenu(fileMenu);
    _menuBar->addMenu(addMenu);
    _menuBar->addMenu(helpMenu);

    setMenuBar(_menuBar);
    menuBar()->hide();

    connect(exit, &QAction::triggered, [this]() { qApp->exit(); });
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    Input::keyStates[event->key()] = true;

    // Immediately close the app, might repurpose this later
    // if (event->key() == Qt::Key_Escape) qApp->exit();

    // // Toggle the menubar visibility
    // if (event->key() == Qt::Key_Alt) {
    //     menuBar()->setHidden(!(menuBar()->isHidden()));
    // }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    Input::keyStates[event->key()] = false;
}

// void MainWindow::mousePressEvent(QMouseEvent *event)
// {
//     _renderer->mousePressEvent(event);
// }

// void MainWindow::mouseReleaseEvent(QMouseEvent *event)
// {
//     _renderer->mouseReleaseEvent(event);
// }

// void MainWindow::mouseMoveEvent(QMouseEvent *event)
// {
//     _renderer->mouseMoveEvent(event);
// }

// void MainWindow::mouseWheelEvent(QWheelEvent *event)
// {
//     _renderer->mouseWheelEvent(event);
// }
