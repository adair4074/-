#include "mainwindow.h"
#include <QApplication>
#include <QApplication>
#include <qmath.h>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QPainter>
#include <Qpen>
#include <QDebug>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QString>
#include <QMainWindow>
#include <QLineEdit>
#include <QTreeWidget>
#include <QApplication>
#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include<QDateTime>


    static int dt=400;

    static unsigned int  pkgDataBuf[100]; // 心电图数据数组ecg2
    static unsigned char pkgDataBuf2[100]; // 数据长度最大为4血压
    static unsigned short int pkgDataBuf1[100]; // 数据长度最大为8血样

    class QWidgetEcgCom : public QWidget
    {
    public:
        QSerialPort *com;



        char status = 0;
        char pkgHead;
        char pkgDataHead;

        char pkgData[10];
        char pkgCrc;
        int  pkgDataCnt;
        int  pkgCnt;
        int ecg1, ecg2, ecg3;

        // 计算心率相关变量
        int ecg2PulseMax;
        int ecg2PulseMid;
        int ecg2Zero = 2048; // 无信号输入时输出结果为2048
        int ecg2Buff[2000]; //采样率为500=每秒钟采样500次，假设心率很慢2s一次，一个周期有1000个数据，这里取2000个
        unsigned int ecg2BuffIndex = 0;
        char prePulse;
        char pulse;
        long long pulseStart = 0, pulseEnd = 0;
        int ecg2Period;
        int ecg2Bpm;

        QWidgetEcgCom(QWidget*parent=0):QWidget(parent)
          {
            this->status=-1;
            this->serialInit();
        }

        int serialInit(void)
        {
            // 打印串口信息
            // foreach: 标准C++中没有这个关键字
            // qt自己实现了这个关键字，如下会便利availablePorts，并把每个结果依次赋值给info，有点像for
            qDebug()<<"serialInit";
            foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
            {
                qDebug() << "Name : " << info.portName();
                qDebug() << "Description : " << info.description();
                qDebug() << "Manufacturer: " << info.manufacturer();
                qDebug() << "Serial Number: " << info.serialNumber();
            }//执行完应该在应用程序输出位置看到串口数据，需要先启动串口模拟器，并创建串口

            // 创建串口对象, 在头文件中定义了指针
            com = new QSerialPort();
            // 配置串口, 根据创建串口号自行选择
            com->setPortName("COM6");
            // 尝试打开串口
            if(!com->open(QIODevice::ReadWrite))
            {
                qDebug("serial open err");
                return -1; //C程序中的返回值，一般正常返回时，返回0，异常为非零
            }
            else
                qDebug()<<"serial open success:"<<com->portName();

            // 配置串口参数
            // 波特率=115200, 每秒钟发送115200个bit，方向=双向均采用这个波特率
            com->setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
            // 数据位
            com->setDataBits(QSerialPort::Data8);
            // 流控=无
            com->setFlowControl(QSerialPort::NoFlowControl);
            // 校验位=无
            com->setParity(QSerialPort::NoParity);
            // 停止位=1
            com->setStopBits(QSerialPort::OneStop);

            // 关联信号与槽函数
            // 当串口发出"可读"信号时，执行serialRx函数
            connect(com, &QSerialPort::readyRead, this, &QWidgetEcgCom::serialRx);

            return 0;
        }


        void serialRx()
        {
            // 读串口数据并处理
            while (!com->atEnd())//缓冲区是否读完了
            {
                QByteArray data = this->com->read(1);//读一个字节
                this->rxDataHandle(data[0]);
                if (this->status == 4)
                {
                    // 发送信号画波形
                    // todo

                    // 计算心率
                    //ecgBpmCalc(ecg2);
                }
            }
        }

        void rxDataHandle(char data)
        {
            char dataTmp;
            // 状态机
            // 状态0：空闲状态， 接收到包头08后进入状态1
            // 状态1：接收数据头状态，收到的数据存入数据头，同时进入下一状态
            // 状态2：数据接收状态，用数据头中的相应数据为填补接收数据的最高位，并缓存数据，接收到特定数据长度后进入下一状态
            // 状态3：数据校验，接收到的数据为校验值，取数据低7为与其他数据部分的低7位进行验证

            data = data&0x0FF;
            int datamask = data&0x80;
            if(datamask == 0x00)//最高位为0
                this->status = 0;
            //qDebug("status:%d,data:%02X,datamask:%02X",this->status,data&0xFF,datamask);

            switch(this->status)
            {
                case 0:
                    if (data == 0x08)//ECG
                    {
                        this->status = 1;
                        this->pkgHead = data;
                        this->pkgDataCnt = 0;
                        //this->pkgCnt = 0;

                        qDebug()<<"get pkgHead";
                    }else
                    {
                        qDebug()<<"unknown cmdId"<<data;
                    }
                    break;
                case 1:
                    this->status = 2;
                    this->pkgDataHead = data;
                    qDebug()<<"get pkgDataHead";
                    break;
                case 2:
                    dataTmp = data & 0x7F; //保留数据的第7位
                    // 第1个数据的最高位在数据头的最低位中
                    //dataTmp = dataTmp | (((this->pkgDataHead >> this->pkgDataCnt) & 0x01) << 7);
                    this->pkgData[this->pkgDataCnt] = dataTmp; // 缓存解析后的数据
                    //this->pkgDataBuf[this->pkgDataCnt] = data; // 缓存解析前的数据
                    // 计数器中的值为数据头后面有效数据的字符数
                    this->pkgDataCnt = this->pkgDataCnt + 1;

                    // 根据通信协议可知数据长度最大为7
                    if (this->pkgDataCnt >= 7)
                        this->status = 3;

                    break;
                case 3:
                    // BCC（异或校验，一个简单的算法）
                    // 不提供，有学生自己设计校验方法
                    /*
                     * if data !=  this->bccCheck(this->pkgData, this->pkgDataCnt)
                     *      this->status = 0;
                     *      return ;
                     */

                    // 得到ecg数据
                    ecg1 = this->pkgData[0];
                    ecg1 = (ecg1 |((pkgDataHead &0x01)<<7 ))  <<8;
                    ecg1 = ecg1 | ((this->pkgData[1] | ((pkgDataHead &0x02)<<6))&0xFF);

                    ecg2 = this->pkgData[2];
                    //qDebug("ecg2=%02X mask=%02X ",ecg2,((pkgDataHead &0x04)<<5 ));
                    ecg2 = (ecg2 |((pkgDataHead &0x04)<<5 ))  <<8;
                    //qDebug("ecg2=%02X mask=%02X ",ecg2,((pkgDataHead &0x08)<<4));
                    ecg2 = ecg2 | ((this->pkgData[3] | ((pkgDataHead &0x08)<<4))&0xFF);
                    //qDebug("ecg2=%u",ecg2);

                    pkgDataBuf[this->pkgCnt] = ecg2;
                    //int ecg;
                    //ecg=this->pkgDataBuf[this->pkgCnt];
                    qDebug("ecg2=%u",pkgDataBuf[this->pkgCnt]);
                    //qDebug("ecg2:%x",pkgDataBuf[this->pkgCnt]);
                    if(pkgCnt<=100)
                    {
                    this->pkgCnt = this->pkgCnt + 1;
                    }
                    ecg3 = this->pkgData[4];
                    ecg3 = (ecg3 |((pkgDataHead &0x10)<<3 ))  <<8;
                    ecg3 = ecg3 | ((this->pkgData[5] | ((pkgDataHead &0x20)<<2))&0xFF);

                    // 调试用数据打印，绘制波形时应注释掉，不然会打印很多数据
                    qDebug()<<"ecg1="<<ecg1<<"ecg2="<<ecg2<<"ecg3="<<ecg3;

                    this->status = 4;
                    break;
            }
        }
};

