#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , triangle_finder_(new TriangleFinderAdapter(this))
{
    ui->setupUi(this);
    setAcceptDrops(true);
    connect(ui->imageGraphicsView,
            &DropEnabledGraphicsView::successfull_drop_image_data,
            triangle_finder_,
            &TriangleFinderAdapter::drag_and_drop_image_data);

    connect(ui->imageGraphicsView,
            &DropEnabledGraphicsView::successfull_drop_image_file,
            triangle_finder_,
            &TriangleFinderAdapter::drag_and_drop_image_file);

    connect(triangle_finder_,
            &TriangleFinderAdapter::scene_changed,
            ui->imageGraphicsView,
            &QGraphicsView::setScene);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete triangle_finder_;
}

