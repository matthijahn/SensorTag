#include "sensortag.h"
#include "ui_sensortag.h"


sensortag::sensortag(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sensortag)
{
    ui->setupUi(this);
    connect(ui->btn_1,SIGNAL(clicked()),this,SLOT(openScanWindow()));
}

sensortag::~sensortag()
{
    delete ui;
}

void sensortag::openScanWindow()
{
   m_scanwindow = new scanner();
   m_scanwindow->show();
}

