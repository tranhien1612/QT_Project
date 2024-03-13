#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QProcess>

#include <QEventLoop>
#include <QTimer>
#include <QDateTime>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QThread>
#include <QQuaternion>

#include <QMutex>
#include <QHostInfo>

#include "myserial.h"
#include "myudp.h"
#include "mytcpclient.h"
#include "mytcpserver.h"
#include "pair.h"

#include "ipthread.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tcp_connectBtn_clicked();
    void on_tcp_sendBtn_clicked();
    void on_udp_connectBtn_clicked();
    void on_serial_connectBtn_clicked();
    void on_serial_sendBtn_clicked();

    void on_tcpServer_btnListen_clicked();
    void on_tcpServer_btnClear_clicked();
    void on_tcpServer_btnSend_clicked();

    void on_udp_btnClear_clicked();
    void on_serial_btnClear_clicked();
    void on_tcp_btnClear_clicked();

    void on_tabWidget_currentChanged(int index);
    void on_pair_btnPair_clicked();
    void on_pair_btnClear_clicked();
    void on_pair_btnSend_clicked();
    void on_pair_checkBoxSerialEnable_stateChanged(int arg1);
    void on_pair_checkBoxTcpEnable_stateChanged(int arg1);

    //Ip
    void on_scan_btnPing_clicked();
    void on_scan_btnPort_clicked();
    void on_scan_btnIp_clicked();
    void on_scan_btnClear_clicked();

    void on_pair_checkBoxTcpClient_stateChanged(int arg1);

    void on_pair_checkBoxTcpServer_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    bool isTcpClientConnect = false;
    MyTcpClient* tcpClient = nullptr;

    bool isTcpServerListen = false;
    MyTcpServer* tcpSever = nullptr;

    MyUdp* udp = nullptr;
    QThread *udpThread;

    MySerial* serial = nullptr;
    bool serialIsConnect = false;

    Pair* pair = nullptr;
    bool isPair = false;

    IpThread* ipScan = nullptr;


public slots:
    //tcp
    void isConnectTcpClient(bool status);
    void readTcpClientData(QByteArray data);
    //udp
    void readUdpData(QByteArray data);
    //serial
    void isConnectSerial(bool isConnect);
    void readSerialData(QByteArray data);
    //tcpServer
    void isListenTcpServer(bool status);
    void readTcpSeverData(QByteArray data);

    //pair
    void isPairStatus(bool status);
    void readPairTcpData(QByteArray data);
    void readPairSerialData(QByteArray data);

};
#endif // MAINWINDOW_H
