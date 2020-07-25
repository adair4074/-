#include "mainwindow.h"
#include <QApplication>
#include<QtSerialPort/QSerialPortInfo>
#include<QtSerialPort/QSerialPort>
#include<QDebug>
#include<QWidget>
#include<QTimer>
const  int ecgData []= {
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2008, 2016, 2016, 2016, 2024, 2032, 2048,
    2064, 2064, 2064, 2072, 2080, 2080, 2080, 2088, 2096, 2104,
    2112, 2112, 2112, 2112, 2112, 2112, 2104, 2096, 2088,
    2080, 2080, 2080, 2072, 2064, 2064, 2064, 2048, 2032, 2032,
    2032, 2016, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 1992, 1984, 1976,
    1968, 1960, 1952, 1944, 1936, 1944, 1952, 2016, 2080, 2136,
    2192, 2256, 2320, 2376, 2432, 2488, 2544, 2568, 2592, 2536,
    2480, 2424, 2368, 2304, 2240, 2184, 2128, 2072, 2016, 1968,
    1920, 1928, 1936, 1944, 1952, 1960, 1968, 1984, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2008, 2016, 2024, 2032, 2032,
    2032, 2048, 2064, 2064, 2064, 2072, 2080, 2088, 2096, 2104,
    2112, 2112, 2112, 2120, 2128, 2136, 2144, 2152, 2160, 2160,
    2160, 2160, 2160, 2168, 2176, 2176, 2176, 2184, 2192,
    2192, 2192, 2192, 2200, 2208, 2208, 2208, 2208, 2208, 2208,
    2208, 2200, 2192, 2192, 2192, 2184, 2176, 2176, 2176, 2168,
    2160, 2160, 2160, 2144, 2128, 2128, 2128, 2128, 2128, 2112,
    2096, 2088, 2080, 2072, 2064, 2064, 2064, 2048, 2032, 2024,
    2016, 2016, 2016, 2008, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000
    };
