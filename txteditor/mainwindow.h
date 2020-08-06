#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QString>
#include<QTextCursor>
#include"find_widget.h"
#include"ui_find_widget.h"
#include"replace_part.h"
#include"aboutmyself.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
//class Find_Widget;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool SaveThePath(const QString &path);
    bool UseSelectPath();
    void OpenFile();
    void  ChangeIsChange();
    void ChangeTitle(const QString &NewName);
    void SetNewTxt();
    void Find_Text(QWidget &TheBox, QString const &TheText, bool FindDirection, bool IsLoop, bool IsDifferentUp);
    bool FindText_Son(QString const &TheText,bool IsDifferentUp, bool FindDirection);
    bool IsUnSave();
    void RePlace_Text(QString const &Replace_Text);
    void RePlaceAllText(QString const &Befor_Text, QString const &Replace_Text);
    void ChangeFont();
    QString GetCorrectUnicode(const QByteArray &TheText);
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *Mainui;
    QString FilePath;
    QString FileName;
    bool IsChanged = false;
    QString UserDesktopPath;
    Find_Widget find_box;
    Replace_Part Replace_Box;
    QString TheRightUniocode;
    QTextCursor TheTextCursor;
    AboutMySelf AboutDialog;
    int ZoomSize;
    bool IsNew;
signals:
    void UnicodeChanged();
};
#endif // MAINWINDOW_H
