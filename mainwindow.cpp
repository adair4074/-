#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include<stdio.h>

//这是一个宏，打印当前的函数名，行数，线程
//__FILE__是打印文件名
#define DEBUG_T qDebug("函数:%s,行:%d,线程Thread:%x",__FUNCTION__,__LINE__,QThread::currentThread())


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mserver= new QTcpServer(this);
    mserver->setMaxPendingConnections(2);//最大连接客户端数

    connect(mserver,SIGNAL(newConnection()),this,SLOT(tcpnewconnect()));
    //connect(mserver,&QTcpServer::newConnection,this,&MainWindow::tcpnewconnect);//另一种写法

    mserver->listen(QHostAddress::Any,1234);//开始监听
    DEBUG_T;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tcpnewconnect()
{

    msocket=mserver->nextPendingConnection();
    if(msocket)
    {
        mytcpsocket *mysoket=new mytcpsocket();//创建新的socke
        mysoket->setSocketDescriptor(msocket->socketDescriptor());//将新的socke描述符和旧的
        QThread *thread=new QThread();//创建新线程
        mysoket->moveToThread(thread);//把新的socke放到新线程

        connect(mysoket,SIGNAL(signalrec(QByteArray)),this,SLOT(tcpreadda(QByteArray)));
//        connect(mysoket,SIGNAL(disconnected()),thread,SLOT(quit()));
//        connect(mysoket,SIGNAL(disconnected()),thread,SLOT(deleteLater()));
        thread->start();


        qDebug()<<"客户端已连接";
        DEBUG_T;
        connect(msocket,SIGNAL(readyRead()),this,SLOT(tcpreadyread()));
        connect(msocket,SIGNAL(disconnected()),this,SLOT(tcpdisconnect()));

    }
}

void MainWindow::tcpreadyread()
{
    qDebug()<<"收到数据";
    ui->textEdit->append( msocket->readAll());
}



void MainWindow::tcpdisconnect()
{
    qDebug()<<"客户端已断开连接";
    msocket->deleteLater();
    msocket=0;
}

void MainWindow::tcpreadda(QByteArray ba)
{
    ui->textEdit->append( ba);
}

void MainWindow::on_pushButton_clicked()

{
    if(msocket)
    {
        msocket->write(ui->textEdit_2->toPlainText().toLatin1());

    }

}