const  unsigned  short int  spoData []={
    0x21,  0x21,  0x22,  0x22,  0x23,  0x22,  0x22,  0x21,  0x21,
    0x20,  0x20,  0x1f,  0x1e,  0x1d,  0x1c,  0x1b,  0x1b,  0x1a,
    0x19,  0x18,  0x18,  0x17,  0x16,  0x15,  0x14,  0x13,  0x12,
    0x11,  0x11,  0x10,  0x10,  0x0f,  0x0f,  0x0e,  0x0d,  0x0c,
    0x0c,  0x0b,  0x0a,  0x09,  0x09,  0x08,  0x07,  0x06,  0x06,
    0x05,  0x05,  0x04,  0x04,  0x03,  0x03,  0x02,  0x02,  0x01,
    0x01,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,
    0x00,  0x01,  0x03,  0x06,  0x0a,  0x0e,  0x13,  0x18,  0x1d,
    0x23,  0x28,  0x2d,  0x32,  0x37,  0x3b,  0x40,  0x44,  0x49,
    0x4d,  0x51,  0x54,  0x57,  0x58,  0x5a,  0x5a,  0x5a,  0x59,
    0x59,  0x57,  0x55,  0x52,  0x50,  0x4c,  0x49,  0x45,  0x42,
    0x3e,  0x3b,  0x37,  0x34,  0x31,  0x2f,  0x2c,  0x29,  0x26,
    0x24,  0x22,  0x20,  0x1f,  0x1f,  0x1e,  0x1e,  0x1d,  0x1d,
    0x1e,  0x1f,  0x20,  0x21,  0x21,  0x22,  0x22,  0x23,  0x22,
    0x22,  0x21,  0x21,  0x20,  0x20,  0x1f,  0x1e,  0x1d,  0x1c,
    0x1b,  0x1b,  0x1a,  0x19,  0x18,  0x18,  0x17,  0x16,  0x15,
    0x14,  0x13,  0x12,  0x11,  0x11,  0x10,  0x10,  0x0f,  0x0f,
    0x0e,  0x0d,  0x0c,  0x0c,  0x0b,  0x0a,  0x09,  0x09,  0x08,
    0x07,  0x06,  0x06,  0x05,  0x05,  0x04,  0x04,  0x03,  0x03,
    0x02,  0x02,  0x01,  0x01,  0x00,  0x00,  0x00,  0x00,  0x00,
    0x00,  0x00,  0x00,  0x00,  0x01,  0x03,  0x06,  0x0a,  0x0e,
    0x13,  0x18,  0x1d,  0x23,  0x28,  0x2d,  0x32,  0x37,  0x3b,
    0x40,  0x44,  0x49,  0x4d,  0x51,  0x54,  0x57,  0x58,  0x5a,
    0x5a,  0x5a,  0x59,  0x59,  0x57,  0x55,  0x52,  0x50,  0x4c,
    0x49,  0x45,  0x42,  0x3e,  0x3b,  0x37,  0x34,  0x31,  0x2f,
    0x2c,  0x29,  0x26,  0x24,  0x22,  0x20,  0x1f,  0x1f,  0x1e,
    0x1e,  0x1d,  0x1d,  0x1e,  0x1f,  0x20,  0x21,  0x21,  0x22,
    0x22,  0x23,  0x22,  0x22,  0x21
};
const unsigned short int respData []={
    0x0f, 0x10, 0x10, 0x11, 0x12, 0x12, 0x12, 0x13, 0x13,
    0x14, 0x14, 0x14, 0x14, 0x15, 0x17, 0x17, 0x17, 0x18,
    0x18, 0x19, 0x19, 0x19, 0x19, 0x1a, 0x19, 0x1a, 0x1a,
    0x1b, 0x1b, 0x1b, 0x1c, 0x1c, 0x1b, 0x1b, 0x1a, 0x1a,
    0x1a, 0x19, 0x19, 0x18, 0x18, 0x18, 0x17, 0x17, 0x15,
    0x15, 0x15, 0x14, 0x15, 0x14, 0x14, 0x14, 0x15, 0x15,
    0x14, 0x14, 0x15, 0x14, 0x14, 0x14, 0x15, 0x17, 0x17,
    0x17, 0x17, 0x18, 0x18, 0x19, 0x19, 0x19, 0x1a, 0x1b,
    0x1c, 0x1d, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x24,
    0x24, 0x26, 0x26, 0x26, 0x26, 0x24, 0x23, 0x23, 0x23,
    0x23, 0x22, 0x21, 0x20, 0x1d, 0x1d, 0x1c, 0x1c, 0x1c,
    0x1c, 0x1c, 0x1c, 0x1c, 0x1b, 0x1c, 0x1b, 0x1a, 0x1a,
    0x19, 0x18, 0x17, 0x15, 0x14, 0x12, 0x12, 0x11, 0x10,
    0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,0x09
  };
class QWidgetSerialTx:public QWidget
{
public:
    QWidgetSerialTx(QWidget *parent =nullptr):QWidget(parent)
    {
        if(0 !=this->serialInit()){
            qDebug()<<"SerialInit error";
            return;
        }
        timer =new QTimer();
        connect(timer,&QTimer::timeout,this,&QWidgetSerialTx::sendEcgPkg);
        timer->start(2);

        timer1 =new QTimer();
        connect(timer,&QTimer::timeout,this,&QWidgetSerialTx::sendSpoPkg);
        timer1->start(1000/248);

        timer2 =new QTimer();
        connect(timer,&QTimer::timeout,this,&QWidgetSerialTx::sendRespPkg);
        timer2->start(1000/125);

    }
public:
    QSerialPort *com;
    QTimer *timer;
    QSerialPort *com1;//血氧
    QTimer *timer1;//血氧
    QSerialPort *com2;//血压
    QTimer *timer2;//血压
    int ecgDataIndex =0;
    int spoDataIndex =0;
    int respDataIndex =0;

public:
    char bccCheck(char *data,int len);

