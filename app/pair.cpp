
#include "pair.h"

Pair::Pair(QObject *parent) :
    QObject(parent)
{
    m_serial = new QSerialPort(this);
    connect(m_serial, SIGNAL(readyRead()), this, SLOT(serialReadData()));

    m_client = new QTcpSocket(this);
    connect(m_client, SIGNAL(connected()), this, SLOT(tcpConnected()));
    connect(m_client, SIGNAL(disconnected()), this, SLOT(tcpDisconnected()));
    connect(m_client, SIGNAL(readyRead()), this, SLOT(tcpReadData()));

    m_server = new QTcpServer();
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newTcpClientConnection()));
}

void Pair::start(const QString &portName, qint32 baudRate, QString host, quint16 port){
    m_serial->setPortName(portName);
    m_serial->setBaudRate(baudRate);
    m_serial->setParity(QSerialPort::Parity::NoParity);
    m_serial->setDataBits(QSerialPort::DataBits::Data8);
    m_serial->setStopBits(QSerialPort::StopBits::OneStop);
    m_serial->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    m_serial->open(QIODevice::ReadWrite);

    if(m_serial->isOpen()){
        qDebug() << "Serial is opened!";
        isSerialConnect = true;
    }else{
        qDebug() << m_serial->error();
        isSerialConnect = false;
        emit isPair(false);
    }

    if(host.isEmpty()){ //TcpServer
        isTcpMode = true;
        //Tcp Server
        if(!m_server->listen(QHostAddress::Any, port)){
            emit isPair(false);
        }else{
            isTcpConnect = true;
            if(isTcpConnect && isSerialConnect)
                emit isPair(true);
            else
                emit isPair(false);
        }
    }else{ //TcpClient
        isTcpMode = false;
        //Tcp Client
        m_client->connectToHost(host, port);

        if(!m_client->waitForConnected(10000)){
            qDebug() << m_client->errorString();
            m_client->abort();
            m_client->close();
        }
    }

}

void Pair::stop(){
    m_serial->close();
    if(isTcpMode){
        m_server->close();
        m_socket->abort();
        m_socket->close();
        qDebug() << "TcpServer: " << m_server->isListening();
    }else{
        m_client->abort();
        m_client->close();
    }
}

//TCP Server
void Pair::newTcpClientConnection(){
    m_socket = m_server->nextPendingConnection();
    m_socket->setReadBufferSize(512);
    connect(m_socket, SIGNAL(readyRead()), SLOT(tcpServerReadyRead()));
}

void Pair::tcpServerReadyRead(){
    if (m_socket->bytesAvailable()){
        QByteArray data = m_socket->readAll();
        qDebug() << "Receiver from: " << m_socket->peerAddress().toString() << ", data: "<< data;
        emit readPairTcpData(data);
        serialSendMsg(data);
    }
}

void Pair::tcpServerSendMsg(const char *data){
    m_socket->write(data);
}

void Pair::tcpServerSendMsg(const QByteArray &data){
    m_socket->write(data);
}

//Tcp Client
void Pair::tcpConnected(){
    qDebug() << "TCP Client is connected!";
    isTcpConnect = true;
    if(isTcpConnect && isSerialConnect)
        emit isPair(true);
    else
        emit isPair(false);
}

void Pair::tcpDisconnected(){
    qDebug() << "TCP Client is disconnected!";
    isTcpConnect = false;
    emit isPair(false);
}

void Pair::tcpReadData(){
    QByteArray data = m_client->readAll();
    qDebug() << "TCP Client: " << data;
    emit readPairTcpData(data);
    serialSendMsg(data);
}

void Pair::serialReadData(){
    QByteArray data = m_serial->readAll();
    emit readPairSerialData(data);
    qDebug() << "Serial: " << data;
    if(isTcpMode)
        tcpServerSendMsg(data);
    else
        tcpClientSendMsg(data);
}

void Pair::tcpClientSendMsg(const char *data){
    m_client->write(data);
}

void Pair::tcpClientSendMsg(const QByteArray &data){
    m_client->write(data);
}

void Pair::serialSendMsg(const char *data){
    m_serial->write(data);
}

void Pair::serialSendMsg(const QByteArray &data){
    m_serial->write(data);
}
