#include "widget.h"
#include "ui_widget.h"
#include "SqliteDbHelper.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    bl = new SampleInfoBl();
}

Widget::~Widget()
{
    delete ui;
    if(bl != nullptr){
        delete  bl;
        bl = nullptr;
    }

}

void Widget::on_pushButton_clicked()
{
    //清空样本数据库
    bl->deleteAllSampleInfo();

    SqliteDbHelper::Instance()->getDB().transaction();


    int value = ui->lineEdit->text().toInt();
    if(value <= 0)
    {
       QMessageBox msgBox;
       msgBox.setText("数据不合法");
       msgBox.show();
       return;
    }

    for(int i = 0; i < value; i++){
        QString sampleNum = "Genrui_"+QString::number(i);
        bl->insertSampleInfo(sampleNum);
    }

    SqliteDbHelper::Instance()->getDB().commit();

    QMessageBox msgBox;
    msgBox.setText("数据生成完成");
    msgBox.exec();

}
