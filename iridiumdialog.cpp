#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QByteArray>
#include <QDebug>

#include "iridiumdialog.h"
#include "ui_iridiumdialog.h"

IridiumDialog::IridiumDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IridiumDialog)
{
    ui->setupUi(this);

    gssPort = 10800;
    termialPort = 8888;
    termialClient = new QTcpSocket;
    terminalServer = new QTcpServer;
    initServerService();
    connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(connectToGssServer()));
    connect(termialClient, SIGNAL(connected()), this, SLOT(connectedToGssServer()));
    connect(termialClient, SIGNAL(readyRead()), this, SLOT(processConfirm()));
    connect(termialClient, SIGNAL(error(QAbstractSocket::SocketError)), this ,SLOT(displayError(QAbstractSocket::SocketError)));
}

IridiumDialog::~IridiumDialog()
{
    delete ui;
}

void IridiumDialog::initServerService()
{

    terminalServer->listen(QHostAddress::Any, termialPort);
    connect(terminalServer, SIGNAL(newConnection()),this, SLOT(processConnection()));

}

void IridiumDialog::processConnection()
{
    ui->msgRcvEdit->append("new connection coming");
    gssClient = NULL;
    gssClient = terminalServer->nextPendingConnection();
    connect(gssClient, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
    connect(gssClient, SIGNAL(disconnected()), this, SLOT(processDisconnect()));
}

void IridiumDialog::processReadyRead()
{
    int bytesSize = gssClient->bytesAvailable();
    if (bytesSize == 0)
        return;
    quint8 proNumber;
    quint16 msgLen;

    quint16 payloadLen;
    QByteArray payload;
    QDataStream in(gssClient);

    in >> proNumber >> msgLen;
    if (msgLen <= 31)
    {
        ui->msgRcvEdit->append("Faile to receive msg");
        return;
    }

    in.skipRawData(46);
    in >> payloadLen;
    payload.resize(payloadLen);
    in.readRawData(payload.data(), payloadLen);
    ui->msgRcvEdit->append(payload);

}

void IridiumDialog::processDisconnect()
{
    ui->msgRcvEdit->append("gss client disconnected");
}



void IridiumDialog::connectToGssServer()
{

    ui->msgRcvEdit->append("prepare to connect to gss server");
    termialClient->abort();
    termialClient->connectToHost("12.47.179.12", gssPort);
    ui->sendButton->setEnabled(false);

}

void IridiumDialog::connectedToGssServer()
{
    ui->msgRcvEdit->append("connected to gss server");
    sendMsg();
    ui->msgRcvEdit->append("semd msg end...");


}

void IridiumDialog::processConfirm()
{
    QDataStream in(termialClient);
    in.skipRawData(29);
    short status;
    in >> status;
    ui->msgRcvEdit->append(QString::number(status));
    termialClient->close();
    ui->sendButton->setEnabled(true);
    ui->msgSendEdit->clear();
}

void IridiumDialog::displayError(QAbstractSocket::SocketError socketError)
{
    //qDebug() << socketError;
    ui->msgRcvEdit->append(QString(socketError));
}

void IridiumDialog::sendMsg()
{

    ui->msgRcvEdit->append("send msg...");

    quint8 proNumber = 1;
    quint16 msgLen = 27;
    quint8 headerIEI = 65;
    quint16 headerLen = 21;
    const char* clientID = "Msg1";
    const char* IMEI = "300234060747240";
    quint16  disposionFlags = 0;
    quint8 payloadIEI = 66;
    QString msg = ui->msgSendEdit->text();
    quint16 payloadLen = msg.length();

    msgLen += payloadLen;
    ui->msgRcvEdit->append("msgLen = " + QString::number(msgLen));
    ui->msgRcvEdit->append("payloadLen = " + QString::number(payloadLen));
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << proNumber << msgLen << headerIEI << headerLen;
    out.writeRawData(clientID, 4);
    out.writeRawData(IMEI, 15);

    out << disposionFlags << payloadIEI << payloadLen;
    out.writeRawData(msg.toStdString().c_str(), payloadLen);

    termialClient->write(block);

}


