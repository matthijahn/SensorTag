#include "sensortag.h"
#include "ui_sensortag.h"

sensortag::sensortag(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sensortag)
{
    ui->setupUi(this);
}

sensortag::~sensortag()
{
    delete ui;
}
