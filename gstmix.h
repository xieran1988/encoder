#ifndef GSTMIX_H
#define GSTMIX_H

#include <QObject>
#include "dlgmix.h"
#include <mydata.h>
#include <QGst/Pipeline>
#include <QGst/Parse>
#include <QGst/ChildProxy>
#include <QGst/PropertyProbe>
#include "mixview.h"
#include "mixbox.h"
#include <vlc/vlc.h>

class GSTMIX : public QObject
{
    Q_OBJECT
public:
    explicit GSTMIX(QObject *parent = 0);
    GSTMIX(_mix *d, QObject *parent = 0);
    ~GSTMIX();
public:
     _mix *data;
     QList<MIXBox*> animations;
     bool start();
     bool stop();
     void showData();
private:
     QGst::PipelinePtr pipeline;
    DLGMIX *dlg;
    MIXView *view;
    libvlc_instance_t *inst;
    libvlc_media_player_t *player;
    libvlc_media_t *media;

signals:
    
public slots:
    void onstart();
    void onstop();
    void onshow();
    
};

#endif // GSTMIX_H
