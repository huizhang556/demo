#ifndef DATABASE_H
#define DATABASE_H

#include <QDialog>

namespace Ui {
class dataBase;
}

class dataBase : public QDialog
{
    Q_OBJECT

public:
    explicit dataBase(QWidget *parent = 0);
    ~dataBase();
    void connectDB();//连接数据库

private slots:
    void on_conndbBtn_clicked();//连接数据库按钮

    void on_exitBtn_clicked();//退出
signals:
    void staDbConStatus();

private:
    Ui::dataBase *ui;

};

#endif // DATABASE_H
