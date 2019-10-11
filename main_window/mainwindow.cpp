#include "mainwindow.h"
#include "ui_mainwindow.h"

constexpr int MAX_CANNY_VAL = 255;
constexpr int MIN_CANNY_VAL = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    triangle_finder_(this),
    step_window_(new StepWindow(this)),
    side_by_side_viewer_(new SideBySideViewer(this))

{
    ui->setupUi(this);

    //enable drops
    setAcceptDrops(true);

    //set range for slider and spinbox
    ui->adjustCannyUpperHorizontalSlider->setRange(MIN_CANNY_VAL, MAX_CANNY_VAL);
    ui->adjustCannyLowerHorizontalSlider->setRange(MIN_CANNY_VAL, MAX_CANNY_VAL);
    ui->adjustCannyUpperSpinbox->setRange(MIN_CANNY_VAL, MAX_CANNY_VAL);
    ui->adjustCannyLowerSpinbox->setRange(MIN_CANNY_VAL, MAX_CANNY_VAL);

    setWindowTitle("Triangle Finder v0.1");

    //************************DRAG AND DROP**********************************//
    //image file drop -> load image
    connect(ui->imageGraphicsView,
            &DropEnabledGraphicsView::successfull_drop_image_file_event,
            &triangle_finder_,
            &TriangleFinderAdapter::on_drag_and_drop);


    //update viewer when image changed
    connect(&triangle_finder_,
            &TriangleFinderAdapter::scene_changed,
            ui->imageGraphicsView,
            &DropEnabledGraphicsView::on_scene_changed);

    //***********************CANNY EDGE DETECTOR PREVIEW*********************//
    //show preview for canny upper threshold
    connect(ui->adjustCannyUpperHorizontalSlider,
            &QSlider::valueChanged, &triangle_finder_,
            &TriangleFinderAdapter::on_canny_u_slider_moved);

    //show preview for canny lower threshold
    connect(ui->adjustCannyLowerHorizontalSlider,
            &QSlider::valueChanged, &triangle_finder_,
            &TriangleFinderAdapter::on_canny_l_slider_moved);

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
            &QPushButton::clicked, &triangle_finder_,
            &TriangleFinderAdapter::on_reset_view_button_clicked);

    //***********************METHOD SELECTOR RADIO BUTTON*********************//
    connect(ui->method1RadioButton,
            &QRadioButton::clicked,
            &triangle_finder_,
            &TriangleFinderAdapter::on_method1_checked);

    connect(ui->method2RadioButton,
            &QRadioButton::clicked,
            &triangle_finder_,
            &TriangleFinderAdapter::on_method2_checked);

    //***************************FIND TRIANGLES BUTTON************************//
    connect(ui->findTrianglesPushButton,
            &QPushButton::clicked,
            [this](){
        triangle_finder_.on_find_triangles_button_clicked(ui->showStepsCheckBox->isChecked());
    }
    );

    //***************************SHOW STEPS***********************************//
    connect(&triangle_finder_,
            &TriangleFinderAdapter::steps_are_ready,
            step_window_,
            &StepWindow::update_steps);

    /*****************************COMPARE METHODS*******************************/
    connect(ui->compareMethodsPushButton,
            &QPushButton::clicked,
            &triangle_finder_,
            &TriangleFinderAdapter::on_compare_methods_button_clicked);

     connect(&triangle_finder_,
             &TriangleFinderAdapter::compare_ready,
             side_by_side_viewer_,
             &SideBySideViewer::on_compare_ready);
     /**************************SAMPLE PICKER*********************************/
     connect(ui->samplePickerComboBox,
             static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
             &triangle_finder_,
             &TriangleFinderAdapter::load_sample);

}

MainWindow::~MainWindow()
{
    delete ui;
}

