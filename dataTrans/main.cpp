#include "dataexport.h"
#include <QApplication>
#include "database.h"
#include "dataimport.h"
#include <QPixmap>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QPixmap pixmap("splash.JPG");
//    QSplashScreen splash(pixmap);
//    splash.show();
//    a.processEvents();
    dataExport w;
    w.show();
//    dataBase dlg;
//    dlg.show();
//    dataImport dlg1;
//    dlg1.show();
//    splash.finish(&w);

    return a.exec();
}
