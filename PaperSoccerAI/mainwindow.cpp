#include "mainwindow.h"
#include "psplayer.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  PSGame *game = new PSGame();
  PSGui *scene = new PSGui(this, game);
  scene->setSceneRect(0, 0, 450, 650);
  ui->PSGui->setScene(scene);

  // Add players to combo boxes
  ui->p1combo->addItem(PSPlayer::name);
  ui->p2combo->addItem(PSPlayer::name);
  ui->firstcombo->addItem(QString("Player 1"));
  ui->firstcombo->addItem(QString("Player 2"));

  // Set default players
  game->setP1(new PSPlayer(game->getBoard()));
  game->setP2(new PSPlayer(game->getBoard()));

  // Hide game over frame, set its background
  ui->gameover->hide();
  QPalette gameover_palette;
  gameover_palette.setBrush(QPalette::Window, QColor(0, 0, 0, 100));
  ui->gameover->setPalette(gameover_palette);

  // Connect signals
  connect(ui->undobutton, SIGNAL(released()), scene, SLOT(undo()));
  connect(ui->resetbutton, SIGNAL(released()), scene, SLOT(resetGame()));
  connect(ui->firstcombo, SIGNAL(currentIndexChanged(int)), scene,
          SLOT(setFirstPlayer(int)));
  connect(ui->alternatecheck, SIGNAL(stateChanged(int)), scene,
          SLOT(setAlternate(int)));
  connect(scene, SIGNAL(firstPlayerSignal(int)), ui->firstcombo,
          SLOT(setCurrentIndex(int)));
  connect(scene, SIGNAL(turnSignal(QString)), ui->turnlabel,
          SLOT(setText(QString)));
  connect(scene, SIGNAL(moveLengthSignal(QString)), ui->movelenlabel,
          SLOT(setText(QString)));
  connect(scene, SIGNAL(moveDescSignal(QString)), ui->movelabel,
          SLOT(setText(QString)));
  connect(scene, SIGNAL(gameWinnerSignal(QString)), ui->winnerlabel,
          SLOT(setText(QString)));
  connect(scene, SIGNAL(gameOverSignal()), ui->gameover, SLOT(show()));

  // Update ui after connecting signals
  scene->resetGame();
  scene->updateUI();
}

MainWindow::~MainWindow() { delete ui; }
