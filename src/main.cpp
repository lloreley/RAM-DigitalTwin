#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.setWindowTitle("Симулятор ОЗУ с дефектами");
    w.resize(600, 600);
    w.show();

    return app.exec();
}