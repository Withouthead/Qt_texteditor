#ifndef ABOUTMYSELF_H
#define ABOUTMYSELF_H

#include <QDialog>
#include<QPixmap>
#include<QBitmap>
#include<QLabel>
#include<QPainter>
#include<QSize>
namespace Ui {
class AboutMySelf;
}

class AboutMySelf : public QDialog
{
    Q_OBJECT

public:
    explicit AboutMySelf(QWidget *parent = nullptr);
    ~AboutMySelf();
    QPixmap PixmapToRound(QPixmap const &src, int radius);
private:
    Ui::AboutMySelf *ui;
};

#endif // ABOUTMYSELF_H
