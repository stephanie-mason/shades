      #include <FastLED.h>

#define LED_PIN      1   // which pin your pixels are connected to
#define NUM_LEDS    22   // how many LEDs you have
#define BRIGHTNESS  20   // 0-255, higher number is brighter. 
#define SATURATION  50   // 0-255, 0 is pure white, 255 is fully saturated color
#define SPEED       80   // How fast the colors move.  Higher numbers = faster motion
#define STEPS        10   // How wide the bands of color are.  1 = more like a gradient, 10 = more like stripes
#define BUTTON_PIN   2   // button is connected to pin 2 and GND

#define COLOR_ORDER GRB  // Try mixing up the letters (RGB, GBR, BRG, etc) for a whole new world of color combinations

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette( PartyColors_p );
TBlendType    currentBlending;
int ledMode = 0;
boolean scannerMode = false;
int scanIndex = 0;
int scanDirection = 0;

const TProgmemPalette16 PurpleColors_p PROGMEM =
{
  CRGB::Purple,
  CRGB::Purple, 
  CRGB::MidnightBlue,
  CRGB::MidnightBlue,

  CRGB::Purple,
  CRGB::Purple,
  CRGB::BlueViolet,
  CRGB::BlueViolet,

  CRGB::DarkTurquoise,
  CRGB::DarkTurquoise,
  CRGB::DarkBlue,
  CRGB::DarkBlue,

  CRGB::Purple,
  CRGB::Purple,
  CRGB::BlueViolet,
  CRGB::BlueViolet
};

unsigned long keyPrevMillis = 0;
const unsigned long keySampleIntervalMs = 25;
byte longKeyPressCountMax = 80;    // 80 * 25 = 2000 ms
byte longKeyPressCount = 0;

byte prevKeyState = HIGH;         // button is active low

void setup() {
  delay( 2000 ); // power-up safety delay
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  currentBlending =LINEARBLEND;
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {

  byte currKeyState = digitalRead(BUTTON_PIN);

  if ((prevKeyState == LOW) && (currKeyState == HIGH)) {
    shortKeyPress();
  }
  prevKeyState = currKeyState;

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  static uint8_t hue = 0;

  switch (ledMode) {

  case 0:
    scannerMode = true;
    break;
  case 1:
    scannerMode = false;
    currentPalette = HeatColors_p;    //Red & Yellow, Fire Colors
    break;
  case 2:
    currentPalette = ForestColors_p;    //Foresty greens and yellows
    break;
  case 3: 
    currentPalette = OceanColors_p;  //Oceans are pretty and filled with mermaids
    break;
  case 4: 
    currentPalette = PurpleColors_p;  //My custom palette from above
    break;
  case 5:
    currentPalette = RainbowColors_p;  //All the colors!
    break;
  case 6:
    currentPalette = RainbowStripeColors_p;   //Rainbow stripes
    break;      
  case 7:
    currentPalette = PartyColors_p; //All the colors except the greens, which make people look a bit washed out
    break;
  } 

  if (scannerMode == true ) {
    findScanIndex();
    fillScanner(hue);
  }
  else {
    FillLEDsFromPaletteColors(startIndex);
    FastLED.show();
    FastLED.delay(1000 / SPEED);  
  }  
}

void FillLEDsFromPaletteColors( uint8_t colorIndex) {
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, LINEARBLEND);
    colorIndex += STEPS;              
  }
}

void shortKeyPress() {
  ledMode++;
  if (ledMode > 7) {
    ledMode=0; 
  }  
}

void fillScanner(uint8_t hue) {
    // Set the i'th led to red 
    if (scanIndex > 0) { leds[scanIndex-1] = 0x020202; }
    leds[scanIndex] = 0x474747;
    leds[scanIndex+1] = 0x020202;
    // Show the leds
    FastLED.show(); 
    FastLED.delay(80);
    // now that we've shown the leds, reset the i'th led to black
    if (scanIndex > 0) { leds[scanIndex-1] = CRGB::Black; }
    leds[scanIndex] = CRGB::Black;
    leds[scanIndex+1] = CRGB::Black;
    fadeall();
}

void fadeall() { 
  for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } 
}

void findScanIndex() {
  // counting up / scan goes forward
  if (scanDirection == 0) {
    scanIndex ++;
  }
  else {
    scanIndex --;
  }

  if (scanIndex >= 19 | scanIndex <= 0) {
    changeDirection();
  }
}

void changeDirection() {
  if (scanDirection == 0) {
    scanDirection = 1;
  } else { 
    scanDirection = 0;
  }
}

    