    int serialInit(void);//初始化串口
    void sendEcgPkg();
    void sendSpoPkg();//发送血氧
    void sendRespPkg();//发送血氧
};

int QWidgetSerialTx::serialInit(void)
{
    //打印串口信息

    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        qDebug()<<"Name:"<<info.portName();
        qDebug()<<"Description:"<<info.description();
        qDebug()<<"Manufacture:"<<info.manufacturer();
        qDebug()<<"Serial Number:"<<info.serialNumber();
    }
    com =new QSerialPort();
    com->setPortName("COM5");
    com1 =new QSerialPort();
    com1->setPortName("COM3");
    com2 =new QSerialPort();
    com2->setPortName("COM7");
    if(!com->open(QIODevice::ReadWrite))
    {
        qDebug("serial open err");
        return -1;
    }
    else {
        qDebug()<<"serail open success"<<com->portName();
    }
    if(!com1->open(QIODevice::ReadWrite))
    {
        qDebug("serial open err");
        return -1;
    }
    else {
        qDebug()<<"serail open success"<<com1->portName();
    }
    if(!com2->open(QIODevice::ReadWrite))
    {
        qDebug("serial open err");
        return -1;
    }
    else {
        qDebug()<<"serail open success"<<com2->portName();
    }
    com->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);
    com->setDataBits(QSerialPort::Data8);
    com->setFlowControl(QSerialPort::NoFlowControl);
    com->setParity(QSerialPort::NoParity);
    com->setStopBits(QSerialPort::OneStop);

    com1->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);
    com1->setDataBits(QSerialPort::Data8);
    com1->setFlowControl(QSerialPort::NoFlowControl);
    com1->setParity(QSerialPort::NoParity);
    com1->setStopBits(QSerialPort::OneStop);

    com2->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);
    com2->setDataBits(QSerialPort::Data8);
    com2->setFlowControl(QSerialPort::NoFlowControl);
    com2->setParity(QSerialPort::NoParity);
    com2->setStopBits(QSerialPort::OneStop);
    return 0;
}
char QWidgetSerialTx::bccCheck(char *data, int len)
{
    char check=data[0];//校验和初始化
      int i;
      for(i=1;i<len;i++)
      {
          check=check^data[i];//异或
      }
      return check;  //返回校验和

}
void QWidgetSerialTx::sendSpoPkg()
{
    //char dataBuf1[4];
    //unsigned int spo[1];
    //int len1=(int)sizeof(spoData)/sizeof(int);//计算数组长度

    //if(spoDataIndex >=len1)//判断下标越界
      //  spoDataIndex =0;
    //spo[1]=spoData[spoDataIndex++];//取SPO数据

    //dataBuf1[0] =0x09;//包类型ID
    //dataBuf1[1] =0x80;//包头
    //dataBuf1[1] =dataBuf1[1] | ((spo[1]&0x80)>>6);
    //dataBuf1[2] =0x80 |(spo[1]&0x7F);
    //dataBuf1[3]=0x80;//检验和
    //this->com1->write(dataBuf1,sizeof(dataBuf1));
    char dataBuf1[4];
    unsigned int spo[1]={0};

    int i;
    int len1 =(int)sizeof(spoData)/sizeof(int);//计算数组长度

    if(spoDataIndex >=len1)//判断下标越界
        spoDataIndex =0;
    spo[0]=spoData[spoDataIndex++];//取ECG2数据

    //封包
    dataBuf1[0] =0x09;//包类型ID
    dataBuf1[1] =0x80;//包头
    for(i=0; i<1; i++)
    {
        dataBuf1[1] =dataBuf1[1] | ((spo[i]&0x80)>>1);
        //dataBuf[1] =dataBuf[1] | ((ecg[i]&0x8000)>>(15-i*2));
        //dataBuf[i*2+3] =0x80 | (ecg[i]&0x7F);
        dataBuf1[i*2+2] =0x80 |(spo[i]&0x7F);
    }
    dataBuf1[3]=0x80;
    //dataBuf[9]=bccCheck(&dataBuf[0],9)|0x80;

    this->com1->write(dataBuf1,sizeof(dataBuf1));

}
void QWidgetSerialTx::sendRespPkg()
{
    //char dataBuf2[4];
    //unsigned int resp[1]={0};
    //int len2=(int)sizeof(respData)/sizeof(int);//计算数组长度

    //if(respDataIndex >=len2)//判断下标越界
      //  respDataIndex =0;
    //resp[1]=respData[respDataIndex++];//取RESP数据

    //dataBuf2[0] =0x0A;//包类型ID
    //dataBuf2[1] =0x80;//包头
    //dataBuf2[1] =dataBuf2[1] | ((resp[1]&0x80)>>6);
    //dataBuf2[2] =0x80 |(resp[1]&0x7F);
    //dataBuf2[3]=0x80;//检验和
    //this->com2->write(dataBuf2,sizeof(dataBuf2));
    char dataBuf2[4];
    unsigned int resp[1]={0};

    int i;
    int len2 =(int)sizeof(respData)/sizeof(int);//计算数组长度

    if(respDataIndex >=len2)//判断下标越界
        respDataIndex =0;
    resp[0]=respData[respDataIndex++];//取ECG2数据

    //封包
    dataBuf2[0] =0x0A;//包类型ID
    dataBuf2[1] =0x80;//包头
    for(i=0; i<1; i++)
    {
        dataBuf2[1] =dataBuf2[1] | ((resp[i]&0x80)>>1);
        //dataBuf[1] =dataBuf[1] | ((ecg[i]&0x8000)>>(15-i*2));
        //dataBuf[i*2+3] =0x80 | (ecg[i]&0x7F);
        dataBuf2[i*2+2] =0x80 |(resp[i]&0x7F);
    }
    dataBuf2[3]=0x80;
    //dataBuf[9]=bccCheck(&dataBuf[0],9)|0x80;

    this->com2->write(dataBuf2,sizeof(dataBuf2));
}

