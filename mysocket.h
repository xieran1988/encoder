#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QUdpSocket>
#include <QHostAddress>
#include <QObject>

class MySocket : public QUdpSocket
{
    Q_OBJECT
public:
    explicit MySocket(QObject *parent = 0);
    void start(QString &arg);
signals:
private:
    QHostAddress clientIP;
    quint16 clientPort;
    void myWrite(const QString &str);
    
public slots:
private slots:
    void myOnReceive();
};
extern MySocket *mySocket;

#endif // MYSOCKET_H
