
#ifndef MYUDP_H
#define MYUDP_H

#include <QEventLoop>
#include <QTimer>
#include <QDateTime>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QThread>
#include <QQuaternion>

class MyUdp : public QObject
{
    Q_OBJECT
public:
    explicit MyUdp(QObject* parent = 0);
    void start(quint16 port);

public slots:
    void connected();
    void disconnected();
    void readyRead();

signals:
    void isConnect(bool status);
    void udpReceiveData(QByteArray data);

private:
    QUdpSocket* udp_;
};

#endif // MYUDP_H
