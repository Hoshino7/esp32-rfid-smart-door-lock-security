#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

// Definisi Pin ESP32
#define SS_PIN    5   // VSPI Chip Select
#define RST_PIN   4   // Reset Pin
#define PIR_PIN   34  // GPIO 34 (Input Only)
#define SERVO_PIN 13  // Pin PWM Servo
#define BUZZER_PIN 14 // Active Buzzer
#define LED_RED   2   // LED Merah
#define LED_GREEN 15  // LED Hijau

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo lockServo;

// UID Kartu/Keyfob Valid Anda (4 Byte Hex)
byte validUID[] = {0x73, 0xA2, 0x1B, 0x4F};

void setup() {
  Serial.begin(115200);

  // Inisialisasi SPI khusus ESP32 (SCK=18, MISO=19, MOSI=23, SS=5)
  SPI.begin(18, 19, 23, 5);
  rfid.PCD_Init();

  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // Inisialisasi PWM Servo untuk ESP32
  ESP32PWM::allocateTimer(0);
  lockServo.setPeriodHertz(50);
  lockServo.attach(SERVO_PIN, 500, 2400);
  lockServo.write(0); // Posisi Awal: Terkunci (0 Derajat)

  // Inisialisasi I2C ESP32 (SDA=GPIO 21, SCL=GPIO 22)
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" ESP32 DOOR LOCK");
  lcd.setCursor(0, 1);
  lcd.print(" INITIALIZING...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Kondisi Standby
  lockServo.write(0);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);

  lcd.setCursor(0, 0);
  lcd.print(" ESP32 DOOR LOCK");
  lcd.setCursor(0, 1);
  lcd.print("TEMPELKAN KARTU ");

  // Indikasi Gerakan via PIR saat pintu terkunci
  if (digitalRead(PIR_PIN) == HIGH) {
    lcd.setCursor(0, 1);
    lcd.print("ADA GERAKAN!    ");
  }

  // Cek Keberadaan Kartu RFID
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    delay(100);
    return;
  }

  // Verifikasi UID Kartu
  bool match = true;
  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != validUID[i]) {
      match = false;
      break;
    }
  }

  if (match) {
    // AKSES DITERIMA
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    tone(BUZZER_PIN, 1000, 200);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AKSES DITERIMA ");
    lcd.setCursor(0, 1);
    lcd.print("SILAKAN MASUK! ");

    lockServo.write(90); // Buka Pintu
    delay(5000);          // Tahan 5 Detik

    lockServo.write(0);  // Kunci Kembali
    lcd.clear();
  } else {
    // AKSES DITOLAK
    digitalWrite(LED_RED, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" AKSES DITOLAK! ");
    lcd.setCursor(0, 1);
    lcd.print(" KARTU UNKNOWN ");

    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_RED, HIGH);
      tone(BUZZER_PIN, 1500, 150);
      delay(200);
      digitalWrite(LED_RED, LOW);
      delay(150);
    }
    lcd.clear();
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}