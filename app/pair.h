
#ifndef PAIR_H
#define PAIR_H


#include <QObject>

#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTcpSocket>
#include <QAbstractSocket>

class Pair : public QObject
{
    Q_OBJECT
public:
    explicit Pair(QObject* parent = 0);
    void start(const QString &portName, qint32 baudRate, QString host, quint16 port);
    void stop();
    void tcpSendMsg(const char *data);
    void tcpSendMsg(const QByteArray &data);
    void serialSendMsg(const char *data);
    void serialSendMsg(const QByteArray &data);

public slots:
    void tcpConnected();
    void tcpDisconnected();
    void tcpReadData();
    void serialReadData();

signals:
    void isPair(bool status);
    void readPairTcpData(QByteArray data);
    void readPairSerialData(QByteArray data);

private:
    QTcpSocket* m_client;
    QSerialPort* m_serial;
    bool isSerialConnect = false;
    bool isTcpConnect = false;
};

#endif // PAIR_H
