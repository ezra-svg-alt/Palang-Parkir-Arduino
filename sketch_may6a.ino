#include <Servo.h>

const int echoPin = 9;
const int trigPin = 10;
const int servoPin = 3;
const int LED1 = 6;
const int LED2 = 7;
const int irPin = 8;  // Pin baru untuk Sensor Infrared

Servo gateServo; 
bool palangTerbuka = false; 

// --- PENGATURAN SUDUT SERVO 180 DERAJAT ---
const int sudutTutup = 90;   
const int sudutBuka = 0;   

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  // Inisialisasi pin IR sebagai INPUT
  pinMode(irPin, INPUT); 

  gateServo.attach(servoPin);
  
  // Memastikan posisi awal tertutup
  gateServo.write(sudutTutup); 
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH); 
  
  Serial.println("Sistem Ultrasonik (Buka) & IR (Tutup) Siap!");
}

void loop() {
  // --- 1. BACA SENSOR ULTRASONIK ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); 
  int distance;
  
  if (duration == 0) distance = 100;
  else distance = duration * 0.034 / 2;

  // --- 2. BACA SENSOR INFRARED ---
  // Membaca status IR (LOW = terdeteksi halangan, HIGH = kosong)
  int irStatus = digitalRead(irPin);

  // --- 3. LOGIKA BUKA (Dari Ultrasonik) ---
  if (distance > 0 && distance <= 7) {
    if (palangTerbuka == false) {
      gateServo.write(sudutBuka); 
      palangTerbuka = true;
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW); 
      
      delay(500); // Jeda agar servo selesai bergerak
    }
  } 
  
  // --- 4. LOGIKA TUTUP (Dari Infrared) ---
  // Jika palang sedang terbuka DAN mobil menyentuh sensor IR
  else if (palangTerbuka == true && irStatus == LOW) {
    
    // JEDA KEAMANAN (Opsional tapi penting)
    // Beri waktu 1.5 detik agar mobil benar-benar lewat sebelum palang turun
    delay(1500); 
    
    gateServo.write(sudutTutup); 
    palangTerbuka = false;
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH); 
    
    delay(500); // Jeda agar servo selesai bergerak
  }
  
  delay(50); // Kestabilan loop
}