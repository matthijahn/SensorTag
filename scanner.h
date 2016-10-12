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
    QLowEnergyController *m_control;


private slots:
    void deviceDiscover(void);
    void showDevices(const QBluetoothDeviceInfo&);
    void controlDevice(QListWidgetItem*);
    void scanService(void);
    void status(void);
    void serviceStatus(const QBluetoothUuid ble_id);
};

#endif // SCANNER_H
