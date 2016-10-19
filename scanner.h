#ifndef SCANNER_H
#define SCANNER_H

#include <QWidget>
#include <QMap>
#include <QListWidgetItem>


#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QBluetoothUuid>

#include <QDebug>

#define ACC_UUID "{f000aa10-0451-4000-b000-000000000000}"
#define GYRO_UUID "{f000aa50-0451-4000-b000-000000000000}"

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
    QMap<QString, QBluetoothUuid> m_service_map;
    QLowEnergyService *m_service;


private slots:
    void deviceDiscover(void);
    void showDevices(const QBluetoothDeviceInfo&);
    void controlDevice(QListWidgetItem*);
    void scanService(void);
    void status(void);
    void serviceStatus(const QBluetoothUuid ble_id);
    void readService(void);
    void readCharacteristic(void);
};

#endif // SCANNER_H
