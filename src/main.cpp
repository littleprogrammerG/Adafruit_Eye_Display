#include <Arduino_GFX_Library.h>
#include "Adafruit_FT6206.h"

Arduino_XCA9554SWSPI *expander = new Arduino_XCA9554SWSPI(
    PCA_TFT_RESET, PCA_TFT_CS, PCA_TFT_SCK, PCA_TFT_MOSI,
    &Wire, 0x3F);

Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    TFT_DE, TFT_VSYNC, TFT_HSYNC, TFT_PCLK,
    TFT_R1, TFT_R2, TFT_R3, TFT_R4, TFT_R5,
    TFT_G0, TFT_G1, TFT_G2, TFT_G3, TFT_G4, TFT_G5,
    TFT_B1, TFT_B2, TFT_B3, TFT_B4, TFT_B5,
    1 /* hync_polarity */, 46 /* hsync_front_porch */, 2 /* hsync_pulse_width */, 44 /* hsync_back_porch */,
    1 /* vsync_polarity */, 50 /* vsync_front_porch */, 16 /* vsync_pulse_width */, 16 /* vsync_back_porch */);

Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, false /* auto_flush */,
    expander, GFX_NOT_DEFINED /* RST */, TL021WVC02_init_operations, sizeof(TL021WVC02_init_operations));


Adafruit_FT6206 ctp = Adafruit_FT6206();  // This library also supports FT6336U!
#define I2C_TOUCH_ADDR 0x15

int blinkCount = 0;
int centerX = gfx->width() / 2;
int centerY = gfx->height() / 2;
int boundaryRadius = (gfx->width() / 2);

void setup(void)
{
  #ifdef GFX_EXTRA_PRE_INIT
    GFX_EXTRA_PRE_INIT();
  #endif

  // Init Display
  Wire.setClock(400000); // speed up I2C
  if (!gfx->begin()) {
    Serial.println("gfx->begin() failed!");
    while (1) yield();
  }

  expander->pinMode(PCA_TFT_BACKLIGHT, OUTPUT);
  expander->digitalWrite(PCA_TFT_BACKLIGHT, HIGH);

  gfx->fillScreen(BLACK);
}

void loop() {
  if (blinkCount >= 63) {
    gfx->fillEllipse(centerX, centerY, 200, 100, BLACK);

    for (int i = 0; i < 10; i++) {
      gfx->fillEllipse(centerX, centerY, 200, 100, BLACK);
      gfx->fillEllipse(centerX, centerY, 200, 100 - i * 10, WHITE);
      gfx->flush();
    }
    gfx->fillEllipse(centerX, centerY, 200, 100, BLACK);

    gfx->drawLine(centerX - 200, centerY, centerX + 200, centerY, WHITE);
    gfx->flush();
    delay(500);

    for (int i = 0; i < 10; i++) {
      gfx->fillEllipse(centerX, centerY, 200, 100, BLACK);
      gfx->fillEllipse(centerX, centerY, 200, i * 10, WHITE);
      gfx->flush();
    }
    gfx->fillEllipse(centerX, centerY, 200, 100, BLACK);

    blinkCount = 0;
    gfx->drawLine(centerX - 200, centerY, centerX + 200, centerY, BLACK);
  } else {
    gfx->fillEllipse(centerX, centerY, 200, 100, WHITE);
    blinkCount++;
  }

  gfx->flush();
  delay(16);
}