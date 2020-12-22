#include "database.h"
#include "ui_database.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>

dataBase::dataBase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dataBase)
{
    ui->setupUi(this);
     this->setWindowTitle("数据库连接");
    ui->label_status->setText("等候连接数据库...");
}

dataBase::~dataBase()
{
    delete ui;

}

//连接数据库
void dataBase::connectDB()
{
    //打印Qt支持的数据库驱动
    //QSqlDatabase::drivers();
    //qDebug()<<QSqlDatabase::drivers();
    //添加mysql数据库，用的是MySQL数据库
    QSqlDatabase db1 = QSqlDatabase::addDatabase("QMYSQL");
    //连接数据库
    db1.setHostName("127.0.0.1");//数据库服务器ip
    db1.setUserName("root");//数据库用户名
    db1.setPassword("123456");//密码
    db1.setDatabaseName("test");//是用哪个数据库

    //打开数据库
     if(!db1.open())
      {
         //QMessageBox::information(this,"连接提示","数据库连接失败！");
         ui->label_status->setText("数据库连接失败！");
         QTimer::singleShot(1000,this,[=]{
         ui->label_status->setText("数据库未连接。。。");
         });
     }
     else
     {
         //QMessageBox::information(this,"连接提示","数据库连接成功！");
         ui->label_status->setText("数据库连接成功！");
         QTimer::singleShot(1000,this,[=]{
         ui->label_status->setText("数据库已连接。。。");
         });

     }

}
//连接数据库按钮
void dataBase::on_conndbBtn_clicked()
{
    connectDB();

}
//数据库连接：退出
void dataBase::on_exitBtn_clicked()
{
    this->close();
}
