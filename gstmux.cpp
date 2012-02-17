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
    state=0;
    dlg=NULL;
    dlg=new DLGMUX();
    ((Group*)this->parent())->dlg->vbox->addWidget(dlg);
    dlg->show();
    connect(dlg,SIGNAL(start()),this,SLOT(onstart()));
    connect(dlg,SIGNAL(stop()),this,SLOT(onstop()));

    inst = libvlc_new (0, NULL);
    player=NULL;
}

GSTMUX::~GSTMUX()
{
    dlg->close();
    delete dlg;
    dlg=NULL;
}

bool GSTMUX::start(int type)
{
    if(state!=0)
    {
        return true;
    }

    for(int i=0;i<data->src.count();i++)
    {
        qDebug("%s",data->src.at(i)->src->url.toAscii().data());
    }

    QString video;
    QString audio;
    QString pipeDescr;

    video="";
    audio="";
    pipeDescr="";
    for(int i=0;i<data->src.count();i++)
    {
        pipeDescr+=QString("rtspsrc location=%1 latency=100 name=source%2 ")
                            .arg(data->src.at(i)->src->url)
                            .arg(i+1);
        if(data->src.at(i)->video)
        {
            video+=QString(" source%1. ! rtph264depay ! h264parse ! mux. ")
                            .arg(i+1);
        }
        if(data->src.at(i)->audio)
        {
            audio+=QString(" source%1. ! rtpmp4gdepay ! aacparse ! mux.").arg(i+1);
        }

    }

    int port=myData.getNextPort();
    QDateTime tm;
    tm=QDateTime::currentDateTime();
    QString filesink;
    if(type&1)
    {
        filesink="t. ! queue ! filesink location="
                +data->pGroup->filePath + "/"
                + data->pGroup->fileName.replace("$T",tm.toString("yyyyMMdd-hh:mm:ss"))
                + ".ts";
    }

    pipeDescr+=QString(" %1 %2 mpegtsmux name=mux ! mpegtsparse ! queue ! tee name=t t. ! queue ! multiudpsink clients=127.0.0.1:%3 "
                       "%4")
            .arg(video)
            .arg(audio)
            .arg(port).arg(filesink);


    if(type&2)
    {
        startRtsp(port);
    }

    pipeline = QGst::Parse::launch(pipeDescr).dynamicCast<QGst::Pipeline>();


    bool rt=pipeline->setState(QGst::StatePlaying);
    if(!rt)
    {
        return false;
    }else
    {
        state=1;
    }

    return true;
}

bool GSTMUX::stop()
{
    if(state==0)
    {
        return true;
    }


    pipeline->setState(QGst::StateNull);
    if(player!=NULL)
    {
        libvlc_media_player_stop(player);
        libvlc_media_player_release(player);
        player=NULL;
    }
    state=0;

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
    start(3);
}

void GSTMUX::onstop()
{
    stop();
}



bool GSTMUX::startRtsp(int port)
{
    QString url;
    url.sprintf("UDP://@0.0.0.0:%d",port);
    media = libvlc_media_new_path(inst, url.toAscii().data());
    QString sout;
    sout.sprintf(":sout=#rtp{sdp=%s}", data->sout.toAscii().data());
    libvlc_media_add_option(media, sout.toAscii().data());
    libvlc_media_add_option(media, ":sout-caching=0");
    libvlc_media_add_option(media, ":udp-caching=80");
    libvlc_media_add_option(media, ":sout-rtsp-caching=0");
    libvlc_media_add_option(media, ":sout-rtp-caching=0");
    libvlc_media_add_option(media, ":rtp-caching=0");
    libvlc_media_add_option(media, ":sout-mux-caching=0");
    libvlc_media_add_option(media, ":sout-all");

    //libvlc_media_add_option(media, ":clock-jitter=0");

    player = libvlc_media_player_new_from_media(media);
    libvlc_media_player_play(player);
    libvlc_media_release(media);
    return true;
}

int GSTMUX::getState()
{
    return state;
}

bool GSTMUX::pause()
{
    if(state==2)
    {
        state=1;
        pipeline->setState(QGst::StatePlaying);
    }
    else if(state==1)
    {
        state=2;
        pipeline->setState(QGst::StatePaused);
    }
    return true;
}
