
#include "ipthread.h"

IpThread::IpThread(QObject *parent) :
    QThread(parent)
{

}

QString IpThread::pingIp(QString ip){
    QProcess process;
    process.start(QString("ping ") + ip);
    process.waitForFinished();
    QByteArray ba = process.readAllStandardOutput();
    QString data = QString::fromLocal8Bit(ba);
    return data;
}

QString IpThread::scanPort(QString host, quint16 portStart, quint16 portEnd){
    QTcpSocket socket;
    QString data;
    for(quint16 i = portStart; i <= portEnd; i++){
        socket.connectToHost(host, i);
        if(socket.waitForConnected(30)){
            data +=   "Port: " + QString::number(i) + " Available\n";
            socket.disconnectFromHost();
        }
    }
    return data;
}

QString IpThread::scanIp(QString ipStart, QString ipEnd){
    QString data;
    return data;
}
