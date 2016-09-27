#include "sensortag.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sensortag w;
    w.show();

    return a.exec();
}
