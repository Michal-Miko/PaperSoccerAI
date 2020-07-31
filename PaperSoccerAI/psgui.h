#ifndef PSGUI_H
#define PSGUI_H

#include "psgame.h"
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
  void updateUI();
  void connectUI(QLabel *turn_label);

signals:

private:
  void setupBoundries();
  QGraphicsLineItem *lineBetweenFields(int x1, int y1, int x2, int y2,
                                       const QPen &pen);

  static const int separation;
  static const int offset;
  static const int item_id;
  static const int item_name;

  PSGame *game;
  std::vector<QGraphicsPixmapItem *> fields;
  QGraphicsTextItem *p1text;
  QGraphicsTextItem *p2text;

  // UI elements
  QLabel *turn_label;

  // QGraphicsScene interface
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // PSGUI_H