class QWidgetSpoCom : public QWidget
{
public:
    QSerialPort *com1;

    char status1 = 0;
    char pkgHead1;
    char pkgDataHead1;

   //unsigned int pkgDataBuf1[]; // 数据长度最大为8血样
    char pkgData1[4];
    char pkgCrc1;
    int  pkgDataCnt1=0 ;
    int spo;
    char dataTmp;
    //int sp;

    QWidgetSpoCom(QWidget*parent=0):QWidget(parent)
    {
        this->status1=-1;
        this->serialInit1();
    }

    int serialInit1(void)
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

    void serialRx1()
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

    void rxDataHandle1(char data1)
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
                pkgDataBuf1[this->pkgDataCnt1] = dataTmp; // 缓存解析前的数据
                // 计数器中的值为数据头后面有效数据的字符数

                // 调试用数据打印，绘制波形时应注释掉，不然会打印很多数据

                qDebug("spo:%u",pkgDataBuf1[this->pkgDataCnt1]);
                //qDebug("spo11:%u",pkgDataBuf1[50]);
               if(pkgDataCnt1<=100)
               {
                this->pkgDataCnt1 = this->pkgDataCnt1 + 1;
                 }
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
};


class QWidgetRespCom : public QWidget
{
public:
    QSerialPort *com2;

    char status2 = 0;
    char pkgHead2;
    char pkgDataHead2;

   //unsigned int pkgDataBuf1[]; // 数据长度最大为8血样
    char pkgData2[4];
    char pkgCrc2;
    int  pkgDataCnt2=0 ;
    int spo;
    char dataTmp;
    //int sp;

    QWidgetRespCom(QWidget*parent=0):QWidget(parent)
    {
        this->status2=-1;
        this->serialInit2();
    }

