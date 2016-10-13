#include "scanner.h"
#include "ui_scanner.h"



scanner::scanner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scanner)
{
    ui->setupUi(this);
    connect(ui->btn_start_scan,SIGNAL(clicked(bool)),this,SLOT(deviceDiscover()));
    //connect(ui->btn_start_scan,SIGNAL(clicked(bool)),this,SLOT(ui->lst_devices->)
    connect(ui->lst_devices,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(controlDevice(QListWidgetItem*)));
}

scanner::~scanner()
{
    delete ui;
}

void scanner::deviceDiscover(void)
{
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),this, SLOT(showDevices(QBluetoothDeviceInfo)));
    //connect(discoveryAgent, SIGNAL(finished()),this, SLOT

       // Start a discovery
       discoveryAgent->start();
}

void scanner::showDevices(const QBluetoothDeviceInfo &device)
{
    ui->lst_devices->clear();
    m_address_map.clear();
    m_service_map.clear();
    if(device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
    {
        m_address_map[device.address().toString()] = device.address();
        //qInfo() << device.address();
        ui->lst_devices->addItem(device.address().toString() );//+ " " + device.name());
        //qInfo() << m_address_map[device.address().toString()];
    }

}

void scanner::controlDevice(QListWidgetItem* address)
{

    m_control = new QLowEnergyController(m_address_map[address->text()], this);
    //connect(m_control,SIGNAL(connected()),this,SLOT(status()));
    connect(m_control,SIGNAL(connected()),this,SLOT(scanService()));
    qInfo() << m_control->state();
    m_control->connectToDevice();
    qInfo() << m_control->state();

}

void scanner::scanService(void)
{
    connect(m_control, SIGNAL(serviceDiscovered(QBluetoothUuid)),this,SLOT(serviceStatus(QBluetoothUuid)));
    connect(m_control, SIGNAL(discoveryFinished()),this,SLOT(readService()));
    m_control->discoverServices();
}

void scanner::readService(void)
{
    //QBluetoothUuid acc_sensor;
    //acc_sensor.QBluetoothUuid(ACC_UUID);
    //acc_sensor.QBluetoothUuid(ACC_UUID);
    m_service = m_control->createServiceObject(m_service_map[ACC_UUID]);

    connect(m_service,SIGNAL(stateChanged(QLowEnergyService::ServiceState)),this,SLOT(readCharacteristic()));

    m_service->discoverDetails();
    qInfo() << "Service Object created";

    //qInfo() << service->characteristics();

}

void scanner::readCharacteristic(void)
{
    //qInfo() << char_id.name();
    //qInfo() << char_value;
    qInfo() << "State Changed";
    QList<QLowEnergyCharacteristic> foo = m_service->characteristics();

    //qInfo() << m_service->characteristics()[1].value();
}

void scanner::status(void)
{
    qInfo() << m_control->state();
}

void scanner::serviceStatus(const QBluetoothUuid ble_id)
{
    qInfo() << ble_id;
    m_service_map[ble_id.toString()] = ble_id;
}
