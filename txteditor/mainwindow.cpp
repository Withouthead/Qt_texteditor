#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"ui_replace_part.h"
#include"ui_find_widget.h"
#include<QFile>
#include<QFileDialog>
#include<QStandardPaths>
#include<QTextCursor>
#include<QDialog>
#include<QMessageBox>
#include<QDebug>
#include<QTextStream>
#include<QPushButton>
#include<QTextDocument>
#include<QWidget>
#include<QFont>
#include<QFontDialog>
#include<QTextCodec>
#include<QCloseEvent>
#include<QRegExp>
#include<QKeySequence>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , Mainui(new Ui::MainWindow)
    ,TheRightUniocode("UTF-8")
    ,ZoomSize(0)
    ,IsNew(true)
   // ,TheTextCursor(Mainui->textEdit->textCursor())
{
    setWindowIcon(QPixmap(":/image/notebook.png"));
   // AboutDialog.setParent(this);
    //find_box.setParent(this);
    //Replace_Box.setParent(this);
    UserDesktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    FilePath = UserDesktopPath;
    FileName = "新建文本文档.txt";
    Mainui->setupUi(this);
    TheTextCursor = Mainui->textEdit->textCursor();
    Mainui->textEdit->setFont(QFont("微软雅黑",10));
    setWindowTitle(FileName + "   -记事本复刻版");
    QMenu *Menu_file = Mainui->menubar->addMenu("文件");
    QMenu *Menu_Edit = Mainui->menubar->addMenu("编辑");
    QMenu *Menu_Format = Mainui->menubar->addMenu("格式");
    QMenu *Menu_Check = Mainui->menubar->addMenu("查看");
    QAction *Menu_about = Mainui->menubar->addAction("关于");

    QAction *MenuEdit_Undo = Menu_Edit->addAction("撤销");

    Menu_Edit->addSeparator();

    QAction *MenuEdit_Cut = Menu_Edit->addAction("剪切");
    QAction *MenuEdit_Copy = Menu_Edit->addAction("复制");
    QAction *MenuEdit_Paste = Menu_Edit->addAction("粘贴");
    QAction *MenuEdit_Delete = Menu_Edit->addAction("删除");

    Menu_Edit->addSeparator();
    QAction *MenuEdit_SelectAll = Menu_Edit->addAction("全选");
    Menu_Edit->addSeparator();

   QAction *MenuEdit_Find = Menu_Edit->addAction("查找");
   QAction *MenuEdit_Replace = Menu_Edit->addAction("替换");
   QAction *MenuFlieNew_text = Menu_file->addAction("新建");
   QAction *MenuFlieOpen_text = Menu_file->addAction("打开");
   QAction *MenuFlieSave_text = Menu_file->addAction("保存");
   QAction *MenuFlieSaveother_text = Menu_file->addAction("另存为");


   QAction *MenuFormat_Font = Menu_Format->addAction("字体");
   //查看下的按钮
   QMenu *MenuCheck_Zoom = Menu_Check->addMenu("缩放");
   QAction *MenuCheckZoomBigger = MenuCheck_Zoom->addAction("放大");
   QAction *MenuCheckZoomSmaller = MenuCheck_Zoom->addAction("缩小");
   QAction *MenuCheckZoomReSet = MenuCheck_Zoom->addAction("恢复默认缩放");

   //statusbar
    QString TempOfPos = "第 %1 行， 第 %2 列     ";
    QLabel *StatusbarPos = new QLabel(TempOfPos.arg(QString::number(TheTextCursor.blockNumber()+1))
                                      .arg(QString::number(TheTextCursor.columnNumber()+1)),this);
    QLabel *StatusbarCode = new QLabel(TheRightUniocode+"     ",this);
    QLabel *StatusbarZoom = new QLabel(QString::number(100 + ZoomSize)+"%",this);
    qDebug()<<StatusbarPos->text();
   Mainui->statusbar->addPermanentWidget(StatusbarPos);
   Mainui->statusbar->addPermanentWidget(StatusbarZoom);
   Mainui->statusbar->addPermanentWidget(StatusbarCode);


   //各种快捷键
    MenuFlieNew_text->setShortcut(QKeySequence::New);
    MenuFlieOpen_text->setShortcut(QKeySequence::Open);
    MenuFlieSave_text->setShortcut(QKeySequence::Save);
    MenuFlieSaveother_text->setShortcut(QKeySequence("Ctrl+Shift+S"));
    MenuEdit_Undo->setShortcut(QKeySequence::Undo);
    MenuEdit_Cut->setShortcut(QKeySequence::Cut);
    MenuEdit_Copy->setShortcut(QKeySequence::Copy);
    MenuEdit_Paste->setShortcut(QKeySequence::Paste);
    MenuEdit_SelectAll->setShortcut(QKeySequence::SelectAll);
    MenuEdit_Delete->setShortcut(QKeySequence::Delete);
    MenuCheckZoomBigger->setShortcut(QKeySequence::ZoomIn);
    MenuCheckZoomSmaller->setShortcut(QKeySequence::ZoomOut);
    MenuCheckZoomReSet->setShortcut(QKeySequence(tr("Ctrl+0")));
    MenuEdit_Find->setShortcut(QKeySequence::Find);
    MenuEdit_Replace->setShortcut(QKeySequence::Replace);


   connect(MenuFlieOpen_text, &QAction::triggered, this,&MainWindow::OpenFile);
   connect(Mainui->textEdit, &QTextEdit::textChanged, this, [=](){
       ChangeIsChange();
       setWindowTitle("*"+FileName + "   -记事本复刻版");
      // qDebug() << "文本已经改变";
   });
   connect(MenuFlieSaveother_text, &QAction::triggered, this, &MainWindow::UseSelectPath);
   connect(MenuFlieSave_text, &QAction::triggered,[=](){
       if(IsNew)
           UseSelectPath();
        else
           this->SaveThePath(FilePath);
   });
   //撤销
   connect(MenuEdit_Undo, &QAction::triggered, Mainui->textEdit, &QTextEdit::undo);
   //剪切复制粘贴
    connect(MenuEdit_Cut, &QAction::triggered, Mainui->textEdit, &QTextEdit::cut);
    connect(MenuEdit_Copy, &QAction::triggered, Mainui->textEdit, &QTextEdit::copy);
    connect(MenuEdit_Paste, &QAction::triggered, Mainui->textEdit, &QTextEdit::paste);
    //全选
    connect(MenuEdit_SelectAll, &QAction::triggered, Mainui->textEdit, &QTextEdit::selectAll);
   //查找和替换
   connect(MenuFlieNew_text, &QAction::triggered, this, &MainWindow::SetNewTxt);
   connect(MenuEdit_Find, &QAction::triggered, [=](){
      find_box.show();
      find_box.ui->lineEdit->setFocus();
   });
   connect(MenuEdit_Delete, &QAction::triggered, [=](){
       Mainui->textEdit->textCursor().removeSelectedText();
   });
   connect(MenuEdit_Replace, &QAction::triggered, [=](){
       Replace_Box.show();
       Replace_Box.ui->Find_LineEdit->setFocus();
   });
   connect(find_box.ui->Button_CheckNext, &QPushButton::clicked, this, [=](){
       Find_Text(find_box, find_box.ReturnFindText(), find_box.ReturnFindDirection(), find_box.IsLoop(), find_box.IsDifferentUp());
   });
   connect(Replace_Box.ui->FindNextButton, &QPushButton::clicked, [=](){
       Find_Text(Replace_Box, Replace_Box.ReturnFindText(), false, Replace_Box.IsLoop(), Replace_Box.IsDifferentUp());
       Replace_Box.ui->Find_LineEdit->setFocus();
   });
   connect(Replace_Box.ui->ReplaceButton, &QPushButton::clicked, [=](){
       RePlace_Text(Replace_Box.ReturnReplaceText());
       emit Replace_Box.ui->FindNextButton->clicked();
   });
   connect(Replace_Box.ui->ReplaceAllButton, &QPushButton::clicked, [=](){
       RePlaceAllText(Replace_Box.ui->Find_LineEdit->text(), Replace_Box.ui->Replace_LineEdit->text());
   });
   connect(MenuFormat_Font, &QAction::triggered, this, &MainWindow::ChangeFont);

   //statusbar
   connect(Mainui->textEdit, &QTextEdit::cursorPositionChanged, [=](){
       qDebug()<<"光标改变了";
     //  qDebug()<<
       TheTextCursor = TheTextCursor = Mainui->textEdit->textCursor();
       StatusbarPos->setText(TempOfPos.arg(QString::number(TheTextCursor.blockNumber()+1))
                             .arg(QString::number(TheTextCursor.columnNumber()+1)));
   });
   connect(this, &MainWindow::UnicodeChanged, [=](){
       qDebug()<<"Code has changed";
      StatusbarCode->setText(TheRightUniocode + "     ");
   });

   connect(MenuCheckZoomBigger, &QAction::triggered, [=](){
     //  qDebug()<<"放大";
       if(ZoomSize<=90){
         Mainui->textEdit->zoomIn(10);
         ZoomSize+=10;
         StatusbarZoom->setText(QString::number(100 + ZoomSize)+"%");
       }
   });
   connect(MenuCheckZoomSmaller, &QAction::triggered, [=](){
       qDebug()<<"缩小";
       if(ZoomSize>=-90){
         Mainui->textEdit->zoomOut(10);
         ZoomSize-=10;
         StatusbarZoom->setText(QString::number(100 + ZoomSize)+"%");
       }
   });
   connect(MenuCheckZoomReSet, &QAction::triggered, [=](){
       if(ZoomSize)
            Mainui->textEdit->zoomOut(ZoomSize);
       else
            Mainui->textEdit->zoomIn(ZoomSize);
       ZoomSize = 0;
        StatusbarZoom->setText(QString::number(100 + ZoomSize)+"%");
   });

   connect(Menu_about, &QAction::triggered,[=](){
       AboutDialog.show();
   });
}

