#include <Wire.h>
#include <Adafruit_GFX.h>
#include "VideoFrame.h" 

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// ========================================================
// ⚙️ PILIH TIPE OLED DI SINI (Aktifkan salah satu saja!)
// ========================================================
#define PAKAI_SSD1306
//#define PAKAI_SH1106
// ========================================================

// Konfigurasi otomatis berdasarkan pilihan di atas
#ifdef PAKAI_SSD1306
  #include <Adafruit_SSD1306.h>
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  #define WARNA_LAYAR WHITE  // SSD1306 pakai "WHITE"

#elif defined(PAKAI_SH1106)
  #include <Adafruit_SH110X.h>
  Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  #define WARNA_LAYAR SH110X_WHITE // SH110X pakai "SH110X_WHITE"
#endif

unsigned long previousMillis = 0;
int currentFrame = 0;

void setup() {
  Serial.begin(115200);

  // Inisialisasi otomatis menyesuaikan chip
  #ifdef PAKAI_SSD1306
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
      Serial.println(F("OLED SSD1306 gagal terhubung."));
      for(;;); 
    }
  #elif defined(PAKAI_SH1106)
    if(!display.begin(0x3C, true)) { // SH1106 format begin-nya beda
      Serial.println(F("OLED SH1106 gagal terhubung."));
      for(;;); 
    }
  #endif

  // I2C ngebut 800 kHz biar FPS video aman
  Wire.setClock(800000); 

  display.clearDisplay();
  display.display();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= FRAME_DELAY) {
    
    previousMillis = currentMillis;

    display.clearDisplay();
    
    // Variabel WARNA_LAYAR otomatis berubah ngikutin chip yang dipilih
    display.drawBitmap(0, 0, video_frames[currentFrame], SCREEN_WIDTH, SCREEN_HEIGHT, WARNA_LAYAR);
    display.display();

    currentFrame++;

    if (currentFrame >= TOTAL_FRAMES) {
      currentFrame = 0;
    }
  }
}
        