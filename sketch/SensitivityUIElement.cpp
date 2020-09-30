// SensitivityUIElement.cpp

#include "AllUIElement.h"

/* handle touch screen on this page
 * @param x - the x coordinate of the touch 
 * @param y - the y coordinate of the touch 
 * @returns bool - true if the touch is on the switcher
 */
bool SensitivityUIElement::handleTouch(long x, long y) {
  // If in min section, pass x and y to minModification 
  if((x > 45 && x < 135) && (y > 115 && y < 240)) {
    minMod(x, y);
  }
  // If in max section, pass x and y to maxModification 
  if((x > 170 && x < 260) && (y > 115 && y < 240)) {
    maxMod(x,y);
  }
  // If in range section, pass x and y to rangeModification 
  if((x > 10 && x < 300) && (y > 340 && y < 380)) {
    rangeMod(x,y);
  }
  // If in escape section, pass x and y to menue
  if(y < BOXSIZE && x > (BOXSIZE * SWITCHER)){
    return true;
  }
  return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Call other methods to draw the display
 */
void SensitivityUIElement::draw(){
  drawText();
  drawValues();
  drawTextBoxes();
  drawSwitcher();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Display the titles for each of the elements and place them at the 
 *  required locations 
 */
void SensitivityUIElement::drawText(){
  // Title
  m_tft->setCursor(90, 0);
  m_tft->setTextSize(2);
  m_tft->setTextColor(WHITE);
  m_tft->print("Sensitivity");
  // Min
  m_tft->setCursor(40,170);
  m_tft->setTextColor(HX8357_WHITE);
  m_tft->setTextSize(2);
  m_tft->print("Min: ");
  // Max
  m_tft->setCursor(170,170);
  m_tft->setTextColor(HX8357_WHITE);
  m_tft->setTextSize(2);
  m_tft->print("Max: ");
  // Range
  m_tft->setCursor(100,300);
  m_tft->setTextColor(HX8357_WHITE);
  m_tft->setTextSize(2);
  m_tft->print("Range rate");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Deisplay the min and max global values stored in the UIController at 
 * thier specified locations
 */
void SensitivityUIElement::drawValues(){
  // Min value
  m_tft->setCursor(90,170);
  m_tft->setTextColor(HX8357_WHITE);
  m_tft->setTextSize(2);
  m_tft->print(minValue);
  // Max value
  m_tft->setCursor(220,170);
  m_tft->setTextColor(HX8357_WHITE);
  m_tft->setTextSize(2);
  m_tft->print(maxValue);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Display the modification buttons for min and max and range selectors, 
 * if range selector 5 is selected then increment min and max by 5
 * Define what clour the box will be and display the box
 */
void SensitivityUIElement::drawTextBoxes(){
  // Min Modifierers 
  // Increase
  m_tft->setCursor(80,120);
  m_tft->drawRect(45, 115, 90, 40, HX8357_WHITE);
  m_tft->setTextColor(HX8357_WHITE);
  m_tft->setTextSize(4);
  m_tft->print("+");
  // Decrease
  m_tft->setCursor(80,210);
  m_tft->drawRect(45, 200, 90, 40, HX8357_WHITE);
  m_tft->setTextColor(HX8357_WHITE);
  m_tft->setTextSize(4);
  m_tft->print("-");
  
  // Max Modifiers
  // Increase
  m_tft->setCursor(205,120);
  m_tft->drawRect(170, 115, 90, 40, HX8357_WHITE);
  m_tft->setTextColor(HX8357_WHITE);
  m_tft->setTextSize(4);
  m_tft->print("+");
  // Deacrease
  m_tft->setCursor(205,210);
  m_tft->drawRect(170, 200, 90, 40, HX8357_WHITE);
  m_tft->setTextColor(HX8357_WHITE);
  m_tft->setTextSize(4);
  m_tft->print("-");
  
  // Range Selectors
  if(rangeValue == 1){
    m_tft->fillRect(10, 340, 90, 40, GREEN);
    m_tft->fillRect(110, 340, 90, 40, BLACK);
    m_tft->fillRect(210, 340, 90, 40, BLACK);
  }
  if(rangeValue == 2){
    m_tft->fillRect(10, 340, 90, 40, BLACK);
    m_tft->fillRect(110, 340, 90, 40, GREEN);
    m_tft->fillRect(210, 340, 90, 40, BLACK);
  }
  if(rangeValue == 5){
    m_tft->fillRect(10, 340, 90, 40, BLACK);
    m_tft->fillRect(110, 340, 90, 40, BLACK);
    m_tft->fillRect(210, 340, 90, 40, GREEN);
  }
  // Increase by 1
  m_tft->setCursor(50,350);
  m_tft->drawRect(10, 340, 90, 40, HX8357_WHITE);
  m_tft->setTextColor(HX8357_WHITE);
  m_tft->setTextSize(3);
  m_tft->print("1");
  // Increase by 2
  m_tft->setCursor(150,350);
  m_tft->drawRect(110, 340, 90, 40, HX8357_WHITE);
  m_tft->setTextColor(HX8357_WHITE);
  m_tft->setTextSize(3);
  m_tft->print("2");
  // Increase by 5
  m_tft->setCursor(250,350);
  m_tft->drawRect(210, 340, 90, 40, HX8357_WHITE);
  m_tft->setTextColor(HX8357_WHITE);
  m_tft->setTextSize(3);
  m_tft->print("5");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* minMod is used to modify the minium gloabl value stored in the UIController. 
 * By incrementing the minValue by the specified range 
 * The minValue has a minimum of 0 and a min range of 10 form the max value.
 * The minValue has a maximunm of 80. Meaning that the min and max can never 
 * cross or be too close to eachother 
 * @param x - the x coordinate of the touch 
 * @param y - the y coordinate of the touch 
 */
// if (x,y) is range of +coord increase if -coord deacrease
void SensitivityUIElement::minMod(long x, long y){
  // co-ord check on Min Increase  
  if((x > 45 && x < 135) && (y > 115 && y < 155)) {
    minValue += rangeValue;
    if(minValue >= 80)
      minValue = 80;
    if((maxValue - minValue) <= 2) 
      minValue = maxValue - 2;
    m_tft->fillRect(90, 170, 40, 25, BLACK);
    m_tft->setTextColor(WHITE);
    m_tft->setTextSize(2);
    m_tft->setCursor(90,170); 
    m_tft->print(minValue); 
  }
  // co-ord check on Min Decrease 
  if((x > 45 && x < 135) && (y > 200 && y < 240)) {
    minValue -= rangeValue;
    if(minValue <= 0)
      minValue = 0;
    if((maxValue - minValue) <= 2) 
      minValue = maxValue - 2;
    m_tft->fillRect(90, 170, 40, 25, BLACK);
    m_tft->setTextColor(WHITE);
    m_tft->setTextSize(2);
    m_tft->setCursor(90,170); 
    m_tft->print(minValue); 
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* maxMod is used to modify the maximum gloabl value stored in the UIController. 
 * By incrementing the maxValue by the specified range 
 * The maxValue has a minimum of +10 form the min value.
 * The minValue has a maximunm of 120. Meaning that the min and max can never 
 * cross or be too close to eachother 
 * @param x - the x coordinate of the touch 
 * @param y - the y coordinate of the touch 
 */
// if (x,y) is range of + coord increase if -coord deacrease
void SensitivityUIElement::maxMod(long x, long y){  
  // co-ord check on Max Increase  
  if((x > 170 && x < 260) && (y > 115 && y < 155)) {
    maxValue += rangeValue;
    if(maxValue >= 120)
      maxValue = 120;
    if((maxValue - minValue) <= 2) 
      maxValue = minValue + 2;
    m_tft->fillRect(220, 170, 40, 25, BLACK);
    m_tft->setTextColor(WHITE);
    m_tft->setTextSize(2);
    m_tft->setCursor(220,170);
    m_tft->print(maxValue);
  }
  // co-ord check on Max Decrease 
  if((x > 170 && x < 260) && (y > 200 && y < 240)) {
    maxValue -= rangeValue;
    if(maxValue <= 0)
      maxValue = 0;
    if((maxValue - minValue) <= 2) 
      maxValue = minValue +2;
    m_tft->fillRect(220, 170, 40, 25, BLACK);
    m_tft->setTextColor(WHITE);
    m_tft->setTextSize(2);
    m_tft->setCursor(220,170);
    m_tft->print(maxValue);
  }
  
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* rangeMod is used to modify the range gloabl value stored in the UIController. 
 * By selecting a specifed rangeValue, the user can increment the min and max 
 * values by this range. resulting in a fast and accurate change in values.
 * The method chnages the clours of the selected and non selected ranges and 
 * updates the display.
 * @param x - the x coordinate of the touch 
 * @param y - the y coordinate of the touch 
 */
// Modify the range, allows for quicker change in min and max values
void SensitivityUIElement::rangeMod(long x, long y){
  // co-ord check on Range value 1  
  if((x > 10 && x < 100) && (y > 340 && y < 380)) {
    rangeValue = 1;    
    m_tft->fillRect(10, 340, 90, 40, GREEN);
    m_tft->fillRect(110, 340, 90, 40, BLACK);
    m_tft->fillRect(210, 340, 90, 40, BLACK);
  }
  // co-ord check on Range value 2
  if((x > 110 && x < 200) && (y > 340 && y < 380)) {
    rangeValue = 2;  
    m_tft->fillRect(10, 340, 90, 40, BLACK);
    m_tft->fillRect(110, 340, 90, 40, GREEN);
    m_tft->fillRect(210, 340, 90, 40, BLACK);
  }
  // co-ord check on Range value 5
  if((x > 210 && x < 300) && (y > 340 && y < 380)) {
    rangeValue = 5;
    m_tft->fillRect(10, 340, 90, 40, BLACK);
    m_tft->fillRect(110, 340, 90, 40, BLACK);
    m_tft->fillRect(210, 340, 90, 40, GREEN);
  }
  // Draw Range values + boxes
  m_tft->setTextColor(WHITE);
  m_tft->setTextSize(3);
  m_tft->drawRect(10, 340, 90, 40, WHITE);
  m_tft->setCursor(50,350);
  m_tft->print("1");
  m_tft->drawRect(110, 340, 90, 40, WHITE);
  m_tft->setCursor(150,350);
  m_tft->print("2"); 
  m_tft->drawRect(210, 340, 90, 40, WHITE);
  m_tft->setCursor(250,350);
  m_tft->print("5"); 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Text page UI, run each turn and do nothing
 */
void SensitivityUIElement::runEachTurn(){}
