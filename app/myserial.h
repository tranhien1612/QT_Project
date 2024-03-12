
#ifndef MYSERIAL_H
#define MYSERIAL_H

#include <QObject>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <vector>


class MySerial : public QObject
{
    Q_OBJECT
public:
    explicit MySerial(QObject* parent = 0);
    void open(const QString &portName, qint32 baudRate);
    void close();
    void sendMsg(const char *data);
    void sendMsg(const QByteArray &data);

public slots:
    void readData();

signals:
    void isConnect(bool status);
    void serialReceiveData(QByteArray data);

private:
    QSerialPort* serial_;
};

#endif // MYSERIAL_H
