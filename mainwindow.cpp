#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <windows.h>
#include<QFile>
#include<QTextStream>
#include "../rizhi/rizhi.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),wjm("baikehuifu.txt")
{
    ui->setupUi(this);
    //w.setWindowFlags(Qt::Dialog);
    //::SetWindowPos(HWND(this->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    setWindowFlags(this->windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint );

    ui->textEdit->hide();
    loadtxt();
    gengxinListTip();
}

void MainWindow::gengxinListTip(){
    for(int i=0;i<ui->listWidget->count();i++){
        auto it=ui->listWidget->item(i);
        it->setToolTip(it->text());
    }
}

/// 启动打开外部文件，替换当前内容。若不存在则终止。
void MainWindow::loadtxt(){
    //打开文件
    QFile filog(wjm);
    bool issuc=filog.open(QIODevice::ReadOnly);
    if(!issuc)
    {
        return;
    }

    QStringList lb;
    //关联ts
    QTextStream tsnew(&filog);
    tsnew.setCodec("UTF-8");//必须加，否则为locale=gbk
    while(!tsnew.atEnd()){
        lb.append(tsnew.readLine());
    }
    filog.close();
    ui->listWidget->clear();
    ui->listWidget->addItems(lb);
}
void MainWindow::savetxt(){
    //打开文件
    QFile filog(wjm);
    bool issuc=filog.open(QIODevice::WriteOnly);
    if(!issuc)
    {
        return;
    }

    //关联ts
    QTextStream tsnew(&filog);
    tsnew.setCodec("UTF-8");//必须加，否则为locale=gbk
    tsnew<<ui->textEdit->toPlainText();
    filog.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QClipboard *jtb = QApplication::clipboard();
    QString jgStr=item->text().replace("http","\r\nhttp");
    jgStr=jgStr.replace(QRegularExpression("根据$"),"根据\r\n");
    jtb->setText(jgStr);
}

/// 点击打开---显示编辑--加载list全部--显示为关闭
/// 点击关闭---关闭编辑--更新list--改为打开--存入本地
void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->text()=="打开"){
        QString x;
        for(int i=0;i<ui->listWidget->count();i++){
            x.append(ui->listWidget->item(i)->text());
            x.append("\r\n");
        }
        ui->textEdit->setPlainText(x);
        ui->textEdit->show();
        ui->pushButton->setText("关闭");
    }else{
        //snd<<ui->textEdit->toPlainText();
        QStringList lb=ui->textEdit->toPlainText().split("\n");
        ui->listWidget->clear();
        ui->listWidget->addItems(lb);
        //snd<<lb.count();
        savetxt();
        ui->textEdit->hide();
        gengxinListTip();
        ui->listWidget->update();
        ui->pushButton->setText("打开");
    }
}
