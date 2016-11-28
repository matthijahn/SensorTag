#include "sensortag.h"
#include "ui_sensortag.h"

SensorTag::SensorTag(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SensorTag)
{
    ui->setupUi(this);
    ui->btn_stop->setEnabled(false);

    connect(ui->btn_add_body,SIGNAL(clicked()),this,SLOT(OpenBodyCfg()));
    connect(ui->btn_start,SIGNAL(clicked(bool)),this,SLOT(ConnectToDevices()));
    connect(ui->btn_stop,SIGNAL(clicked(bool)),this,SLOT(StopAquasition()));
}

SensorTag::~SensorTag()
{
    delete ui;
}

void SensorTag::OpenBodyCfg()
{
    openScanWindow(BODY);
}

void SensorTag::OpenLACfg()
{
    openScanWindow(LEFT_ARM);
}

void SensorTag::OpenRACfg()
{
    openScanWindow(RIGHT_ARM);
}

void SensorTag::OpenLLCfg()
{
    openScanWindow(LEFT_LEG);
}

void SensorTag::OpenRLCfg()
{
    openScanWindow(RIGHT_LEG);
}


void SensorTag::openScanWindow(int BodyPart)
{
    switch (BodyPart) {
    case BODY:
        if(!m_BodyScanWindow)
        {
            m_BodyScanWindow = new scanner();
            connect(m_BodyScanWindow,SIGNAL(ConfigStays()),this,SLOT(NoChangedConfig()));
        }

        m_BodyScanWindow->show();
        this->setEnabled(false);
        break;
    case LEFT_ARM:
        if(!m_LAScanWindow)
        {
            m_LAScanWindow = new scanner();
        }
        m_LAScanWindow->show();
        break;
    case RIGHT_ARM:
        if(!m_RAScanWindow)
        {
            m_RAScanWindow = new scanner();
        }
        m_RAScanWindow->show();
        break;
    case LEFT_LEG:
        if(!m_LLScanWindow)
        {
            m_LLScanWindow = new scanner();
        }
        m_LLScanWindow->show();
        break;
    case RIGHT_LEG:
        if(!m_RLScanWindow)
        {
            m_RLScanWindow = new scanner();
        }
        m_RLScanWindow->show();
        break;
    default:
        break;
    }

   connect(m_BodyScanWindow,SIGNAL(ConfigChanged()),this,SLOT(UpdateConfig()));
}


void SensorTag::ConnectToDevices()
{
    ui->btn_start->setEnabled(false);
    ui->btn_stop->setEnabled(true);

    if(ui->le_1->text() != "")
    {
        m_Controllers.append(new QLowEnergyController(QBluetoothAddress(ui->le_1->text())));
    }
    if(ui->le_2->text() != "")
    {
        m_Controllers.append(new QLowEnergyController(QBluetoothAddress(ui->le_2->text())));
    }
    if(ui->le_3->text() != "")
    {
        m_Controllers.append(new QLowEnergyController(QBluetoothAddress(ui->le_3->text())));
    }
    if(ui->le_4->text() != "")
    {
        m_Controllers.append(new QLowEnergyController(QBluetoothAddress(ui->le_4->text())));
    }
    if(ui->le_5->text() != "")
    {
        m_Controllers.append(new QLowEnergyController(QBluetoothAddress(ui->le_5->text())));
    }



    foreach(QLowEnergyController *Controller, m_Controllers)
    {
        connect(Controller,SIGNAL(connected()),this,SLOT(DiscoverServices()));
        Controller->connectToDevice();
    }



}

void SensorTag::DiscoverServicesBody()
{
    ui->statusBar->showMessage("Body Device Connected");
}

void SensorTag::DiscoverServices()
{
    foreach (const QLowEnergyController *Controller, m_Controllers)
    {
        if(Controller->state() != QLowEnergyController::ConnectedState)
        {
            return;
        }
    }
    foreach (QLowEnergyController *Controller, m_Controllers)
    {
        ui->statusBar->showMessage("Devices Connected");
        connect(Controller,SIGNAL(discoveryFinished()),this,SLOT(ConnectToService()));
        Controller->discoverServices();
    }

    //qInfo() << "DeviceConnected";

    //m_
}

