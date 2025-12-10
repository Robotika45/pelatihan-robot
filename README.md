# 🤖 Konfigurasi & Troubleshooting Robot Soccer NodeMCU 1.0 (ESP-12E Module)

## 📋 Persiapan Awal

Sebelum memulai, pastikan Anda telah menginstal **Arduino IDE** di komputer Anda. Jika belum, unduh di [arduino.cc](https://www.arduino.cc/en/software).

-----

## 🚀 Langkah 1: Menambahkan Board NodeMCU 1.0 (ESP-12E Module) ke Arduino IDE

Agar Arduino IDE dapat mengenali board NodeMCU, Anda perlu menambahkan URL Manager-nya terlebih dahulu.

1.  Buka Arduino IDE.
2.  Pergi ke menu **File** \> **Preferences**.
3.  Pada kolom **"Additional Boards Manager URLs"**, masukkan link berikut:

    ```text
    http://arduino.esp8266.com/stable/package_esp8266com_index.json
    ```
4.  Klik **OK**.
5.  Pergi ke menu **Tools** \> **Board** \> **Boards Manager...**
6.  Ketik **"esp8266"** (atau **"NodeMCU"**) pada kolom pencarian.
7.  Pilih **esp8266 by ESP8266 Community** dan klik **Install**.
8.  **Pilih Board:**
    *   Pergi ke menu **Tools** \> **Board** \> **NodeMCU 1.0 (ESP-12E Module)** (sesuaikan jika ada submenu *ESP8266 Boards*).
    *   *Catatan untuk pengguna Arduino IDE 2.0:* Anda juga bisa menggunakan *dropdown* pemilihan board di bagian atas toolbar. Klik "Select Board", cari "NodeMCU 1.0", lalu klik OK.
9.  **Pilih Port:**
    *   Hubungkan NodeMCU ke komputer via USB.
    *   Pergi ke menu **Tools** \> **Port** dan pilih port yang sesuai (biasanya `COMx` atau `/dev/ttyUSBx`).
    *   *Catatan untuk pengguna Arduino IDE 2.0:* Port juga bisa dipilih melalui *dropdown* yang sama di toolbar. Jika port tidak muncul, cek bagian Troubleshooting di bawah.

-----

## 📦 Langkah 2: Menginstal Library Tambahan

Kode program ini menggunakan library `ESPAsyncWebServer` yang tidak tersedia secara langsung di Library Manager standar, sehingga perlu diinstal secara manual (via .zip).

1.  **Download Library:**

      * **ESPAsyncWebServer**: [Klik disini untuk download ZIP](https://github.com/lacamera/ESPAsyncWebServer/archive/master.zip)
      * **ESPAsyncTCP**: [Klik disini untuk download ZIP](https://github.com/dvarrel/ESPAsyncTCP/archive/master.zip)

2.  **Install di Arduino IDE:**

      * Buka Arduino IDE.
      * Pilih menu **Sketch** \> **Include Library** \> **Add .ZIP Library...**
      * Cari file `.zip` yang baru saja didownload (lakukan satu per satu untuk kedua library).

-----

## ⚙️ Langkah 3: Konfigurasi Kode & Wiring

Sebelum upload, sesuaikan kode program `soccer_robot.ino` dengan kebutuhan Anda.

### 1\. Pengaturan WiFi & IP Address

Buka file `soccer_robot.ino` dan cari bagian berikut (baris 6-12):

```cpp
const char* ssid = "RobotSoccer2";      // Ganti nama Wifi sesuai keinginan
const char* password = "12345678";      // Ganti password Wifi (min 8 karakter)

// Konfigurasi IP Static berdasarkan NBI (Nomor Mahasiswa/Siswa)
// Format: 192.168.xxx.xxx
IPAddress local_IP(192, 168, 20, 1); 
// Panduan ubah IP:
// Angka ke-1 (192): Tetap
// Angka ke-2 (168): Ubah ke 3 digit awal NBI Anda
// Angka ke-3 (20) : Ubah ke 3 digit akhir NBI Anda
// Angka ke-4 (1)  : Tetap
```

-----

# 🛠️ Pemecahan Masalah (Troubleshooting)

Jika Anda mengalami kendala saat upload atau pengoperasian robot, silakan cek panduan di bawah ini.

## ⚠️ 1. Masalah Upload & Koneksi USB

### Masalah: Port Tidak Terdeteksi di Arduino IDE

**Gejala:**

  * Saat NodeMCU dicolokkan ke laptop, tidak ada bunyi "ting-tung" dari Windows.
  * Menu **Tools \> Port** di Arduino IDE berwarna abu-abu (disable).
  * Tidak muncul `COMx` baru.

**Solusi:**

1.  **Cek Kabel USB:** Pastikan Anda menggunakan **Kabel Data**, bukan kabel *charging* biasa. Coba ganti kabel USB lain yang berkualitas.
2.  **Install Driver USB:** NodeMCU membutuhkan driver agar terbaca oleh komputer. Cek tulisan pada chip hitam persegi panjang di dekat port USB board Anda:
      * **Chip CH340G** (Board Lolin/Clone):
          * 👉 [Download Driver CH340 (Windows/Mac/Linux)](https://sparks.gogo.co.nz/ch340.html)
      * **Chip CP2102** (Board Amica/Original):
          * 👉 [Download Driver CP210x](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads)
3.  **Restart Komputer:** Setelah install driver, *restart* laptop Anda agar driver berjalan sempurna.

### Masalah: Gagal Upload ("Packet Header Error" / "Espcomm\_sync failed")

**Gejala:**

  * Proses upload berhenti dengan pesan error oranye di bawah.
  * Muncul tulisan `Connecting........_____....._____`.

**Solusi:**

1.  **Cek Kabel:** Pastikan kabel USB tidak longgar.
2.  **Mode Flash Manual:**
      * Tekan dan tahan tombol **FLASH** pada NodeMCU.
      * Tekan tombol **RST** (Reset) sekali, lalu lepas.
      * Lepas tombol **FLASH**.
      * Coba tekan tombol **Upload** di Arduino IDE lagi.
3.  **Cabut Periferal:** Terkadang driver motor atau servo yang terpasang di pin `RX/TX` (GPIO 1/3) atau pin `D3/D4` (GPIO 0/2) bisa mengganggu proses upload. Coba cabut sementara kabel yang terhubung ke pin tersebut saat upload.

-----

## 📶 2. Masalah Koneksi WiFi

### Masalah: Nama WiFi (SSID) Tidak Muncul di HP

**Solusi:**

1.  Tekan tombol **RST** (Reset) pada NodeMCU.
2.  Cek Serial Monitor di Arduino IDE (Baudrate 115200) untuk melihat apakah ada pesan error saat *booting*.
3.  Pastikan kodingan `soccer_robot.ino` sudah ter-upload dengan sukses ("Done uploading").

### Masalah: Gagal Terhubung ke WiFi (Wrong Password/Authentication Error)

**Solusi:**

1.  Cek panjang password di kodingan. Password mode AP (Access Point) **WAJIB minimal 8 karakter**.
      * ❌ Salah: `password = "1234"`
      * ✅ Benar: `password = "12345678"`

### Masalah: Web Controller Tidak Bisa Dibuka (192.168.xxx.xxx)

**Gejala:**

  * Sudah connect WiFi robot, tapi saat buka browser loading terus atau "Site can't be reached".

**Solusi:**

1.  **Matikan Data Seluler:** HP seringkali otomatis memutus WiFi robot karena "Tidak ada koneksi internet" dan beralih ke data seluler (4G). Matikan data seluler saat mengontrol robot.
2.  **Cek IP Address:** Pastikan alamat IP yang diketik di browser **SAMA PERSIS** dengan yang ada di Serial Monitor atau kodingan.
3.  **Gunakan Browser Chrome:** Disarankan menggunakan Google Chrome agar kompatibilitas joystick maksimal.

-----

## 🎮 3. Masalah Kontrol & Motor

### Masalah: Tampilan Joystick Hitam / Minta Putar Layar

**Solusi:**

  * Web controller dirancang untuk mode **Landscape**. Aktifkan fitur *Auto-rotate* di HP Anda dan posisikan HP secara mendatar.

### Masalah: Motor Tidak Bergerak

**Solusi:**

1.  **Cek Sumber Daya (Power):** Motor L298N **TIDAK BISA** hanya mengandalkan daya dari USB laptop. Anda wajib memasang baterai eksternal (minimal 2x Li-ion 18650 atau baterai 9V-12V yang arusnya cukup).
2.  **Saklar Driver:** Pastikan tombol switch pada modul L298N (jika ada) sudah ditekan.
3.  **Kabel Enable:** Pastikan pin `ENA` dan `ENB` pada driver motor terhubung ke pin PWM ESP8266 (D3/D8) atau jumper bawaan driver terpasang (jika tidak pakai kontrol speed).

### Masalah: Gerakan Robot Terbalik / Ngaco

  * **Maju jadi Mundur:** Tukar posisi kabel motor pada terminal hijau L298N (misal: kabel kiri A ditukar dengan kiri B).
  * **Belok Kiri jadi Kanan:** Tukar definisi pin motor Kiri dan Kanan di dalam kodingan, atau tukar kabel motor kiri ke terminal kanan driver (dan sebaliknya).

### Masalah: Motor Bergerak Sendiri Tanpa Disentuh

**Solusi:**

  * Refresh halaman web di HP.
  * Pastikan tidak ada jari yang menyentuh area joystick saat halaman baru dimuat (titik tengah joystick dikalibrasi saat *load* awal).

-----

## ❓ Masih Bermasalah?

Jika semua cara di atas sudah dicoba namun masih gagal:

1.  Cek kembali **Wiring** (kabel-kabel) Anda, mungkin ada kabel putus atau salah colok pin.
2.  Cek grup komunitas atau forum Arduino terkait masalah spesifik pada board ESP8266 Anda.
