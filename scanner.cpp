#include "scanner.h"
#include "ui_scanner.h"



scanner::scanner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scanner)
{
    ui->setupUi(this);
    connect(ui->btn_start_scan,SIGNAL(clicked(bool)),this,SLOT(deviceDiscover()));
    connect(ui->lst_devices,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(controlDevice(QListWidgetItem*)));
}

scanner::~scanner()
{
    delete ui;
}

void scanner::deviceDiscover()
{
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),this, SLOT(showDevices(QBluetoothDeviceInfo)));

       // Start a discovery
       discoveryAgent->start();
}

void scanner::showDevices(const QBluetoothDeviceInfo &device)
{

    if(device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
    {
        m_address_map[device.address().toString()] = device.address();
        ui->lst_devices->addItem(device.address().toString() + " " + device.name());
    }

}

void scanner::controlDevice(QListWidgetItem* address)
{
    QLowEnergyController *control;
    control = new QLowEnergyController(m_address_map[address->text()], this);
    qInfo() << "Hallo";
}
