//  AllUIElement.h

#ifndef ALLUIELEMENT_H_
#define ALLUIELEMENT_H_

#include "UIController.h"


/* All UIEliments need to have thier methods decalred in this file as it is the header for all of them
 * Passes valuable objects such as the touch screen and display
 */
 
class MenuUIElement: public UIElement { /////////////////////////////////////
  private:
    void drawTextBoxes();
    uint8_t mapTextTouch(long, long);
    int8_t menuItemSelected = -1;
  public:
    MenuUIElement (Adafruit_HX8357* tft, Adafruit_STMPE610* ts)
    : UIElement(tft, ts) { };
    bool handleTouch(long x, long y);
    void draw();
    void runEachTurn();
    int8_t getMenuItemSelected() { return menuItemSelected; }
};

class ThermalCameraUIElement: public UIElement { ///////////////////////////////
  private:
    void drawBBC();
    void drawTestcard();
    void rotatepixels(float*, float*, uint8_t, uint8_t);
    void drawpixels(float *, uint8_t, uint8_t, uint8_t, uint8_t);
    void showVal(float *, uint8_t, uint8_t);
    void tempGradient();
    void save_image_sd();
    void capture_image();
    void capture_image_isr();
    void imageSaved(float *, float*, int);
    void writeFile(fs::FS &, char * , String );
    void deleteFile(fs::FS &, const char *);
  public:
    ThermalCameraUIElement(Adafruit_HX8357* tft, Adafruit_STMPE610* ts) 
      : UIElement(tft, ts) { };
    bool handleTouch(long, long);
    void draw();
    void runEachTurn();
};
class SensitivityUIElement: public UIElement { /////////////////////////////////////
  private:
    void drawText();
    void drawValues();
    void drawTextBoxes();
    void showTemperatures(); 
    void minMod(long, long);
    void maxMod(long, long);
    void rangeMod(long, long);
    uint8_t mapTextTouch(long, long);
    int8_t menuItemSelected = -1;
  public:
    SensitivityUIElement (Adafruit_HX8357* tft, Adafruit_STMPE610* ts)
    : UIElement(tft, ts) { };
    bool handleTouch(long x, long y);
    void draw();
    void runEachTurn();
    int8_t getMenuItemSelected() { return menuItemSelected; }
};

class InterpolationUIElement: public UIElement { /////////////////////////////////////
  private:
    void drawMenu();
    void interpMod(long, long);
    void updateInterpValue();
    void updateInterp();
    void updateBoxes();
  public:
    InterpolationUIElement (Adafruit_HX8357* tft, Adafruit_STMPE610* ts)
    : UIElement(tft, ts) { };
    bool handleTouch(long, long);
    void getInterpolation();
    void draw();
    void runEachTurn();
};

class ImagePageUIElement: public UIElement { /////////////////////////////////////
  private:
    void drawTextBoxes();
    void drawMenu();
    void drawpixels(float *, uint8_t, uint8_t, uint8_t, uint8_t, int, int);
    void pageChange(long, long);
    void expandImage(float *); 
    void readFile(fs::FS &, const char *, float *);
  public:
    ImagePageUIElement (Adafruit_HX8357* tft, Adafruit_STMPE610* ts)
    : UIElement(tft, ts) { };
    bool handleTouch(long, long);
    void getInterpolation();
    void draw();
    void runEachTurn();
};
#endif
