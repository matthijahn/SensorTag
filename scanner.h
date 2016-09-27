#ifndef SCANNER_H
#define SCANNER_H

#include <QWidget>

namespace Ui {
class scanner;
}

class scanner : public QWidget
{
    Q_OBJECT

public:
    explicit scanner(QWidget *parent = 0);
    ~scanner();

private:
    Ui::scanner *ui;
};

#endif // SCANNER_H
