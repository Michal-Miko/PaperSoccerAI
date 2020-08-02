#include "mainwindow.h"
#include "psplayer.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  PSGame *game = new PSGame();
  PSGui *psgui = new PSGui(this, game);
  psgui->setSceneRect(0, 0, 450, 650);
  ui->PSGui->setScene(psgui);

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
  connect(ui->undobutton, SIGNAL(released()), psgui, SLOT(undo()));
  connect(ui->resetbutton, SIGNAL(released()), psgui, SLOT(resetGame()));
  connect(ui->firstcombo, SIGNAL(currentIndexChanged(int)), psgui,
          SLOT(setFirstPlayer(int)));
  connect(ui->alternatecheck, SIGNAL(stateChanged(int)), psgui,
          SLOT(setAlternate(int)));
  connect(ui->screenareabutton, SIGNAL(released()), psgui->getScs_widget(),
          SLOT(show()));
  connect(ui->screenareabutton, SIGNAL(released()), psgui->getScs_widget(),
          SLOT(captureScreen()));
  connect(ui->screenareabutton, SIGNAL(released()), psgui->getScs_widget(),
          SLOT(getScreens()));

  connect(psgui, SIGNAL(firstPlayerSignal(int)), ui->firstcombo,
          SLOT(setCurrentIndex(int)));
  connect(psgui, SIGNAL(turnSignal(QString)), ui->turnlabel,
          SLOT(setText(QString)));
  connect(psgui, SIGNAL(moveLengthSignal(QString)), ui->movelenlabel,
          SLOT(setText(QString)));
  connect(psgui, SIGNAL(moveDescSignal(QString)), ui->movelabel,
          SLOT(setText(QString)));
  connect(psgui, SIGNAL(gameWinnerSignal(QString)), ui->winnerlabel,
          SLOT(setText(QString)));
  connect(psgui, SIGNAL(gameOverSignal()), ui->gameover, SLOT(show()));

  //  connect(ui->screenareabutton, SIGNAL(released()), psgui,
  //  SLOT(screenshot())); connect(psgui, SIGNAL(screenGrab(QPixmap)),
  //  ui->screenpreview,
  //          SLOT(setPixmap(QPixmap)));

  // Update ui after connecting signals
  psgui->resetGame();
  psgui->updateUI();
}

MainWindow::~MainWindow() { delete ui; }
