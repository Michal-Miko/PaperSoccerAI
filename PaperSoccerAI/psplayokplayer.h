#ifndef PSPLAYOKPLAYER_H
#define PSPLAYOKPLAYER_H

#include "psplayer.h"

class PSPlayOKPlayer : public PSPlayer {
public:
  PSPlayOKPlayer(PSBoard *b);
  virtual ~PSPlayOKPlayer() override;

  virtual std::vector<NodeDir> getMove() override;

  void analyzeScreen();

  QScreen *getScreen() const;
  void setScreen(QScreen *value);

  QRect *getScreenpos() const;
  void setScreenpos(QRect *value);

  QPixmap getScreencap() const;

  PlayerID getPlayer_id() const;
  void setPlayer_id(const PlayerID &value);

private:
  void captureScreen();
  int boundry_thickness;
  PlayerID player_id;
  QPixmap screencap;
  QScreen *screen;
  QRect *screenpos;
};

#endif // PSPLAYOKPLAYER_H
