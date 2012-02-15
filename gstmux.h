#ifndef GSTMUX_H
#define GSTMUX_H

#include <QObject>
#include "dlgmux.h"
#include <mydata.h>
#include <QGst/Pipeline>
#include <QGst/Parse>
#include <QGst/ChildProxy>
#include <QGst/PropertyProbe>
#include <vlc/vlc.h>

class GSTMUX : public QObject
{
    Q_OBJECT
public:
    explicit GSTMUX(QObject *parent = 0);
    GSTMUX( _mux *d, QObject *parent = 0);
    ~GSTMUX();
public:
    _mux *data;
    bool isPlaying();
    bool start();
    bool stop();
    void showData();
private:
    bool bPlaying;
    QGst::PipelinePtr pipeline;
    DLGMUX *dlg;
    libvlc_instance_t *inst;
    libvlc_media_player_t *player;
    libvlc_media_t *media;
signals:
    
public slots:
    void onstart();
    void onstop();
    
};

#endif // GSTMUX_H
