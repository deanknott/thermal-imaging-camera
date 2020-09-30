// ImagePageUIElement.cpp

#include "AllUIElement.h"

/*  low (blue) and high (red) range of the sensor 
 *  set to the base min 27 and max 37 
 *  but can be changed in the sesnitivity page
 *  minValue and maxValue are global variables
 */
#define MINTEMP minValue // sugested 20
#define MAXTEMP maxValue// sugested 28
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* the colors we will be using
 * predefined by Adafruit for the amg8833
 */
const uint16_t camColors[] = {0x480F,
0x400F,0x400F,0x400F,0x4010,0x3810,0x3810,0x3810,0x3810,0x3010,0x3010,
0x3010,0x2810,0x2810,0x2810,0x2810,0x2010,0x2010,0x2010,0x1810,0x1810,
0x1811,0x1811,0x1011,0x1011,0x1011,0x0811,0x0811,0x0811,0x0011,0x0011,
0x0011,0x0011,0x0011,0x0031,0x0031,0x0051,0x0072,0x0072,0x0092,0x00B2,
0x00B2,0x00D2,0x00F2,0x00F2,0x0112,0x0132,0x0152,0x0152,0x0172,0x0192,
0x0192,0x01B2,0x01D2,0x01F3,0x01F3,0x0213,0x0233,0x0253,0x0253,0x0273,
0x0293,0x02B3,0x02D3,0x02D3,0x02F3,0x0313,0x0333,0x0333,0x0353,0x0373,
0x0394,0x03B4,0x03D4,0x03D4,0x03F4,0x0414,0x0434,0x0454,0x0474,0x0474,
0x0494,0x04B4,0x04D4,0x04F4,0x0514,0x0534,0x0534,0x0554,0x0554,0x0574,
0x0574,0x0573,0x0573,0x0573,0x0572,0x0572,0x0572,0x0571,0x0591,0x0591,
0x0590,0x0590,0x058F,0x058F,0x058F,0x058E,0x05AE,0x05AE,0x05AD,0x05AD,
0x05AD,0x05AC,0x05AC,0x05AB,0x05CB,0x05CB,0x05CA,0x05CA,0x05CA,0x05C9,
0x05C9,0x05C8,0x05E8,0x05E8,0x05E7,0x05E7,0x05E6,0x05E6,0x05E6,0x05E5,
0x05E5,0x0604,0x0604,0x0604,0x0603,0x0603,0x0602,0x0602,0x0601,0x0621,
0x0621,0x0620,0x0620,0x0620,0x0620,0x0E20,0x0E20,0x0E40,0x1640,0x1640,
0x1E40,0x1E40,0x2640,0x2640,0x2E40,0x2E60,0x3660,0x3660,0x3E60,0x3E60,
0x3E60,0x4660,0x4660,0x4E60,0x4E80,0x5680,0x5680,0x5E80,0x5E80,0x6680,
0x6680,0x6E80,0x6EA0,0x76A0,0x76A0,0x7EA0,0x7EA0,0x86A0,0x86A0,0x8EA0,
0x8EC0,0x96C0,0x96C0,0x9EC0,0x9EC0,0xA6C0,0xAEC0,0xAEC0,0xB6E0,0xB6E0,
0xBEE0,0xBEE0,0xC6E0,0xC6E0,0xCEE0,0xCEE0,0xD6E0,0xD700,0xDF00,0xDEE0,
0xDEC0,0xDEA0,0xDE80,0xDE80,0xE660,0xE640,0xE620,0xE600,0xE5E0,0xE5C0,
0xE5A0,0xE580,0xE560,0xE540,0xE520,0xE500,0xE4E0,0xE4C0,0xE4A0,0xE480,
0xE460,0xEC40,0xEC20,0xEC00,0xEBE0,0xEBC0,0xEBA0,0xEB80,0xEB60,0xEB40,
0xEB20,0xEB00,0xEAE0,0xEAC0,0xEAA0,0xEA80,0xEA60,0xEA40,0xF220,0xF200,
0xF1E0,0xF1C0,0xF1A0,0xF180,0xF160,0xF140,0xF100,0xF0E0,0xF0C0,0xF0A0,
0xF080,0xF060,0xF040,0xF020,0xF800,};
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* fucntions that have been defined by adafruit for the interpolation.cpp 
 * of the 64x1 array into an 8x8 display
 * predefined by Adafruit for the amg8833
 */