void QWidgetSerialTx::sendEcgPkg()
{
   // char dataBuf[10]={(char)0x01,(char)0x02,(char)0x03,(char)0x04,(char)0x01,(char)0x01,
   //                 (char)0x01,(char)0x01,(char)0x01,(char)0x01, };
    //this->com->write(dataBuf,sizeof(dataBuf));
    char dataBuf[10];
    unsigned int ecg[3]={2048,2048,2048};


    int i;
    int len =(int)sizeof(ecgData)/sizeof(int);//计算数组长度

    if(ecgDataIndex >=len)//判断下标越界
        ecgDataIndex =0;
    ecg[1]=ecgData[ecgDataIndex++];//取ECG2数据

    //封包
    dataBuf[0] =0x08;//包类型ID
    dataBuf[1] =0x80;//包头
    for(i=0; i<3; i++)
    {
        dataBuf[1] =dataBuf[1] | ((ecg[i]&0x80)>>(7-i*2-1));
        dataBuf[1] =dataBuf[1] | ((ecg[i]&0x8000)>>(15-i*2));
        dataBuf[i*2+3] =0x80 | (ecg[i]&0x7F);
        dataBuf[i*2+2] =0x80 |((ecg[i]>>8)&0x7F);
    }
    dataBuf[8]=0x80;
    dataBuf[9]=bccCheck(&dataBuf[0],9)|0x80;

    this->com->write(dataBuf,sizeof(dataBuf));

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;
    QWidgetSerialTx *com =new QWidgetSerialTx();
    //w.show();

    return a.exec();
}
