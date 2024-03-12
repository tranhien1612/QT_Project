
#include "mytcp.h"

MyTcp::MyTcp(QObject *parent) :
    QObject(parent)
{
    tcp_ = new QTcpSocket(this);
    connect(tcp_, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcp_, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(tcp_, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void MyTcp::run(QString host, quint16 port)
{
    host_ = host; port_ = port;
    tcp_->connectToHost(host_, port_);

    if(!tcp_->waitForConnected(10000)){
        qDebug() << tcp_->errorString();
        tcp_->abort();
        tcp_->close();
        emit isConnect(false);
    }
}

void MyTcp::connected(){
    _isConnected = true;
    emit isConnect(_isConnected);
}

void MyTcp::disconnected(){
    _isConnected = false;
    emit isConnect(_isConnected);
}

void MyTcp::readyRead(){
    QByteArray data = tcp_->readAll();
    emit tcpReceiveData(data);
}

void MyTcp::sendMsg(const char *data){
    tcp_->write(data);
}

void MyTcp::sendMsg(const QByteArray &data){
    tcp_->write(data);
}

void MyTcp::close(){
    tcp_->abort();
    tcp_->close();
}
