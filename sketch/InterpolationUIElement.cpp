// InterpolationUIElement.cpp

#include "AllUIElement.h"

/* handle touch screen on this page
 * @param x - the x coordinate of the touch 
 * @param y - the y coordinate of the touch 
 * @returns bool - true if the touch is on the switcher
 */
bool InterpolationUIElement::handleTouch(long x, long y) {
  /* if screen is touched in the top right of the screen where the switcher is
   * then back out of the current UIElement and transition back to the menu UIElement
   */
  if(y < BOXSIZE && x > (BOXSIZE * SWITCHER)){
    return true;
  }
  /* if screen is touched in interpolation section then pass the x,y coordinates to 
   * the interpMod method to determine what to do with it
   */
  if((x > 50 && x < 260) && (y > 0 && y < ((m_tft->height())/2 + 160))) {
    interpMod(x, y);
  }
  return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Call other methods to draw the display
 */
void InterpolationUIElement::draw(){
  getInterpolation();
  drawMenu();
  drawSwitcher();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Display titles and interpulate value 
 */
void InterpolationUIElement::getInterpolation() {
  m_tft->setTextSize(2);
  m_tft->setTextColor(WHITE);
  m_tft->setCursor(80, 0);
  m_tft->print("Interpolation");

  m_tft->setTextColor(WHITE);
  m_tft->setTextSize(2);
  
  m_tft->setCursor(15,50);
  m_tft->print("current");
  m_tft->setCursor(12,70);
  m_tft->print("interpolation");

  m_tft->setCursor(200,50);
  m_tft->setTextSize(6);
  m_tft->print(":");

  m_tft->fillRect(250, 55, 50, 50, BLACK);
  m_tft->setCursor(260,60);
  m_tft->setTextSize(3);
  m_tft->print(interpValue);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Display the variable interp value, but select thecurrent one with a red block
 * As the interpolation value is predefines as 1 in the UIController and is saved 
 * as a global variable.
 * So this section checks what the value is and colours the interpolation box 
 * accordingly.
 */
void InterpolationUIElement::drawMenu() {
  if(interpValue == 1 && !interpState){
    m_tft->fillRect((m_tft->width())/2 - 20, 140, 55, 40, GREEN);
    m_tft->fillRect((m_tft->width())/2 - 20, 250, 55, 40, BLACK);
    m_tft->fillRect((m_tft->width())/2 - 20, (m_tft->height())/2 + 120, 55, 40, BLACK);
  }
  if(interpValue == 2 && interpState){
    m_tft->fillRect((m_tft->width())/2 - 20, 140, 55, 40, BLACK);
    m_tft->fillRect((m_tft->width())/2 - 20, 250, 55, 40, GREEN);
    m_tft->fillRect((m_tft->width())/2 - 20, (m_tft->height())/2 + 120, 55, 40, BLACK);
  }
  if(interpValue == 3 && interpState){
    m_tft->fillRect((m_tft->width())/2 - 20, 140, 55, 40, BLACK);
    m_tft->fillRect((m_tft->width())/2 - 20, 250, 55, 40, BLACK);
    m_tft->fillRect((m_tft->width())/2 - 20, (m_tft->height())/2 + 120, 55, 40, GREEN);
  }
  /* Reset the interolation values text on the screen and the boxes
   */
  updateInterp();
  updateBoxes();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Reset the boxes after they have been edited 
 */
void InterpolationUIElement::updateInterp(){
  m_tft->setTextColor(WHITE);
  m_tft->setTextSize(3);
  m_tft->setCursor((m_tft->width())/2,150); m_tft->print("1");
  m_tft->setCursor((m_tft->width())/2,260); m_tft->print("2");
  m_tft->setCursor((m_tft->width())/2,(m_tft->height())/2 + 130); m_tft->print("3");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Update the gloabal value shown at the top of the screen
 */
void InterpolationUIElement::updateInterpValue() {
  m_tft->setTextColor(WHITE);
  m_tft->setTextSize(3);
  m_tft->fillRect(250, 55, 50, 50, BLACK);
  m_tft->setCursor(260,60);
  m_tft->print(interpValue);
} 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
 
/* Update the outline of the boxes when changes have been made
 */
void InterpolationUIElement::updateBoxes() {
  m_tft->drawRect((m_tft->width())/2 - 20, 140, 55, 40, WHITE);
  m_tft->drawRect((m_tft->width())/2 - 20, 250, 55, 40, WHITE);
  m_tft->drawRect((m_tft->width())/2 - 20, (m_tft->height())/2 + 120, 55, 40, WHITE);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Depending on what aspect of the screen has been touched will determine what if 
 * statement will be true. 
 * The first checks if the '1' has been pressed
 * The second checks if the '2' has been pressed
 * The third checks if the '3' has been pressed
 * After determinein whihc interpolation to choose, update the display with specified 
 * methods
 * @param x - the x coordinate of the touch 
 * @param y - the y coordinate of the touch 
 */
void InterpolationUIElement::interpMod(long x, long y) {
  // Check co-ord of the 1
  if((x > (m_tft->width())/2 - 20 && x < (m_tft->width())/2 + 35) && (y > 140 && y < 190)) {
    interpValue = 1;
    interpState = true;
    m_tft->fillRect((m_tft->width())/2 - 20, 140, 55, 40, GREEN);
    m_tft->fillRect((m_tft->width())/2 - 20, 250, 55, 40, BLACK);
    m_tft->fillRect((m_tft->width())/2 - 20, (m_tft->height())/2 + 120, 55, 40, BLACK);
  }
  // Check co-ord of the 2
  if((x > (m_tft->width())/2 - 20 && x < (m_tft->width())/2 + 35) && (y > 250 && y < 290)) {
    interpValue = 2;
    interpState = true;
    m_tft->fillRect((m_tft->width())/2 - 20, 140, 55, 40, BLACK);
    m_tft->fillRect((m_tft->width())/2 - 20, 250, 55, 40, GREEN);
    m_tft->fillRect((m_tft->width())/2 - 20, (m_tft->height())/2 + 120, 55, 40, BLACK);
  }
  // Check co-ord of the 3
  if((x > (m_tft->width())/2 - 20 && x < (m_tft->width())/2 + 35) && (y > ((m_tft->height())/2 + 120) && y < ((m_tft->height())/2 + 160))) {
    interpValue = 3;
    interpState = true;
    m_tft->fillRect((m_tft->width())/2 - 20, 140, 55, 40, BLACK);
    m_tft->fillRect((m_tft->width())/2 - 20, 250, 55, 40, BLACK);
    m_tft->fillRect((m_tft->width())/2 - 20, (m_tft->height())/2 + 120, 55, 40, GREEN);
  }
  // Update screen
  updateInterpValue();
  updateInterp();
  updateBoxes();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Text page UI, run each turn and do nothing
 */
void InterpolationUIElement::runEachTurn(){}
