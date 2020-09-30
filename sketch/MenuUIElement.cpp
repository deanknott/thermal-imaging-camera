// MenuUIElement.cpp

#include "AllUIElement.h"

/* handle touch screen on this page
 * @param x - the x coordinate of the touch 
 * @param y - the y coordinate of the touch 
 * @returns bool - true if the touch is on the switcher
 */
bool MenuUIElement::handleTouch(long x, long y) {
  // D("text mode: responding to touch @ %d/%d/%d: ", x, y,-1)
  m_tft->setTextColor(WHITE, BLACK);
  uint8_t menuItem = mapTextTouch(x, y);
  D("menuItem=%d, ", menuItem)

  if(menuItem > 0 && menuItem <= NUM_UI_ELEMENTS) {
    menuItemSelected = menuItem;
    return true;
  }
  return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Managaes all fo the different touches with regards to the different UIElements
 * @param x - the x coordinate of the touch 
 * @param y - the y coordinate of the touch 
 * @ return the item number
 */
uint8_t MenuUIElement::mapTextTouch(long xInput, long yInput) {
  for(int y = 30, i = 1; y < 480; y += 48, i++)
    if(xInput > 270 && yInput > y && yInput < y + 48)
      return i;
  return -1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Draws the menu and all displays the switchers for each of the UIElements
 */
void MenuUIElement::draw(){
  m_tft->setTextSize(2);
  m_tft->setTextColor(WHITE);

  m_tft->setCursor(230, 0);
  m_tft->print("MENU");

  uint16_t yCursor = 30;
  m_tft->drawFastHLine(0, yCursor, 320, GREEN);
  yCursor += 16;

  for(int i = 1; i < NUM_UI_ELEMENTS; i++) {
    m_tft->setCursor(0, yCursor);
    m_tft->print(ui_mode_names[i]);
    drawSwitcher(288, yCursor - 12);
    yCursor += 32;
    m_tft->drawFastHLine(0, yCursor, 320, GREEN);
    yCursor += 16;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Text page UI, run each turn and do nothing
 */
void MenuUIElement::runEachTurn(){}
