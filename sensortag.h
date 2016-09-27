#ifndef SENSORTAG_H
#define SENSORTAG_H

#include <QMainWindow>

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
};

#endif // SENSORTAG_H
