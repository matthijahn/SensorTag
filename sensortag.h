#ifndef SENSORTAG_H
#define SENSORTAG_H

#include <QMainWindow>

#include "scanner.h"


namespace Ui {
class sensortag;
}

class sensortag : public QMainWindow
{
    Q_OBJECT

public:
    explicit sensortag(QWidget *parent = 0);
    ~sensortag();

private:
    Ui::sensortag *ui;
    scanner *m_scanwindow;

private slots:
    void openScanWindow();

};

#endif // SENSORTAG_H
