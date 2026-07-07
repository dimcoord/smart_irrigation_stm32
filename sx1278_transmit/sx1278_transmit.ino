#include <SPI.h>

#define NSS_PIN    PA4
#define RESET_PIN  PA3
#define LED_PC13   PC13      // On-board LED (active LOW)
#define LED_PB0    PB0       // External LED (active HIGH)

void sxWrite(uint8_t reg, uint8_t val) {
  digitalWrite(NSS_PIN, LOW);
  SPI.transfer(reg | 0x80);
  SPI.transfer(val);
  digitalWrite(NSS_PIN, HIGH);
}

uint8_t sxRead(uint8_t reg) {
  digitalWrite(NSS_PIN, LOW);
  SPI.transfer(reg & 0x7F);
  uint8_t val = SPI.transfer(0x00);
  digitalWrite(NSS_PIN, HIGH);
  return val;
}

void blinkPC13(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED_PC13, LOW);
    delay(150);
    digitalWrite(LED_PC13, HIGH);
    delay(150);
  }
}

void blinkPB0(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED_PB0, HIGH);
    delay(150);
    digitalWrite(LED_PB0, LOW);
    delay(150);
  }
}

void setup() {

  pinMode(LED_PC13, OUTPUT);
  pinMode(LED_PB0, OUTPUT);

  digitalWrite(LED_PC13, HIGH);
  digitalWrite(LED_PB0, LOW);

  pinMode(NSS_PIN, OUTPUT);
  digitalWrite(NSS_PIN, HIGH);

  pinMode(RESET_PIN, OUTPUT);

  SPI.begin();

  // ------------------------
  // Hardware Reset
  // ------------------------
  digitalWrite(RESET_PIN, LOW);
  delay(20);
  digitalWrite(RESET_PIN, HIGH);
  delay(50);

  // Check Version register
  if (sxRead(0x42) != 0x12) {
    while (1) {
      blinkPC13(1);
      delay(100);
    }
  }

  blinkPC13(1);

  // Sleep
  sxWrite(0x01, 0x80);
  delay(10);

  // Standby
  sxWrite(0x01, 0x81);
  delay(10);

  // 433 MHz
  sxWrite(0x06, 0x6C);
  sxWrite(0x07, 0x80);
  sxWrite(0x08, 0x00);

  // PA_BOOST 17 dBm
  sxWrite(0x09, 0x8F);

  // LNA Boost
  sxWrite(0x0C, 0x23);

  // FIFO TX/RX base
  sxWrite(0x0E, 0x00);
  sxWrite(0x0F, 0x00);

  // FIFO pointer
  sxWrite(0x0D, 0x00);

  // BW125 CR4/5 Explicit Header
  sxWrite(0x1D, 0x72);

  // SF7 CRC OFF
  sxWrite(0x1E, 0x70);

  // AGC ON
  sxWrite(0x26, 0x04);

  // Preamble = 8
  sxWrite(0x20, 0x00);
  sxWrite(0x21, 0x08);

  // Sync Word
  sxWrite(0x39, 0x12);

  // Max payload length
  sxWrite(0x23, 0xFF);

  // Clear IRQ
  sxWrite(0x12, 0xFF);

  blinkPB0(2);
}

int counter = 0;

void loop() {

  char msg[32];
  sprintf(msg, "Hello %d", counter++);

  // Standby
  sxWrite(0x01, 0x81);

  // FIFO pointer
  sxWrite(0x0D, 0x00);

  // Payload
  for (uint8_t i = 0; i < strlen(msg); i++) {
    sxWrite(0x00, msg[i]);
  }

  // Payload length
  sxWrite(0x22, strlen(msg));

  // Clear IRQ
  sxWrite(0x12, 0xFF);

  // TX Mode
  sxWrite(0x01, 0x83);

  // Wait TxDone
  unsigned long t = millis();

  while (!(sxRead(0x12) & 0x08)) {
    if (millis() - t > 3000) {
      // Timeout
      while (1) {
        blinkPC13(2);
        delay(100);
      }
    }
  }

  // Clear TxDone
  sxWrite(0x12, 0x08);

  // Standby
  sxWrite(0x01, 0x81);

  // Successful transmission
  blinkPB0(1);

  delay(1000);
}