
#include "myserial.h"

MySerial::MySerial(QObject *parent) :
    QObject(parent)
{
    serial_ = new QSerialPort(this);
    connect(serial_, SIGNAL(readyRead()), this, SLOT(readData()));

}

void MySerial::open(const QString &portName, qint32 baudRate){
    serial_->setPortName(portName);
    serial_->setBaudRate(baudRate);
    serial_->setParity(QSerialPort::Parity::NoParity);
    serial_->setDataBits(QSerialPort::DataBits::Data8);
    serial_->setStopBits(QSerialPort::StopBits::OneStop);
    serial_->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    serial_->open(QIODevice::ReadWrite);
    if(serial_->isOpen()){
        emit isConnect(true);
    }else{
        qDebug() << serial_->error();
        emit isConnect(false);
    }
}
void MySerial::close(){
    serial_->close();
    if(serial_->isOpen()){
        emit isConnect(true);
    }else
        emit isConnect(false);
}

void MySerial::readData(){
    QByteArray data = serial_->readAll();
    emit serialReceiveData(data);
}

void MySerial::sendMsg(const char *data){
    serial_->write(data);
}

void MySerial::sendMsg(const QByteArray &data){
    serial_->write(data);
}
