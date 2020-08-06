#include "find_widget.h"
#include "ui_find_widget.h"

Find_Widget::Find_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Find_Widget)
{
    ui->setupUi(this);
    connect(ui->FindButton_Cancle, &QPushButton::clicked, this, &Find_Widget::close);
    ui->RadioDown->setChecked(true);
}

Find_Widget::~Find_Widget()
{
    delete ui;
}
bool Find_Widget::ReturnFindDirection()
{
    return !ui->RadioDown->isChecked();
}
QString Find_Widget::ReturnFindText()
{
    return ui->lineEdit->text();
}
bool Find_Widget::IsDifferentUp()
{
    return  ui->FindCheckBox_Isupper->isChecked();
}
bool Find_Widget::IsLoop()
{
    return ui->CheckBox_Loop->isChecked();
}
