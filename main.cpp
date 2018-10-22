#include "mainwindow.h"
#include <QApplication>
//#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
