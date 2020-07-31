#include "widget.h"
#include "psplayer.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);

  PSGame *game = new PSGame();
  PSGui *scene = new PSGui(this, game);
  scene->setSceneRect(0, 0, 450, 650);
  ui->graphicsView->setScene(scene);

  // Add players to combo boxes
  ui->p1combo->addItem(PSPlayer::name);
  ui->p2combo->addItem(PSPlayer::name);

  // Set default players
  game->setP1(new PSPlayer(game->getBoard()));
  game->setP2(new PSPlayer(game->getBoard()));

  scene->connectUI(ui->turnlabel);
}

Widget::~Widget() { delete ui; }
