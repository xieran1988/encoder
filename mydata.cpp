#include "mydata.h"
#include <QFile>
#include <QtXml/QDomDocument>
#include <QTextStream>
#include <QUdpSocket>
MyData::MyData(QObject *parent) :
    QObject(parent)
{
    inst=NULL;
    nextport=10000;
}
bool MyData::LoadConfig(const QString &path)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        qDebug("open file faild");
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        qDebug("setContent faild");
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    _group *tgroup;
    _mux *tmux;
    _mix *tmix;
    _src *tsrc;
    _msrc *tmsrc;
    _box *tbox;
    group.clear();



    QDomNodeList gps;
    QDomNodeList srcs;
    QDomNodeList muxs;
    QDomNodeList mixs;
    QDomNodeList msrcs;
    QDomNodeList boxs;
    QDomNodeList formats;

    //group
    gps= root.elementsByTagName("Group");
    for(int i=0;i<gps.count();i++)
    {
        tgroup=new _group();
        tgroup->ID=gps.at(i).attributes().namedItem("ID").firstChild().nodeValue().toInt();
        if(tgroup->ID==gid)
        {
            mainGroup=tgroup;
        }
        tgroup->fileName=gps.at(i).namedItem("File").namedItem("Name").firstChild().toText().data();
        tgroup->filePath=gps.at(i).namedItem("File").namedItem("Path").firstChild().toText().data();

        //group>src
        srcs= gps.at(i).namedItem("SRC").toElement().elementsByTagName("S");
        for(int j=0;j<srcs.count();j++)
        {
            tsrc=new _src();
            tsrc->url=srcs.at(j).firstChild().toText().data();

            tsrc->pGroup=tgroup;
            tgroup->src.push_back(tsrc);
        }

        //group>mix
        mixs=gps.at(i).toElement().elementsByTagName("MIX");
        for(int j=0;j<mixs.count();j++)
        {
            tmix=new _mix();

            tmix->background=mixs.at(j).namedItem("Layout").attributes().namedItem("img").firstChild().toText().data();
            tmix->size.setWidth(mixs.at(j).namedItem("Layout").attributes().namedItem("width").firstChild().nodeValue().toInt());
            tmix->size.setHeight(mixs.at(j).namedItem("Layout").attributes().namedItem("height").firstChild().nodeValue().toInt());
            tmix->sout=mixs.at(j).attributes().namedItem("SOUT").firstChild().toText().data();
            tmix->encoder=mixs.at(j).attributes().namedItem("ENCODER").firstChild().toText().data();
            tmix->bitrate=mixs.at(j).attributes().namedItem("BITRATE").firstChild().toText().data().toInt();
            //group>mix>format
            formats=mixs.at(j).namedItem("Format").toElement().elementsByTagName("F");
            for(int k=0;k<formats.count();k++)
            {
                tmix->format.append(formats.at(k).firstChild().toText().data());
            }

            //group>mix>box
            boxs=mixs.at(j).namedItem("Layout").toElement().elementsByTagName("BOX");
            for(int k=0;k<boxs.count();k++)
            {
                tbox=new _box();
                tbox->src=tgroup->src.at(boxs.at(k).attributes().namedItem("SRC").firstChild().nodeValue().toInt());
                tbox->audio=(boxs.at(k).attributes().namedItem("audio").firstChild().toText().data().toLower()=="true");
                tbox->video=(boxs.at(k).attributes().namedItem("video").firstChild().toText().data().toLower()=="true");
                tbox->rect.setX(boxs.at(k).namedItem("RECT").attributes().namedItem("X").firstChild().nodeValue().toInt());
                tbox->rect.setY(boxs.at(k).namedItem("RECT").attributes().namedItem("Y").firstChild().nodeValue().toInt());
                tbox->rect.setWidth(boxs.at(k).namedItem("RECT").attributes().namedItem("W").firstChild().nodeValue().toInt());
                tbox->rect.setHeight(boxs.at(k).namedItem("RECT").attributes().namedItem("H").firstChild().nodeValue().toInt());
                tbox->crop.setLeft(boxs.at(k).namedItem("CROP").attributes().namedItem("L").firstChild().nodeValue().toInt());
                tbox->crop.setTop(boxs.at(k).namedItem("CROP").attributes().namedItem("T").firstChild().nodeValue().toInt());
                tbox->crop.setRight(boxs.at(k).namedItem("CROP").attributes().namedItem("R").firstChild().nodeValue().toInt());
                tbox->crop.setBottom(boxs.at(k).namedItem("CROP").attributes().namedItem("B").firstChild().nodeValue().toInt());

                tbox->pMix=tmix;
                tmix->box.push_back(tbox);
            }

            tmix->pGroup=tgroup;
            tgroup->mix.push_back(tmix);
        }

        //group>mux
        muxs=gps.at(i).toElement().elementsByTagName("MUX");
        for(int j=0;j<muxs.count();j++)
        {
            tmux=new _mux();
            tmux->sout=muxs.at(j).attributes().namedItem("SOUT").firstChild().toText().data();

            //group>mux>format
            formats=muxs.at(j).namedItem("Format").toElement().elementsByTagName("F");
            for(int k=0;k<formats.count();k++)
            {
                tmux->format.append(formats.at(k).firstChild().toText().data());
            }

            //group>mux>msrc
            msrcs=muxs.at(j).namedItem("SRC").toElement().elementsByTagName("S");
            for(int k=0;k<msrcs.count();k++)
            {
                tmsrc=new _msrc();
                tmsrc->src=tgroup->src.at(msrcs.at(k).firstChild().nodeValue().toInt());
                tmsrc->audio=(msrcs.at(k).attributes().namedItem("audio").firstChild().toText().data().toLower()=="true");
                tmsrc->video=(msrcs.at(k).attributes().namedItem("video").firstChild().toText().data().toLower()=="true");
                tmsrc->pMux=tmux;
                tmux->src.push_back(tmsrc);
            }
            tmux->pGroup=tgroup;
            tgroup->mux.push_back(tmux);
        }

        group.push_back(tgroup);
        //qDebug(tgroup->fileName.toAscii().data());
    }

    return true;
}

quint16 MyData::getNextPort()
{
    QUdpSocket socket;
    nextport++;
    while(!socket.bind(nextport))
    {
        nextport++;
    }
    socket.close();
    return nextport;
}

bool MyData::SaveConfig(const QString &path)
{
    QFile file(path);
    if (!file.open(QFile::ReadWrite)) {
        qDebug("open file faild");
        return false;
    }


    QDomDocument doc;

    if (!doc.setContent(&file)) {
        file.close();
        qDebug("setContent faild");
        return false;
    }
    file.close();

//    QDomElement root = doc.documentElement();
//    if(autoRun)
//        root.attributes().namedItem("autorun").firstChild().setNodeValue("true");
//    else
//        root.attributes().namedItem("autorun").firstChild().setNodeValue("false");

    QFile f(path);
    if (!f.open(QFile::ReadWrite | QFile::Text))
        return false;

    QTextStream out(&f);
    doc.save(out, 3);
    f.close();

    return true;
}
