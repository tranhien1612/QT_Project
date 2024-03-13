
#ifndef IPTHREAD_H
#define IPTHREAD_H

#include <QThread>
#include <QProcess>
#include <QTcpSocket>

class IpThread : public QThread
{
public:
    explicit IpThread(QObject* parent = 0);
    QString pingIp(QString ip);
    QString scanPort(QString host, quint16 portStart, quint16 portEnd);
};

#endif // IPTHREAD_H
