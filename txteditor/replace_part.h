#ifndef REPLACE_PART_H
#define REPLACE_PART_H

#include <QWidget>

namespace Ui {
class Replace_Part;
}

class Replace_Part : public QWidget
{
    Q_OBJECT

public:
    explicit Replace_Part(QWidget *parent = nullptr);
    ~Replace_Part();
    bool IsDifferentUp();//是否区分大小写
    bool IsLoop();
    QString ReturnFindText();
    QString ReturnReplaceText();
    Ui::Replace_Part *ui;
private:
};

#endif // REPLACE_PART_H