void SensorTag::ConnectToService()
{
    foreach (const QLowEnergyController *Controller, m_Controllers)
    {
        if(Controller->state() != QLowEnergyController::DiscoveredState)
        {
            return;
        }
    }


    m_AccId = QBluetoothUuid(m_DeviceData.AccUuid);
    m_GyroId = QBluetoothUuid(m_DeviceData.GyroUuid);
    ui->statusBar->showMessage("Connecting To Service");
    foreach (QLowEnergyController *Controller, m_Controllers)
    {
        m_AccServices.append(Controller->createServiceObject(m_AccId));
        m_GyroServices.append(Controller->createServiceObject(m_GyroId));
    }
//    for(int i = 0; i < m_AccServices.length(); i++)
//    {
//        connect(m_AccServices[i], SIGNAL(stateChanged(QLowEnergyService::ServiceState)),this,SLOT(ConfigServices()));
//        connect(m_GyroServices[i], SIGNAL(stateChanged(QLowEnergyService::ServiceState)),this,SLOT(ConfigServices()));
//    }
    foreach (QLowEnergyService *Service, m_AccServices) {
       connect(Service,SIGNAL(stateChanged(QLowEnergyService::ServiceState)),this,SLOT(ConfigServices()));
    }
    foreach (QLowEnergyService *Service, m_GyroServices) {
       connect(Service,SIGNAL(stateChanged(QLowEnergyService::ServiceState)),this,SLOT(ConfigServices()));
    }
    foreach (QLowEnergyService *Service, m_AccServices)
    {
        Service->discoverDetails();
    }

    foreach (QLowEnergyService *Service, m_GyroServices)
    {
        Service->discoverDetails();
    }


    //qInfo() << "Connecting to Service";
    //m_AccService = m_DeviceController->createServiceObject(m_AccId);
    //m_GyroService = m_DeviceController->createServiceObject(m_GyroId);
    //m_AccService->discoverDetails();
    //m_GyroService->discoverDetails();
    //connect(m_AccService,SIGNAL(stateChanged(QLowEnergyService::ServiceState)),this,SLOT(ConfigDevice(QLowEnergyService::ServiceState)));
}

void SensorTag::ConfigServices()
{
    foreach (QLowEnergyService *Service, m_AccServices)
    {
        if(Service->state() != QLowEnergyService::ServiceDiscovered)
        {
            return;
        }

    }
    foreach (QLowEnergyService *Service, m_GyroServices)
    {
        if(Service->state() != QLowEnergyService::ServiceDiscovered)
        {
            return;
        }
    }
    ui->statusBar->showMessage("Config Services");
    foreach (QLowEnergyService *Service, m_AccServices)
    {
        m_AccConfigChars.append(Service->characteristic((QBluetoothUuid(QString(ACC_CONFIG_REG)))));
        m_AccPeriodChars.append(Service->characteristic((QBluetoothUuid(QString(ACC_PERIOD_REG)))));
        m_AccDataChars.append(Service->characteristic((QBluetoothUuid(QString(ACC_DATA_REG)))));


    }
    foreach (QLowEnergyService *Service, m_GyroServices)
    {
        m_GyroConfigChars.append(Service->characteristic((QBluetoothUuid(QString(GYRO_CONFIG_REG)))));
        m_GyroPeriodChars.append(Service->characteristic((QBluetoothUuid(QString(GYRO_PERIOD_REG)))));
        m_GyroDataChars.append(Service->characteristic((QBluetoothUuid(QString(GYRO_DATA_REG)))));
    }

    // Here Be Dragons

    for (int i = 0; i < m_AccServices.length(); i++)
    {
        m_AccServices[i]->writeCharacteristic(m_AccConfigChars[i],QByteArray::fromHex("1"));
        m_AccServices[i]->writeCharacteristic(m_AccPeriodChars[i],QByteArray::fromHex("A"));

    }
//    foreach (QLowEnergyService *Service, m_AccServices)
//    {
//        foreach (QLowEnergyCharacteristic Char, m_AccConfigChars)
//        {
//            Service->writeCharacteristic(Char, QByteArray::fromHex("1"));
//        }
//        foreach (QLowEnergyCharacteristic Char, m_AccPeriodChars)
//        {
//            Service->writeCharacteristic(Char,QByteArray::fromHex("A"));
//        }

        //m_AccService->writeCharacteristic(m_AccConfigChar,QByteArray::fromHex("1"));
        //m_AccService->writeCharacteristic(m_AccPeriodChar,QByteArray::fromHex("A"));
    //}
    m_NotificationDesc = m_AccDataChars[0].descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
    m_AccServices[0]->writeDescriptor(m_NotificationDesc,QByteArray::fromHex("0100"));
    connect(m_AccServices.at(0),SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),this,SLOT(showData(QLowEnergyCharacteristic,QByteArray)));
}



