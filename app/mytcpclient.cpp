
#include "mytcpclient.h"

MyTcpClient::MyTcpClient(QObject *parent) :
    QObject(parent)
{
    m_client = new QTcpSocket(this);
    connect(m_client, SIGNAL(connected()), this, SLOT(connected()));
    connect(m_client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(m_client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_client, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            this, SLOT(errorOccurred(QAbstractSocket::SocketError)));
}

void MyTcpClient::run(QString host, quint16 port)
{
    m_client->connectToHost(host, port);

    if(!m_client->waitForConnected(10000)){
        qDebug() << m_client->errorString();
        m_client->abort();
        m_client->close();
        emit isConnect(false);
    }
}

void MyTcpClient::errorOccurred(QAbstractSocket::SocketError error){
    qDebug() << "error in connection: " << m_client->errorString();
}

void MyTcpClient::connected(){
    emit isConnect(true);
}

void MyTcpClient::disconnected(){
    emit isConnect(false);
}

void MyTcpClient::readyRead(){
    QByteArray data = m_client->readAll();
    emit tcpReceiveData(data);
}

void MyTcpClient::sendMsg(const char *data){
    m_client->write(data);
}

void MyTcpClient::sendMsg(const QByteArray &data){
    m_client->write(data);
}

void MyTcpClient::close(){
    m_client->abort();
    m_client->close();
}
