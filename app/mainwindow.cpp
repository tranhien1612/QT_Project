#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(1);

    ui->serial_sendBtn->setEnabled(false);
    ui->tcp_sendBtn->setEnabled(false);
    ui->tcpServer_btnSend->setEnabled(false);
    ui->pair_btnSend->setEnabled(false);

    //Tcp Server
    tcpSever = new MyTcpServer(this);
    connect(tcpSever, &MyTcpServer::isListen, this, &MainWindow::isListenTcpServer);
    connect(tcpSever, &MyTcpServer::tcpReceiveData, this, &MainWindow::readTcpSeverData);

    //Udp
    udp = new MyUdp(this);
    connect(udp, &MyUdp::udpReceiveData, this, &MainWindow::readUdpData);

    //Pair
    pair = new Pair(this);
    connect(pair, &Pair::isPair, this, &MainWindow::isPairStatus);
    connect(pair, &Pair::readPairSerialData, this, &MainWindow::readPairSerialData);
    connect(pair, &Pair::readPairTcpData, this, &MainWindow::readPairTcpData);

    ipScan = new IpThread(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Serial
void MainWindow::isConnectSerial(bool isConnect){
    serialIsConnect = isConnect;
    if(serialIsConnect){
        ui->serial_comBox->setEnabled(false);
        ui->baudrate_comBox->setEnabled(false);
        ui->serial_sendBtn->setEnabled(false);

        ui->serial_sendBtn->setEnabled(true);
        ui->serial_connectBtn->setText("Disconnect");
        QString text = "Connected to " + ui->serial_comBox->currentText() + ":" + ui->baudrate_comBox->currentText();
        ui->serial_logTxt->setTextColor(QColor(0,0,0));
        ui->serial_logTxt->append(text);
    }else{
        ui->serial_comBox->setEnabled(true);
        ui->baudrate_comBox->setEnabled(true);
        ui->serial_sendBtn->setEnabled(false);
        ui->serial_connectBtn->setText("Connect");

        ui->serial_logTxt->setTextColor(QColor(0,0,0));
        ui->serial_logTxt->append("Disconnected");
    }
}

void MainWindow::readSerialData(QByteArray data){
    QString text;
//    qInfo() << "Rx:" << data.toHex(' ');
    if(ui->serial_checkBoxHexa->isChecked()){
        text = "Receiver: " + data.toHex(' ');
    }else{
        text = "Receiver: " + data;
    }

    ui->serial_logTxt->setTextColor(QColor(0,0,255));
    ui->serial_logTxt->append(text);
}

void MainWindow::on_serial_btnClear_clicked()
{
    ui->serial_logTxt->clear();
}

void MainWindow::on_serial_connectBtn_clicked()
{
    bool isOk;
    if(!serialIsConnect){
        serial = new MySerial(this);
        connect(serial, &MySerial::isConnect, this, &MainWindow::isConnectSerial);
        connect(serial, &MySerial::serialReceiveData, this, &MainWindow::readSerialData);

        qint32 baudrate = ui->baudrate_comBox->currentText().toInt(&isOk);
        QString portName = ui->serial_comBox->currentText();
        qDebug() << "Port:" << portName <<"baudrate: " << baudrate;
        serial->open(portName, baudrate);
    }else{
        serial->close();
        delete serial;
    }
}

void MainWindow::on_serial_sendBtn_clicked()
{
    QString str = ui->serial_sendTxt->text();
    if(serialIsConnect && !str.isEmpty()){
        QByteArray data = str.toLocal8Bit();

        if(ui->serial_checkBoxHexaSend->isChecked()){
            QByteArray msg = QByteArray::fromHex(data);
            serial->sendMsg(msg);
        }else{
            const char *msg = data.data();
            serial->sendMsg(msg);
        }

        QString text = "Send: " + str;
        ui->serial_logTxt->setTextColor(QColor(255,0,0));
        ui->serial_logTxt->append(text);
    }
}

//Tcp Client
void MainWindow::isConnectTcpClient(bool status){
    isTcpClientConnect = status;
    if(isTcpClientConnect){
        ui->tcp_ipTxt->setEnabled(false);
        ui->tcp_portTxt->setEnabled(false);
        ui->tcp_sendBtn->setEnabled(true);
        ui->tcp_connectBtn->setText("Disconnect");
        ui->tcp_logTxt->setTextColor(QColor(0,0,0));
        ui->tcp_logTxt->append("Connected");
    }else{
        ui->tcp_ipTxt->setEnabled(true);
        ui->tcp_portTxt->setEnabled(true);
        ui->tcp_sendBtn->setEnabled(false);
        ui->tcp_connectBtn->setText("Connect");
        ui->tcp_sendBtn->setEnabled(false);
        ui->tcp_logTxt->setTextColor(QColor(0,0,0));
        ui->tcp_logTxt->append("Disconnect");
    }
}

void MainWindow::readTcpClientData(QByteArray data){
    QString text;
    if(ui->tcp_checkBoxHexa->isChecked()){
        text = "Receiver: " + data.toHex(' ');
    }else{
        text = "Receiver: " + data;
    }
    ui->tcp_logTxt->setTextColor(QColor(0,0,255));
    ui->tcp_logTxt->append(text);
}

void MainWindow::on_tcp_connectBtn_clicked()
{
    bool isOk;
    QString host = ui->tcp_ipTxt->text();
    quint16 port = ui->tcp_portTxt->text().toUInt(&isOk);
    qDebug() << host << port;

    QHostAddress tmp(host);
    if (QAbstractSocket::IPv4Protocol != tmp.protocol() || port == 0){
        ui->tcp_logTxt->setTextColor(QColor(0,0,0));
        ui->tcp_logTxt->append("Unknown or invalid address or port");
        return;
    }


    if(!isTcpClientConnect && !host.isEmpty() && port != 0){

        tcpClient = new MyTcpClient(this);
        connect(tcpClient, &MyTcpClient::isConnect, this, &MainWindow::isConnectTcpClient);
        connect(tcpClient, &MyTcpClient::tcpReceiveData, this, &MainWindow::readTcpClientData);

        QString text = "Connecting to " + host + ":" + QString::number(port);
        ui->tcp_logTxt->setTextColor(QColor(0,0,0));
        ui->tcp_logTxt->append(text);
        tcpClient->run(host, port);
    }else{
        tcpClient->close();
        delete tcpClient;
    }
}

void MainWindow::on_tcp_sendBtn_clicked()
{
    QString str = ui->tcp_sendTxt->text();
    if(isTcpClientConnect && !str.isEmpty()){
        QByteArray data = str.toLocal8Bit();

        if(ui->tcp_checkBoxHexaSend->isChecked()){
            QByteArray msg = QByteArray::fromHex(data);
            tcpClient->sendMsg(msg);
        }else{
            const char *msg = data.data();
            tcpClient->sendMsg(msg);
        }

        QString text = "Send: " + str;
        ui->tcp_logTxt->setTextColor(QColor(255,0,0));
        ui->tcp_logTxt->append(text);
    }
}

void MainWindow::on_tcp_btnClear_clicked()
{
    ui->tcp_logTxt->clear();
}

//Tcp Server
void MainWindow::isListenTcpServer(bool status){
    isTcpServerListen = status;
    qDebug() << "Status: " << status;
    if(isTcpServerListen){
        ui->tcpServer_LineTxtPort->setEnabled(false);
        ui->tcpServer_btnListen->setText("Stop");
        ui->tcpServer_btnSend->setEnabled(true);

        ui->tcpServer_logTxt->setTextColor(QColor(0,0,0));
        ui->tcpServer_logTxt->append("Listened!");

    }else{
        ui->tcpServer_LineTxtPort->setEnabled(true);
        ui->tcpServer_btnSend->setEnabled(false);
        ui->tcpServer_btnListen->setText("Listen");

        ui->tcpServer_logTxt->setTextColor(QColor(0,0,0));
        ui->tcpServer_logTxt->append("Stop!");

    }
}

void MainWindow::readTcpSeverData(QByteArray data){
    QString text;
    if(ui->tcpServer_checkBoxHexaReceive->isChecked()){
        text = "Receiver: " + data.toHex(' ');
    }else{
        text = "Receiver: " + data;
    }
    ui->tcpServer_logTxt->setTextColor(QColor(0,0,255));
    ui->tcpServer_logTxt->append(text);
}

void MainWindow::on_tcpServer_btnListen_clicked()
{
    bool isOk;
    quint16 port = ui->tcpServer_LineTxtPort->text().toUInt(&isOk);
    if(port == 0){
        ui->tcpServer_logTxt->setTextColor(QColor(0,0,0));
        ui->tcpServer_logTxt->append("Unknown or invalid port.");
        return;
    }

    if(!isTcpServerListen && port != 0){
        QString text = "Listenning to port: "  + QString::number(port);
        ui->tcpServer_logTxt->setTextColor(QColor(0,0,0));
        ui->tcpServer_logTxt->append(text);
        tcpSever->open(port);
    }else{
        tcpSever->close();
    }
}

void MainWindow::on_tcpServer_btnClear_clicked()
{
    ui->tcpServer_logTxt->clear();
}

void MainWindow::on_tcpServer_btnSend_clicked()
{
    QString str = ui->tcpServer_LineTxtSend->text();
    if(isTcpServerListen && !str.isEmpty()){
        QByteArray data = str.toLocal8Bit();

        if(ui->tcpServer_checkBoxHexaSend->isChecked()){
            QByteArray msg = QByteArray::fromHex(data);
            tcpSever->sendMsg(msg);
        }else{
            const char *msg = data.data();
            tcpSever->sendMsg(msg);
        }

        QString text = "Send: " + str;
        ui->tcpServer_logTxt->setTextColor(QColor(255,0,0));
        ui->tcpServer_logTxt->append(text);
    }
}

//Udp
void MainWindow::readUdpData(QByteArray data){
    QString text;
    if(ui->udp_checkBoxHexa->isChecked()){
        text = "Receiver: " + data.toHex(' ');
    }else{
        text = "Receiver: " + data;
    }
    ui->udp_logTxt->setTextColor(QColor(0,0,255));
    ui->udp_logTxt->append(text);

}

void MainWindow::on_udp_btnClear_clicked()
{
    ui->udp_logTxt->clear();
}

void MainWindow::on_udp_connectBtn_clicked()
{
    bool isOk;
    quint16 udpPort = ui->udp_portTxt->text().toUInt(&isOk);
    udp->start(udpPort);
}

//Pair
void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(ui->tabWidget->tabText(index) == "Pair"){
        ui->pair_checkBoxSerialEnable->setCheckState(Qt::CheckState::Checked);
        ui->pair_checkBoxTcpEnable->setCheckState(Qt::CheckState::Unchecked);

        ui->pair_checkBoxTcpClient->setCheckState(Qt::CheckState::Checked);
        ui->pair_checkBoxTcpServer->setCheckState(Qt::CheckState::Unchecked);
    }

    ui->pair_comboBox->clear();
    ui->serial_comBox->clear();
    foreach (const QSerialPortInfo &_port, QSerialPortInfo::availablePorts()){
        ui->serial_comBox->addItem(_port.portName());
        ui->pair_comboBox->addItem(_port.portName());
    }

}

