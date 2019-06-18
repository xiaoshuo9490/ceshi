#include <OLED_I2C.h> //https://www.drive2.ru/c/479335393737572613/
OLED  myOLED(SDA, SCL, 8);
extern uint8_t MediumNumbers[];
extern uint8_t RusFont[];
extern uint8_t SmallFont[];
#define K_line_RX 0 
#define K_line_TX 1 
int n; 
int Temp1 = 75;
int Temp2 = 25;
int PMM = 847;
int SPEED = 0;
String s;
int pac =0;
int tm = 10000;
byte    init_obd[] = {0xC1,0x33,0xF1,0x81,0x66};      // инициализация K-line шины  C1 33 F1 81 66 
byte     pmm_obd[] = {0xC2,0x33,0xF1,0x01,0x0C,0xF3}; // запрос оборотов двигателя  C2 33 F1 01 0C F3
byte   temp1_obd[] = {0xC2,0x33,0xF1,0x01,0x05,0xEC}; // запрос температуры ож      C2 33 F1 01 05 EC
byte   temp2_obd[] = {0xC2,0x33,0xF1,0x01,0x0F,0xF6}; // запрос температуры воздуха C2 33 F1 01 0F F6
byte   speed_obd[] = {0xC2,0x33,0xF1,0x01,0x0D,0xF4}; // запрос скорости автомобиля C2 33 F1 01 0D F4

void setup()  { 
  pinMode(K_line_RX, INPUT); 
  pinMode(K_line_TX, OUTPUT); 
  myOLED.begin();
  myOLED.clrScr();

              } 

void loop(){  
   read_CAN();
   tm--;
   if (tm <0){
    tm=10000;   
    Serial.flush();   
    for(int i=0;i<6;i++) Serial.write(temp1_obd[i]), delay (10);  
    delay(100);
           } 
}


void read_CAN(){ 
  
if (pac == 0) {
  digitalWrite(K_line_TX, HIGH), delay(300); 
  digitalWrite(K_line_TX, LOW), delay(25);   
  digitalWrite(K_line_TX, HIGH), delay(25); //-----------_-
  Serial.begin(10400);  // ------------_- ISO 14230-4 KWP 10.4 Kbaud
  for(int i=0;i<5;i++) Serial.write(init_obd[i]), delay (10); // отправляем команду инициализации K-line шины 
  delay(100);  
  myOLED.setFont(SmallFont), myOLED.print("SEND> C1 33 F1 81 66", 0, 0),myOLED.update(); 
               }
 
  char byfer[30];
  n = Serial.available();
  if (n > 0) {  pac++;
  for (int i=0;i<n;i++) byfer[i]=Serial.read();
  myOLED.setFont(RusFont), myOLED.print("ghbyznj ,fqn", 0, 10), myOLED.printNumI(n, 80, 10);
  myOLED.print("Gfrtnjd", 0, 44), myOLED.printNumI(pac,60,44),myOLED.update(); 
  String byte8 = String(byfer[8],DEC);   // С1 (HEX) = 193 (DEC) // С1 успешный ответ
  String byte10 = String(byfer[10],DEC); // 05 HEX = 05 DEC, 0F HEX = 15 DEC, 0C HEX = 12 DEC, 0D HEX = 13 DEC

// if  (n == 5)   myOLED.setFont(SmallFont), myOLED.print("EHO>", 35, 0), myOLED.update(); 

if  (n == 12 && byte8 ==  "193")   {  // ждем инициализхации шины
                                        myOLED.setFont(SmallFont), myOLED.print("83 F1 10 C1 E9 8F BD ", CENTER, 20), myOLED.update(); 
                                        Serial.flush();   
                                        for(int i=0;i<6;i++) Serial.write(temp1_obd[i]), delay (10);  
                                        delay(100);
                                         }
 if (n == 13  && byte10 ==  "5" )       { // читаем температуру ОЖ двигателя из 12-го байта пакета
                                        s = String(byfer[11],DEC);
                                        Temp1 = s.toInt() - 40;  
                                        for(int i=0;i<6;i++) Serial.write(temp2_obd[i]), delay (10); 
                                        delay(100);
                                        }

                                          
 if (n == 13  && byte10 ==  "15" )     {  // читаем температуру воздуха на впуске из 12-го байта пакета
                                        s = String(byfer[11],DEC); 
                                        Temp2 = s.toInt() - 40; 
                                        for(int i=0;i<6;i++) Serial.write(pmm_obd[i]), delay (10);
                                        delay(100);                          
                                       }


 if (n == 14  && byte10 ==  "12" )     {  // читаем обороты двигателя из 12-го и 13-го байта пакета
                                        s = String(byfer[11],DEC);
                                        int h = s.toInt();
                                        s = String(byfer[12],DEC);
                                        int l = s.toInt();
                                        PMM = word(h, l)/4;
                                        for(int i=0;i<6;i++) Serial.write(speed_obd[i]), delay (10);
                                        delay(100);  
                                        }
                                          
  if (n == 13  && byte10 ==  "13" )    {  // если  читаем скорость из 12-го байта пакета
                                        s = String(byfer[11],DEC);
                                        SPEED = s.toInt();


  myOLED.clrScr();
  myOLED.setFont(MediumNumbers);
  myOLED.printNumI(Temp1, RIGHT, 0), myOLED.printNumI(Temp2, RIGHT, 16),myOLED.printNumI(PMM, RIGHT, 32), myOLED.printNumI(SPEED, RIGHT, 48);
  myOLED.setFont(RusFont);
  myOLED.print("NTVGTHFNEHF LDBU", LEFT, 0), myOLED.print("NTVGTHFNEHF DJPL", LEFT, 20),myOLED.print("J<JHJNS LDBU", LEFT, 36), myOLED.print("CRJHJCNM FDNJ", LEFT, 54);
  myOLED.update();
  



                                        }
                                          
        }
              
}
