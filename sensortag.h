#ifndef SENSORTAG_H
#define SENSORTAG_H

#include <QMainWindow>

#include "scanner.h"

namespace Ui {
class SensorTag;
}

class SensorTag : public QMainWindow
{
    Q_OBJECT

public:
    explicit SensorTag(QWidget *parent = 0);
    ~SensorTag();

private:
    Ui::SensorTag *ui;
    scanner *m_scanwindow;

private slots:
    void openScanWindow();

};

#endif // SENSORTAG_H
