#include "gstmux.h"
#include "group.h"
#include <QLabel>
GSTMUX::GSTMUX(QObject *parent) :
    QObject(parent)
{
}

GSTMUX::GSTMUX(_mux *d, QObject *parent) :
    QObject(parent)
{
    data=d;

    dlg=NULL;
    dlg=new DLGMUX();
    ((Group*)this->parent())->dlg->vbox->addWidget(dlg);
    dlg->show();
    connect(dlg,SIGNAL(start()),this,SLOT(onstart()));
    connect(dlg,SIGNAL(stop()),this,SLOT(onstop()));
}

GSTMUX::~GSTMUX()
{
    dlg->close();
    delete dlg;
    dlg=NULL;
}

bool GSTMUX::start()
{
    for(int i=0;i<data->src.count();i++)
    {
        qDebug(data->src.at(i)->src->url.toAscii().data());
    }

    QString pipeDescr = "videotestsrc pattern=1 ! video/x-raw-yuv, framerate=30/1, width=320, height=240 ! videobox border-alpha=0 top=0 left=0 ! mix. "
            "videotestsrc pattern=15 ! video/x-raw-yuv, framerate=30/1, width=640, height=480 ! videobox border-alpha=0 top=0 left=-320 ! mix. "
            "videotestsrc pattern=13 ! video/x-raw-yuv, framerate=30/1, width=320, height=240 ! videobox name=\"vb\" border-alpha=0 top=-240 left=0 ! mix. "
            "videomixer name=mix ! ffmpegcolorspace ! autovideosink ";
    pipeline = QGst::Parse::launch(pipeDescr).dynamicCast<QGst::Pipeline>();
    pipeline->setState(QGst::StatePlaying);
    return true;
}

bool GSTMUX::stop()
{
    pipeline->setState(QGst::StateNull);
    return true;
}

void GSTMUX::showData()
{
    QString str;
    for(int i=0;i<data->src.count();i++)
    {
        str="";
        if(data->src.at(i)->video)
            str+="V";
        if(data->src.at(i)->audio)
            str+="A";
        dlg->fbox->addRow(str,new QLabel(data->src.at(i)->src->url));
    }
}

void GSTMUX::onstart()
{
    start();
}

void GSTMUX::onstop()
{
    stop();
}
