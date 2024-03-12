
#include "myudp.h"

MyUdp::MyUdp(QObject *parent) :
    QObject(parent)
{
    udp_ = new QUdpSocket(this);
    connect(udp_, SIGNAL(connected()), this, SLOT(connected()));
    connect(udp_, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(udp_, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void MyUdp::start(quint16 port){

    if(!udp_->bind(QHostAddress::LocalHost, port)) // QAbstractSocket::ShareAddress
    {
        qInfo() << udp_->errorString();
        return;
    }
    qInfo() << "UDP Listening on " << udp_->localAddress() << ":" << udp_->localPort();
}

void MyUdp::connected(){
    emit isConnect(true);
}

void MyUdp::disconnected(){
    emit isConnect(false);
}

void MyUdp::readyRead(){
//    while(udp_->hasPendingDatagrams()){
//        QNetworkDatagram datagram = udp_->receiveDatagram();
//        qInfo() << "Receiver UDP: " << datagram.data();
//    }
    QByteArray buffer;
    buffer.resize(udp_->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;
    udp_->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);
//    qInfo() << "Data: " << buffer.data() << "from: " << sender.toString() << senderPort;

    emit udpReceiveData(buffer.data());

}