MainWindow::~MainWindow()
{
    delete Mainui;
}
void MainWindow::ChangeIsChange(){
    IsChanged = true;
}
QString MainWindow::GetCorrectUnicode(const QByteArray &TheText)
{
    QTextCodec::ConverterState state;//貌似是个转换器
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QString TheTransText = code->toUnicode(TheText.constData(), TheText.size(), &state);
    if(state.invalidChars > 0){
         TheTransText = QTextCodec::codecForName("GBK")->toUnicode(TheText);
         TheRightUniocode = "GBK";
         emit UnicodeChanged();
    }
    else
    {
        TheTransText = TheText;
        TheRightUniocode = "UTF-8";
        emit UnicodeChanged();
    }
    return TheTransText;
}
bool MainWindow::IsUnSave()
{
    if(IsChanged == true){
        qDebug()<<"Have arrived";
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("警告");
        msgBox.setText("当前文件尚未保存，是否按照原地址保存?");
        QPushButton *Save_Button = msgBox.addButton("保存" , QMessageBox::AcceptRole);
        QPushButton *DontSave = msgBox.addButton("不保存", QMessageBox::NoRole);
        QPushButton *Cancle_Button = msgBox.addButton("取消", QMessageBox::RejectRole);
        msgBox.exec();
        if(msgBox.clickedButton() == Save_Button){

            qDebug()<<FilePath;
            if(IsNew)
                return UseSelectPath();
            else
                return SaveThePath(FilePath);
        }
        if(msgBox.clickedButton() == Cancle_Button)
               return false;
    }
    return true;//true意思是往下执行
}
void MainWindow::SetNewTxt()//新建
{
    if(IsUnSave()){
        FileName = "新建文本文档.txt";
        FilePath = UserDesktopPath;
        FilePath = "";
        Mainui->textEdit->clear();
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
        TheRightUniocode = "UTF-8";
        emit UnicodeChanged();
        IsChanged = false;
        IsNew = true;
    }
}
void MainWindow::OpenFile()
{
    if(IsUnSave()){
        do{
            FilePath = QFileDialog::getOpenFileName(this,tr("打开文件"), UserDesktopPath);
            if(FilePath == "")
                return;
            QFile file(FilePath);
            while(!file.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,"错误","文件无法打开");
                continue;
            }
            while(!file.isReadable()){
                QMessageBox::warning(this,"错误", "文件无法读取");
                continue;
            }
            Mainui->textEdit->setText(GetCorrectUnicode(file.readAll()));
            Mainui->textEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
            file.close();
            break;
        }while(1);
        QRegExp RxRuler("([^<>/\\\\|:""\\*\?]+\\.\\w+$)");
        RxRuler.indexIn(FilePath);
        FileName = RxRuler.cap();
        qDebug()<<FilePath;
        qDebug()<<FileName;
        setWindowTitle(FileName + "   -记事本复刻版");
        IsChanged = false;
        IsNew = false;
    }
}
bool MainWindow::UseSelectPath()//另存为 并调用保存
{
    QString FilePathAddName = FilePath;
    qDebug()<<FilePathAddName;
    qDebug()<<IsNew;
    if(IsNew)
           FilePathAddName += "./新建文本文档";
    qDebug()<<FileName;
    QString SavePath = QFileDialog::getSaveFileName(this, "另存为",
                                                    FilePathAddName,"TXT(*.txt);;All file(*.)");
    return SaveThePath(SavePath);
}
bool MainWindow::SaveThePath(const QString &path)//保存
{
    if(path == "")
        return false;
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "错误", "保存失败！");
        return false;
    }
    QTextStream TextFile(&file);
    QString TheTxtForEdit = Mainui->textEdit->toPlainText();
    TextFile << TheTxtForEdit;
    file.close();
    IsChanged = false;
    IsNew = false;
    setWindowTitle(FileName + "   -记事本复刻版");
    return true;
}

