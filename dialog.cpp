#include "dialog.h"
#include "ui_dialog.h"
#include<QDebug>
#include<QMessageBox>
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->templcd->display("-------");
    ui->humlcd->display("-------");
    serialBuffer = "";
    parsed_datatemp = "";
    parsed_datahum= "";
    temp_val= 0.0;
    hum_val= 0.0;
/*
    qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length() << "\n";
       foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
           qDebug() << "Description: " << serialPortInfo.description() << "\n";
           qDebug() << "Has vendor id?: " << serialPortInfo.hasVendorIdentifier() << "\n";
           qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier() << "\n";
           qDebug() << "Has product id?: " << serialPortInfo.hasProductIdentifier() << "\n";
           qDebug() << "Product ID: " << serialPortInfo.productIdentifier() << "\n";
       }*/
    bool esp_is_available = false;
       QString esp_port_name;
       //
       //  For each available serial port
       foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
           //  check if the serialport has both a product identifier and a vendor identifier
           if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
               //  check if the product ID and the vendor ID match those of the esp
               if((serialPortInfo.productIdentifier() == esp_pid)
                       && (serialPortInfo.vendorIdentifier() == esp_vid)){
                   esp_is_available = true; //    esp is available on this port
                   esp_port_name = serialPortInfo.portName();
               }
           }
       }

    //serialport definition
       if(esp_is_available){
    esp =new QSerialPort(this);
    esp->setPortName(esp_port_name);
    esp->open(QSerialPort::ReadOnly);
    esp->setBaudRate(QSerialPort::Baud115200);
    esp->setDataBits(QSerialPort::Data8);
    esp->setParity(QSerialPort::NoParity);
    esp->setStopBits(QSerialPort::OneStop);
    esp->setFlowControl(QSerialPort::NoFlowControl);
    QObject::connect(esp,SIGNAL(readyRead()),this,SLOT(readSerial()));
}else{
           qDebug() << "Couldn't find the correct port for the esp.\n";
          QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to esp.");

       }
}

Dialog::~Dialog()
{
    if(esp->isOpen()){

        esp->close();
    }
    delete ui;
}
void Dialog::readSerial()
{
QStringList buffer_split = serialBuffer.split(",");
if(buffer_split.length() <3){
    serialData = esp->readAll();
    serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
    serialData.clear();
}
else{
    serialBuffer= "";
    qDebug() << buffer_split <<"\n";
    parsed_datatemp = buffer_split[1];
    temp_val= parsed_datatemp.toDouble();
    qDebug() << temp_val<< "\n";
    parsed_datatemp= QString::number(temp_val,'g',4);
    Dialog::updatetemp(parsed_datatemp);

   /* parsed_datahum =  buffer_split[2];
    hum_val =parsed_datahum.toDouble();
    parsed_datahum= QString::number(temp_val,'g',4);
    Dialog::updatehum(parsed_datahum);
*/
}

}
void Dialog::updatetemp(QString sensortemp){
ui->templcd->display(sensortemp);

}
void Dialog::updatehum(QString sensorhum){
ui->humlcd->display(sensorhum);

}
