#include "gstmix.h"
#include "group.h"
GSTMIX::GSTMIX(QObject *parent) :
    QObject(parent)
{
}

GSTMIX::GSTMIX(_mix *d, QObject *parent) :
    QObject(parent)
{
    data=d;
    state=0;

    MIXBox *tmp;
    animations.clear();
    for(int i=0;i<data->box.count();i++)
    {
        tmp=new MIXBox();
        tmp->boxRect=data->box.at(i)->rect;
        tmp->setGeometry(tmp->boxRect);
        tmp->pip=&pipeline;
        tmp->index=i;
        animations.push_back(tmp);
    }

    dlg=NULL;
    dlg=new DLGMIX();
    ((Group*)this->parent())->dlg->vbox->addWidget(dlg);
    dlg->show();
    connect(dlg,SIGNAL(start()),this,SLOT(onstart()));
    connect(dlg,SIGNAL(pause()),this,SLOT(onpause()));
    connect(dlg,SIGNAL(stop()),this,SLOT(onstop()));
    connect(dlg,SIGNAL(showSignal()),this,SLOT(onshow()));

    view=NULL;
    view=new MIXView();

    inst = libvlc_new (0, NULL);
    player=NULL;
    // view->show();
}

GSTMIX::~GSTMIX()
{
    dlg->close();
    delete dlg;
    dlg=NULL;

}
bool GSTMIX::start(int type)
{
    if(state!=0)
    {
        return true;
    }

    for(int i=0;i<data->box.count();i++)
    {
        qDebug("%s",data->box.at(i)->src->url.toAscii().data());
    }

    QString layout;
    QString audio;
    QString pipeDescr;
    pipeDescr.sprintf("multifilesrc  location=%s caps=\"image/jpeg\" ! jpegdec ! videobox ! videoscale name=\"scale0\" ! video/x-raw-yuv, width=%d, height=%d ! mix. ",
            data->background.toAscii().data(),data->size.width(),data->size.height());

    layout="";
    audio="";
    for(int i=0;i<data->box.count();i++)
    {
        pipeDescr+=QString("rtspsrc location=%1 latency=100 name=source%2 source%3. ! rtph264depay ! decodebin2  ! videobox ! "
                            "videoscale ! capsfilter name=\"scale%4\" caps=\"video/x-raw-yuv, width=%5, height=%6\" ! mix. ")
                            .arg(data->box.at(i)->src->url)
                            .arg(i+1).arg(i+1).arg(i+1).arg(data->box.at(i)->rect.width()).arg(data->box.at(i)->rect.height());
        if(data->box.at(i)->audio)
        {
            audio+=QString(" source%1. ! rtpmp4gdepay ! aacparse ! mux. ").arg(i+1);
        }
        layout+=QString("sink_%1::xpos=%2 sink_%3::ypos=%4 ")
                        .arg(i+1).arg(data->box.at(i)->rect.x())
                        .arg(i+1).arg(data->box.at(i)->rect.y());
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


    pipeDescr+=QString("videomixer name=mix %1 ! ffmpegcolorspace !  tee name=t "
                       "t. ! queue max-size-bytes=500000000 max-size-buffers=100 max-size-time=1000000 ! autovideosink "
                       "t. ! queue max-size-bytes=500000000 max-size-buffers=100 max-size-time=1000000 ! "
                       "%2 bitrate=%3 ! mux. %4 mpegtsmux name=mux ! mpegtsparse ! tee name=t2 t2. ! queue ! multiudpsink clients=127.0.0.1:%5  "
                       "%6")
            .arg(layout)
            .arg(data->encoder).arg(data->bitrate)
            .arg(audio).arg(port).arg(filesink);
    if(type&2)
    {
        startRtsp(port);
    }



    pipeline = QGst::Parse::launch(pipeDescr).dynamicCast<QGst::Pipeline>();
    view->watchPipeline(pipeline);

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

bool GSTMIX::stop()
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
    view->stopPipelineWatch();
    state=0;

    return true;
}

void GSTMIX::showData()
{
}

void GSTMIX::onstart()
{
    start(3);
}

void GSTMIX::onstop()
{
    stop();
}

void GSTMIX::onshow()
{
    if(view->isHidden())
        view->show();
    else
    {
        animations.at(1)->AnimationSwap(this->animations.at(0));
        //view->hide();
    }
}



bool GSTMIX::startRtsp(int port)
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

void GSTMIX::onpause()
{
    pause();
}
bool GSTMIX::pause()
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

int GSTMIX::getState()
{
    return state;
}