void set_point(float *p, uint8_t rows, uint8_t cols, int8_t x, int8_t y, float f);
void get_adjacents_2d(float *src, float *dest, uint8_t rows, uint8_t cols, int8_t x, int8_t y);
float cubicInterpolate(float p[], float x);
float bicubicInterpolate(float p[], float x, float y);
void interpolate_image(float *src, uint8_t src_rows, uint8_t src_cols, float *dest, uint8_t dest_rows, uint8_t dest_cols);
float get_point(float *p, uint8_t rows, uint8_t cols, int8_t x, int8_t y);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*counter to monitor the number of presses on the display
 * used in a mod later on
 */
int expandCounter = 0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Local array fo size 8x8, to store the SD files when they are read in
 * 6 variations as the 6 will roatate the SD files on mod 6
 */
// SD files
float sdA[8 * 8];
float sdB[8 * 8];
float sdC[8 * 8];
float sdD[8 * 8];
float sdE[8 * 8];
float sdF[8 * 8];
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* handle touch screen on this page
 * @param x - the x coordinate of the touch 
 * @param y - the y coordinate of the touch 
 * @returns bool - true if the touch is on the switcher
 */
bool ImagePageUIElement::handleTouch(long x, long y) {
  /* if screen is touched in the top right of the screen where the switcher is
   * then back out of the current UIElement and transition back to the menu UIElement
   */
  if(y < BOXSIZE && x > (BOXSIZE * SWITCHER)){
    expandCounter = 0;
    return true;
  }
  /* if screen is touched in the bottom of the screen where the page number section is
   * then pass the x,y coordinate to the pageChange method to determine what to do 
   * with it
   */
  if((x > 15 && x < 320) && (y > 410 && y < 450)) {
    pageChange(x, y);
  }
  /* When the expand counter = 0, this means that the image at a secpiefed location should be expanded
   */
  int expandChecker = expandCounter % 2;
  if(expandChecker == 0){
    if ((x>0 && x < 80) && (y> 40&& y< 120)){
      expandCounter += 1;
      expandImage(a);
    }
    if ((x>100 && x < 180) && (y> 40&& y< 120)){
      expandCounter += 1;
      expandImage(b);
    }
    if ((x>200 && x < 280) && (y> 40&& y< 120)){
      expandCounter += 1;
      expandImage(c);
    }
  }  
  /* if the expander checker is 1, this means that the screen has either not been touched 
   * or touched twice. If touched twice it means that an image has been expanded and colapsed.
   * And a black reset screen is used to cover the screen 
   */
  if ((x>0 && x < 350 ) && (y> 40&& y< 400) && expandChecker == 1){
    expandCounter += 1;
    m_tft->fillRect(0,0,m_tft->width(), m_tft->height(), BLACK);
    draw();
  }
  return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* call other methods to draw the display
 */
void ImagePageUIElement::draw(){
  getInterpolation();
  drawSwitcher();
  drawMenu();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Draw titles
 */
void ImagePageUIElement::getInterpolation() {
  m_tft->setTextSize(2);
  m_tft->setTextColor(WHITE);
  m_tft->setCursor(110, 0);
  m_tft->print("Image page");
  m_tft->setTextSize(2);
  m_tft->setCursor(100, 40);
  m_tft->print("Local images");

  m_tft->setCursor(115, 180);
  m_tft->print("SD images");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Used to call other fucntions to draw the display and call methods
 * page number, local images when taken and SD images. Reads from the SD card before displaying
 */
void ImagePageUIElement::drawMenu() {
  m_tft->setTextSize(2);
  m_tft->setTextColor(WHITE);
  m_tft->setCursor(75, 420);
  m_tft->print("page number: "); m_tft->print(imagePageNumber);
  drawReverseSwitcher(25, 410);
  drawSwitcher(270, 410);

  /* Display local images on a cycle of 3 
   * calling arrays a,b,c, at differeing x and y coordinates
   */
  if(localCounter > 0){
    drawpixels(a, 8, 8, 10, 10, 0, 30);
    if(localCounter > 1){
      drawpixels(b, 8, 8, 10, 10, 100, 30);
      if(localCounter > 2){
        drawpixels(c, 8, 8, 10, 10, 200, 30);
      }
    }
  }  
  /* Read from SD card with a mutable filename and display
   * Depending on page number, read mutable filenames into local 6 arrays
   */
  char filename[15] = "/image_##.txt";
  for (uint8_t i = 0 + ((imagePageNumber -1)*6); i < imagePageNumber * 6; i++) {
    filename[7] = '0' + i / 10;
    filename[8] = '0' + i % 10;
    int iMod = i%7;
    if (SD.exists(filename)) {
      switch(iMod) {
        case 0 : readFile(SD, filename, sdA);break;
        case 1 : readFile(SD, filename, sdB);break;
        case 2 : readFile(SD, filename, sdC);break;
        case 3 : readFile(SD, filename, sdD);break;
        case 4 : readFile(SD, filename, sdE);break;
        case 5 : readFile(SD, filename, sdF);break;
      }      
    }
  }
  /* Display the first 6 array without the 0 as this breaks the display
   * Then display the remaining SD files, on a loop of 6
   */
  int sdMod = sdCounter %7; 
  if(sdCounter <= 6 && sdCounter >=0){
    switch(sdMod) {
      case 1 : 
        drawpixels(sdA, 8, 8, 10, 10, 0, 150);
        break;
      case 2 : 
        drawpixels(sdA, 8, 8, 10, 10, 0, 15); 
        drawpixels(sdB, 8, 8, 10, 10, 100, 150);
        break;
      case 3 : 
        drawpixels(sdA, 8, 8, 10, 10, 0, 150); 
        drawpixels(sdB, 8, 8, 10, 10, 100, 150);
        drawpixels(sdC, 8, 8, 10, 10, 200, 150);
        break;
      case 4 : 
        drawpixels(sdA, 8, 8, 10, 10, 0, 150);
        drawpixels(sdB, 8, 8, 10, 10, 100, 150);
        drawpixels(sdC, 8, 8, 10, 10, 200, 150);
        drawpixels(sdD, 8, 8, 10, 10, 0, 250);
        break;
      case 5 : 
        drawpixels(sdA, 8, 8, 10, 10, 0, 150);
        drawpixels(sdB, 8, 8, 10, 10, 100, 150);
        drawpixels(sdC, 8, 8, 10, 10, 200, 150);
        drawpixels(sdD, 8, 8, 10, 10, 0, 250);
        drawpixels(sdE, 8, 8, 10, 10, 100, 250);
        break;
      case 6 : 
        drawpixels(sdA, 8, 8, 10, 10, 0, 150);
        drawpixels(sdB, 8, 8, 10, 10, 100, 150);
        drawpixels(sdC, 8, 8, 10, 10, 200, 150);
        drawpixels(sdD, 8, 8, 10, 10, 0, 250);
        drawpixels(sdE, 8, 8, 10, 10, 100, 250);
        drawpixels(sdF, 8, 8, 10, 10, 200, 250);
        break;
    }
  }
  int sdMod_ = sdCounter %6; 
  if(sdCounter > 6 ){
    switch(sdMod) {
      case 0 : 
        drawpixels(sdA, 8, 8, 10, 10, 0, 150);
        break;
      case 1 : 
        drawpixels(sdA, 8, 8, 10, 10, 0, 150); 
        drawpixels(sdB, 8, 8, 10, 10, 100, 150);
        break;
      case 2 : 
        drawpixels(sdA, 8, 8, 10, 10, 0, 150); 
        drawpixels(sdB, 8, 8, 10, 10, 100, 150);
        drawpixels(sdC, 8, 8, 10, 10, 200, 150);
        break;
      case 3 : 
        drawpixels(sdA, 8, 8, 10, 10, 0, 150);
        drawpixels(sdB, 8, 8, 10, 10, 100, 150);
        drawpixels(sdC, 8, 8, 10, 10, 200, 150);
        drawpixels(sdD, 8, 8, 10, 10, 0, 250);
        break;
      case 4 : 
        drawpixels(sdA, 8, 8, 10, 10, 0, 150);
        drawpixels(sdB, 8, 8, 10, 10, 100, 150);
        drawpixels(sdC, 8, 8, 10, 10, 200, 150);
        drawpixels(sdD, 8, 8, 10, 10, 0, 250);
        drawpixels(sdE, 8, 8, 10, 10, 100, 250);
        break;
      case 5 : 
        drawpixels(sdA, 8, 8, 10, 10, 0, 150);
        drawpixels(sdB, 8, 8, 10, 10, 100, 150);
        drawpixels(sdC, 8, 8, 10, 10, 200, 150);
        drawpixels(sdD, 8, 8, 10, 10, 0, 250);
        drawpixels(sdE, 8, 8, 10, 10, 100, 250);
        drawpixels(sdF, 8, 8, 10, 10, 200, 250);
        break;
    }
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Display the 8x8 or interpolated images 
 * @param p  - float array that holds the thermal data 
 * @param row - holds the array dimensions 
 * @param cols - holds the array dimensions 
 * @param boxWidth - holds the specified pixels 
 * @param boxHeightd - holds the specified pixels
 * @param x_coord - holds the modification of 8x8 arrays to display
 * @param y_coord - holds the modification of 8x8 arrays to display
 */
void ImagePageUIElement::drawpixels(float *p, uint8_t rows, uint8_t cols, uint8_t boxWidth, uint8_t boxHeight, int x_coord, int y_coord) {
  int colorTemp;
  for (int y=0; y<rows; y++) {
    for (int x=0; x<cols; x++) {
      float val = get_point(p, rows, cols, x, y);
      if(val >= MAXTEMP) colorTemp = MAXTEMP;
      else if(val <= MINTEMP) colorTemp = MINTEMP;
      else colorTemp = val;
      
      uint8_t colorIndex = map(colorTemp, MINTEMP, MAXTEMP, 0, 255);
      colorIndex = constrain(colorIndex, 0, 255);
      //draw the pixels!
      m_tft->fillRect(25+x_coord+boxWidth * x, y_coord+55+boxHeight * y, boxWidth, boxHeight, camColors[colorIndex]);
    } 
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/* When an image is pressed the specief array will be expanded and cover the 
 * other 8x8 arrays
 * If interpolation is eddited, use that to give a more detailed image
 * @param image - float array that holds the thermal data 
 */
void ImagePageUIElement::expandImage(float* image){
  int interpMulti = (8 * interpValue);
  #define INTERPOLATED_COLS interpMulti 
  #define INTERPOLATED_ROWS interpMulti
  float dest_2d[INTERPOLATED_ROWS * INTERPOLATED_COLS]; 
      
  interpolate_image(image, 8, 8, dest_2d, INTERPOLATED_ROWS, INTERPOLATED_COLS);
  uint16_t boxsize = min(m_tft->width() / INTERPOLATED_COLS, m_tft->height() / INTERPOLATED_ROWS);
  drawpixels(dest_2d, INTERPOLATED_ROWS, INTERPOLATED_COLS, boxsize, boxsize, -25, 0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Method called when touch screen is pressed within a specifed location
 * min = 1 : max = infin 
 * cover number with block and re-write
 * @param x - the x coordinate of the touch 
 * @param y - the y coordinate of the touch 
 */
void ImagePageUIElement::pageChange(long x, long y) {
  // Check co-ord of the 1
  if((x > 10 && x < 50) && (y > 410 && y < 440)) {
    m_tft->fillRect(230, 415, 20, 20, BLACK);
    imagePageNumber = imagePageNumber - 1;
    if(imagePageNumber <=1){
      imagePageNumber =1;
    }
  }
  if((x > 270 && x < 310) && (y > 410 && y < 440)) {
    m_tft->fillRect(230, 415, 20, 20, BLACK);
    imagePageNumber = imagePageNumber + 1;
  }
  
  m_tft->setTextSize(2);
  m_tft->setTextColor(GREEN);
  m_tft->setCursor(230, 420);
  m_tft->print(imagePageNumber);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Reads from the SD card and write to an array, after splitting the string 
 * into number and converting to floats
 * @param fs  - fs which is inturn the SD class
 * @param path  - char array which defines what file to open
 * @param sdArray  - float array that holds the thermal data 
 */
void ImagePageUIElement::readFile(fs::FS &fs, const char * path, float* sdArray){
  Serial.printf("Reading file: %s\n", path);
  File file = fs.open(path);
  if(!file){
      Serial.println("Failed to open file for reading");
      return;
  }
  String finalString="";
  while (file.available()){
    finalString+=(char)file.read();
  }
  for(int i = 0; i< 63; i++){
    sdArray[i] = finalString.substring((i*6),(i*6)+5).toFloat();   
  }  
  file.close();  
  Serial.print("Successful transfer");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Text page UI, run each turn and do nothing
 */
void ImagePageUIElement::runEachTurn(){ 
}
