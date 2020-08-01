#ifndef WIDGET_H
#define WIDGET_H

#include "psgame.h"
#include "psgui.h"
#include <QGraphicsPixmapItem>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  PSGui *scene;
};
#endif // WIDGET_H
