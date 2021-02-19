#ifndef PSGUI_H
#define PSGUI_H

#include "psgame.h"
#include "scsettings.h"
#include <QComboBox>
#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QLabel>
#include <QObject>
#include <vector>

class PSGui : public QGraphicsScene {
  Q_OBJECT
public:
  explicit PSGui(QObject *parent, PSGame *game);
  void gameOver(PlayerID winner);
  void updateUI();

  ScreenCaptureSettings *getScs_widget() const;

public slots:
  void setP1(QString str);
  void setP2(QString str);
  void setCaptureArea(QScreen *screen, QRect capture_area);
  void setAlternate(int state);
  void setFirstPlayer(int player);
  void resetGame();
  void undo();

signals:
  void firstPlayerSignal(int first_player);
  void turnSignal(QString text);
  void moveLengthSignal(QString text);
  void moveDescSignal(QString text);
  void gameWinnerSignal(QString text);
  void gameOverSignal();
  void screenGrab(QPixmap screencap);

private:
  QPixmap *getCapturePreview();
  void redrawEdges();
  void setupFields();
  void setupBoundaries();
  void highlightMove(const std::vector<NodeDir> &move);
  void highlightDirections();
  QGraphicsLineItem *lineBetweenFields(int x1, int y1, int x2, int y2,
                                       const QPen &pen);
  QGraphicsEllipseItem *ellipseAtField(int x, int y, int w, int h,
                                       const QPen &pen);

  static const int separation;
  static const int offset;
  static const int item_id;
  static const int item_name;

  PSGame *game;
  std::vector<QGraphicsPixmapItem *> fields;
  std::vector<QGraphicsLineItem *> edges;
  QGraphicsEllipseItem *ball;
  QGraphicsTextItem *p1text;
  QGraphicsTextItem *p2text;

  // Screen capture settings
  ScreenCaptureSettings *scs_widget;

  // QGraphicsScene interface
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // PSGUI_H
