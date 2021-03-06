#ifndef _CG_SELECT_PLAYERS_SCENE_H_
#define _CG_SELECT_PLAYERS_SCENE_H_

#include "Sprite2D.h"

#include "Scene.h"
#include "GameScene.h"
#include "ToggleButton.h"
#include "RadioGroup.h"
#include "Button.h"

#include "Graphic.h"

class SelectPlayersScene : Scene
{
  Sprite2D* mBackground;
  Button* mBtnStart;
  Button* mBtnBack;

  Sprite2D* mFrame[4];
  Sprite2D* mPiece[4];
  ToggleButton* mHuman[4];
  ToggleButton* mComputer[4];
  ToggleButton* mDisable[4];

  RadioGroup* mPlayer[4];

  void drawScene();
public:
  SelectPlayersScene(void);
  ~SelectPlayersScene(void);

  void initSprite();

  static SelectPlayersScene& inst();
  void loop();

  void processMouseBegan(int x, int y);
  void processMouseEnded(int x, int y);
  void processMousePassiveMotion(int x, int y);
};  
#endif // !_CG_SELECT_PLAYERS_SCENE_H_


