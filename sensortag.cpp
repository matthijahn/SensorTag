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
    connect(ui->btn_sel_out_fname,SIGNAL(clicked(bool)),this,SLOT(OpenOutFile()));
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

    m_OutFile = new QFile(m_OutputFileName);
    if (!m_OutFile->open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    m_OutStream.setDevice(m_OutFile);

    for(int i = 0; i < m_Controllers.length(); i++)
    {
        switch (i)
        {
        case BODY:
            m_OutStream << "Body ACC X, Body ACC Y, Body ACC Z, Body Gyro Z, Body Gyro Y, Body Gyro Z";
            break;
        case LEFT_ARM:
            m_OutStream << ", LA ACC X, LA ACC Y, LA ACC Z, LA Gyro Z, LA Gyro Y, LA Gyro Z";
            break;
        case RIGHT_ARM:
            m_OutStream << ", RA ACC X, RA ACC Y, RA ACC Z, RA Gyro Z, RA Gyro Y, RA Gyro Z";
            break;
        case LEFT_LEG:
            m_OutStream << ", LL ACC X, LL ACC Y, LL ACC Z, LL Gyro Z, LL Gyro Y, LL Gyro Z";
            break;
        case RIGHT_LEG:
            m_OutStream << ", RL ACC X, RL ACC Y, RL ACC Z, RL Gyro Z, RL Gyro Y, RL Gyro Z";
        default:
            break;
        }
    }
    m_OutStream << "\n";
    m_OutStream.flush();

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
    foreach (QLowEnergyService *Service, m_GyroServices)
    {
        // Period Characteristic is not set ???
        // A SensorTag Firmwreupdate to 1.5 schould fix the Problem
        QBluetoothUuid bar = QBluetoothUuid(QString(GYRO_PERIOD_REG));
        QLowEnergyCharacteristic foo = Service->characteristic(bar);

        m_GyroPeriodChars.append(Service->characteristic((QBluetoothUuid(QString(GYRO_PERIOD_REG)))));

        m_GyroConfigChars.append(Service->characteristic((QBluetoothUuid(QString(GYRO_CONFIG_REG)))));

        m_GyroDataChars.append(Service->characteristic((QBluetoothUuid(QString(GYRO_DATA_REG)))));
    }
    foreach (QLowEnergyService *Service, m_AccServices)
    {
        m_AccConfigChars.append(Service->characteristic((QBluetoothUuid(QString(ACC_CONFIG_REG)))));
        m_AccPeriodChars.append(Service->characteristic((QBluetoothUuid(QString(ACC_PERIOD_REG)))));
        m_AccDataChars.append(Service->characteristic((QBluetoothUuid(QString(ACC_DATA_REG)))));


    }


    foreach (QLowEnergyCharacteristic Char, m_AccDataChars)
    {
        m_AccNotificationDesc.append(Char.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration));
    }

    foreach (QLowEnergyCharacteristic Char, m_GyroDataChars)
    {
        m_GyroNotificationDesc.append(Char.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration));
    }

    // Here Be Dragons
    // TODO Dynamicly add Values for Config



    for (int i = 0; i < m_AccServices.length(); i++)
    {
        m_AccServices[i]->writeCharacteristic(m_AccConfigChars[i],QByteArray::fromHex("1"));
        m_AccServices[i]->writeCharacteristic(m_AccPeriodChars[i],QByteArray::fromHex("A"));
        m_AccServices[i]->writeDescriptor(m_AccNotificationDesc[i],QByteArray::fromHex("0100"));

    }
    for (int i = 0; i < m_GyroServices.length(); i++)
    {
        m_GyroServices[i]->writeCharacteristic(m_GyroConfigChars[i],QByteArray::fromHex("7"));
        m_GyroServices[i]->writeCharacteristic(m_GyroPeriodChars[i],QByteArray::fromHex("A"));
        m_GyroServices[i]->writeDescriptor(m_GyroNotificationDesc[i],QByteArray::fromHex("0100"));

    }

    foreach (QLowEnergyService *Service, m_AccServices) {
       connect(Service,SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),this,SLOT(aquireData(QLowEnergyCharacteristic,QByteArray)));
    }
    foreach (QLowEnergyService *Service, m_GyroServices) {
       connect(Service,SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),this,SLOT(aquireData(QLowEnergyCharacteristic,QByteArray)));
    }
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

void SensorTag::aquireData(QLowEnergyCharacteristic Char, QByteArray value)
{
    //m_DataMap.insert(Char, value);
    if(m_AccDataChars.contains(Char))
    {
         m_AccDataMap.insert(m_AccDataChars.indexOf(Char),value);
        qInfo() << "Acc";

    }
    if(m_GyroDataChars.contains(Char))
    {
        m_GyroDataMap.insert(m_AccDataChars.indexOf(Char),value);
        qInfo() << "Gyro";
    }
    if(m_GyroDataMap.size() == m_GyroDataChars.length() && m_AccDataMap.size() == m_AccDataChars.length())
    {
        ShowData();
    }
//    if(Char == m_AccDataChars.at(0))
//    {
//        for(int i = 0; i < m_AccServices.length(); i++)
//        {

//            m_AccServices[i]->readCharacteristic(m_AccDataChars[i]);
//            m_GyroServices[i]->readCharacteristic(m_GyroDataChars[i]);

//        }
//        ui->le_acc_x->setText(QString::number(value.at(0)));
//        ui->le_acc_y->setText(QString::number(value.at(1)));
//        ui->le_acc_z->setText(QString::number(value.at(2)));
//       int x = value.at(0);
//       int y = value.at(1);
//       int z = value.at(2);
//       qInfo() << x << y << z;
//    }
//    if(Char == m_GyroDataChar)
//    {
//        ui->le_gyro_x->setText(QString::number(value.at(0)));
//        ui->le_gyro_y->setText(QString::number(value.at(1)));
//        ui->le_gyro_z->setText(QString::number(value.at(2)));
//    }
    ui->statusBar->showMessage("Aquarring");

}

void SensorTag::ShowData()
{
    for(int i = 0; i < m_AccDataMap.size(); i++)
    {
        QByteArray Acc = m_AccDataMap.value(i);
        QByteArray Gyro = m_GyroDataMap.value(i);
        if(i!=0)
        {
            m_OutStream << ",";
        }
        m_OutStream << int8_t(Acc[0]) << "," << int8_t(Acc[1]) << "," << int8_t(Acc[2]) << ",";
        m_OutStream << int16_t(Gyro[1] << 8 & Gyro[0]) << "," <<  int16_t(Gyro[3] << 8 & Gyro[2]) << "," << int16_t(Gyro[5] << 8 & Gyro[4]);
    }
    m_OutStream << "\n";

//    foreach (QByteArray value, m_AccDataMap) {
//       qInfo() << value;
//    }
//    foreach (QByteArray value, m_GyroDataMap) {
//       qInfo() << value;
//    }
    m_AccDataMap.clear();
    m_GyroDataMap.clear();
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
    m_OutStream.flush();
    //disconnect(m_AccServices.at(0),SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),this,SLOT(showData(QLowEnergyCharacteristic,QByteArray)));

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

void SensorTag::OpenOutFile()
{

    m_OutputFileName = QFileDialog::getSaveFileName(this,tr("Open Datafile"), "", tr("Datafiles (*.csv)"));
    ui->le_out_fname->setText(m_OutputFileName);
}

void SensorTag::CloseOutFile()
{

}



//}
