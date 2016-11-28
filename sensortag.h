#ifndef SENSORTAG_H
#define SENSORTAG_H

#include <QMainWindow>

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QBluetoothUuid>
#include <QLowEnergyCharacteristic>

#include "scanner.h"

#define ACC_UUID    "{f000aa10-0451-4000-b000-000000000000}"
#define GYRO_UUID   "{f000aa50-0451-4000-b000-000000000000}"

#define ACC_CONFIG_REG  "{f000aa12-0451-4000-b000-000000000000}"
#define ACC_PERIOD_REG  "{f000aa13-0451-4000-b000-000000000000}"
#define ACC_DATA_REG    "{f000aa11-0451-4000-b000-000000000000}"

#define GYRO_CONFIG_REG "{f000aa52-0451-4000-b000-000000000000}"
#define GYRO_PERIOD_REG "{f000aa53-0451-4000-b000-000000000000}"
#define GYRO_DATA_REG   "{f000aa51-0451-4000-b000-000000000000}"


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

    scanner *m_BodyScanWindow = NULL;
    scanner *m_LAScanWindow = NULL;
    scanner *m_RAScanWindow = NULL;
    scanner *m_LLScanWindow = NULL;
    scanner *m_RLScanWindow = NULL;

    QBluetoothAddress m_DeviceAdress = QBluetoothAddress("BC:6A:29:AB:09:39");
    QBluetoothUuid m_AccId;
    QBluetoothUuid m_GyroId;
    QList<QLowEnergyController *> m_Controllers;

    //QLowEnergyController *m_ControllerBody;

    QList<QLowEnergyService *> m_AccServices;
    QList<QLowEnergyService *> m_GyroServices;

    QList<QLowEnergyCharacteristic> m_AccConfigChars;
    QList<QLowEnergyCharacteristic> m_AccPeriodChars;
    QList<QLowEnergyCharacteristic> m_AccDataChars;

    QList<QLowEnergyCharacteristic> m_GyroConfigChars;
    QList<QLowEnergyCharacteristic> m_GyroPeriodChars;
    QList<QLowEnergyCharacteristic> m_GyroDataChars;

    QLowEnergyDescriptor m_NotificationDesc;

    struct
    {
        //QString Address = "BC:6A:29:AB:09:39";
        QString AccUuid = ACC_UUID;
        QString AccDataId = ACC_DATA_REG;
        QString AccConfigId = ACC_CONFIG_REG;
        QString AccPeriodId = ACC_PERIOD_REG;

        QString GyroUuid = GYRO_UUID;
        QString GyroDataId = GYRO_DATA_REG;
        QString GyroConfigId = GYRO_CONFIG_REG;
        QString GyroPeriodId = GYRO_PERIOD_REG;

    } m_DeviceData;






    enum
    {
        BODY,
        LEFT_ARM,
        RIGHT_ARM,
        LEFT_LEG,
        RIGHT_LEG
    };



private slots:
    void openScanWindow(int BodyPart);
    void OpenBodyCfg();
    void OpenLACfg();
    void OpenRACfg();
    void OpenLLCfg();
    void OpenRLCfg();

    void ConnectToDevices();
    void DiscoverServicesBody();

    //void ConfigAccServices();
    void ConfigServices();

    void readAccData(QLowEnergyDescriptor desc, QByteArray value);
    //void ConnectToDevice();
    void DiscoverServices();
    void ConnectToService();
    void ConfigDevice(QLowEnergyService::ServiceState BleServiceState);
    void showData(QLowEnergyCharacteristic Char, QByteArray value);
    void UpdateConfig();
    void NoChangedConfig();

    void StopAquasition();
    //void ReadChar(QLowEnergyService::ServiceState BleServiceState);

};

#endif // SENSORTAG_H