void MainWindow::isPairStatus(bool status){
    isPair = status;
    qDebug() << "Status: " << status;

    ui->pair_checkBoxTcpClient->setEnabled(!isPair);
    ui->pair_checkBoxTcpServer->setEnabled(!isPair);

    ui->pair_btnSend->setEnabled(isPair);
    ui->pair_comboBox->setEnabled(!isPair);
    ui->part_comBoxBaudrate->setEnabled(!isPair);
    ui->pair_lineTxtPort->setEnabled(!isPair);

    if(ui->pair_checkBoxTcpServer->isChecked())
        ui->pair_lineTxtIp->setEnabled(false);
    else
        ui->pair_lineTxtIp->setEnabled(!isPair);

    if(isPair){
        ui->pair_btnPair->setText("Stop");
        ui->pair_LogTxt->setTextColor(QColor(0,0,0));
        ui->pair_LogTxt->append("Pair Successfully!");
    }else{
        ui->pair_btnPair->setText("Pair");
        ui->pair_LogTxt->setTextColor(QColor(0,0,0));
        ui->pair_LogTxt->append("Pair Stop!");
//        delete pair;
    }

}

void MainWindow::readPairTcpData(QByteArray data){
    QString text;
    if(ui->pair_checkBoxHexaReceive->isChecked()){
        text = "Receiver TCP: " + data.toHex(' ');
    }else{
        text = "Receiver TCP: " + data;
    }
    ui->pair_LogTxt->setTextColor(QColor(0,0,255));
    ui->pair_LogTxt->append(text);
}

