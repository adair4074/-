#include "mainwindow.h"
#include <QApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QWidget>
#include <QTimer>

int ecgData []= {
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
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
        2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000};
//define QWidgetSerialTx类
class QWidgetSerialTx :public QWidget
{
    //Q_OBJECT
public:
     QWidgetSerialTx(QWidget *parent=nullptr):QWidget(parent)
     {
         if(0!=this->serialInit())//不等于
         {
             qDebug()<<"serialInit error";
             return;
         }
         timer =new QTimer();
         connect(timer,&QTimer::timeout,this,&QWidgetSerialTx::sendEcgPkg);
         timer->start(2);//每秒500个数据 采样率
     }
public:
     QSerialPort *com;
     QTimer *timer;
     int ecgDataIndex=0;
public:
     int serialInit(void);//初始化
     char bccCheck(char *data,int len);
public slots://槽函数声明
     void sendEcgPkg();//发送数据包

};
//发送初始化
int QWidgetSerialTx::serialInit(void)
{
    //打印可使用的串口信息  QSerialPortInfo::availablePorts()里面的数
    foreach(const QSerialPortInfo &info ,QSerialPortInfo::availablePorts())
    {
        qDebug()<<"Name: "<<info.portName();
        qDebug()<<"Description:"<<info.description();
        qDebug()<<"Manufacturer:"<<info.manufacturer();
        qDebug()<<"Serial Number:"<<info.serialNumber();

    }
    //创建串口对象，在头文件中定义指针
    com=new QSerialPort();
    //配置发送端口串口2，
    com->setPortName("COM3");
    //尝试打开串口
    if(!com->open(QIODevice::ReadWrite))
        {
            qDebug()<<"Serialinfo open error";
            return -1;//返回值一般正常返回时，返回0，异常为非0

        }
    else
        qDebug()<<"serial open success !"<<com->portName();

    //波特率配置
        com->setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
    //数据位
        com->setDataBits(QSerialPort::Data8);
    //流控
        com->setFlowControl(QSerialPort::NoFlowControl);
    //校验位
       com->setParity(QSerialPort::NoParity);
    //停止位=1
        com->setStopBits(QSerialPort::OneStop);

    return 0;//返回值一般正常返回时，返回0
}
char QWidgetSerialTx::bccCheck(char *data,int len)
{

    //return 0x00;
     int i = 0;
     char ucCRCHi = 0x00;
     char ucCRCLo=0x80;
     ucCRCLo = (char)(ucCRCHi ^ data[0]);//异或
     //qDebug("%02X",data[0]&0xFF);
     for(i=1;i < len;i++)
     {
      ucCRCLo = (char)(ucCRCLo ^ data[i]);
      //qDebug("%02X",data[i]&0xFF);
     }
     return ucCRCLo;


}
//发送数据
void QWidgetSerialTx::sendEcgPkg()
{
    //char dataBuf[10]={(char)0x01,(char)0x02,(char)0x03,(char)0x04,(char)0x01,(char)0x02,(char)0x03,(char)0x04,(char)0x05,(char)0xFF};
    //this->com->write(dataBuf,sizeof(dataBuf));//向串口4写数据
    char dataBuf[10];
    unsigned int ecg[3]={2048,2048,2048};
    int i;
    int len=(int)sizeof(ecgData)/sizeof(int);
    //取ECG2数据
    if(ecgDataIndex>len)
        ecgDataIndex=0;
    ecg[1]=ecgData[ecgDataIndex++];
    //封包
    dataBuf[0]=0x08;//包类型ID
    dataBuf[1]=0x80;//数据头
    //qDebug("%02X %02X",ecg[1]&0x80,ecg[1]&0x8000);

    for(i=0; i < 3 ; i++)
    {
        dataBuf[1]=dataBuf[1]|((ecg[i]&0x80)>>(7-i*2-1));//去除低字节的最高位
        dataBuf[1]=dataBuf[1]|((ecg[i]&0x8000)>>(15-i*2));//去除高字节的最高位
        dataBuf[i*2+3]=0x80|(ecg[i]&0x7F);//低八位
        dataBuf[i*2+2]=0x80|((ecg[i]>>8)&0x7F);//高八位

    }
    dataBuf[8]=0x80;//ECG的状态位
    dataBuf[9]=(bccCheck(&dataBuf[0],9)|0x80);//校验位   0x80;解码时也需要进行相同的|0x80运算之后再比较
    //qDebug("%02X",dataBuf[9]&0xFF);
    this->com->write(dataBuf,sizeof(dataBuf));//向串口4写数据 一次发送10字节数据
}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    QWidgetSerialTx *com=new QWidgetSerialTx();//实例化类
    return a.exec();
}
