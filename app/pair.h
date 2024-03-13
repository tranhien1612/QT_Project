
#ifndef PAIR_H
#define PAIR_H


#include <QObject>

#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTcpSocket>
#include <QTcpServer>
#include <QAbstractSocket>

class Pair : public QObject
{
    Q_OBJECT
public:
    explicit Pair(QObject* parent = 0);
    void start(const QString &portName, qint32 baudRate, QString host, quint16 port);
    void stop();
    void tcpClientSendMsg(const char *data);
    void tcpClientSendMsg(const QByteArray &data);
    void serialSendMsg(const char *data);
    void serialSendMsg(const QByteArray &data);
    void tcpServerSendMsg(const char *data);
    void tcpServerSendMsg(const QByteArray &data);

public slots:
    void tcpConnected();
    void tcpDisconnected();
    void tcpReadData();
    void serialReadData();

private slots://tcp Server
    void newTcpClientConnection();
    void tcpServerReadyRead();

signals:
    void isPair(bool status);
    void readPairTcpData(QByteArray data);
    void readPairSerialData(QByteArray data);

private:
    QTcpSocket* m_client;
    QSerialPort* m_serial;
    bool isSerialConnect = false;
    bool isTcpConnect = false;

    bool isTcpMode = false; //TcpClient: false, TCPServer: true;

    QTcpServer *m_server;
    QTcpSocket *m_socket;

};

#endif // PAIR_H
