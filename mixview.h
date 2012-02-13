#ifndef MIXVIEW_H
#define MIXVIEW_H
#include <QGst/Pipeline>
#include <QGst/Ui/VideoWidget>


class MIXView : public QGst::Ui::VideoWidget
{
public:
    MIXView(QWidget *parent = 0);
};

#endif // MIXVIEW_H
