#ifndef _CG_BUTTON_H_
#define _CG_BUTTON_H_

#include "Sprite2D.h"
#include "Constants.h"

class Button
{
  GLSYNTHESIZE(Vector2, mPos, Position);
  GLSYNTHESIZE(Vector2, mAnchor, AnchorPoint);
  GLSYNTHESIZE(ButtonState, mState, ButtonState)

  Sprite2D* mImg[4]; // Normal, Hover, Pressed, Disable

  bool mVisible;

public:
  Button(void);
  Button(Sprite2D* pNormalImg, Sprite2D* pHoverImg);
  Button(Sprite2D* pNormalImg, Sprite2D* pHoverImg, Sprite2D* pPressedImg);
  Button(Sprite2D* pNormalImg, Sprite2D* pHoverImg, Sprite2D* pPressedImg, Sprite2D* pDisabledImg);

  Button(const char* pNormalImg, const char* pHoverImg);
  Button(const char* pNormalImg, const char* pHoverImg, const char* pPressedImg);
  Button(const char* pNormalImg, const char* pHoverImg, const char* pPressedImg, const char* pDisabledImg);
  ~Button(void);
  
  void initWithSprite(Sprite2D* pNormalImg, Sprite2D* pHoverImg, Sprite2D* pPressedImg,
    Sprite2D* pDisabledImg);

  void drawImg();

  void setVisible(bool visible);
  bool isVisible();

  void setDisable(bool disable);
  bool isDisable();

  Rect boundingbox();
};  

inline void Button::setVisible(bool visible)
{
  mVisible = visible;
}

inline bool Button::isVisible()
{
  return mVisible;
}

inline void Button::setDisable(bool disable)
{
  if (disable)
    mState = BUTTON_DISABLE;
  else
    mState = BUTTON_NORMAL;
}

inline bool Button::isDisable()
{
  return mState == BUTTON_DISABLE;
}

#endif // !_BUTTON_H_