    int serialInit2(void)
    {
        // 打印串口信息
        // foreach: 标准C++中没有这个关键字
        // qt自己实现了这个关键字，如下会便利availablePorts，并把每个结果依次赋值给info，有点像for
        qDebug()<<"serialInit2";
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            qDebug() << "Name : " << info.portName();
            qDebug() << "Description : " << info.description();
            qDebug() << "Manufacturer: " << info.manufacturer();
            qDebug() << "Serial Number: " << info.serialNumber();
        }//执行完应该在应用程序输出位置看到串口数据，需要先启动串口模拟器，并创建串口

        // 创建串口对象, 在头文件中定义了指针
        com2 = new QSerialPort();
        // 配置串口, 根据创建串口号自行选择
        com2->setPortName("COM8");
        // 尝试打开串口
        if(!com2->open(QIODevice::ReadWrite))
        {
            qDebug("serial open err");
            return -1; //C程序中的返回值，一般正常返回时，返回0，异常为非零
        }
        else
            qDebug()<<"serial open success:"<<com2->portName();

        // 配置串口参数
        // 波特率=115200, 每秒钟发送115200个bit，方向=双向均采用这个波特率
        com2->setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
        // 数据位
        com2->setDataBits(QSerialPort::Data8);
        // 流控=无
        com2->setFlowControl(QSerialPort::NoFlowControl);
        // 校验位=无
        com2->setParity(QSerialPort::NoParity);
        // 停止位=1
        com2->setStopBits(QSerialPort::OneStop);

        // 关联信号与槽函数
        // 当串口发出"可读"信号时，执行serialRx函数
        connect(com2, &QSerialPort::readyRead, this, &QWidgetRespCom::serialRx2);

        return 0;
    }

    void serialRx2()
    {
        // 读串口数据并处理
        while (!com2->atEnd())//缓冲区是否读完了
        {
            QByteArray data2 = this->com2->read(1);//读一个字节
            this->rxDataHandle2(data2[0]);
            if (this->status2 == 4)
            {
                // 发送信号画波形
                // todo

                // 计算心率
                //ecgBpmCalc(ecg2);
            }
        }
    }

    void rxDataHandle2(char data2)
    {
        //char dataTmp;
        // 状态机
        // 状态0：空闲状态， 接收到包头09后进入状态1
        // 状态1：接收数据头状态，收到的数据存入数据头，同时进入下一状态
        // 状态2：数据接收状态，用数据头中的相应数据为填补接收数据的最高位，并缓存数据，接收到特定数据长度后进入下一状态
        // 状态3：数据校验，接收到的数据为校验值，取数据低7为与其他数据部分的低7位进行验证

        data2 = data2&0xFF;
        int datamask = data2&0x80;
        if(datamask == 0x00)//最高位为0
            this->status2 = 0;

        //qDebug("status1:%d,data1:%02X,datamask1:%02X",this->status1,data1&0xFF,datamask);

        switch(this->status2)
        {
            case 0:
                if (data2 == 0x0a)//spo
                {
                    this->status2 = 1;
                    this->pkgHead2 = data2;
                    //this->pkgDataCnt2 = 0;
                    qDebug()<<"get pkgHead1";
                    //qDebug()<<data1;
                }else
                {
                    qDebug()<<"unknown cmdId1"<<data2;
                }
                break;
            case 1:
                this->status2 = 2;
                this->pkgDataHead2 = data2;
                dataTmp =data2&0x40;
                dataTmp =dataTmp <<1;
                qDebug()<<"get pkgDataHead2";
                break;
            case 2:


                dataTmp =dataTmp;
                data2=data2&0x7F;
                dataTmp = dataTmp |data2;
                //sp =dataTmp;
                pkgDataBuf2[this->pkgDataCnt2] = dataTmp; // 缓存解析前的数据
                // 计数器中的值为数据头后面有效数据的字符数

                // 调试用数据打印，绘制波形时应注释掉，不然会打印很多数据

                qDebug("spo:%u",pkgDataBuf2[this->pkgDataCnt2]);
                //qDebug("spo11:%u",pkgDataBuf1[50]);
               if(pkgDataCnt2<=100)
               {
                this->pkgDataCnt2 = this->pkgDataCnt2 + 1;
                 }
                // 根据通信协议可知数据长度最大为4
                //if (this->pkgDataCnt1 >= 4)
                    //this->status1 = 3;
                this->status2 = 3;

                break;
            case 3:


                this->status2 = 4;
                break;
        }
    }
};








class QWidgetDraw :public QWidget
{
public:
    int ecgDataIndex;
    qreal index;
    qreal range;
    QPixmap*pixmap;

    QWidgetDraw(QWidget*parent=0):QWidget(parent)
    {
        QBasicTimer*timer =new QBasicTimer();
        timer->start(10,this);//调速度

        pixmap=new QPixmap(1200,300);

        pixmap->fill(Qt::black);

        index=0;
        ecgDataIndex=0;
    }

    virtual void timerEvent(QTimerEvent*event)
    {

        drawWaveToPixmap();
        this->update();

    }

drawWaveToPixmap()
{
    QPainter pixPainter;
    pixPainter.begin(this->pixmap);
    drawEcgWave(&pixPainter);
    pixPainter.end();
}

