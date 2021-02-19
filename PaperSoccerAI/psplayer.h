#ifndef PLAYER_H
#define PLAYER_H

#include "psboard.h"
#include "psnode.h"
#include <QPixmap>
#include <QScreen>
#include <QString>
#include <map>
#include <vector>

enum PlayerType { player, ai, playok, ai_random };
const std::map<PlayerType, QString> PTName{{PlayerType::player, "Player"},
                                           {PlayerType::ai, "AI"},
                                           {PlayerType::ai_random, "Random"},
                                           {PlayerType::playok, "PlayOK"}};
const std::map<QString, PlayerType> PNType{{"Player", PlayerType::player},
                                           {"AI", PlayerType::ai},
                                           {"Random", PlayerType::ai_random},
                                           {"PlayOK", PlayerType::playok}};

class PSPlayer {
public:
  PSPlayer(PSBoard *b);
  virtual ~PSPlayer();

  // Returns true if the move is complete -- the ball is in the final position,
  // false otherwise
  bool playerInput(NodeDir dir);
  // Undoes the last input if the move is not complete
  void undoInput();
  // Reset player attributes
  void reset();

  virtual std::vector<NodeDir> getMove();

  const QString name;
  const PlayerType type;

protected:
  PSPlayer(PSBoard *b, QString name, PlayerType type);
  PSBoard *board;
  std::vector<NodeDir> move;
  bool move_complete;
};

#endif // PLAYER_H
