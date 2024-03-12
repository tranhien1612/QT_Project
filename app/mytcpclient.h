
#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>


class MyTcpClient : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpClient(QObject* parent = 0);
    void run(QString host, quint16 port);
    void close();
    void sendMsg(const char *data);
    void sendMsg(const QByteArray &data);

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void errorOccurred(QAbstractSocket::SocketError error);

signals:
    void isConnect(bool status);
    void tcpReceiveData(QByteArray data);

private:
    QTcpSocket* m_client;
};

#endif // MYTCPCLIENT_H
