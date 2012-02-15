#ifndef MYDATA_H
#define MYDATA_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <vlc/vlc.h>
#include <QRect>

struct _src;
struct _msrc;
struct _box;
struct _mix;
struct _mux;
struct _group;


struct _src
{
    QString url;
    _group *pGroup;
};

struct _msrc
{
    _src *src;
    bool video;
    bool audio;
    _mux *pMux;
};

struct _box
{
    _src *src;
    bool video;
    bool audio;
    QRect rect;
    QRect crop;
    _mix *pMix;
};

struct _mix
{
    QStringList format;
    QList<_box*> box;
    QSize size;
    QString background;
    QString sout;
    QString encoder;
    quint32 bitrate;
    _group *pGroup;
};

struct _mux
{
    QStringList format;
    QList<_msrc*> src;
    QString sout;
    _group *pGroup;
};



struct _group
{
    int ID;
    QList<_src*> src;
    QString fileName;
    QString filePath;
    QList<_mix*> mix;
    QList<_mux*> mux;
};

class MyData : public QObject
{
    Q_OBJECT
public:
    explicit MyData(QObject *parent = 0);

public:
    QList<_group*> group;
    int gid;
    _group* mainGroup;
    libvlc_instance_t *inst;
public:
    bool SaveConfig(const QString &path);
    bool LoadConfig(const QString &path);
    quint16 getNextPort();
private:
    quint16 nextport;

signals:
    
public slots:
    
};
extern MyData myData;
#endif // MYDATA_H
