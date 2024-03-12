
#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H


#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSet>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject* parent = 0);
    void open(quint16 port);
    void close();
    void sendMsg(const char *data);
    void sendMsg(const QByteArray &data);

private slots:
    void newTcpConnection();
    void readyRead();

signals:
    void isListen(bool status);
    void tcpReceiveData(QByteArray data);

private:
    QTcpServer *m_server;
    QTcpSocket *m_socket;
};

#endif // MYTCPSERVER_H
