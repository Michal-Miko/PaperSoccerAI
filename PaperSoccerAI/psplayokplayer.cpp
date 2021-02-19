#include "psplayokplayer.h"

PSPlayOKPlayer::PSPlayOKPlayer(PSBoard *b)
    : PSPlayer(b, "PlayOK", PlayerType::playok) {}

PSPlayOKPlayer::~PSPlayOKPlayer() {
  delete screen;
  delete screenpos;
}

std::vector<NodeDir> PSPlayOKPlayer::getMove() {
  // TODO
  return std::vector<NodeDir>{};
}

QScreen *PSPlayOKPlayer::getScreen() const { return screen; }

void PSPlayOKPlayer::setScreen(QScreen *value) { screen = value; }

QRect *PSPlayOKPlayer::getScreenpos() const { return screenpos; }

void PSPlayOKPlayer::setScreenpos(QRect *value) { screenpos = value; }

QPixmap PSPlayOKPlayer::getScreencap() const { return screencap; }

void PSPlayOKPlayer::captureScreen() { screencap = screen->grabWindow(0); }

PlayerID PSPlayOKPlayer::getPlayer_id() const { return player_id; }

void PSPlayOKPlayer::setPlayer_id(const PlayerID &value) { player_id = value; }
