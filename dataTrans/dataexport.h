#ifndef DATAEXPORT_H
#define DATAEXPORT_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QMenu>
#include <QAction>
#include <QStringList>

namespace Ui {
class dataExport;
}

class dataExport : public QMainWindow
{
    Q_OBJECT

public:
    explicit dataExport(QWidget *parent = 0);
    ~dataExport();
    void createRightMenu();//右键创建菜单
    void createRightMenu2();//关于表名的右键菜单
    QMenu *popMenu;//菜单, 不能声明为私有变量，编译不通过
    QMenu *popMenu2;
    QString tableName;//表名
    QStringList m_tableNameList;
    void loadTableNameList();//加载数据库表

    void closeEvent(QCloseEvent *e);//关闭窗口事件


private slots:


    void on_referBtn_clicked();//表搜索

    void on_tableView_tables_clicked(const QModelIndex &index);//tableview中的clicked函数

    void on_addRecBtn_clicked();//添加记录

    void on_deletRecBtn_clicked();//删除记录

//    void on_resRecBtn_clicked();//撤销

    void on_subRecBtn_clicked();//提交

    void on_commitRecBtn_clicked();//保存

    void on_searchBtn_clicked();//根据输入的字符串，查询数据内容

    void slotContextMenu(QPoint pos);//右键菜单响应函数

    void slotContextMenu2(QPoint pos);

    void on_lineEdit_textChanged(const QString &arg1);//自动搜索

    void addTablesToList();//将表名添加到导出列表

private:
    Ui::dataExport *ui;
    QSqlTableModel *model;

};

#endif // DATAEXPORT_H
