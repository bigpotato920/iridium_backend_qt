#ifndef IRIDIUMDIALOG_H
#define IRIDIUMDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QAbstractSocket>

class QTcpServer;
class QTcpSocket;
class QHostAddress;
class Coordinate;

namespace Ui {
class IridiumDialog;
}



class IridiumDialog : public QDialog
{
    Q_OBJECT

private slots:

    void sendMsg();
    void processConnection();
    void processReadyRead();
    void processDisconnect();
    void connectToGssServer();
    void connectedToGssServer();
    void processConfirm();
    void displayError(QAbstractSocket::SocketError socketError);
    void test();
    void refreshMap(double longitude, double latitude);
    void addJavaScriptObject();
public:
    explicit IridiumDialog(QWidget *parent = 0);
    ~IridiumDialog();

private:

    void initServerService();

    QTcpServer *terminalServer;
    QTcpSocket *gssClient;
    QTcpSocket *termialClient;

    quint16 gssPort;
    quint16 termialPort;

    enum {MAX_SENDBUF_LEN = 340};
    Coordinate *m_coordinate;

    Ui::IridiumDialog *ui;

};



#endif // IRIDIUMDIALOG_H
