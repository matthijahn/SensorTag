#ifndef SCANNER_H
#define SCANNER_H

#include <QWidget>
#include <QMap>
#include <QList>
#include <QListWidgetItem>


#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
//#include <QLowEnergyController>
//#include <QLowEnergyService>
//#include <QBluetoothUuid>
//#include <QLowEnergyCharacteristic>

#include <QDebug>
typedef struct
{
    QString Address;
    int Acc_SRate;
    int Acc_Res;
    int Gyro_SRate;
} settings_t;



namespace Ui {
class scanner;
}

class scanner : public QWidget
{
    Q_OBJECT

public:
    explicit scanner(QWidget *parent = 0);
    ~scanner();
    settings_t p_DeviceSettings;

signals:
    void ConfigChanged();
    void ConfigStays();

private:
    Ui::scanner *ui;
    QBluetoothDeviceDiscoveryAgent *m_DiscoveryAgent;



private slots:
    void DeviceDiscover(void);
    void DeviceDiscoverFinished(void);
    void DeviceDiscovered(QBluetoothDeviceInfo DeviceInfo);
    void SetSettings();
    void HoldConfig();



};

#endif // SCANNER_H
