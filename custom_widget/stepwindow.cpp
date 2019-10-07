#include "stepwindow.h"
#include "ui_stepwindow.h"

StepWindow::StepWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StepWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setTabText(0, "Step 1");
    ui->tabWidget->setTabText(1, "Step 2");
    ui->tabWidget->setTabText(2, "Step 3");
}

StepWindow::~StepWindow()
{
    delete ui;
}

void StepWindow::set_step_images_action(const std::vector<QPixmap> &pixmaps)
{
    assert(pixmaps.size() == 3);
    ui->step1Label->setPixmap(pixmaps[0]);
    ui->step2Label->setPixmap(pixmaps[1]);
    ui->step3Label->setPixmap(pixmaps[2]);
}