    virtual void paintEvent(QPaintEvent*event)
    {
        QPainter*painter=new QPainter();

        painter->begin(this);
        painter->resetTransform();

        painter->drawPixmap(0,0,*pixmap);

        painter->resetTransform();
        painter->end();
    }

    drawEcgWave(QPainter*painter)
    {


    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor("#00FF00"));
    painter->setPen(pen);

    index=index+1;
    if(index>width())
        index=1;
    //擦除当前位置
    painter->save();
    pen .setColor(Qt::black);
    painter->setPen(pen);
    painter->drawLine(index,0,index,height());
    painter->restore();

qreal height=this->height();

QPoint lineStart;
QPoint lineEnd;

lineStart.setX(index -1);
lineEnd.setX(index);

qreal value=pkgDataBuf[ecgDataIndex];

int y=round(height/2-((value-2048)/dt) * height/2);
qDebug()<<y;
lineStart.setY(y);


    ecgDataIndex=ecgDataIndex+5;
    if(ecgDataIndex>=sizeof(pkgDataBuf)/sizeof(int))
    ecgDataIndex=0;
    value=pkgDataBuf[ecgDataIndex];
    y=round(height/2-((value-2048)/dt) * height/2);
    lineEnd.setY(y);
    painter->drawLine(lineStart,lineEnd);
}
};



////////////////////////////////////////////////////////



class QWidgetDraw_SOp2 :public QWidget
{
public:
    int ecgDataIndex;
    qreal index;
    qreal range;
    QPixmap*pixmap;

    QWidgetDraw_SOp2(QWidget*parent=0):QWidget(parent)
    {
        QBasicTimer*timer =new QBasicTimer();
        timer->start(10,this);//调速度

        pixmap=new QPixmap(1200,300);

        pixmap->fill(Qt::black);

        index=0;
        ecgDataIndex=0;
    }

    virtual void timerEvent(QTimerEvent*event)
    {

        drawWaveToPixmap();
        this->update();

    }

drawWaveToPixmap()
{
    QPainter pixPainter;
    pixPainter.begin(this->pixmap);
    drawEcgWave(&pixPainter);
    pixPainter.end();
}

    virtual void paintEvent(QPaintEvent*event)
    {
        QPainter*painter=new QPainter();

        painter->begin(this);
        painter->resetTransform();


        painter->drawPixmap(0,0,*pixmap);

        painter->resetTransform();
        painter->end();
    }

    drawEcgWave(QPainter*painter)
    {

    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor("red"));
    painter->setPen(pen);

    index=index+1;
    if(index>width())
        index=1;
    //擦除当前位置
    painter->save();
    pen .setColor(Qt::black);
    painter->setPen(pen);
    painter->drawLine(index,0,index,height());
    painter->restore();

qreal height=this->height();

QPoint lineStart;
QPoint lineEnd;

lineStart.setX(index -1);
lineEnd.setX(index);

qreal value=pkgDataBuf1[ecgDataIndex];
int y=round(height/2-((value)/200) * height/2);
//qDebug()<<y;
lineStart.setY(y);

    ecgDataIndex=ecgDataIndex+5;
    if(ecgDataIndex>=sizeof(pkgDataBuf1)/sizeof(short int))
    ecgDataIndex=0;
    value=pkgDataBuf1[ecgDataIndex];
    y=round(height/2-((value)/200) * height/2);
    lineEnd.setY(y);
    painter->drawLine(lineStart,lineEnd);
}
};



////////////////////////////////////////


class QWidgetDraw_RESP :public QWidget
{
public:
    int ecgDataIndex;
    qreal index;
    qreal range;
    QPixmap*pixmap;

    QWidgetDraw_RESP(QWidget*parent=0):QWidget(parent)
    {
        QBasicTimer*timer =new QBasicTimer();
        timer->start(10,this);//调速度

        pixmap=new QPixmap(1200,300);

        pixmap->fill(Qt::black);

        index=0;
        ecgDataIndex=0;
    }

    virtual void timerEvent(QTimerEvent*event)
    {
        drawWaveToPixmap();
        this->update();

    }

drawWaveToPixmap()
{
    QPainter pixPainter;
    pixPainter.begin(this->pixmap);
    drawEcgWave(&pixPainter);
    pixPainter.end();
}

    virtual void paintEvent(QPaintEvent*event)
    {
        QPainter*painter=new QPainter();

        painter->begin(this);
        painter->resetTransform();



        painter->drawPixmap(0,0,*pixmap);

        painter->resetTransform();
        painter->end();
    }

    drawEcgWave(QPainter*painter)
    {


    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor("yellow"));
    painter->setPen(pen);

