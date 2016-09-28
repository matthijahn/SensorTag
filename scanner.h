#ifndef SCANNER_H
#define SCANNER_H

#include <QWidget>
#include <QMap>
#include <QListWidgetItem>

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>

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
    QMap<QString, QBluetoothAddress> m_address_map;


private slots:
    void deviceDiscover();
    void showDevices(const QBluetoothDeviceInfo&);
    void controlDevice(QListWidgetItem*);
};

#endif // SCANNER_H
