#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , triangle_finder_(new TriangleFinderAdapter(this))
{
    ui->setupUi(this);
    setAcceptDrops(true);

    //set range for slider and spinbox
    ui->adjustCannyUpperHorizontalSlider->setRange(0, 255);
    ui->adjustCannyLowerHorizontalSlider->setRange(0, 255);
    ui->adjustCannyUpperSpinbox->setRange(0, 255);
    ui->adjustCannyLowerSpinbox->setRange(0, 255);

    //************************DRAG AND DROP**********************************//
    //image data drop -> load image
    connect(ui->imageGraphicsView,
            &DropEnabledGraphicsView::successfull_drop_image_data_event,
            triangle_finder_,
            &TriangleFinderAdapter::drag_and_drop_image_data_action);

    //image file drop -> load image
    connect(ui->imageGraphicsView,
            &DropEnabledGraphicsView::successfull_drop_image_file_event,
            triangle_finder_,
            &TriangleFinderAdapter::drag_and_drop_image_file_action);


    //update viewer when image changed
    connect(triangle_finder_,
            &TriangleFinderAdapter::scene_changed,
            ui->imageGraphicsView,
            &DropEnabledGraphicsView::update_scene_action);

    //***********************CANNY EDGE DETECTOR PREVIEW*********************//
    //show preview for canny upper threshold
    connect(ui->adjustCannyUpperHorizontalSlider,
            &QSlider::valueChanged, triangle_finder_,
            &TriangleFinderAdapter::canny_upper_threshold_action);

    //show preview for canny lower threshold
    connect(ui->adjustCannyLowerHorizontalSlider,
            &QSlider::valueChanged, triangle_finder_,
            &TriangleFinderAdapter::canny_lower_threshold_action);

     //***********************SPINBOX AND SLIDER SYNC************************//
    //link spinbox and slider and vice versa
    connect(ui->adjustCannyLowerHorizontalSlider,
            &QSlider::valueChanged,
            ui->adjustCannyLowerSpinbox,
            &QSpinBox::setValue);

    connect(ui->adjustCannyUpperHorizontalSlider,
            &QSlider::valueChanged,
            ui->adjustCannyUpperSpinbox,
            &QSpinBox::setValue);


    connect(ui->adjustCannyUpperSpinbox,
            /*explicit cast is required because value changed function is
             *  overloaded*/
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            ui->adjustCannyUpperHorizontalSlider,
            &QSlider::setValue);

    connect(ui->adjustCannyLowerSpinbox,
            /*explicit cast is required because value changed function is
             *  overloaded*/
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            ui->adjustCannyLowerHorizontalSlider,
            &QSlider::setValue);

    //***************************RESET VIEW**********************************//
    connect(ui->resetViewPushbutton,
            &QPushButton::pressed, triangle_finder_,
            &TriangleFinderAdapter::show_original);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete triangle_finder_;
}

