#include <QtGui/QApplication>
#include "mainwindow.h"
#include "mydata.h"
#include "mysocket.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QString>
#include <QGst/Init>
MyData myData;
MainWindow *myWindow;
MySocket *mySocket;
void customMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    QDateTime tm;
    tm=QDateTime::currentDateTime();
    txt=tm.toString("yyyyMMdd_hh:mm:ss ");
    switch (type) {
    case QtDebugMsg:
        txt += QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt += QString("Warning: %1").arg(msg);
    break;
    case QtCriticalMsg:
        txt += QString("Critical: %1").arg(msg);
    break;
    case QtFatalMsg:
        txt += QString("Fatal: %1").arg(msg);
        abort();
    }


    QFile outFile("debug.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGst::init(&argc, &argv);
    //qInstallMsgHandler(customMessageHandler);
    qDebug("Running");
    if(argc<2)
    {
        qFatal("argc<2");
        return 0;
    }
    myData.inst = libvlc_new (0, NULL);
    QString str;
    str=argv[1];
    myData.gid=str.toInt();

    if(!myData.LoadConfig("Group.xml"))
    {
        qFatal("XML load failed!");
        return 0;
    }

    myWindow=new MainWindow();
    myWindow->init();
    myWindow->show();
    mySocket=new MySocket();
    mySocket->bind(8888);

    int rt=a.exec();
    delete myWindow;
    libvlc_release(myData.inst);
    myData.inst=NULL;

    return rt;
}
