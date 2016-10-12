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
    m_control->discoverServices();
}

void scanner::status(void)
{
    qInfo() << m_control->state();
}

void scanner::serviceStatus(const QBluetoothUuid ble_id)
{
    qInfo() << ble_id;
}
