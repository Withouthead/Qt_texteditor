#ifndef FIND_WIDGET_H
#define FIND_WIDGET_H

#include <QWidget>
#include<QString>
//#include"mainwindow.h"
namespace Ui {
class Find_Widget;
}
class Find_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Find_Widget(QWidget *parent = nullptr);
    ~Find_Widget();
    bool ReturnFindDirection();
    bool IsDifferentUp();//是否区分大小写
    bool IsLoop();
    QString ReturnFindText();
    Ui::Find_Widget *ui;
   // void Test();
private:
    QString Find_Txt;
};

#endif // FIND_WIDGET_H