    index=index+1;
    if(index>width())
        index=1;
    //擦除当前位置
    painter->save();
    pen .setColor(Qt::black);
    painter->setPen(pen);
    painter->drawLine(index,0,index,height());
    painter->restore();

qreal height=this->height();


QPoint lineStart;
QPoint lineEnd;

lineStart.setX(index -1);
lineEnd.setX(index);

qreal value=pkgDataBuf2[ecgDataIndex];
//qDebug()<<value;
int y=round(height/2-((value)/200) * height/2);
//qDebug()<<y;
lineStart.setY(y);


    ecgDataIndex=ecgDataIndex+5;
    if(ecgDataIndex>=sizeof(pkgDataBuf2)/sizeof(short int))
    ecgDataIndex=0;
    value=pkgDataBuf2[ecgDataIndex];
    y=round(height/2-((value)/200) * height/2);
    lineEnd.setY(y);
    painter->drawLine(lineStart,lineEnd);
}
};

////////////////////////////////////////



class showvt : public QLabel
{
public:
    showvt(QLabel*parent=0):QLabel(parent)
    {

        startTimer(10);

    }
    void timerEvent(QTimerEvent*ev)
    {
        static  int  ecgDataIndex=0;

        qreal height=151;

        qreal value=pkgDataBuf[ecgDataIndex];
        int y=round(height/2-((value-2048)/600) * height/2);


         ecgDataIndex=ecgDataIndex+5;
         if(ecgDataIndex>=sizeof(pkgDataBuf)/sizeof(int))
         ecgDataIndex=0;
         value=pkgDataBuf[ecgDataIndex];
         y=round(height/2-((value-2048)/600) * height/2);

        QString strValue;

        strValue = QString::number(y);

        this->setText(strValue);
        this->setStyleSheet("color:green;font:50px");
        this->update();

    }

};

class showco2 : public QLabel
{
public:
    showco2(QLabel*parent=0):QLabel(parent)
    {

        startTimer(10);

    }
    void timerEvent(QTimerEvent*ev)
    {
        static  int  ecgDataIndex=0;

        qreal height=151;

        qreal value=pkgDataBuf1[ecgDataIndex];
        int y=round(height/2-((value)/200) * height/2);


         ecgDataIndex=ecgDataIndex+5;
         if(ecgDataIndex>=sizeof(pkgDataBuf1)/sizeof(char))
         ecgDataIndex=0;
         value=pkgDataBuf1[ecgDataIndex];
         y=round(height/2-((value)/200) * height/2);

        QString strValue;

        strValue = QString::number(y);

        this->setText(strValue);
        this->setStyleSheet("color:red;font:50px");
        this->update();

    }

};


class showco3 : public QLabel
{
public:
    showco3(QLabel*parent=0):QLabel(parent)
    {

        startTimer(10);

    }
    void timerEvent(QTimerEvent*ev)
    {
        static  int  ecgDataIndex=0;

        qreal height=151;

        qreal value=pkgDataBuf1[ecgDataIndex];
        int y=round((value)/2 +63);


         ecgDataIndex=ecgDataIndex+5;
         if(ecgDataIndex>=sizeof(pkgDataBuf1)/sizeof(char))
         ecgDataIndex=0;
         value=pkgDataBuf1[ecgDataIndex];
         y=round((value)/2 +63 );

        QString strValue;

        strValue = QString::number(y);

        this->setText(strValue);
        this->setStyleSheet("color:red;font:25px");
        this->update();

    }

};

////////////////////////////////////////////////////////////////////////

class cha : public QPushButton
{


public:
    cha(QPushButton*parent=0):QPushButton(parent)
    {
        QWidget*ty=new QWidget();
        this->setText("×1");
        this->setStyleSheet("color:white;font:18px;background-color:rgb(0,0,0)");
        QPushButton*t1=new QPushButton();
        QPushButton*t2=new QPushButton();
        QPushButton*t3=new QPushButton();

        t1->setParent(ty);
        t1->move(30,0);
        t2->setParent(ty);
        t2->move(30,50);
        t3->setParent(ty);
        t3->move(30,100);
        t1->setText("×1");
        t2->setText("×1.5");
        t3->setText("×2");
        ty->resize(100,150);
        connect(t1,&QPushButton::clicked,this,wij);
        connect(t2,&QPushButton::clicked,this,wik);
        connect(t3,&QPushButton::clicked,this,wii);

        connect(this,&QPushButton::clicked,ty,&QWidget::show);
    }

//改变按钮文字
    void wij()
    {
        this->setText("×1");
        dt=400;
    }
    void wik()
    {
        this->setText("×1.5");
        dt=200;

    }
    void wii()
    {
    this->setText("×2");
        dt=100;
    }

};

/////////////////////////////////////////////////////////

class DatetimeLable : public QLabel
{
public:


