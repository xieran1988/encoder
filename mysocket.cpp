#include "mysocket.h"
#include "mainwindow.h"

MySocket::MySocket(QObject *parent) :
    QUdpSocket(parent)
{
    connect(this, SIGNAL(readyRead()),
            this, SLOT(myOnReceive()));
}

void MySocket::start(QString &arg)
{
    QStringList args=arg.split("-");
    int groupID=args.at(0).toInt();
    int type=args.at(1).toInt();
    int index=args.at(2).toInt();

    for(int i=0;i<myWindow->group.count();i++)
    {
        if(myWindow->group.at(i)->data->ID==groupID)
        {
            if(type==0)
            {
                if(myWindow->group.at(i)->startMix(index))
                {
                    myWrite("OK");
                }
                else
                {
                    myWrite("Start faild");
                }
                return;
            }else if(type==1){
                if(myWindow->group.at(i)->startMux(index))
                {
                    myWrite("OK");
                }
                else
                {
                    myWrite("Start faild");
                }
                return;
            }

        }
    }
    myWrite("Not found");
}

void MySocket::myWrite(const QString &str)
{
    writeDatagram(str.toAscii().data(),str.length(),clientIP,clientPort);
}

void MySocket::myOnReceive()
{
    while (hasPendingDatagrams())
        {
            QByteArray buf;
            int len=pendingDatagramSize();
            buf.resize(len);
            this->readDatagram(buf.data(),len,&clientIP,&clientPort);
            qDebug("%s",buf.data());
            QString str(buf);
            int x=str.indexOf(" ");
            QString funcName=str.left(x);
            QString funcArgs=str.mid(x);
            if(funcName=="start")
            {
                    start(funcArgs);
            }else{
            }

        }
}