void MainWindow::readPairSerialData(QByteArray data){
    QString text;
    if(ui->pair_checkBoxHexaReceive->isChecked()){
        text = "Receiver Serial: " + data.toHex(' ');
    }else{
        text = "Receiver Serial: " + data;
    }
    ui->pair_LogTxt->setTextColor(QColor(0,0,255));
    ui->pair_LogTxt->append(text);
}

void MainWindow::on_pair_btnPair_clicked()
{
    bool isOk;
    QString portName = ui->pair_comboBox->currentText();
    qint32 baudrate = ui->part_comBoxBaudrate->currentText().toInt(&isOk);

    QString host = ui->pair_lineTxtIp->text();
    quint16 port = ui->pair_lineTxtPort->text().toUInt(&isOk);

    if(!isPair){
        if(ui->pair_checkBoxTcpServer->isChecked()){
            if(port != 0){
                QString text = "Pair " + portName +":"+ QString::number(baudrate) + " and TcpServer port " + QString::number(port);
                ui->pair_LogTxt->setTextColor(QColor(0,0,0));
                ui->pair_LogTxt->append(text);

                pair->start(portName, baudrate, host, port);
            }
        }else{ //TCP Client
            QHostAddress tmp(host);
            if (QAbstractSocket::IPv4Protocol != tmp.protocol()){
                ui->pair_LogTxt->setTextColor(QColor(0,0,0));
                ui->pair_LogTxt->append("Unknown or invalid address or port");
                return;
            }
            if(!host.isEmpty() && port != 0){
                QString text = "Pair " + portName +":"+ QString::number(baudrate) + " and TcpClient " + host + ":" + QString::number(port);
                ui->pair_LogTxt->setTextColor(QColor(0,0,0));
                ui->pair_LogTxt->append(text);

                pair->start(portName, baudrate, host, port);
            }
        }
    }else{
        pair->stop();
        if(ui->pair_checkBoxTcpServer->isChecked())
            isPairStatus(false);
    }
}