void SensorTag::ConfigDevice(QLowEnergyService::ServiceState BleServiceState)
{
//    if (BleServiceState == QLowEnergyService::ServiceDiscovered)
//    {
//        qInfo() << "Service Connected";
//        foreach (const QLowEnergyCharacteristic &Characteristic, m_AccService->characteristics())
//        {
//            qInfo() << Characteristic.uuid().toString();
//            foreach(const QLowEnergyDescriptor &Descriptor, Characteristic.descriptors())
//                qInfo() << Descriptor.value();
//        }
//    }
   // m_AccDataChar = m_AccService->characteristic(QBluetoothUuid(QString(ACC_DATA_REG)));
    //m_AccDataNotDesc = m_AccDataChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
    //m_AccPeriodChar = m_AccService->characteristic(QBluetoothUuid(QString(ACC_PERIOD_REG)));
    //m_AccConfigChar = m_AccService->characteristic(QBluetoothUuid(QString(ACC_CONFIG_REG)));

    //m_AccService->writeCharacteristic(m_AccConfigChar,QByteArray::fromHex("1"));
    //m_AccService->writeCharacteristic(m_AccPeriodChar,QByteArray::fromHex("A"));

//    if(m_AccDataNotDesc.isValid())
//    {
//        connect(m_AccService,SIGNAL(descriptorWritten(QLowEnergyDescriptor,QByteArray)),this,SLOT(readAccData(QLowEnergyDescriptor,QByteArray)));
//        qInfo() << "Write Notification";
//        m_AccService->writeDescriptor(m_AccDataNotDesc,QByteArray::fromHex("0100"));
//    }
//    connect(m_AccService,SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),this,SLOT(showData(QLowEnergyCharacteristic,QByteArray)));

//    m_AccService->writeCharacteristic(QLowEnergyCharacteristic);
}

void SensorTag::readAccData(QLowEnergyDescriptor desc, QByteArray value)
{
    //m_AccService->readDescriptor(desc);
    qInfo() << desc.value();
}

void SensorTag::showData(QLowEnergyCharacteristic Char, QByteArray value)
{
    if(Char == m_AccDataChars.at(0))
    {
        ui->le_acc_x->setText(QString::number(value.at(0)));
        ui->le_acc_y->setText(QString::number(value.at(1)));
        ui->le_acc_z->setText(QString::number(value.at(2)));
//       int x = value.at(0);
//       int y = value.at(1);
//       int z = value.at(2);
//       qInfo() << x << y << z;
    }
//    if(Char == m_GyroDataChar)
//    {
//        ui->le_gyro_x->setText(QString::number(value.at(0)));
//        ui->le_gyro_y->setText(QString::number(value.at(1)));
//        ui->le_gyro_z->setText(QString::number(value.at(2)));
//    }
    ui->statusBar->showMessage("Aquarring");

}

void SensorTag::UpdateConfig()
{
    this->setEnabled(true);
    ui->le_1->setText(m_BodyScanWindow->p_DeviceSettings.Address);

}

void SensorTag::NoChangedConfig()
{
    this->setEnabled(true);
}

void SensorTag::StopAquasition()
{
    disconnect(m_AccServices.at(0),SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),this,SLOT(showData(QLowEnergyCharacteristic,QByteArray)));

    foreach (QLowEnergyService *Service, m_AccServices) {
       connect(Service,SIGNAL(stateChanged(QLowEnergyService::ServiceState)),this,SLOT(ConfigServices()));
    }
    foreach (QLowEnergyService *Service, m_GyroServices) {
       connect(Service,SIGNAL(stateChanged(QLowEnergyService::ServiceState)),this,SLOT(ConfigServices()));
    }
    foreach(QLowEnergyController *Controller, m_Controllers)
    {
        disconnect(Controller,SIGNAL(connected()),this,SLOT(DiscoverServices()));
        disconnect(Controller,SIGNAL(discoveryFinished()),this,SLOT(ConnectToService()));
        Controller->disconnectFromDevice();
    }

    m_AccDataChars.clear();
    m_AccServices.clear();
    m_AccPeriodChars.clear();
    m_AccConfigChars.clear();

    m_GyroDataChars.clear();
    m_GyroServices.clear();
    m_GyroPeriodChars.clear();
    m_GyroConfigChars.clear();
    m_Controllers.clear();

    ui->btn_start->setEnabled(true);
    ui->btn_stop->setEnabled(false);

}



//}
