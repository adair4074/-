#include "qwidgetspocom.h"
#include "qwidgetecgcom.h"
#include <QApplication>
#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>



int QWidgetSpoCom::serialInit1(void)
{
    // 打印串口信息
    // foreach: 标准C++中没有这个关键字
    // qt自己实现了这个关键字，如下会便利availablePorts，并把每个结果依次赋值给info，有点像for
    qDebug()<<"serialInit1";
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "Serial Number: " << info.serialNumber();
    }//执行完应该在应用程序输出位置看到串口数据，需要先启动串口模拟器，并创建串口

    // 创建串口对象, 在头文件中定义了指针
    com1 = new QSerialPort();
    // 配置串口, 根据创建串口号自行选择
    com1->setPortName("COM4");
    // 尝试打开串口
    if(!com1->open(QIODevice::ReadWrite))
    {
        qDebug("serial open err");
        return -1; //C程序中的返回值，一般正常返回时，返回0，异常为非零
    }
    else
        qDebug()<<"serial open success:"<<com1->portName();

    // 配置串口参数
    // 波特率=115200, 每秒钟发送115200个bit，方向=双向均采用这个波特率
    com1->setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
    // 数据位
    com1->setDataBits(QSerialPort::Data8);
    // 流控=无
    com1->setFlowControl(QSerialPort::NoFlowControl);
    // 校验位=无
    com1->setParity(QSerialPort::NoParity);
    // 停止位=1
    com1->setStopBits(QSerialPort::OneStop);

    // 关联信号与槽函数
    // 当串口发出"可读"信号时，执行serialRx函数
    connect(com1, &QSerialPort::readyRead, this, &QWidgetSpoCom::serialRx1);

    return 0;
}

void QWidgetSpoCom::serialRx1()
{
    // 读串口数据并处理
    while (!com1->atEnd())//缓冲区是否读完了
    {
        QByteArray data1 = this->com1->read(1);//读一个字节
        this->rxDataHandle1(data1[0]);
        if (this->status1 == 4)
        {
            // 发送信号画波形
            // todo

            // 计算心率
            //ecgBpmCalc(ecg2);
        }
    }
}

void QWidgetSpoCom::rxDataHandle1(char data1)
{
    //char dataTmp;
    // 状态机
    // 状态0：空闲状态， 接收到包头09后进入状态1
    // 状态1：接收数据头状态，收到的数据存入数据头，同时进入下一状态
    // 状态2：数据接收状态，用数据头中的相应数据为填补接收数据的最高位，并缓存数据，接收到特定数据长度后进入下一状态
    // 状态3：数据校验，接收到的数据为校验值，取数据低7为与其他数据部分的低7位进行验证

    data1 = data1&0xFF;
    int datamask = data1&0x80;
    if(datamask == 0x00)//最高位为0
        this->status1 = 0;

    //qDebug("status1:%d,data1:%02X,datamask1:%02X",this->status1,data1&0xFF,datamask);

    switch(this->status1)
    {
        case 0:
            if (data1 == 0x09)//spo
            {
                this->status1 = 1;
                this->pkgHead1 = data1;
                this->pkgDataCnt1 = 0;
                qDebug()<<"get pkgHead1";
                //qDebug()<<data1;
            }else
            {
                qDebug()<<"unknown cmdId1"<<data1;
            }
            break;
        case 1:
            this->status1 = 2;
            this->pkgDataHead1 = data1;
            dataTmp =data1&0x40;
            dataTmp =dataTmp <<1;
            qDebug()<<"get pkgDataHead1";
            break;
        case 2:


            dataTmp =dataTmp;
            data1=data1&0x7F;
            dataTmp = dataTmp |data1;
            //sp =dataTmp;
            this->pkgDataBuf1[this->pkgDataCnt1] = dataTmp; // 缓存解析前的数据
            // 计数器中的值为数据头后面有效数据的字符数

            // 调试用数据打印，绘制波形时应注释掉，不然会打印很多数据

            qDebug("spo:%u",pkgDataBuf1[this->pkgDataCnt1]);
            //qDebug("spo11:%u",pkgDataBuf1[50]);

            this->pkgDataCnt1 = this->pkgDataCnt1 + 1;

            // 根据通信协议可知数据长度最大为4
            //if (this->pkgDataCnt1 >= 4)
                //this->status1 = 3;
            this->status1 = 3;

            break;
        case 3:


            this->status1 = 4;
            break;
    }
}

