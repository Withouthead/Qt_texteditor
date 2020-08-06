#include "aboutmyself.h"
#include "ui_aboutmyself.h"

#include<QPixmap>
AboutMySelf::AboutMySelf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutMySelf)
{
    ui->setupUi(this);
    QPixmap MyHead;
    MyHead.load(":/image/head.png");
    QPixmap NewMyHead = MyHead.scaled(150,150, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//前者无视长宽比
    QPixmap ShowMyHead = PixmapToRound(NewMyHead, 75);
    ui->MyheadLabel->setPixmap(ShowMyHead);
    QPixmap GitHubPixmap(":/image/Github.png");
    GitHubPixmap = GitHubPixmap.scaled(30,30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->GithubLabel->setPixmap(GitHubPixmap);
    ui->UrlLabel->setText("<a href = https://github.com/Withouthead >Welcome to visit my Github</a>");
    ui->UrlLabel->setOpenExternalLinks(true);

}

AboutMySelf::~AboutMySelf()
{
    delete ui;
}

QPixmap AboutMySelf::PixmapToRound(const QPixmap &src, int radius)
{
    if(src.isNull())
        return QPixmap();
    QSize size(2*radius, 2*radius);
    QBitmap TheMask(size);
    QPainter Painer(&TheMask);
    Painer.setRenderHint(QPainter::Antialiasing);//抗锯齿
    Painer.setRenderHint(QPainter::SmoothPixmapTransform);//曲线光滑
    Painer.fillRect(0,0,size.width(),size.height(),Qt::white);
    Painer.setBrush(Qt::black);
    Painer.drawRoundedRect(0,0,size.width(),size.height(),99,99);

    QPixmap image = src;//.scaled(size);
    image.setMask(TheMask);
    return image;
}
