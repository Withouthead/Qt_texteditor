#include "replace_part.h"
#include "ui_replace_part.h"

Replace_Part::Replace_Part(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Replace_Part)
{
    ui->setupUi(this);
    connect(ui->CancleButton, &QPushButton::clicked, this, &Replace_Part::close);
}

Replace_Part::~Replace_Part()
{
    delete ui;
}
bool Replace_Part::IsLoop()
{
    return ui->ReplaceLoop->isChecked();
}
QString Replace_Part::ReturnFindText()
{
    return ui->Find_LineEdit->text();
}
QString Replace_Part::ReturnReplaceText()
{
    return ui->Replace_LineEdit->text();
}
bool Replace_Part::IsDifferentUp()
{
    return ui->ReplaceIsUpper->isChecked();
}
