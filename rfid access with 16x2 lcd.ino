//RFID home  By: subhang mokkarala
//RFID access v1 with lcd
//pinout RFID (MFRC-522)

//• 3.3v = 3.3v

//• RST = D3 / PIN20

//• GND = GND

//• IRQ = Not Used

//• MISO = D6 / PIN40

//• MOSI = D7 / PIN44

//• SCK = D5 / PIN36

//y• SDA = D4 / PIN24

//pinout lcd

//GND = (GND) 

//VCC = Vin=. (5v to LCD)

//SDA = D4 of the NodeMCU.

//SCL = D3
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address check in google please

#define SS_PIN D4
#define RST_PIN D3
#define LED_G D1 //define green LED pin
#define LED_R D2 //define red LED
#define RELAY D2 //relay pin
#define BUZZER D8 //buzzer pin
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() 
{lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  Serial.begin(115200);   // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  digitalWrite(RELAY, HIGH);
   lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("rebooting...!");
  Serial.println("Put your card to the reader...");
  Serial.println();

}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "A8 D8 55 27") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
     // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  // Print a message to the LCD.
  lcd.print("welcome home!");
    digitalWrite(RELAY, LOW);
    digitalWrite(LED_G, HIGH);
    delay(ACCESS_DELAY);
    digitalWrite(RELAY, HIGH);
    digitalWrite(LED_G, LOW);
  }
 
 else   {
        // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  // Print a message to the LCD.
  lcd.print("intruder alert");
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    for(int i=0;i<10;i++)
    {
    tone(BUZZER, 450);
     delay(200);
    //delay(DENIED_DELAY);
    tone(BUZZER,300);
     delay(200);
   noTone(BUZZER);
    delay(500);
    }
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }
}
