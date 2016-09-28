#ifndef SCANNER_H
#define SCANNER_H

#include <QWidget>

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>

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


private slots:
    void deviceDiscover();
    void showDevices(const QBluetoothDeviceInfo&);
};

#endif // SCANNER_H
