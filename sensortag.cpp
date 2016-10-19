#include "sensortag.h"
#include "ui_sensortag.h"

SensorTag::SensorTag(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SensorTag)
{
    ui->setupUi(this);
    connect(ui->btn_1,SIGNAL(clicked()),this,SLOT(openScanWindow()));
}

SensorTag::~SensorTag()
{
    delete ui;
}

void SensorTag::openScanWindow()
{
   m_scanwindow = new scanner();
   m_scanwindow->show();
}