    QTimer *tm;//定时器变量

DatetimeLable(QLabel*parent=0):QLabel(parent)
{
        this->setText(this->getCurrentTimeString());
        this->setStyleSheet("font:40px");
        tm = new QTimer(this);
        connect(tm,&QTimer::timeout,this,&DatetimeLable::refreshTime);//建立信号和曹的连接
        tm->start(1);//定时器开始工作
}

QString getCurrentTimeString()
{
return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
}

void refreshTime()
{
    this->setText(this->getCurrentTimeString());
}



};

//////////////////////////////////////////

class mainwin:public QMainWindow
{
public:
    mainwin(QMainWindow*parent=0):QMainWindow(parent)
    {
        QWidget*jj=new QWidget();//核心

        QLabel*title=new QLabel("ECGⅡ");
        title->setStyleSheet("color:white");
        QWidgetDraw*waveWin=new QWidgetDraw();

        QLabel*title_1=new QLabel("SPO2");
        title_1->setStyleSheet("color:white");
        QWidgetDraw_SOp2*waveWin_1=new QWidgetDraw_SOp2();

        QLabel*title_2=new QLabel("RESP");
        title_2->setStyleSheet("color:white");
        QWidgetDraw_RESP*waveWin_2=new QWidgetDraw_RESP();


        QVBoxLayout*layout=new QVBoxLayout();
        layout->addWidget(title,1);
        layout->addWidget(waveWin,5);

        QVBoxLayout*layout1=new QVBoxLayout();
        layout1->addWidget(title_1,1);
        layout1->addWidget(waveWin_1,5);

        QVBoxLayout*layout2=new QVBoxLayout();
        layout2->addWidget(title_2,1);
        layout2->addWidget(waveWin_2,5);

        QVBoxLayout*layoutx=new QVBoxLayout();
        layoutx->addLayout(layout);
        layoutx->addLayout(layout1);
        layoutx->addLayout(layout2);



        QWidget*widgetWaveEcg=new QWidget();
        widgetWaveEcg->setStyleSheet("background-color:rgb(0,0,0)");



        //////////////显示窗部分 第一个窗//////////
        QWidget * widgetHr =new QWidget();  // 显示窗部分 第一个窗
        widgetHr->setStyleSheet("background-color:black");
        QLabel*title_a=new QLabel("HR");
        title_a->setStyleSheet("color:white;font:20px;background-color:rgb(30,30,30)");


        QLabel*title_b=new QLabel("bpm");
        title_b->setStyleSheet("color:green;font:18px");
        title_b->setParent(widgetHr);
        title_b->move(150,0);

        QVBoxLayout*layout_1= new QVBoxLayout();
        layout_1->addWidget(title_a);
        layout_1->addWidget(widgetHr);
        layout_1->setStretch(0,1);
        layout_1->setStretch(1,20);

        showvt * Win_1=new showvt();
        Win_1->resize(50,40);
        Win_1->setParent(widgetHr);
        Win_1->move(70,30);

        //////////////显示窗部分 第二个窗//////////
        QWidget * widget111 =new QWidget();
        widget111->setStyleSheet("background-color:rgb(0,0,0)");
        QLabel*title_3=new QLabel("ST");
        title_3->setStyleSheet("color:white;font:20px;background-color:rgb(30,30,30)");

        QLabel*title_4=new QLabel("ST1-?-");
        title_4->setStyleSheet("color:green;font:25px");
        title_4->setParent(widget111);
        title_4->move(65,0);

        QLabel*title_5=new QLabel("ST2-?-");
        title_5->setStyleSheet("color:green;font:25px");
        title_5->setParent(widget111);
        title_5->move(65,30);

        QLabel*title_6=new QLabel("PVCs-?-");
        title_6->setStyleSheet("color:green;font:25px");
        title_6->setParent(widget111);
        title_6->move(60,60);

        QVBoxLayout*layout_2= new QVBoxLayout();
        layout_2->addWidget(title_3);
        layout_2->addWidget(widget111);
        layout_2->setStretch(0,1);
        layout_2->setStretch(1,20);

        //////////////显示窗部分 第三个窗//////////
        QWidget * widgetHr1 =new QWidget();
        widgetHr1->setStyleSheet("background-color:rgb(0,0,0)");
        QLabel*title_7=new QLabel("NIBP");
        title_7->setStyleSheet("color:white;font:20px;background-color:rgb(30,30,30)");

        QLabel*title_8=new QLabel("-?-");
        title_8->setStyleSheet("color:blue;font:20px");
        title_8->setParent(widgetHr1);
        title_8->move(30,50);

        QLabel*title_9=new QLabel("/-?-");
        title_9->setStyleSheet("color:blue;font:50px");
        title_9->setParent(widgetHr1);
        title_9->move(150,40);

        QLabel*title_10=new QLabel("-?-");
        title_10->setStyleSheet("color:blue;font:22px");
        title_10->setParent(widgetHr1);
        title_10->move(320,50);

        QVBoxLayout*layout_3= new QVBoxLayout();
        layout_3->addWidget(title_7);
        layout_3->addWidget(widgetHr1);
        layout_3->setStretch(0,1);
        layout_3->setStretch(1,20);

        //////////////显示窗部分 第四个窗//////////
        QWidget * widgetHr2 =new QWidget();
        widgetHr2->setStyleSheet("background-color:rgb(0,0,0)");
        QLabel*title_11=new QLabel("SpO2");
        title_11->setStyleSheet("color:white;font:20px;background-color:rgb(30,30,30)");

        QLabel*title_12=new QLabel("bpm");
        title_12->setStyleSheet("color:red;font:18px");
        title_12->setParent(widgetHr2);
        title_12->move(150,98);

        QVBoxLayout*layout_4= new QVBoxLayout();
        layout_4->addWidget(title_11);
        layout_4->addWidget(widgetHr2);
        layout_4->setStretch(0,1);
        layout_4->setStretch(1,20);

        showco2*Win_2=new showco2();
        Win_2->resize(50,40);
        Win_2->setParent(widgetHr2);
        Win_2->move(70,30);

        showco3*Win_x=new showco3();
        Win_x->resize(30,30);
        Win_x->setParent(widgetHr2);
        Win_x->move(110,90);

        //////////////显示窗部分 第五个窗//////////
        QWidget * widget333 =new QWidget();
        widget333->setStyleSheet("background-color:rgb(0,0,0)");
        QLabel*title_13=new QLabel("TEMP");
        title_13->setStyleSheet("color:white;font:20px;background-color:rgb(30,30,30)");

        QLabel*title_14=new QLabel("T1-?-");
        title_14->setStyleSheet("color:red;font:25px");
        title_14->setParent(widget333);
        title_14->move(20,40);

        QLabel*title_15=new QLabel("T2-?-");
        title_15->setStyleSheet("color:red;font:25px");
        title_15->setParent(widget333);
        title_15->move(20,80);

        QLabel*title_16=new QLabel("TD-?-");
        title_16->setStyleSheet("color:red;font:25px");
        title_16->setParent(widget333);
        title_16->move(100,60);

        QLabel*title_17=new QLabel("°C");
        title_17->setStyleSheet("color:red;font:15px");
        title_17->setParent(widget333);
        title_17->move(150,10);

        QVBoxLayout*layout_5= new QVBoxLayout();
        layout_5->addWidget(title_13);
        layout_5->addWidget(widget333);
        layout_5->setStretch(0,1);
        layout_5->setStretch(1,20);


        ///////////////显示窗部分 第六个窗//////////

        QWidget * widgetHr3 =new QWidget();
        widgetHr3->setStyleSheet("background-color:rgb(0,0,0)");
        QLabel*title_18=new QLabel("RESP");
        title_18->setStyleSheet("color:white;font:20px;background-color:rgb(30,30,30)");

        QLabel*title_19=new QLabel("-?-");
        title_19->setStyleSheet("color:yellow;font:50px");
        title_19->setParent(widgetHr3);
        title_19->move(60,30);

        QLabel*title_20=new QLabel("bpm");
        title_20->setStyleSheet("color:yellow;font:18px");
        title_20->setParent(widgetHr3);
        title_20->move(150,80);

        QVBoxLayout*layout_6= new QVBoxLayout();
        layout_6->addWidget(title_18);
        layout_6->addWidget(widgetHr3);
        layout_6->setStretch(0,1);
        layout_6->setStretch(1,20);

        ///////////////显示窗部分 第七个窗//////////
        QWidget * widget444 =new QWidget();
        widget444->setStyleSheet("background-color:rgb(0,0,0)");
        QLabel*title_21=new QLabel("CO2");
        title_21->setStyleSheet("color:white;font:20px;background-color:rgb(30,30,30)");

        QLabel*title_22=new QLabel("-?-");
        title_22->setStyleSheet("color:yellow;font:35px");
        title_22->setParent(widget444);
        title_22->move(60,30);

        QLabel*title_23=new QLabel("ins -?-");
        title_23->setStyleSheet("color:yellow;font:15px");
        title_23->setParent(widget444);
        title_23->move(30,90);

        QLabel*title_24=new QLabel("BWTT -?-");
        title_24->setStyleSheet("color:yellow;font:15px");
        title_24->setParent(widget444);
        title_24->move(120,90);

        QLabel*title_25=new QLabel("mmHg");
        title_25->setStyleSheet("color:yellow;font:15px");
        title_25->setParent(widget444);
        title_25->move(155,10);

        QVBoxLayout*layout_7= new QVBoxLayout();
        layout_7->addWidget(title_21);
        layout_7->addWidget(widget444);
        layout_7->setStretch(0,1);
        layout_7->setStretch(1,20);


        QHBoxLayout*layout111= new QHBoxLayout();
        layout111->addLayout(layout_1);
        layout111->addLayout(layout_2);

        QHBoxLayout*layout222= new QHBoxLayout();
        layout222->addLayout(layout_3);

        QHBoxLayout*layout333= new QHBoxLayout();
        layout333->addLayout(layout_4);
        layout333->addLayout(layout_5);


        QHBoxLayout*layout444= new QHBoxLayout();
        layout444->addLayout(layout_6);
        layout444->addLayout(layout_7);

        QVBoxLayout*layouththt= new QVBoxLayout();
        layouththt->addLayout(layout111);
        layouththt->addLayout(layout222);
        layouththt->addLayout(layout333);
        layouththt->addLayout(layout444);

        QHBoxLayout*layoutzyu=new QHBoxLayout();
        layoutzyu->addLayout(layoutx);
        layoutzyu->addLayout(layouththt);
        layoutzyu->setStretch(0,2);
        layoutzyu->setStretch(1,1);

        QWidget * widgetHruio =new QWidget(); //标题栏
        widgetHruio->setStyleSheet("background-color:rgb(30,30,30)");

        QLabel*title0=new QLabel();
        title0->setText("设备1");
        title0->move(20,15);
        title0->setStyleSheet("background-color:rgb(30,30,30);font:25px;color:white");
        title0->setParent(widgetHruio);


        QVBoxLayout*layoutMain=new QVBoxLayout();
        layoutMain->addWidget(widgetHruio);
        layoutMain->addLayout(layoutzyu);
        layoutMain->setStretch(0,1);
        layoutMain->setStretch(1,10);
        layoutMain->setSpacing(0);

        QPushButton*close=new QPushButton();
        close->setStyleSheet("background-color:rgb(30,30,30);color:white;font:20px");
        close->setText("×");
        close->resize(28,20);
        close->setParent(widgetHruio);
        close->move(1150,20);
        close->setFlat(true);
        connect(close,&QPushButton::clicked,this,&mainwin::close);

        QPushButton*close_1=new QPushButton();
        close_1->setStyleSheet("background-color:rgb(30,30,30);color:white;font:20px");
        close_1->setText("×");
        close_1->resize(28,20);
        close_1->setParent(widgetHruio);
        close_1->move(1850,20);
        close_1->setFlat(true);
        close_1->hide();
        connect(close_1,&QPushButton::clicked,this,&mainwin::close);



        QPushButton*fullwin=new QPushButton();
        fullwin->setStyleSheet("background-color:rgb(30,30,30);color:white;font:20px");
        fullwin->setText("□");
        fullwin->resize(28,20);
        fullwin->move(1100,20);
        fullwin->setParent(widgetHruio);
        fullwin->setFlat(true);

        QPushButton*swin_1=new QPushButton();
        swin_1->setStyleSheet("background-color:rgb(30,30,30);color:white;font:20px");
        swin_1->setText("❐");
        swin_1->resize(28,20);
        swin_1->move(1800,20);
        swin_1->setFlat(true);
        swin_1->setParent(widgetHruio);
        swin_1->hide();


        connect(fullwin,&QPushButton::clicked,this,&mainwin::showFullScreen);
        connect(fullwin,&QPushButton::clicked,fullwin,&QPushButton::hide);
        connect(fullwin,&QPushButton::clicked,swin_1,&QPushButton::show);
        connect(fullwin,&QPushButton::clicked,close_1,&QPushButton::show);
        connect(fullwin,&QPushButton::clicked,close,&QPushButton::hide);


        connect(swin_1,&QPushButton::clicked,this,&mainwin::showNormal);
        connect(swin_1,&QPushButton::clicked,swin_1,&QPushButton::hide);
        connect(swin_1,&QPushButton::clicked,fullwin,&QPushButton::show);
        connect(swin_1,&QPushButton::clicked,close_1,&QPushButton::hide);
        connect(swin_1,&QPushButton::clicked,close,&QPushButton::show);

        jj->setLayout(layoutMain);
        setCentralWidget(jj);

        this->resize(1200,660);
        this->setWindowFlags(Qt::FramelessWindowHint);
        this->setStyleSheet("background-color:rgb(0,0,0)");


        cha*tt=new cha();
        tt->resize(48,20);
        tt->setParent(this);
        tt->move(150,75);

        DatetimeLable*tmer=new DatetimeLable();
        tmer->setStyleSheet("background-color:rgb(30,30,30);color:red");
        tmer->resize(185,20);
        tmer->setParent(this);
        tmer->move(500,10);

    }



//    void paintEvent(QPaintEvent*)
//    {
//             QPainter ff(this);
//             QPen pen;
//             pen.setColor(QColor(255,255,255));
//             pen.setWidth(3);
//             ff.setPen(pen);
//             ff.drawLine(5,5,100,5);
//             ff.drawRect(QRect(5,5,1905,1150));

//    }


};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidgetEcgCom *com = new QWidgetEcgCom();
    QWidgetSpoCom *com1 = new QWidgetSpoCom();
    QWidgetRespCom *com2 = new QWidgetRespCom();

    mainwin  w;
    w.show();
    return a.exec();
}
