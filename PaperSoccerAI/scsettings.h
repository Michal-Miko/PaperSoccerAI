#ifndef SCSETTINGS_H
#define SCSETTINGS_H

#include <QDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QLabel>
#include <QScreen>

class ScreenCapScene : public QGraphicsScene {
  Q_OBJECT
public:
  explicit ScreenCapScene(QObject *parent);

public slots:

signals:
  void mousePosChanged(int x, int y);
  void topLeftSelected(int x, int y);
  void bottomRightSelected(int x, int y);

private:
  int x1;
  int y1;

  // QGraphicsScene interface
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

namespace Ui {
class ScreenCaptureSettings;
}

class ScreenCaptureSettings : public QDialog {
  Q_OBJECT

public:
  explicit ScreenCaptureSettings(QWidget *parent = nullptr);
  ~ScreenCaptureSettings();

public slots:
  void confirmSelection();
  void getScreens();
  void setScreen(int index);
  void captureScreen();
  void mousePosUpdate(int x, int y);
  void setTopLeft(int x, int y);
  void setBottomRight(int x, int y);

signals:
  void screenCaptured(QPixmap);
  void selectionConfirmed(QScreen *screen, QRect capture_area);

private:
  Ui::ScreenCaptureSettings *ui;
  QScreen *screen;
  QRect capture_area;
  QGraphicsRectItem *capture_area_item;
  QGraphicsPixmapItem *captured_pixmap;
};

#endif // SCSETTINGS_H
