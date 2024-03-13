
#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent) :
    QObject(parent)
{
    m_server = new QTcpServer();
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newTcpConnection()));
}

void MyTcpServer::open(quint16 port){
    if(!m_server->listen(QHostAddress::Any, port)){
        emit isListen(false);
    }else{
        emit isListen(true);
    }
}

void MyTcpServer::close(){
//    m_socket->abort();
//    m_socket->close();
    m_server->close();
    emit isListen(m_server->isListening());
}

void MyTcpServer::sendMsg(const char *data){
    m_socket->write(data);
}

void MyTcpServer::sendMsg(const QByteArray &data){
    m_socket->write(data);
}

void MyTcpServer::newTcpConnection(){
    m_socket = m_server->nextPendingConnection();
    m_socket->setReadBufferSize(512);
    connect(m_socket, SIGNAL(readyRead()), SLOT(readyRead()));
//    qDebug() << "New Client Connecttion: " << m_socket->peerAddress().toString();
}

void MyTcpServer::readyRead(){
    if (m_socket->bytesAvailable()){
        QByteArray data = m_socket->readAll();
        emit tcpReceiveData(data);
//        qDebug() << "Receiver from: " << m_socket->peerAddress().toString() << ", data: "<< data;
    }

}