void MainWindow::on_pair_btnSend_clicked()
{
    QString str = ui->pair_lineTxtSend->text();
    if(isPair && !str.isEmpty()){
        QByteArray data = str.toLocal8Bit();
        QString text = "Send: " + str;

        if(ui->pair_checkBoxHexaSend->isChecked()){
            QByteArray msg = QByteArray::fromHex(data);

            if(ui->pair_checkBoxSerialEnable->isChecked()){
                if(ui->pair_checkBoxTcpClient->isChecked())
                    pair->tcpClientSendMsg(msg);
                else
                    pair->tcpServerSendMsg(msg);
            }else{
                pair->serialSendMsg(msg);
            }

        }else{
            const char *msg = data.data();

            if(ui->pair_checkBoxTcpEnable->isChecked()){
                if(ui->pair_checkBoxTcpClient->isChecked())
                    pair->tcpClientSendMsg(msg);
                else
                    pair->tcpServerSendMsg(msg);
            }else
                pair->serialSendMsg(msg);

        }

        ui->pair_LogTxt->setTextColor(QColor(255,0,0));
        ui->pair_LogTxt->append(text);
    }
}

void MainWindow::on_pair_btnClear_clicked()
{
    ui->pair_LogTxt->clear();
}

void MainWindow::on_pair_checkBoxSerialEnable_stateChanged(int arg1)
{
    if(ui->pair_checkBoxSerialEnable->isChecked()){
        ui->pair_checkBoxTcpEnable->setCheckState(Qt::CheckState::Unchecked);
    }else{
        ui->pair_checkBoxTcpEnable->setCheckState(Qt::CheckState::Checked);
    }
}

void MainWindow::on_pair_checkBoxTcpEnable_stateChanged(int arg1)
{
    if(ui->pair_checkBoxTcpEnable->isChecked()){
        ui->pair_checkBoxSerialEnable->setCheckState(Qt::CheckState::Unchecked);
    }else{
        ui->pair_checkBoxSerialEnable->setCheckState(Qt::CheckState::Checked);
    }
}

void MainWindow::on_pair_checkBoxTcpClient_stateChanged(int arg1)
{
    if(ui->pair_checkBoxTcpClient->isChecked()){
        ui->pair_checkBoxTcpServer->setCheckState(Qt::CheckState::Unchecked);
        ui->pair_lineTxtIp->setEnabled(true);
    }else{
        ui->pair_checkBoxTcpServer->setCheckState(Qt::CheckState::Checked);
        ui->pair_lineTxtIp->setEnabled(false);
    }
}

void MainWindow::on_pair_checkBoxTcpServer_stateChanged(int arg1)
{
    if(ui->pair_checkBoxTcpServer->isChecked()){
        ui->pair_checkBoxTcpClient->setCheckState(Qt::CheckState::Unchecked);
        ui->pair_lineTxtIp->setEnabled(false);
        ui->pair_lineTxtIp->clear();
    }else{
        ui->pair_checkBoxTcpClient->setCheckState(Qt::CheckState::Checked);
        ui->pair_lineTxtIp->setEnabled(true);
    }
}

//Scan Port
void MainWindow::on_scan_btnPing_clicked()
{
    QString strIp = ui->scan_IpPing->text();
    QHostAddress tmp(strIp);
    if (QAbstractSocket::IPv4Protocol != tmp.protocol()){
        ui->scan_logTxt->append("Unknown or invalid address");
        return;
    }
    QString data = ipScan->pingIp(strIp);
    ui->scan_logTxt->append(data);
}

void MainWindow::on_scan_btnPort_clicked()
{
    bool isOk;
    quint16 portStart = ui->scan_portStart->text().toInt(&isOk);
    quint16 portEnd = ui->scan_portEnd->text().toInt(&isOk);
    QString host = ui->scan_host->text();
    ui->scan_logTxt->append("\nStart Scan Port from " + ui->scan_portStart->text() + " to " + ui->scan_portEnd->text() + " of " + host);
    QString data = ipScan->scanPort(host, portStart, portEnd);
    ui->scan_logTxt->append(data);
}

void MainWindow::on_scan_btnIp_clicked()
{
    //ToDo Scan Ip
}

void MainWindow::on_scan_btnClear_clicked()
{
    ui->scan_logTxt->clear();
}


