#ifndef QWIDGETSPOCOM_H
#define QWIDGETSPOCOM_H
#include <QApplication>
#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

class QWidgetSpoCom : public QWidget
{
public:
    QSerialPort *com1;



    char status1 = 0;
    char pkgHead1;
    char pkgDataHead1;
unsigned int pkgDataBuf1[];
    //unsigned int pkgDataBuf1[]; // 数据长度最大为8血样
    char pkgData1[4];
    char pkgCrc1;
    int  pkgDataCnt1 ;
    int spo;
    char dataTmp;
    int sp;


    QWidgetSpoCom(QWidget *parent = nullptr) : QWidget(parent)
    {
        this->status1=-1;
        this->serialInit1();
    }

    int serialInit1(void);
    void serialRx1(void);
    void rxDataHandle1(char data1);
    char bccCheck1(char *data1, int len1);
    void ecgBpmCalc1(int spoData);
    int get_max1(int * data1, int len1);
signals:

};

#endif // QWIDGETECGCOM_H
