#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  int res;

  try {
    qDebug() << "bt";
    res = a.exec();
    qDebug() << "at";
  } catch (const std::exception &e) {
    qDebug() << e.what();
  }
  return res;
}
