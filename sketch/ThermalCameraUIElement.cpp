// ThermalCameraUIElement.cpp
/* include fies that are needed for the thermal imagaing camera
 * AMG8833 [8x8] sensor
 * SD card 
 * FS for calling SD methhods
 */
#include "AllUIElement.h"
#include <Adafruit_Sensor.h>    // base class etc. for sensor abstraction
#include <Adafruit_AMG88xx.h>
#include <SD.h>
#include <FS.h>

/* define amg as the sensor
 */
Adafruit_AMG88xx amg;

/*  low (blue) and high (red) range of the sensor 
 *  set to the base min 27 and max 37 
 *  but can be changed in the sesnitivity page
 *  minValue and maxValue are global variables
 */
#define MINTEMP minValue // sugested 20
#define MAXTEMP maxValue// sugested 28

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

/* Defining values that are used to generate the [8x8] array
 * Defining the pixels [8x8] array
 * Defining the roatated [8x8] array
 */
#define AMG_COLS 8
#define AMG_ROWS 8
float pixels[AMG_COLS * AMG_ROWS];
float rotatedpixels[AMG_ROWS * AMG_COLS];
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* min and max value
 */
float savedMax;
float savedMin;
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* fucntions that have been defined by adafruit for the interpolation.cpp 
 * of the 64x1 array into an 8x8 display
 * predefined by Adafruit for the amg8833
 */
float get_point(float *p, uint8_t rows, uint8_t cols, int8_t x, int8_t y);
void set_point(float *p, uint8_t rows, uint8_t cols, int8_t x, int8_t y, float f);
void get_adjacents_2d(float *src, float *dest, uint8_t rows, uint8_t cols, int8_t x, int8_t y);
float cubicInterpolate(float p[], float x);
float bicubicInterpolate(float p[], float x, float y);
void interpolate_image(float *src, uint8_t src_rows, uint8_t src_cols, 
                       float *dest, uint8_t dest_rows, uint8_t dest_cols);            
//////////////////////////////////////////////////////////////////////////

/**
 * Function that handles the touch on this page
 * 
 * @param x - the x coordinate of the touch 
 * @param y - the y coordinate of the touch 
 * @returns bool - true if the touch is on the switcher
 */
