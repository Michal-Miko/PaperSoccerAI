#include "psgui.h"
#include "ui_widget.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <unordered_map>

const int PSGui::separation = 50;
const int PSGui::offset = 25 - 8;
const int PSGui::item_id = 0;
const int PSGui::item_name = 1;

PSGui::PSGui(QObject *parent, PSGame *game)
    : QGraphicsScene(parent), game(game), ball(nullptr) {
  auto board_nodes = game->getBoard()->getNodes();

  // Set up fields (nodes)
  setupFields();

  // Label nets
  p1text = addText("P1");
  p1text->setPos(2 * offset + (PSBoard::width / 2 + 1) * separation,
                 offset + separation / 2 - 8);
  p2text = addText("P2");
  p2text->setPos(2 * offset + (PSBoard::width / 2 + 1) * separation,
                 offset + (PSBoard::height - 1) * separation - separation / 2);

  // Draw boundaries
  setupBoundaries();

  // Draw fields
  for (auto *field : fields)
    addItem(field);
}

void PSGui::gameOver(player winner) {
  emit gameOverSignal();

  if (winner == p1)
    emit gameWinnerSignal("P1 Won");
  else if (winner == p2)
    emit gameWinnerSignal("P2 Won");
}

void PSGui::updateUI() {
  auto board_nodes = game->getBoard()->getNodes();
  auto turn = game->getBoard()->getTurn();
  std::vector<node_dir> move;

  // Update turn information
  if (turn == p1) {
    move = game->getP1()->getMove();
    emit turnSignal("Current turn: P1");
    p1text->setPlainText("P1 <<<");
    p2text->setPlainText("P2");
  } else if (turn == p2) {
    move = game->getP2()->getMove();
    emit turnSignal("Current turn: P2");
    p1text->setPlainText("P1");
    p2text->setPlainText("P2 <<<");
  }

  // Update edges
  redrawEdges();

  // Highlight current move
  highlightMove(move);

  // Update move information
  QString move_str;
  for (int step : move) {
    move_str += QString::number(step);
    if (move_str.length() % 24 == 0)
      move_str += "\n";
  }
  emit moveDescSignal(move_str);
  emit moveLengthSignal(QString("Current move: (") +
                        QString::number(move.size()) + ")");

  // Update field textures
  for (unsigned long i = 0; i < board_nodes.size(); i++) {
    auto node = game->getBoard()->getNode(i);
    if (node->getType() == node_type::empty)
      fields[i]->setPixmap(QPixmap(":/img/res/empty_field.png"));
    else if (node->getType() == node_type::taken)
      fields[i]->setPixmap(QPixmap(":/img/res/taken_field.png"));
  }

  // Check if the game is over
  auto result = game->gameOver();
  if (result != none)
    gameOver(result);
}

void PSGui::setAlternate(int state) {
  game->getBoard()->setAlternate_first(state);
}

void PSGui::setFirstPlayer(int player) {
  game->getBoard()->setFirst_player(static_cast<enum player>(player + 1));
}

void PSGui::resetGame() {
  game->reset();
  updateUI();
  emit firstPlayerSignal(game->getBoard()->getFirst_player() - 1);
}

void PSGui::undo() {
  game->undo();
  updateUI();
}

void PSGui::redrawEdges() {
  // Delete old edges
  for (long i = edges.size() - 1; i >= 0; i--) {
    removeItem(edges[i]);
    delete edges[i];
    edges.pop_back();
  }

  auto board_nodes = game->getBoard()->getNodes();
  // Draw new edges
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

        QLineF line = QLineF(item->pos() + QPointF(8, 8),
                             neighbour_item->pos() + QPointF(8, 8));

        QGraphicsLineItem *current_edge = new QGraphicsLineItem(line);
        current_edge->setPen(pen);
        addItem(current_edge);

        edges.push_back(current_edge);
      }
    }
  }
}

void PSGui::setupFields() {
  auto board_nodes = game->getBoard()->getNodes();

  for (unsigned long i = 0; i < board_nodes.size(); i++) {
    auto node = board_nodes[i];
    QGraphicsPixmapItem *current;

    // Set field texture
    if (node->getType() == node_type::taken)
      current = new QGraphicsPixmapItem(QPixmap(":/img/res/taken_field.png"));
    else if (node->getType() == node_type::empty)
      current = new QGraphicsPixmapItem(QPixmap(":/img/res/empty_field.png"));
    else
      current = new QGraphicsPixmapItem(QPixmap(":/img/res/edge_field2.png"));

    // Set field position, scale, id
    QPointF pos(offset + i % PSBoard::width * separation,
                offset + i / PSBoard::width * separation);
    current->setPos(pos);
    current->setScale(0.5);
    current->setData(item_id, static_cast<uint>(fields.size()));
    current->setData(item_name, "field");

    fields.push_back(current);
  }
}

void PSGui::setupBoundaries() {
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

void PSGui::highlightMove(const std::vector<node_dir> &move) {
  QPen pen(QColor(255, 191, 0));
  pen.setWidth(3);

  // Loop through the move in reverse to find the current path
  auto node_current = game->getBoard()->getBall_node();
  auto node_prev = game->getBoard()->getBall_node();
  for (auto it = move.rbegin(); it != move.rend(); it++) {
    node_dir dir = static_cast<node_dir>((*it + 4) % 8);
    node_prev = node_current;
    node_current = node_prev->getNeighbour(dir);

    int x1 = node_prev->getNode_pos().x;
    int y1 = node_prev->getNode_pos().y;
    int x2 = node_current->getNode_pos().x;
    int y2 = node_current->getNode_pos().y;
    edges.push_back(lineBetweenFields(x1, y1, x2, y2, pen));
  }

  // Highlight current ball position
  if (ball != nullptr)
    delete ball;
  pen.setWidth(5);
  node_current = game->getBoard()->getBall_node();
  int x = node_current->getNode_pos().x;
  int y = node_current->getNode_pos().y;
  ball = ellipseAtField(x, y, 6, 6, pen);
}

QGraphicsLineItem *PSGui::lineBetweenFields(int x1, int y1, int x2, int y2,
                                            const QPen &pen) {
  return addLine(offset + 8 + x1 * separation, offset + 8 + y1 * separation,
                 offset + 8 + x2 * separation, offset + 8 + y2 * separation,
                 pen);
}

QGraphicsEllipseItem *PSGui::ellipseAtField(int x, int y, int w, int h,
                                            const QPen &pen) {
  return addEllipse(offset + 8 + x * separation - w / 2,
                    offset + 8 + y * separation - h / 2, w, h, pen);
}

void PSGui::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    auto items_clicked = items(event->buttonDownScenePos(Qt::LeftButton));

    for (auto item : items_clicked) {
      if (item->data(item_name) == "field") {
        game->clickedOnNode(item->data(item_id).toUInt());
        updateUI();
      }
    }
  }
  QGraphicsScene::mousePressEvent(event);
}
