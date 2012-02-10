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
    connect(dlg,SIGNAL(stop()),this,SLOT(onstop()));
    connect(dlg,SIGNAL(showSignal()),this,SLOT(onshow()));

    view=NULL;
    view=new MIXView();

    inst = libvlc_new (0, NULL);
    // view->show();
}

GSTMIX::~GSTMIX()
{
    dlg->close();
    delete dlg;
    dlg=NULL;

}
bool GSTMIX::start()
{
    for(int i=0;i<data->box.count();i++)
    {
        qDebug(data->box.at(i)->src->url.toAscii().data());
    }

    QString layout;
    QString pipeDescr;
    pipeDescr.sprintf("multifilesrc  location=%s caps=\"image/jpeg,framerate=30/1\" ! jpegdec ! videoscale name=\"scale0\" ! video/x-raw-yuv, width=%d, height=%d ! videobox  ! mix. ",
            data->background.toAscii().data(),data->size.width(),data->size.height());

    layout="";
    for(int i=0;i<data->box.count();i++)
    {
        pipeDescr+=QString("videotestsrc pattern=13 ! video/x-raw-yuv, framerate=30/1, width=320, height=240 ! videobox ! "
                            "videoscale ! capsfilter name=\"scale%1\" caps=\"video/x-raw-yuv, width=%2, height=%3\" ! mix. ")
                            .arg(i+1).arg(data->box.at(i)->rect.width()).arg(data->box.at(i)->rect.height());
        layout+=QString("sink_%1::xpos=%2 sink_%3::ypos=%4 ")
                        .arg(i+1).arg(data->box.at(i)->rect.x())
                        .arg(i+1).arg(data->box.at(i)->rect.y());
    }

    pipeDescr+=QString("videomixer name=mix %1 ! ffmpegcolorspace !  autovideosink ").arg(layout);
    int port=myData.getNextPort();
    QString url;
    url.sprintf("UDP://@0.0.0.0:%d",port);
    media = libvlc_media_new_path(inst, url.toAscii().data());
    QString sout;
    sout.sprintf(":sout=#rtp{sdp=%s}", data->sout.toAscii().data());
    libvlc_media_add_option(media, sout.toAscii().data());
    player = libvlc_media_player_new_from_media(media);
    libvlc_media_player_play(player);
    libvlc_media_release(media);

//    pipeDescr.sprintf("multifilesrc  location=%s caps=\"image/jpeg,framerate=30/1\" ! jpegdec ! videoscale ! video/x-raw-yuv, width=%d, height=%d ! videobox  ! mix. "
//            "videotestsrc pattern=1 ! video/x-raw-yuv, framerate=30/1, width=320, height=240 ! videobox ! "
//            "videoscale ! video/x-raw-yuv, framerate=30/1, width=%d, height=%d ! mix. "
//            "videotestsrc pattern=13 ! video/x-raw-yuv, framerate=30/1, width=320, height=240 ! videobox ! "
//            "videoscale ! video/x-raw-yuv, framerate=30/1, width=%d, height=%d ! mix. "
//            "videotestsrc pattern=15 ! video/x-raw-yuv, framerate=30/1, width=640, height=480 ! videobox ! "
//            "videoscale ! video/x-raw-yuv, framerate=30/1, width=%d, height=%d ! mix. "
//            "videomixer name=mix sink_2::ypos=240 sink_3::xpos=320 ! ffmpegcolorspace !  autovideosink ",
//            data->background.toAscii().data(),data->size.width(),data->size.height());
//    QString pipeDescr = "filesrc location=/home/zc/gst/Encoder/t.jpg ! jpegdec ! videoscale ! video/x-raw-yuv, width=1280, height=240 ! ffmpegcolorspace ! autovideosink ";

    pipeline = QGst::Parse::launch(pipeDescr).dynamicCast<QGst::Pipeline>();
    view->watchPipeline(pipeline);
    pipeline->setState(QGst::StatePlaying);

    return true;
}

bool GSTMIX::stop()
{
    pipeline->setState(QGst::StateNull);
    libvlc_media_player_stop(player);
    libvlc_media_player_release(player);

    return true;
}

void GSTMIX::showData()
{
}

void GSTMIX::onstart()
{
    start();
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
