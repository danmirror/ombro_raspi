#include<LiquidCrystal_I2C.h>   //library lcd_i2c
#include <Wire.h>
#include<DS3231.h>              //library RTC

LiquidCrystal_I2C lcd(0x27, 16, 2);                       //alamat lcd i2c
DS3231  rtc(SDA,SCL);
const int trigPin=12;                                  //pin HC-SR04
const int echoPin=13;                                    //pin HC-SR04
long duration;
float jarak, jarak2, jarak3, jarak4;                               //variabel HC-SR04
int button_backLight = 5;
int button_Swipe = 6;

int data1 = 0;
int offset =20;               // set the correction offset value
double voltage;
double voltage1;
int volt;
int statusRelay;
int waktu=0;
////==============koneksi ke internet================
////String ssid ="Rezki23"; // Nama Wi-Fi
////String password="12345678"; // password Wi-Fi
//String data;
//String server ="mhs.ceerduad.com";
////String server ="192.168.43.60";
//String uri ="/masuk/kabira.php";
//byte dat [5];
Time  t;
int jam= 0, menit= 0, detik=0, hari=0, bulan=0, tahun=0;  //variable interger
String day = "  ";        //variable string
long durasiRTC = 0;
long jedaRTC = 1000;
long durasiHCSR04 = 0;
long jedaHCSR04 = 50;
long durasisensor = 0;
long jedasensor = 500;


//--------

int lock =0;
int brush = 0;
int send_true = 0;
char dataString[50] = {0};
int swipe =1;
int klik = 1;
#define relayPin 8
void setup()
{

  lcd.init();
  lcd.backlight();
  
  uint32_t baud =115200;
  Serial.begin(baud);
  
  Wire.begin();      
  rtc.begin();
  pinMode(button_backLight, INPUT_PULLUP);
  pinMode(button_Swipe, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);              //relay pin 8

  digitalWrite(relayPin, HIGH);
  delay(1000);

  statusRelay = HIGH;
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pinMode(7, OUTPUT);



  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  OMBROMETER");
  lcd.setCursor(0, 1);
  lcd.print(" OBSERVATORIUM");
  delay(1000);
 
}  

void HCSR04() {  // put your main code here, to run repeatedly:
  digitalWrite(trigPin,LOW);  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  //jarak  = duration*0.0343/2;   //opsi ke cm
  jarak = duration/58.2;  //dalam bentuk centimeter
  jarak2 = jarak*10;      //dalam milimeter
  jarak3 = jarak2-147.4;  //set ke 0
  jarak4 = jarak3*(-1);  
  if (jarak4 < 0){
  jarak4=0;
  }
//  Serial.print("jarak4 = ");
//  Serial.println(jarak4);
 if (swipe ==1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time:  ");
    lcd.print(rtc.getTimeStr());
    lcd.setCursor(0,1);
    lcd.print("Date: ");
    lcd.print(rtc.getDateStr());
  }
}
void RTC_DS3231()
{
  t  =  rtc.getTime(); //  mengambil  semua  data  waktu dari RTC
  day = rtc.getDOWStr();   //mengambil data nama hari
  jam  = t.hour,DEC;        //pengambilan data jam
  menit = t.min,DEC;        //pengambilan data menit
  detik = t.sec,DEC;        //pengambilan data detik
  hari  = t.date,DEC;       //pengambilan data hari
  bulan  =  t.mon,DEC;     //pengambilan  data  bulan (dalam desimal)
  tahun = t.year,DEC;       //pengambilan data tahun
}

void sensor_tgn() {
  volt = analogRead(A0);// read the input
  voltage = map(volt,0,1023, 0, 2500) + offset; // map 0-1023 to 0-2500 and add correction offset
  voltage /=100;// divide by 100 to get the decimal values
  voltage1 = 12.2;// divide by 100 to get the decimal values
//  Serial.print("Voltage: ");
//  Serial.print(voltage);//print the voltage
//  Serial.println("V");
  delay(500);
}


void loop()
{ 
  //send
//  sprintf(dataString,"%02X",jarak4);
  Serial.println(jarak4);
  
  //  -------------------rtc
  RTC_DS3231();
  //  ------------ultrasonic

  HCSR04();
//  
//  Serial.println(jam);
//  Serial.println(menit);

  
  if(jam == 7){
    if(brush == 0){
      if(jarak4 > 0){
        digitalWrite(relayPin, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  POMPA HIDUP");
      }
      else{
        digitalWrite(relayPin, HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  POMPA MATI");
        brush = 1;
      }
    }
  }
  else{
     brush = 0; //reset data
  }
  
  if(jarak4 >=100){//    nyala
    lock =1;
  }
  if(lock == 1){
    digitalWrite(relayPin, LOW);
//    Serial.println("MASUK");
    
    //reset
    if(jarak4 <= 0)
      lock=0;
  }
  else{
     digitalWrite(relayPin, HIGH);
  }
//---------tegangan
  sensor_tgn();


  if(digitalRead(button_backLight)==LOW && klik ==1){
    lcd.noBacklight();
    klik=2;
    delay(500);
  }
  if(digitalRead(button_backLight)==LOW && klik==2)
  {
    lcd.backlight();
    klik=1;
    delay(500);
  }
  if (swipe ==2){
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Tinggi  =");
    lcd.setCursor(10, 0); lcd.print(jarak4);
    lcd.setCursor(14, 0);lcd.print("MM");
    lcd.setCursor(0, 1);lcd.print("Baterai =");
    lcd.setCursor(10, 1);lcd.print(voltage);
    lcd.setCursor(15, 1);lcd.print("V");
    delay(500);
  }
  
  if (digitalRead(button_Swipe)== LOW &&  swipe ==2) swipe=1;
  else if (digitalRead(button_Swipe)== LOW &&  swipe ==1) swipe=2;

  
  

}
