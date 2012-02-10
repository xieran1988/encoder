#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mydata.h"
#include "group.h"
#include <QVBoxLayout>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QVBoxLayout *vbox;
    bool init();
    QList<Group*> group;

    void closeEvent ( QCloseEvent * event );
private:
    Ui::MainWindow *ui;
};

extern MainWindow *myWindow;

#endif // MAINWINDOW_H
