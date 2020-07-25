#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTcpServer>
#include<QThread>
#include<QObject>
#include<mytcpsocket.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void tcpnewconnect();//新建绑定server和newconnect的槽函数
    void tcpreadyread();//绑定信号
    void tcpdisconnect();
     void tcpreadda(QByteArray ba);


    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QTcpServer *mserver;
    QTcpSocket *msocket;
};

#endif // MAINWINDOW_H
