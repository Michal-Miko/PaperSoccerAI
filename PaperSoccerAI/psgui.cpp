#include "psgui.h"
#include "ui_widget.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QScreen>
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

  // Setup screen capture settings window
  scs_widget = new ScreenCaptureSettings();

  // Connect screen capture settings window
  connect(scs_widget, SIGNAL(selectionConfirmed(QScreen *, QRect)), this,
          SLOT(setCaptureArea(QScreen *, QRect)));
}

void PSGui::gameOver(PlayerID winner) {
  emit gameOverSignal();

  if (winner == p1)
    emit gameWinnerSignal("P1 Wins");
  else if (winner == p2)
    emit gameWinnerSignal("P2 Wins");
}

void PSGui::updateUI() {
  auto board_nodes = game->getBoard()->getNodes();
  auto turn = game->getBoard()->getTurn();
  std::vector<NodeDir> move;

  // Update turn information
  if (turn == p1) {
    if (game->getP1()->type == PlayerType::player)
      move = game->getP1()->getMove();
    emit turnSignal("Current turn: P1");
    p1text->setPlainText("P1 <<<");
    p2text->setPlainText("P2");
  } else if (turn == p2) {
    if (game->getP2()->type == PlayerType::player)
      move = game->getP2()->getMove();
    emit turnSignal("Current turn: P2");
    p1text->setPlainText("P1");
    p2text->setPlainText("P2 <<<");
  }

  // Update edges
  redrawEdges();

  // TODO: Add a checkboks to hide this
  // Highlight possible mvoe directions
  highlightDirections();

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
    if (node->getType() == NodeType::empty)
      fields[i]->setPixmap(QPixmap(":/img/res/empty_field.png"));
    else if (node->getType() == NodeType::taken)
      fields[i]->setPixmap(QPixmap(":/img/res/taken_field.png"));
  }

  // Check if the game is over
  auto result = game->gameOver();
  if (result != none)
    gameOver(result);
}

void PSGui::setCaptureArea(QScreen *screen, QRect capture_area) {
  emit screenGrab(*getCapturePreview());
}

void PSGui::setAlternate(int state) {
  game->getBoard()->setAlternate_first(state);
}

void PSGui::setFirstPlayer(int player) {
  game->getBoard()->setFirst_player(static_cast<PlayerID>(player + 1));
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

QPixmap *PSGui::getCapturePreview() {
  QScreen *screen = scs_widget->getScreen();
  QRect area = scs_widget->getCapture_area();
  // qDebug() << area.x() << area.y() << area.width() << area.height();

  return new QPixmap(
      screen->grabWindow(0, area.x(), area.y(), area.width(), area.height()));
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
      auto neighbour = node->getNeighbour(static_cast<NodeDir>(i));
      if (neighbour != nullptr && node->getEdge(static_cast<NodeDir>(i))) {
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
    if (node->getType() == NodeType::taken)
      current = new QGraphicsPixmapItem(QPixmap(":/img/res/taken_field.png"));
    else if (node->getType() == NodeType::empty)
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

void PSGui::highlightMove(const std::vector<NodeDir> &move) {
  QPen pen(QColor(255, 191, 0));
  pen.setWidth(3);

  // Loop through the move in reverse to find the current path
  PSNode *node_current = game->getBoard()->getBall_node();
  PSNode *node_prev;
  for (auto it = move.rbegin(); it != move.rend(); it++) {
    NodeDir dir =
        static_cast<NodeDir>((*it + 4) % 8); // TODO: Should be a function
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

void PSGui::highlightDirections() {
  // Pen for moves that extend the player's turn
  QPen pen_ext(QColor(46, 125, 50));
  pen_ext.setWidth(1);
  // Pen for moves that end the player's turn
  QPen pen_end(QColor(100, 100, 100));
  pen_end.setWidth(1);
  // Pen for moves that cause the player to lose
  QPen pen_lose(QColor(198, 40, 40));
  pen_lose.setWidth(3);
  // Pen for moves that cause the player to win
  QPen pen_win(QColor(46, 125, 50));
  pen_win.setWidth(3);

  auto ball_node = game->getBoard()->getBall_node();
  for (auto dir : ball_node->getOpenDirections()) {
    auto neigh = ball_node->getNeighbour(dir);
    int x1 = ball_node->getNode_pos().x;
    int y1 = ball_node->getNode_pos().y;
    int x2 = neigh->getNode_pos().x;
    int y2 = neigh->getNode_pos().y;
    if (game->getBoard()->moveWins(dir))
      edges.push_back(lineBetweenFields(x1, y1, x2, y2, pen_win));
    else if (game->getBoard()->moveLoses(dir))
      edges.push_back(lineBetweenFields(x1, y1, x2, y2, pen_lose));
    else if (neigh->endsMove())
      edges.push_back(lineBetweenFields(x1, y1, x2, y2, pen_end));
    else
      edges.push_back(lineBetweenFields(x1, y1, x2, y2, pen_ext));
  }
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

ScreenCaptureSettings *PSGui::getScs_widget() const { return scs_widget; }

void PSGui::setP1(QString str) {
  // qDebug() << "SETP1 " << str << '\n';
  auto type = PNType.at(str);
  switch (type) {
  case PlayerType::player:
    game->setP1(new PSPlayer(game->getBoard()));
    break;
  case PlayerType::ai:
    game->setP1(new PSAIPlayer(game->getBoard()));
    break;
  case PlayerType::ai_random:
    game->setP1(new PSRandomPlayer(game->getBoard()));
    break;
  case PlayerType::playok:
    game->setP1(new PSPlayOKPlayer(game->getBoard()));
    break;
  }
}

void PSGui::setP2(QString str) {
  // qDebug() << "SETP2 " << str << '\n';
  auto type = PNType.at(str);
  switch (type) {
  case PlayerType::player:
    game->setP2(new PSPlayer(game->getBoard()));
    break;
  case PlayerType::ai:
    game->setP2(new PSAIPlayer(game->getBoard()));
    break;
  case PlayerType::ai_random:
    game->setP2(new PSRandomPlayer(game->getBoard()));
    break;
  case PlayerType::playok:
    game->setP2(new PSPlayOKPlayer(game->getBoard()));
    break;
  }
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
