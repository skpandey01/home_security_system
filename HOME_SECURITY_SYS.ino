#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define Password_Length 5

Servo myservo;
LiquidCrystal lcd(13, 12, A2, A3, A4, A5);

int pos = 0;

char Data[Password_Length];
char Master[Password_Length] = "1234";
byte data_count = 0, master_count = 0;

bool Pass_is_good;
bool door = false;
char customKey;
#define temppin A1
#define gaspin A0
#define buzzerpin 11

/*---preparing keypad---*/

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};


byte rowPins[ROWS] = {8,2,3,4};
byte colPins[COLS] = {5, 6, 7};

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);


/*--- Main Action ---*/
void setup()
{
  myservo.attach(9, 2000, 2400);
  ServoClose();
  lcd.begin(16, 2);
  lcd.print("   SMART DOOR");
  delay(1500);
  lcd.clear();
  lcd.print("system activated");
  delay(1000);
  lcd.clear();
  loading("  Loading");
  lcd.clear();
  Serial.begin(9600);
}


void loop()
{
  //Door lock begin
  if (door == true)
  {
    customKey = customKeypad.getKey();
    if (customKey == '#')
    {
      lcd.clear();
      ServoClose();
      lcd.print("Door is closed");
      delay(3000);
      door = false;
    }
  }
  else
    Open();
  //door lock complete
  
  //beginning gas alarm
  int gasval;
 
 
   gasval = analogRead(gaspin);
   gasval = map(gasval, 85, 370,0,100);
   Serial.print("GAS VAL : ");
  Serial.println(gasval);
   
    
    
  if(gasval> 5 && gasval<30){
      tone(11,200,1000);

  }
  else if(gasval>30 && gasval<70){
    tone(11, 1800,1000);

  }
  else if (gasval > 70)
  {  tone (11 , 5000,1000);

  
  }  
  
  //gas alarm setup complete
  
  //begginng fire alarm
  
  double tempval = analogRead(temppin);
  tempval = tempval * 5/1024;
  tempval = tempval -0.5;
  tempval = tempval *100;
  Serial.print("TEMP : ");
  Serial.println(tempval);
  
  if(tempval> 50 && tempval<60){
      tone(11,200,1000);
   
  }
  else if(tempval>60 && tempval<90){
    tone(11, 1800,1000);
   
  }
  else if(tempval>90){
  tone (11 , 5000,1000);
   
  }
  //fire alarm complete
  
}

void loading (char msg[]) {
  lcd.setCursor(0, 1);
  lcd.print(msg);

  for (int i = 0; i <5;i++) {
    delay(300);
    lcd.print(".");
  }
}

void clearData()
{
  while (data_count != 0)
  { 
    Data[data_count--] = 0;
  }
  return;
}

void ServoClose()
{
  for (pos = 180; pos >= 0; pos -= 10) { 
    myservo.write(pos);
  }
}

void ServoOpen()
{
  for (pos = 0; pos <= 180; pos += 10) {
    myservo.write(pos);  
  }
}

void Open()
{
  lcd.setCursor(0, 0);
  lcd.print("Enter Password");
  
  customKey = customKeypad.getKey();
  if (customKey)
  {
    Data[data_count] = customKey;
    lcd.setCursor(data_count, 1);
    lcd.print(Data[data_count]);
    data_count++;
  }

  if (data_count == Password_Length - 1)
  {
    if (!strcmp(Data, Master))
    {
      lcd.clear();
      ServoOpen();
      lcd.print(" Door is Open ");
      door = true;
      delay(2000);
      lcd.clear();
     // loading("Closing in ");
      for(int i=15;i>0;i--){
         lcd.setCursor(0,0);
        lcd.print("Closing in ");
       lcd.setCursor(12,0);
       lcd.print(i);
      //  loading("Closing in ");
        delay(1000);
        lcd.clear();
      }
     
      delay(1000);
      lcd.print(" Closing Door ");
      delay(1000);
      ServoClose();
      door = false;      
    }
    else
    {
      lcd.clear();
      lcd.print(" Wrong Password ");
      door = false;
    }
    delay(1000);
    lcd.clear();
    clearData();
  }}
