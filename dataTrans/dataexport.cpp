#include "dataexport.h"
#include "ui_dataexport.h"
#include <QSplitter>
#include "database.h"
#include "dataimport.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>

dataExport::dataExport(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::dataExport)
{
    ui->setupUi(this);
    this->setWindowTitle("数据导出");
//  this->setFixedSize(1010,760);//固定宽，高
//    ui->splitter->setStretchFactor(0,4);
//    ui->splitter->setStretchFactor(1,1);
//    ui->splitter_2->setStretchFactor(0,3);
//    ui->splitter_2->setStretchFactor(1,7);
//  this->setCentralWidget(splitter_2);//不起作用
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);//表示这个tableview可以进行右键操作
    createRightMenu();//创建右键菜单函数初始化
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slotContextMenu(QPoint)));//触发右键菜单信号，生产右键菜单

    dataBase *dbWin = new dataBase;
    connect(ui->action_condb,&QAction::triggered,[=]{
        dbWin->show();
    });

    dataImport *impWin = new dataImport;
    connect(ui->action_dataimp,&QAction::triggered,[=]{
        impWin->show();
        this->hide();
    });

    connect(impWin,&dataImport::backtodtexport,[=]{
        impWin->hide();
        this->show();
    });

    connect(ui->action_sfexit,&QAction::triggered,[=]{
        QPushButton *okbtn = new QPushButton("是");
        QPushButton *cancelbtn = new QPushButton("否");
        QMessageBox *mymsgbox = new QMessageBox;

        mymsgbox->setIcon(QMessageBox::Warning);
        mymsgbox->setWindowTitle("提示");
        mymsgbox->setText("是否退出软件？");
        mymsgbox->addButton(okbtn, QMessageBox::AcceptRole);
        mymsgbox->addButton(cancelbtn, QMessageBox::RejectRole);
        mymsgbox->setDefaultButton(okbtn);
        mymsgbox->show();

        mymsgbox->exec();//阻塞等待用户输入
        if (mymsgbox->clickedButton()==okbtn)//点击了OK按钮
        {
            this->close();
        }
        else{
            this->show();
        }
    });
}


dataExport::~dataExport()
{
    delete ui;
}

//右键创建菜单项
void dataExport::createRightMenu()
{
    popMenu = new QMenu;
    popMenu->addAction("添加记录",this,SLOT(on_addRecBtn_clicked()));
    popMenu->addAction("删除记录",this,SLOT(on_deletRecBtn_clicked()));
}

//加载目录列表
void dataExport::on_loadADirListBtn_clicked()
{
    //点击界面“加载目录列表”按钮，加载所连接数据库中的所有数据库表
    QString str;
    str = "show tables";
    QSqlQueryModel *queryModel = new QSqlQueryModel;
    queryModel->setQuery(str);
    queryModel->setHeaderData(0,Qt::Horizontal,tr("表名"));

    ui->tableView_tables->setModel(queryModel);
    ui->tableView_tables->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    ui->tableView_tables->setColumnWidth(0,250);
    ui->tableView_tables->verticalHeader()->hide();//表头排序隐藏
    ui->tableView_tables->show();

}

//表搜索
void dataExport::on_referBtn_clicked()
{
    ui->lineEdit_refer->clear();
    ui->lineEdit_refer->setFocus();
    QString tabinfo = ui->lineEdit_refer->text();
    QString sql = "select * from tables where table_name = '"+tabinfo+"';";

//  QString sql = "select concat('"+tabinfo+"') from tables; ";
    QSqlQueryModel *queryModel = new QSqlQueryModel;
    queryModel->setQuery(sql);

    ui->tableView_tables->setModel(queryModel);
    ui->tableView_tables->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
//  ui->tableView_tables->setColumnWidth(0,300);
    ui->tableView_tables->verticalHeader()->hide();//表头排序隐藏
    ui->tableView_tables->show();

}

//点击某个表名，右侧显示这个表的具体数据内容
void dataExport::on_tableView_tables_clicked(const QModelIndex &index)
{
   QString tabName = index.data().toString();//获取当前点击的行索引号
   model = new QSqlTableModel(this);
   model->setTable(tabName);
   ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动调整列宽
   ui->tableView->resizeRowsToContents();//高度自适应
   ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:rgb(40,143,218)};");//行表头颜色样式
   ui->tableView->verticalHeader()->setStyleSheet("QHeaderView::section{background-color:rgb(40,143,218)};");//列表头样式设置
   ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中
   ui->tableView->setModel(model);
// ui->tableView->verticalHeader()->hide();//隐藏表头
// ui->tableView->setAlternatingRowColors(true);
// ui->tableView->setStyleSheet("QTableView{background-color:green;alternate-background-color:red}");
   model->select();

}

//添加记录（插入）
void dataExport::on_addRecBtn_clicked()
{
    int rowNum = model->rowCount();
    model->insertRow(rowNum);//添加一行数据
    model->submitAll();//直接提交
}

//单行、多行记录删除
void dataExport::on_deletRecBtn_clicked()
{
//    int curRow = ui->tableView->currentIndex().row();
//    model->removeRow(curRow);
//    model->submitAll();
//    model->database().commit();
//    model->select();
    QItemSelectionModel *selections = ui->tableView->selectionModel(); //返回当前的选择模式
    QModelIndexList selecteds = selections->selectedIndexes(); //返回所有选定的模型项目索引列表

    int ok = QMessageBox::warning(this,tr("删除选中的行!"),tr("你确定删除当前选取中的行吗?"),QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No)
    {
     model->revertAll(); //如果不删除，则撤销
    }
    else
    {
        foreach (QModelIndex index, selecteds)
        {
            int  curRow = index.row(); //删除所有被选中的行
            model->removeRow(curRow);
            model->submitAll(); //提交，在数据库中删除该行
            model->select();
        }
    }
}

////撤销
//void dataExport::on_resRecBtn_clicked()
//{
//    model->revertAll();
//}

//提交
void dataExport::on_subRecBtn_clicked()
{
    model->submitAll();
}

//保存
void dataExport::on_commitRecBtn_clicked()
{
    model->database().transaction();
    if(model->submitAll())
        model->database().commit();
    else
        model->database().rollback();
}

//根据输入的字符串，查询数据内容
void dataExport::on_searchBtn_clicked()
{
    QString referStr = ui->lineEdit->text();
    model->setFilter(referStr);
    model->select();
}
//右键菜单响应函数
void dataExport::slotContextMenu(QPoint pos)
{
    popMenu->exec(QCursor::pos());
}
//自动搜索
void dataExport::on_lineEdit_textChanged(const QString &arg1)
{
    if(ui->lineEdit->text()=="")
    {
        for(int i=0;i<ui->tableView->model()->rowCount();i++)
            ui->tableView->setRowHidden(i,false);
    }
    else
    {
        QString str = ui->lineEdit->text();
        str.remove(QRegExp("\\s"));//去掉所有字符串之间的空白行
        for(int i=0;i<ui->tableView->model()->rowCount();i++)
        {
            ui->tableView->setRowHidden(i,true);
            QString r="";
            QAbstractItemModel *model =ui->tableView->model();
            QModelIndex index;
            for(int j=0;j<ui->tableView->model()->columnCount();j++)
            {
                index = model->index(i,j);
                r+=model->data(index).toString();
            }
            r.remove(QRegExp("\\s"));
            if(r.contains(str,Qt::CaseInsensitive))
                ui->tableView->setRowHidden(i,false);
        }
    }
}
