#include "scanner.h"
#include "ui_scanner.h"

scanner::scanner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scanner)
{
    ui->setupUi(this);
    connect(ui->btn_start_scan,SIGNAL(clicked(bool)),this,SLOT(deviceDiscover()));
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
    ui->lst_devices->addItem(device.address().toString());
}
