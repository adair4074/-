#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include<QTcpSocket>

class mytcpsocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit mytcpsocket(QObject *parent = nullptr);

private slots:
    void tcpreadyread();
    void tcpdisconnect();


signals:
    void signalrec(QByteArray ba);

public slots:
};

#endif // MYTCPSOCKET_H
