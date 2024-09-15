/* Suzuno32RVにUSBキーボードを接続して、その入力を読み取ります。
   キーが押下されている間、LEDが点灯します。
   パソコンに対してはUSBデバイスとして、USBキーボードに対してはUSBホストとして、同時に動作します。

   ツール → USB Support → Adafruit TinyUSB with USBD を選択してください。

!!!! このサンプルは、WCHの提供するサンプルコードをもとにしています。将来的にTinyUSBライブラリを使用したものへ置き換わる可能性があります !!!!
*/


#include <Adafruit_TinyUSB.h>
#include <HardwareTimer.h>

#include "USBHost_Keyboard.h"

USBHostKeyboard USBKeyboard;

HardwareTimer Timer3(TIM3);

// USBTimerHandler is defined in app_km.c
extern "C" void USBTimerHandler(void);

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");
  Serial.println("USBHost Keyboard demo");
  Serial.printf("SystemCoreClock = %u\r\n", SystemCoreClock);

  pinMode(PA5, OUTPUT);
  for (int i = 0; i < 4; i++) {
    digitalWrite(PA5, HIGH);
    delay(250);
    digitalWrite(PA5, LOW);
    delay(250);
  }

  // これはArduino環境との整合性を取るための回避策です
  Timer3.attachInterrupt(USBTimerHandler);

  USBKeyboard.init();

  Serial.println("Ready.");
}

void loop() {
  static char prev_key = 0;

  // update()を高頻度に呼び出してください。キーボードの状態を更新します。
  USBKeyboard.update();
  
  if (USBKeyboard.connected()) {
    // get_key()は、USBキーボードとしてのキーコード (Usage ID) を返します。
    // ASCIIコードが必要な場合は、さらに変換が必要です。
    int ch = USBKeyboard.get_key();

    if (ch < 0 && prev_key != 0x00) {
      Serial.printf("<<<< Released 0x%02x\r\n", prev_key);
      prev_key = 0;
      digitalWrite(PA5, LOW);

    } else if (ch > 0 && ch != prev_key) {
      Serial.printf(">>>> Pressed 0x%02x (%d)\r\n", ch, ch);
      prev_key = (char)ch;
      digitalWrite(PA5, HIGH);
    }
  }
}
