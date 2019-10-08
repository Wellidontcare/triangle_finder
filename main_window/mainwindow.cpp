#include "mainwindow.h"
#include "ui_mainwindow.h"

constexpr int MAX_CANNY_VAL = 255;
constexpr int MIN_CANNY_VAL = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,triangle_finder_(this)
    ,step_window_(new StepWindow(this))
{
    ui->setupUi(this);
    setAcceptDrops(true);

    //set range for slider and spinbox
    ui->adjustCannyUpperHorizontalSlider->setRange(MIN_CANNY_VAL, MAX_CANNY_VAL);
    ui->adjustCannyLowerHorizontalSlider->setRange(MIN_CANNY_VAL, MAX_CANNY_VAL);
    ui->adjustCannyUpperSpinbox->setRange(MIN_CANNY_VAL, MAX_CANNY_VAL);
    ui->adjustCannyLowerSpinbox->setRange(MIN_CANNY_VAL, MAX_CANNY_VAL);
    setWindowTitle("Triangle Finder v0.1");

    //************************DRAG AND DROP**********************************//
    //image data drop -> load image
    connect(ui->imageGraphicsView,
            &DropEnabledGraphicsView::successfull_drop_image_data_event,
            &triangle_finder_,
            &TriangleFinderAdapter::drag_and_drop_image_data_action);

    //image file drop -> load image
    connect(ui->imageGraphicsView,
            &DropEnabledGraphicsView::successfull_drop_image_file_event,
            &triangle_finder_,
            &TriangleFinderAdapter::drag_and_drop_image_file_action);


    //update viewer when image changed
    connect(&triangle_finder_,
            &TriangleFinderAdapter::scene_changed,
            ui->imageGraphicsView,
            &DropEnabledGraphicsView::update_scene_action);

    //***********************CANNY EDGE DETECTOR PREVIEW*********************//
    //show preview for canny upper threshold
    connect(ui->adjustCannyUpperHorizontalSlider,
            &QSlider::valueChanged, &triangle_finder_,
            &TriangleFinderAdapter::canny_upper_threshold_action);

    //show preview for canny lower threshold
    connect(ui->adjustCannyLowerHorizontalSlider,
            &QSlider::valueChanged, &triangle_finder_,
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
            &QPushButton::pressed, &triangle_finder_,
            &TriangleFinderAdapter::show_original);

    //***********************METHOD SELECTOR RADIO BUTTON*********************//
    connect(ui->method1RadioButton,
            &QRadioButton::pressed,
            &triangle_finder_,
            &TriangleFinderAdapter::method1_checked);

    connect(ui->method2RadioButton,
            &QRadioButton::pressed,
            &triangle_finder_,
            &TriangleFinderAdapter::method2_checked);

    //***************************FIND TRIANGLES BUTTON************************//
    connect(ui->findTrianglesPushButton,
            &QPushButton::pressed,
            [this](){
                    triangle_finder_.find_triangles(ui->showStepsCheckBox->isChecked());
                     }
    );

        connect(&triangle_finder_, &TriangleFinderAdapter::steps_are_ready, step_window_, &StepWindow::update_steps);



}

MainWindow::~MainWindow()
{
    delete ui;
}

