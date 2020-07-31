#include "psgui.h"
#include "ui_widget.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <unordered_map>

const int PSGui::separation = 50;
const int PSGui::offset = 25 - 8;
const int PSGui::item_id = 0;
const int PSGui::item_name = 1;

PSGui::PSGui(QObject *parent, PSGame *game)
    : QGraphicsScene(parent), game(game) {
  auto board_nodes = game->getBoard()->getNodes();

  // Set up fields
  for (unsigned long i = 0; i < board_nodes.size(); i++) {
    auto node = board_nodes[i];
    QGraphicsPixmapItem *current;

    // Set field texture
    if (node->getType() == node_type::taken)
      current = new QGraphicsPixmapItem(QPixmap(":/img/res/taken_field.png"));
    else if (node->getType() == node_type::empty)
      current = new QGraphicsPixmapItem(QPixmap(":/img/res/empty_field.png"));
    else
      current = new QGraphicsPixmapItem(QPixmap(":/img/res/edge_field.png"));

    // Set field position, scale
    QPointF pos(offset + i % PSBoard::width * separation,
                offset + i / PSBoard::width * separation);
    current->setPos(pos);
    current->setScale(0.5);
    current->setData(item_id, static_cast<uint>(fields.size()));
    current->setData(item_name, "field");

    fields.push_back(current);
  }

  // Label nets
  p1text = addText("P1");
  p1text->setPos(2 * offset + (PSBoard::width / 2 + 1) * separation,
                 offset + separation / 2 - 8);
  p2text = addText("P2");
  p2text->setPos(2 * offset + (PSBoard::width / 2 + 1) * separation,
                 offset + (PSBoard::height - 1) * separation - separation / 2);

  // Draw boundries
  setupBoundries();

  // Draw fields
  for (unsigned long i = 0; i < board_nodes.size(); i++)
    addItem(fields[i]);
}

void PSGui::updateUI() {
  auto board_nodes = game->getBoard()->getNodes();
  auto turn = game->getBoard()->getTurn();

  // Turn information
  if (turn == player::p1) {
    turn_label->setText(QString("Current turn: P1"));
    p1text->setPlainText("P1 <<<");
    p2text->setPlainText("P2");
  } else {
    turn_label->setText(QString("Current turn: P2"));
    p1text->setPlainText("P1");
    p2text->setPlainText("P2 <<<");
  }

  // Draw edges
  for (unsigned long i = 0; i < board_nodes.size(); i++) {
    auto node = board_nodes[i];
    QGraphicsPixmapItem *item = fields[i];
    for (int i = 0; i < 8; i++) {
      auto neighbour = node->getNeighbour(static_cast<node_dir>(i));
      if (neighbour != nullptr && node->getEdge(static_cast<node_dir>(i))) {
        auto neighbour_item_index =
            neighbour->getNode_pos().toIndex(PSBoard::width);
        QGraphicsPixmapItem *neighbour_item = fields[neighbour_item_index];
        auto pen = QPen(Qt::white);
        pen.setWidth(2);
        addLine(QLineF(item->pos() + QPointF(8, 8),
                       neighbour_item->pos() + QPointF(8, 8)),
                pen);
      }
    }
  }
}

void PSGui::connectUI(QLabel *turn_label) {
  this->turn_label = turn_label;
  updateUI();
}

void PSGui::setupBoundries() {
  auto pen = QPen(Qt::white);
  pen.setWidth(4);

  // Left
  lineBetweenFields(0, 1, 0, PSBoard::height - 2, pen);
  // Right
  lineBetweenFields(PSBoard::width - 1, 1, PSBoard::width - 1,
                    PSBoard::height - 2, pen);

  // Top net
  lineBetweenFields(0, 1, PSBoard::width / 2 - 1, 1, pen);
  lineBetweenFields(PSBoard::width / 2 + 1, 1, PSBoard::width - 1, 1, pen);
  lineBetweenFields(PSBoard::width / 2 - 1, 1, PSBoard::width / 2 - 1, 0, pen);
  lineBetweenFields(PSBoard::width / 2 + 1, 1, PSBoard::width / 2 + 1, 0, pen);
  lineBetweenFields(PSBoard::width / 2 - 1, 0, PSBoard::width / 2 + 1, 0, pen);

  // Bottom net
  lineBetweenFields(0, PSBoard::height - 2, PSBoard::width / 2 - 1,
                    PSBoard::height - 2, pen);
  lineBetweenFields(PSBoard::width / 2 + 1, PSBoard::height - 2,
                    PSBoard::width - 1, PSBoard::height - 2, pen);
  lineBetweenFields(PSBoard::width / 2 - 1, PSBoard::height - 2,
                    PSBoard::width / 2 - 1, PSBoard::height - 1, pen);
  lineBetweenFields(PSBoard::width / 2 + 1, PSBoard::height - 2,
                    PSBoard::width / 2 + 1, PSBoard::height - 1, pen);
  lineBetweenFields(PSBoard::width / 2 - 1, PSBoard::height - 1,
                    PSBoard::width / 2 + 1, PSBoard::height - 1, pen);
}

QGraphicsLineItem *PSGui::lineBetweenFields(int x1, int y1, int x2, int y2,
                                            const QPen &pen) {
  return addLine(offset + 8 + x1 * separation, offset + 8 + y1 * separation,
                 offset + 8 + x2 * separation, offset + 8 + y2 * separation,
                 pen);
}

void PSGui::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    auto items_clicked = items(event->buttonDownScenePos(Qt::LeftButton));
    qDebug() << "There are" << items_clicked.size() << "items at position"
             << event->buttonDownScenePos(Qt::LeftButton);

    for (auto item : items_clicked) {
      if (item->data(item_name) == "field") {
        game->clickedOnField(item->data(item_id).toUInt());
        updateUI();
        qDebug() << "Clicked on field:" << item;
      }
    }
  }
  QGraphicsScene::mousePressEvent(event);
}
