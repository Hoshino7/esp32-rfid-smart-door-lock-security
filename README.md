# 🔑 ESP32 Smart RFID Door Lock & Security System

Sistem Keamanan Akses Pintu Pintar (*Smart Keyless Access*) berlapis berbasis **DOIT ESP32 DevKit V1**, **Modul RFID RC522**, dan **Sensor Gerak PIR HC-SR501**.

---

## 🚀 Kenapa Proyek Ini Dibuat & Manfaatnya?

Di era *Smart Home* dan otomasi bangunan, kunci mekanis konvensional memiliki banyak keterbatasan (mudah diduplikasi, hilang, atau dibobol). Proyek ini dirancang sebagai solusi **Sistem Keamanan Pintu Pintar Berlapis** yang memiliki manfaat nyata:

1. **Akses Pintar Tanpa Kunci (*Keyless Door Access*)**:
   * Menggantikan kunci fisik dengan kartu/tag RFID (E-KTP, Kartu Akses, atau Keyfob).
   * Sangat cocok diterapkan pada pintu rumah, kamar kos, laboratorium, atau ruang server kantor.
2. **Keamanan Berlapis dengan Pengawas Gerakan (*Motion Monitor*)**:
   * Tidak hanya menunggu kartu ditempelkan, sistem secara aktif memantau area depan pintu menggunakan sensor PIR. Jika ada pergerakan mencurigakan saat pintu terkunci, sistem langsung menampilkan indikasi peringatan.
3. **Pondasi Siap IoT (*Cloud-Ready*)**:
   * Karena menggunakan mikrokontroler ESP32, sistem ini sangat mudah dikembangkan lebih lanjut ke jaringan internet (misalnya mengirim notifikasi pembobolan ke Telegram atau mengontrol pintu via aplikasi HP).

---

## 🧠 Kenapa Menggunakan Komponen Ini? (Keputusan Arsitektur)

| Komponen | Alasan Pemilihan & Keunggulan |
| :--- | :--- |
| **ESP32 DevKit V1** | Beroperasi pada logika **3.3V (native)** yang aman untuk modul RFID, memiliki kecepatan pemrosesan tinggi (240 MHz Dual-Core), serta sudah dilengkapi konektivitas Wi-Fi/Bluetooth bawaan. |
| **RFID RC522 (VSPI)** | Menggunakan protokol komunikasi **SPI** yang sangat cepat dan stabil dalam membaca UID unik kartu pada frekuensi 13.56 MHz. |
| **Sensor PIR HC-SR501** | Menggunakan teknologi inframerah pasif untuk mendeteksi radiasi panas tubuh manusia dengan jangkauan dan sensitivitas yang dapat diatur. |
| **LCD 16x2 dengan Modul I2C** | Menghemat pin GPIO ESP32 secara signifikan (hanya menggunakan 2 pin: SDA & SCL di GPIO 21 & 22) untuk menampilkan antarmuka pengguna secara interaktif. |
| **Motor Servo SG90** | Bertindak sebagai aktuator simulasi grendel/pengunci mekanis yang presisi (0° = Terkunci, 90° = Terbuka). |

---

## ⚡ Catatan Kelistrikan & Regulasi Tegangan

1. **Tegangan RFID RC522 (⚠️ WAJIB 3.3V)**:
   * Modul RFID RC522 tidak memiliki regulator internal. Menghubungkannya ke pin 5V akan merusak chip. Selalu hubungkan VCC RC522 ke pin **3.3V ESP32**.
2. **Sumber Daya Aktuator (5V / VIN)**:
   * Motor Servo, LCD I2C, dan Sensor PIR membutuhkan daya 5V untuk beroperasi optimal. Pin VCC komponen ini dihubungkan ke pin **VIN (5V USB)** pada ESP32.
3. **Fungsi Pin IRQ pada RC522**:
   * Pin **IRQ (Interrupt Request)** disengaja **TIDAK DIHUBUNGKAN (NC / Not Connected)** karena program membaca kartu secara *polling*. Pin ini baru digunakan jika ingin mengimplementasikan fitur hemat daya (*Deep Sleep*).

---

## 🔌 Skema Pengabelan Presisi (Pinout ESP32)

### Modul RFID RC522 (VSPI Protocol)
* **VCC** >> **3.3V ESP32**
* **RST** >> **GPIO 4**
* **GND** >> **GND**
* **IRQ** >> *NC (Kosong)*
* **MISO** >> **GPIO 19**
* **MOSI** >> **GPIO 23**
* **SCK** >> **GPIO 18**
* **SDA (SS)** >> **GPIO 5**

### Komponen Indikator & Sensor
* **PIR Sensor**: OUT >> **GPIO 34** (Input Only) | VCC \\(\rightarrow\\) **VIN (5V)**
* **Servo SG90**: Signal >> **GPIO 13** | VCC \\(\rightarrow\\) **VIN (5V)**
* **LCD 16x2 I2C**: SDA >> **GPIO 21** | SCL \\(\rightarrow\\) **GPIO 22** | VCC \\(\rightarrow\\) **VIN (5V)**
* **Active Buzzer**: (+) >> **GPIO 14**
* **LED Merah**: Anoda (+) >> **GPIO 2** (Resistor 220Ω)
* **LED Hijau**: Anoda (+) >> **GPIO 15** (Resistor 220Ω)

---
