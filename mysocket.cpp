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
    int runType=args.at(3).toInt();

    for(int i=0;i<myWindow->group.count();i++)
    {
        if(myWindow->group.at(i)->data->ID==groupID)
        {
            if(type==0)
            {
                if(myWindow->group.at(i)->startMix(index,runType))
                {
                    myWrite("OK");
                }
                else
                {
                    myWrite("Start faild");
                }
                return;
            }else if(type==1){
                if(myWindow->group.at(i)->startMux(index,runType))
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
            }else if(funcName=="stop"){
                stop(funcArgs);
            }else if(funcName=="pause"){
                pause(funcArgs);
            }else if(funcName=="state"){
                state(funcArgs);
            }

        }
}

void MySocket::stop(QString &arg)
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
                if(myWindow->group.at(i)->stopMix(index))
                {
                    myWrite("OK");
                }
                else
                {
                    myWrite("Start faild");
                }
                return;
            }else if(type==1){
                if(myWindow->group.at(i)->stopMux(index))
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

void MySocket::pause(QString &arg)
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
                if(myWindow->group.at(i)->pauseMix(index))
                {
                    myWrite("OK");
                }
                else
                {
                    myWrite("Start faild");
                }
                return;
            }else if(type==1){
                if(myWindow->group.at(i)->pauseMux(index))
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

void MySocket::state(QString &arg)
{
    int groupID=arg.toInt();

    for(int i=0;i<myWindow->group.count();i++)
    {
        if(myWindow->group.at(i)->data->ID==groupID)
        {

            qDebug("11111");
            QString str=myWindow->group.at(i)->getAllState();
                myWrite(str);
                return;
        }
    }
    myWrite("Not found");
}
