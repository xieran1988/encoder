#ifndef DLGGROUP_H
#define DLGGROUP_H

#include <QWidget>
#include <QVBoxLayout>
namespace Ui {
class DLGGroup;
}

class DLGGroup : public QWidget
{
    Q_OBJECT
    
public:
    explicit DLGGroup(QWidget *parent = 0);
    ~DLGGroup();
    QVBoxLayout *vbox;
private:
    Ui::DLGGroup *ui;
};

#endif // DLGGROUP_H
