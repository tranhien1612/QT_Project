
#ifndef MYTCP_H
#define MYTCP_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>


class MyTcp : public QObject
{
    Q_OBJECT
public:
    explicit MyTcp(QObject* parent = 0);
    void run(QString host, quint16 port);
    void close();
    void sendMsg(const char *data);
    void sendMsg(const QByteArray &data);
//    bool isConnect(){return _isConnected;};

public slots:
    void connected();
    void disconnected();
    void readyRead();
//    void sendMsg(const char *data);

signals:
    void isConnect(bool status);
    void tcpReceiveData(QByteArray data);

private:
    bool _isConnected = false;
    QTcpSocket* tcp_;
    QString host_;
    quint16 port_;
};

#endif // MYTCP_H
