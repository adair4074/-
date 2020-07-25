#include "mytcpsocket.h"
#include<stdint.h>
#include<QThread>
#define DEBUG_T qDebug("函数:%s,行:%d,线程Thread:%x",__FUNCTION__,__LINE__,QThread::currentThread())
#include <QDebug>

mytcpsocket::mytcpsocket(QObject *parent) : QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(tcpreadyread()));

    connect(this,SIGNAL(disconnected()),this,SLOT(tcpdisconnect()));

}

void mytcpsocket::tcpreadyread()
{
    DEBUG_T;

      QByteArray ba= this->readAll();
      emit signalrec(ba);
       qDebug()<<ba;
}

void mytcpsocket::tcpdisconnect()
{
    qDebug()<<"客户端已断开连接";
    this->deleteLater();
    //this=0;
}

