#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPen>
#include <qmath.h>
#include <QDebug>
#include<QPainter>
#include<QBasicTimer>
#include<QTimerEvent>
const int ecgWave[] = {2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
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


class QWidgetDraw : public QWidget
{
public:
      int ecgDataIndex;

      qreal index;
      qreal range;
      QPixmap *pixmap;
      QWidgetDraw(QWidget* parent=0):QWidget(parent)
      {
          QBasicTimer *timer = new QBasicTimer();
          timer->start(10,this);

          pixmap = new QPixmap(this->width(),this->height());
          pixmap->fill(Qt::black);

          index = 0;
          range = 0;

      }
      virtual void timerEvent(QTimerEvent *event)
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

      virtual void paintEvent(QPaintEvent *event)
      {
          QPainter *painter = new QPainter();
          qDebug() <<"paintEvent";
          painter->begin(this);
          painter->resetTransform();

          //painter->setBrush(QColor("#F40002"));
          //painter->setPen(QColor("#F40002"));

          //drawSineWave(painter);
          painter->drawPixmap(0,0,*pixmap);

          painter->resetTransform();
          painter->end();
      }



      drawEcgWave(QPainter *painter)
      {
          QPen pen;
          pen.setWidth(2);
          pen.setColor(QColor("#F40002"));
          painter->setPen(pen);

          index = index + 1;
          if(index > width())
              index = 1;

          //擦除当前列
          painter->save();
          pen.setColor(Qt::black);
          painter->setPen(pen);
          painter->drawLine(index,0,index,height());
          painter->restore();

          qreal height=this->height();
          QPoint lineStart;
          QPoint lineEnd;

          lineStart.setX(index -1);
          lineEnd.setX(index);


          //qDebug()<<height/4096 * ecgWave[ecgDataIndex];
          //painter->drawPoint(this->index,height/4096 * ecgWave[ecgDataIndex]);

          qreal value = ecgWave[ecgDataIndex];
          int y = round(height/2 - ((value - 2048)/600)*height/2);
          qDebug()<<y;
          lineStart.setY(y);


          ecgDataIndex = ecgDataIndex+5;
          if(ecgDataIndex >= sizeof(ecgWave)/sizeof(int))
              ecgDataIndex = 0;
          value=ecgWave[ecgDataIndex];
          y = round(height/2 - ((value - 2048)/600)*height/2);
          lineEnd.setY(y);
          painter->drawLine(lineStart,lineEnd);
      }

};
class EcgWave:public QWidget
{
public:
    EcgWave(QWidget *parent = 0):QWidget(parent)
    {
        this->setWindowFlags (Qt::FramelessWindowHint);
        this->setStyleSheet("background-color:black");

        QLabel *title = new QLabel("ECG");
        title->setStyleSheet("color:white");
        QWidgetDraw *waveWin = new QWidgetDraw();
        QVBoxLayout *layout = new QVBoxLayout();

        layout->addWidget(title);
        layout->addWidget(waveWin);
        layout->setStretch(0,1);
        layout->setStretch(1,5);
        this->setLayout(layout);
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EcgWave *wave = new EcgWave();
    wave->resize(600,150);
    wave->show();


    return a.exec();
}
