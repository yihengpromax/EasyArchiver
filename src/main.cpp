#include "mainwindow.h"
#include "Common/resourceloader.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(ResourceLoader::loadIconResouse(":/res/app.ico"));
    MainWindow w;
    w.show();

    return QApplication::exec();
}