bool ThermalCameraUIElement::handleTouch(long x, long y) {
  return y < BOXSIZE && x > (BOXSIZE * SWITCHER);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Starts the AMG8833 sensor with begin and calls the other methods to run 
 * the thermal imaging camera and display the data output 
 */
void ThermalCameraUIElement::draw(){
//  amg.begin();
  if (!amg.begin()) {
        Serial.println("Could not find a valid AMG88xxsensor, check wiring!");
      while (1);
  }
  drawTestcard();
  drawSwitcher();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Defines what the interpolated array will need to be based on the 
 * interpolation gloabl value stored in the UIController. 
 * Calls the temp gradient methods, to display the global mina nd max 
 * values and what could they and the gradient will depict.
 * Runs the thermal imaging camera in a while loop
 */
void ThermalCameraUIElement::drawTestcard() {
  savedMin = 25.0;
  
  int interpMulti = (8 * interpValue);
  #define INTERPOLATED_COLS interpMulti
  #define INTERPOLATED_ROWS interpMulti
  float dest_2d[INTERPOLATED_ROWS * INTERPOLATED_COLS];
  tempGradient();
  /* Run in a while loop as bigDemo would nto allow for the thermal imaging 
   * camera to be run more than once. The while loop is broken when button 1 
   * is pressed.
   * The AMG8833 sensor reads the data in to the [8x8] pixels array that was 
   * predefined.
   * Using a rotation method to aligh the camera orientation with the display.
   * Depending on the interpolation value, will determine how the pixels array 
   * will be treated. 
   * If the interpolation is > 1, then use the mutable innterploated array to 
   * interpolate the pixels array.
   * If button 2 is pressed then store the current pixels array into local 
   * storage.
   * If button 3 is pressen then store the current pixels array onto the SD 
   * inserted.
   */
  while(true){ 
    // Store current image counter, to check if button pressed multiple time within same session
    /* get a new sensor event */
    sensors_event_t event;
    amg.readPixels(pixels);
    Serial.print("////////////////////////// Reading ////////////////////////////");
    rotatepixels(pixels, rotatedpixels, AMG_ROWS, AMG_COLS);

    if(interpValue == 1){
      drawpixels(rotatedpixels, AMG_ROWS, AMG_COLS, BOXSIZE, BOXSIZE);
      showVal(rotatedpixels, AMG_ROWS, AMG_COLS);
    }
    
    if(interpValue > 1){
      interpolate_image(rotatedpixels, AMG_ROWS, AMG_COLS, dest_2d, INTERPOLATED_ROWS, INTERPOLATED_COLS);
      uint16_t boxsize = min(m_tft->width() / INTERPOLATED_COLS, m_tft->height() / INTERPOLATED_ROWS);
      drawpixels(dest_2d, INTERPOLATED_ROWS, INTERPOLATED_COLS, boxsize, boxsize);
      showVal(dest_2d, INTERPOLATED_ROWS, INTERPOLATED_COLS);
    }
    if(unPhone::button1()){ break;}
   
    if(unPhone::button2()){
      int localMod = localCounter%3;
      if(localMod == 0){
        imageSaved(rotatedpixels, a, localCounter);
        localCounter += 1;
      }
      if(localMod == 1){
        imageSaved(rotatedpixels, b, localCounter);
        localCounter += 1;
      }
      if(localMod == 2){
        imageSaved(rotatedpixels, c, localCounter);
        localCounter += 1;
      }
      if (localCounter > 6 && localCounter%7 == 0){
        imagePageNumber+= 1;
      }
    }
    if(unPhone::button3()){
      capture_image();
      sdCounter += 1;
    }
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Roatate pixels, takes the [8x8] array and orientates it to the display
 * @param pix - float array, that is the original pixels array
 * @param rotpix - float array, that is the destination array
 * @param row - int deinifng how big the pixels array is
 * @param cols - int deinifng how big the pixels array is
 */
void ThermalCameraUIElement::rotatepixels(float *pix, float *rotpix, uint8_t rows, uint8_t cols){
  for(int i=0; i<rows*cols; i++){
      int x = i / cols;
      int y = i % rows;
      int x_ = (cols -1) -y;
      int y_ =  x;
      int indexIn = x_ * rows + y_;
      float val = pix[indexIn];
      int indexOut = i;
      rotpix[indexOut] = val;
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Draw pixels, takes the [8x8] array and generates a rectangle of boxWidth 
 * and boxHeight. Meanwhile colouring of the rectangle will depend on the 
 * min and max global values mapped to the predefined coulour array
 * @param p - float array, that is the original pixels array
 * @param row - uint8_t deinifng how big the pixels array is
 * @param cols - uint8_t deinifng how big the pixels array is
 * @param boxWidth - uint8_t deinifng how wide rectangle will be
 * @param boxHeight - uint8_t deinifng how tall rectangle will be
 */
void ThermalCameraUIElement::drawpixels(float *p, uint8_t rows, uint8_t cols, uint8_t boxWidth, uint8_t boxHeight) {
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
      m_tft->fillRect(5+boxWidth * x, 150+boxHeight * y, boxWidth, boxHeight, camColors[colorIndex]);
        
    } 
  }
  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Show val looks through the current array and saves, the local min and max. 
 * As well as overwritting the global saved min and max values. 
 * These are then displayed at the top of the screen to give an idea of what 
 * has been seen and what is eing seen.
 * @param p - float array, that is the original pixels array
 * @param row - int deinifng how big the pixels array is
 * @param cols - int deinifng how big the pixels array is
 */
void ThermalCameraUIElement::showVal(float *p, uint8_t rows, uint8_t cols){
  float maxVal = 0.0;
  float minVal = get_point(p, rows, cols, 0, 0);
  
  minLive = minVal;
  maxLive = maxVal;
  
  for (int y=0; y<rows; y++) {
    for (int x=0; x<cols; x++) {
      float val = get_point(p, rows, cols, x, y);
      if(val > maxVal)
        maxVal = val;
      if(val < minVal)
        minVal = val;
      if(val > savedMax)
        savedMax = val;
      if(minVal < savedMin)
        savedMin = minVal;
    }
  }
  m_tft->setCursor(25,20);
  m_tft->print("Interpolation rate:");

  m_tft->setCursor(m_tft->width()-65,20);
  m_tft->print(interpValue);  

  m_tft->fillRect(0, 40, m_tft->width(), 60, HX8357_BLACK);
  m_tft->setCursor(25,40);
  m_tft->print("Live Temps");
  m_tft->setCursor(25,60);
  m_tft->print("Min: "); m_tft->print(minVal);
  m_tft->setCursor(25,80);
  m_tft->print("Max: "); m_tft->print(maxVal);

  m_tft->setCursor(175,40);
  m_tft->print("Saved Temps");
  m_tft->setCursor(183,60);
  m_tft->print("Min: "); m_tft->print(savedMin);
  m_tft->setCursor(183,80);
  m_tft->print("Max: "); m_tft->print(savedMax);

  m_tft->setCursor(78,100);
  m_tft->print("< Temp range >");
  m_tft->setCursor(25,100);
  m_tft->print(minValue);
  m_tft->setCursor(m_tft->width()-42,100);
  m_tft->print(maxValue);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Temp gradient uses the global min and max values set in the sensitivity 
 * UIElement. 
 * To divide the defined temp gradient space into rectangles for each 
 * temperature between the the min and max values. 
 * The colourIndexs are sorted in a seperate array so that the gradient 
 * goes from low (blue) to high (red)
 */
void ThermalCameraUIElement::tempGradient(){
  int range = maxValue-minValue;
  int gradiWidth = 280;
  int boxSize = gradiWidth/range;
  uint8_t rangeArray[range];
  for (int y=0; y<range; y++) {
      float val = y;
      int colorTemp = val;
      
      uint8_t colorIndex = map(colorTemp, MINTEMP, MAXTEMP, 0, 255);
      colorIndex = constrain(colorIndex, 0, 255);
      rangeArray[y] = colorIndex;
  }
 //sorting - ASCENDING ORDER
 for(int i=0;i<range;i++)
  {   
    for(int j=i+1;j<range;j++)
    {
      if(rangeArray[i]>rangeArray[j])
      {
        uint8_t temp = rangeArray[i];
        rangeArray[i]= rangeArray[j];
        rangeArray[j]= temp;
      }
    }
  }
  // Drawing rectangles
  for (int y=0; y<range; y++) {
    m_tft->fillRect(25+ (boxSize * y), 120, boxSize, 20, camColors[rangeArray[y]]);
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Image saved is the local saving method that stores the current [8x8] roated 
 * array in an external a,b or c array. These a,b or c array are gloabla and 
 * can be used in the images page.
 * @param pixels - float array, that is the original roated array
 * @param image - float array, that is the local gloabl array
 * @param lCounter - int deinifng how many local images have been saved
 */
void ThermalCameraUIElement::imageSaved(float* pixels, float* image, int lCounter){
  Serial.println("------------ Image Saved Localy -----------------");
  for (int i = 0; i <= 63; i++) {
    image[i]= pixels[i];
  }
  m_tft->fillRect(170, 0, 200, 20, BLACK);
  m_tft->setCursor(25,0);
  m_tft->print("Stored Image "); m_tft->print(lCounter+1); m_tft->print(" Locally"); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Capture_image is the start of the perminant saving method that stores 
 * the current [8x8] roated array in an external SD text file. 
 * If the SD card is present then call save_image_sd. Meanwhile show the 
 * user that the image hav=s been taken
 * If not then print no SD card
 */
void ThermalCameraUIElement::capture_image() {
  if (SD_present) {
    Serial.println("Starting Image Capture!");
    save_image_sd();
    m_tft->fillRect(170, 0, 200, 20, BLACK);
    m_tft->setCursor(25,0);
    m_tft->print("Stored Image "); m_tft->print(sdCounter+1); m_tft->print(" to SD"); 
  }
  else {
    Serial.println("No SD Card!");
  }
  SPI.setFrequency(80000000L); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Save_image_sd is the second stage of saving the array. After being called it generated multiple 
 * mutable filenames and checks if it exists on the SD card. If it doesnt then use that filename 
 * to save the current array.
 * To save the array, it must be converted to a String and passed to the writeFile method
 */
void ThermalCameraUIElement::save_image_sd() {
  // Pick a numbered filename, 00 to 99.
  char filename[15] = "/image_##.txt";

  for (uint8_t i = 0; i < 100; i++) {
    filename[7] = '0' + i / 10;
    filename[8] = '0' + i % 10;
    if (!SD.exists(filename)) {
      // Use this one!
      break;
    }
  }
  //Save 8x8 Image to SD Card
  String message = "";
  for (int i = 63; i >= 0; i--) {
    message+=String(pixels[i]);
    message+=(',');
  }
  message+=('\n');
  writeFile(SD, filename, message);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* WriteFile, opens the SD file and prints the message given
 * If the write fails, then delete the file. Due to the file being created when 
 * the write starts, if it fails it will remain.
 * @param fs - File System thjat defines the SD class
 * @param path - char array array, file destination
 * @param message - String deinifng the array
 */
void ThermalCameraUIElement::writeFile(fs::FS &fs, char* path, String message){
    Serial.printf("Writing file: %s\n", path);
    
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
        deleteFile(fs, path);
    }
    file.close();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* DeleteFile, deletes a specifed SD file
 * @param fs - File System thjat defines the SD class
 * @param path - char array array, file destination
 */
void ThermalCameraUIElement::deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Text page UI, run each turn and do nothing
 */
void ThermalCameraUIElement::runEachTurn(){}
