/*      raspiCam
 *
 * This is a Pi cam GUI demostration which connects through wifi.
 * Author: Peter Wang
 * Date: 2017/05/28
 *
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
