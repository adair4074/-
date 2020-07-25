#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <qmath.h>
class QWidgetDraw:  public QWidget
{
public:
   QWidgetDraw(){}

   virtual void paintEvent(QPaintEvent *event)
   {
     QPainter * qpaint = new QPainter();
     qpaint->begin(this);
     this->draw(qpaint);
     qpaint->end();
   }

 void  draw(QPainter *qpainter)
   {
     //  qpainter->drawLine(80,100,90,80);
       //qDebug()<<"u can draw";
      this->setStyleSheet("background-color:red");
      //qpainter->drawRect(20,20,660,440);
     // drawDivide( qpainter,20,20,2,2,660,440);
      //drawDivide( qpainter,20,20,20,15,330,220);
//
      int step = 330/15;
    //  qpainter->drawEllipse(20 + step*(5-3),20 + step*(5-3),step*6,step*6);
     // qpainter->drawEllipse(20 + step*(10-1),20 + step*(2-1),step*2,step*2);
    //  qpainter->drawEllipse(20 + step*(12-1),20 + step*(4-1),step*2,step*2);
    //  qpainter->drawEllipse(20 + step*(12-1),20 + step*(7-1),step*2,step*2);
    //  qpainter->drawEllipse(20 + step*(10-1),20 + step*(9-1),step*2,step*2);




      drawStar(qpainter,20 +step*(5),20 + step*(5),step*3,0,Qt::OddEvenFill);
       drawStar(qpainter,20 +step*(12),20 + step*(7),step*1,-(90+qTan(3/5)/M_PI*180),Qt::OddEvenFill);
drawStar(qpainter,20 +step*(12),20 + step*(4),step*1,-(90+qTan(1/7)/M_PI*180),Qt::OddEvenFill);
drawStar(qpainter,20 +step*(10),20 + step*(2),step*1,-(90+qTan(2/7)/M_PI*180),Qt::OddEvenFill);
drawStar(qpainter,20 +step*(10),20 + step*(9),step*1,-(90+qTan(4/5)/M_PI*180),Qt::OddEvenFill);


 }
 void drawDivide(QPainter *qpainter,int x,int y,int wCnt, int hCnt,int w,int h)
 {
     int step =w/wCnt;
     for(int num =0;num<wCnt;num++)
         qpainter->drawLine(x+num*step,y,x+num*step,y+h);

     step = h/hCnt;
     for(int num=0;num<hCnt;num++)
         qpainter->drawLine(x,y+num*step,x+w,y+num*step);
 }
 drawStar(QPainter* painter, qreal x,qreal y,qreal  radius,qreal  rotate,Qt::FillRule fillStyle)
  {
        qreal degree = 2*M_PI*(180/10)/360;

        QVector<QPoint>points;
        points<<QPoint(0,-radius)<<QPoint(-radius*qTan(degree),0)<<QPoint(radius*qTan(degree),0);

        painter->setBrush(QColor("#FAF408"));
         painter->setPen(QColor("#FAF408"));


        painter->resetTransform();
        painter->translate(x,y);
        painter->rotate(rotate);
        for(int cnt=0;cnt<5;cnt++)
        {
           drawTriangle(painter,points,fillStyle);
           painter->rotate(72);
        }
         painter->resetTransform();
  }

 drawTriangle(QPainter *painter,QVector<QPoint>&points,Qt::FillRule fillStyle)
 {
     painter->drawPolygon(points,fillStyle);
 }


};
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

     QWidgetDraw *widgetDraw =new  QWidgetDraw();
     widgetDraw->resize(660+40,440+40);
     widgetDraw->show();
     qDebug()<<"widgetDraw show done";


    return a.exec();
}
