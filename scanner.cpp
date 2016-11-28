#include "scanner.h"
#include "ui_scanner.h"



scanner::scanner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scanner)
{
    ui->setupUi(this);

    // Connect Button Start with Device Discovery
    connect(ui->btn_start_scan,SIGNAL(clicked(bool)),this,SLOT(DeviceDiscover()));
    connect(ui->btn_accept,SIGNAL(clicked(bool)),this,SLOT(SetSettings()));
    connect(ui->btn_chancel,SIGNAL(clicked(bool)),this,SLOT(HoldConfig()));




}

scanner::~scanner()
{
    delete ui;
}

/////////////////////////////////////////////////////////////////////////////////////
/// \brief scanner::DeviceDiscover
///     Discoveres Devices and Prepares all Datastructures to handle adresses
//////////////////////////////////////////////////////////////////////////////////////
void scanner::DeviceDiscover(void)
{
    ui->lst_devices->clear();
    ui->lst_devices->setEnabled(false);
    m_DiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    connect(m_DiscoveryAgent,SIGNAL(finished()),this,SLOT(DeviceDiscoverFinished()));
    connect(m_DiscoveryAgent,SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),this,SLOT(DeviceDiscovered(QBluetoothDeviceInfo)));
    m_DiscoveryAgent->start();
    qInfo() << "Scan For Devices";
}

void scanner::DeviceDiscoverFinished()
{
    ui->lst_devices->setEnabled(true);
    qInfo() << "scan Finished";
}

void scanner::DeviceDiscovered(QBluetoothDeviceInfo DeviceInfo)
{
    if(DeviceInfo.name() == "SensorTag")
    {
        ui->lst_devices->addItem(DeviceInfo.address().toString());
    }
}

void scanner::SetSettings()
{
    if(ui->lst_devices->selectedItems().length() == 1)
    {
        p_DeviceSettings.Address = ui->lst_devices->selectedItems().at(0)->text();
        p_DeviceSettings.Acc_SRate = ui->sb_acc_sr->value();
        p_DeviceSettings.Acc_Res = ui->cmb_acc_res->currentText().toInt();
        p_DeviceSettings.Gyro_SRate = ui->sb_gyro_sr->value();
        emit ConfigChanged();
        this->hide();
    }
    else
    {
        qInfo() << "Select a Adress";
        return;
    }
}

void scanner::HoldConfig()
{
    this->hide();
    emit ConfigStays();
}
