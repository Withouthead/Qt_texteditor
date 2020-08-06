#ifndef REPLACETEXT_H
#define REPLACETEXT_H
#include"find_widget.h"
#include <QWidget>

class ReplaceText : public Find_Widget
{
    Q_OBJECT
public:
    explicit ReplaceText(QWidget *parent = nullptr);

signals:

};

#endif // REPLACETEXT_H
