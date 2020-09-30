// UIController.h Created on: 22-06-2018, Author: @HarryEH

#ifndef UICONTROLLER_H_
#define UICONTROLLER_H_

#include "unphone.h"            // specifics of the unPhone

/* Definitions of gloabal variables
 * Used in the sesnitivity page and modulates the thermal camera
 */
extern int testValue;
extern int minValue;
extern int maxValue;
extern int minLive;
extern int maxLive;
extern int rangeValue;
//////////////////////////////////////////////////////////////////////////

/* Used in the interpolation page and modulates the thermal camera display and image page display
 */
extern int interpValue;
extern bool interpState; 
//////////////////////////////////////////////////////////////////////////

/* Used in the image page, to keep track of page numbers
 */
extern int imagePageNumber;//////////////////////////////////////////////////////////////////////////

/* SD Card Variables
 */
extern const int sd_ss;
extern File image_file;
extern bool SD_present;
extern unsigned long last_image_time;
extern volatile bool get_image;
extern volatile unsigned long last_image;
extern int sdCounter;
//////////////////////////////////////////////////////////////////////////

/* Local image file arrays
 */
extern float a[8 * 8];
extern float b[8 * 8];
extern float c[8 * 8];
extern int localCounter;
//////////////////////////////////////////////////////////////////////////

class UIElement { ///////////////////////////////////////////////////////////
  protected:
    Adafruit_HX8357* m_tft;
    Adafruit_STMPE610* m_ts;

    // colour definitions
    const uint16_t BLACK =   HX8357_BLACK;
    const uint16_t BLUE =    HX8357_BLUE;
    const uint16_t RED =     HX8357_RED;
    const uint16_t GREEN =   HX8357_GREEN;
    const uint16_t CYAN =    HX8357_CYAN;
    const uint16_t MAGENTA = HX8357_MAGENTA;
    const uint16_t YELLOW =  HX8357_YELLOW;
    const uint16_t WHITE =   HX8357_WHITE;
    const uint8_t  BOXSIZE = 40;
    const uint8_t  HALFBOX = (BOXSIZE / 2);
    const uint8_t  QUARTBOX = (BOXSIZE / 4);
    const uint8_t  PENRADIUS = 9; // orig: 3
    static const uint8_t NUM_BOXES = 7;
    const uint16_t colour2box[NUM_BOXES] = {
      RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE,
    };
    const uint8_t SWITCHER = 7; // index of the switcher
    void drawSwitcher(uint16_t xOrigin = 0, uint16_t yOrigin = 0);
    void drawReverseSwitcher(uint16_t xOrigin = 0, uint16_t yOrigin = 0);
    
  public:
    UIElement(Adafruit_HX8357* tft, Adafruit_STMPE610* ts);
    virtual bool handleTouch(long x, long y) = 0;
    virtual void draw() = 0;
    virtual void runEachTurn() = 0;
    void someFuncDummy();
    void showLine(char *buf, uint16_t *yCursor);
   
    
    
};

// the UI elements types (screens) /////////////////////////////////////////
enum ui_modes_t {
  ui_menu = 0,          //  0
  ui_thermalcamera,     //  1
  ui_sensitivity,       //  2
  ui_interpolation,     //  3
  ui_imagepage          //  4
};
extern char *ui_mode_names[];
extern uint8_t NUM_UI_ELEMENTS;  // number of UI elements (screens)

class UIController { ////////////////////////////////////////////////////////
  private:
    UIElement* m_element = 0;
    UIElement* m_menu;
    bool gotTouch();
    void handleTouch();
    void changeMode();
    ui_modes_t m_mode;
    ui_modes_t nextMode = ui_menu; // starting mode
    uint8_t modeCounter = 1; // number of mode changes % # of ui elements
  public:
    UIController(ui_modes_t);
    bool begin();
    UIElement* allocateUIElement(ui_modes_t);
    void run();
    void redraw();
    void message(char *s);
    static bool provisioned;
};

#endif