bool::MainWindow::FindText_Son(QString const &TheText,bool IsDifferentUp, bool FindDirection)
{
    if(FindDirection)
    {
        //qDebug()<<"UP";
        bool flag;
        if(IsDifferentUp)
            flag = Mainui->textEdit->find(TheText, QTextDocument::FindBackward | QTextDocument::FindCaseSensitively);
        else
            flag = Mainui->textEdit->find(TheText, QTextDocument::FindBackward);
        if(!flag)
            return false;
        }
    else
     {
      // qDebug()<<"Down";
        bool flag;
        if(IsDifferentUp)
            flag = Mainui->textEdit->find(TheText,QTextDocument::FindCaseSensitively);
        else
            flag = Mainui->textEdit->find(TheText);
        if(!flag)
            return false;
     }
    return true;
}
void MainWindow::Find_Text(QWidget &TheBox,QString const &TheText, bool FindDirection, bool IsLoop, bool IsDifferentUp)
{
    QString const &EditText = Mainui->textEdit->toPlainText();
   // QString const &TheText = find_box.ReturnFindText();
    int count =EditText.count(TheText);
    qDebug()<<count;
    qDebug()<<TheText;
    bool IsFind = FindText_Son(TheText, IsDifferentUp, FindDirection);
    if(count != 0 && !IsFind && FindDirection && IsLoop){
         Mainui->textEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
         IsFind = FindText_Son(TheText, IsDifferentUp, FindDirection);
    }//实现循环
    if(count != 0 && !IsFind && !FindDirection && IsLoop){
         Mainui->textEdit->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
         IsFind = FindText_Son(TheText, IsDifferentUp, FindDirection);
    }
    if(!IsFind){
        QMessageBox::information(&TheBox,"提示","没有找到 \""+TheText+"\"");
    }

}
void MainWindow::RePlace_Text(QString const &Replace_Text)
{
    Mainui->textEdit->insertPlainText(Replace_Text);
}
void MainWindow::RePlaceAllText(QString const &Befor_Text,  QString const &Replace_Text)
{
    QString S = Mainui->textEdit->toPlainText();
    S.replace(Befor_Text, Replace_Text);
     Mainui->textEdit->clear();
    Mainui->textEdit->setText(S);

}
void MainWindow::ChangeFont()
{
    QFont TheFont = QFontDialog::getFont(0,Mainui->textEdit->font());
    Mainui->textEdit->setFont(TheFont);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!IsUnSave())
         event->ignore();
    else
        event->accept();

}
