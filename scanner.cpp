#include "scanner.h"
#include "ui_scanner.h"

scanner::scanner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scanner)
{
    ui->setupUi(this);
}

scanner::~scanner()
{
    delete ui;
}
