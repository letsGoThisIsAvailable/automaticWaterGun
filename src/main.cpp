#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <EEPROM.h>
#define LGFX_AUTODETECT
#include <LGFX_AUTODETECT.hpp>
#include <Ultrasonic.h>

// クラス"LGFX"を準備します
// #include <lgfx_user/LGFX_ESP32_sample.hpp> // またはユーザ自身が用意したLGFXクラスを準備します
// THIS WORKS !!!!

// cat WORKS!
// cat WORKS!

Ultrasonic ultrasonic(13, 15); //Double check if needs to be reversed
int distance;

static LGFX lcd;

extern const unsigned short cat[];
extern const unsigned short watergun[];
static constexpr unsigned short catWidth = 100;
static constexpr unsigned short catHeight = 100;
static constexpr unsigned short squirtGunWidth = 100;
static constexpr unsigned short SquirtGunHeight = 100;

static uint32_t sec, psec;
static size_t fps = 0, frame_count = 0;
static uint32_t lcd_width;
static uint32_t lcd_height;

static LGFX_Sprite background;
static LGFX_Sprite cat1;
static LGFX_Sprite squirtGun;
static int_fast16_t sprite_height;
int totalAddedReadings;
int avarage_distance_from_cat_gun;
double getAverage(int[], int);
void waitForButton2();
void waitForAnyButtonPress();
void textToLcd(const char *);
void lcdSettings(int, int, int, int);
void waitforButton1();
void intToLcd(int);
int readingsArray[5];
int readingsArraySize = 5;
double maximumAcceptableDistanceToShoot;
int userInputOnIfCalibrationIsGood = 0;
int button1 = 0;
int button2 = 35;
void setup(void)

{
  // EEPROM.begin(512);
  // EEPROM.read(500);
  pinMode(0, INPUT_PULLUP);
  pinMode(35, INPUT_PULLUP);
  pinMode(17, OUTPUT);
  Serial.begin(115200);

  // EEPROM.write(0, getAverage(readingsArray, readingsArraySize));
  // EEPROM.commit();
  lcd.init();
  lcd.setRotation(lcd.getRotation() ^ 1);
  lcd_width = lcd.width();
  lcd_height = lcd.height();
  lcd.startWrite();
  lcdSettings(2, TFT_WHITE, 0, 0);
}

void loop(void)
{
  userInputOnIfCalibrationIsGood = 2;
  while (userInputOnIfCalibrationIsGood == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    textToLcd("Press any button to begin calibration");
    waitForAnyButtonPress();

    if (digitalRead(button1) == LOW || digitalRead(button2) == LOW)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      textToLcd("calibrating in 3 \nseconds");
      for (int i = 0; i < 5; i++)
      {
        readingsArray[i] = ultrasonic.read();
        delay(1000);
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      maximumAcceptableDistanceToShoot = getAverage(readingsArray, readingsArraySize);
      textToLcd("does ");
      intToLcd(maximumAcceptableDistanceToShoot);
      textToLcd(" look right (click top button if good and bottom if\n not):  ");
    }

    waitForAnyButtonPress();
    int button1Pressed = digitalRead(0);
    int button2Pressed = digitalRead(35);

    if (button1Pressed == LOW)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      userInputOnIfCalibrationIsGood = 1;
      textToLcd("calibration successfull");
      delay(1000);
    }
    else if (button2Pressed == LOW)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      userInputOnIfCalibrationIsGood = 2;
      textToLcd("calibration Not Successful, Try Again");
      delay(1000);
    }
    intToLcd(userInputOnIfCalibrationIsGood);
    //} //Moved this down, otherwise userInputOnIfCalibrationIsGood will always be 0 and it will never get to the code below
    while (userInputOnIfCalibrationIsGood == 1)
    {
      if (ultrasonic.read() < maximumAcceptableDistanceToShoot)
      {
        digitalWrite(17, HIGH);
      }
      else
      {
        digitalWrite(17, LOW);
      }
    }
  }
}

void waitForButton2()
{
  while (digitalRead(35) == HIGH)
  {
    delay(10);
  }
}
void waitforButton1()
{
  while (digitalRead(0) == HIGH)
  {
    delay(10);
  }
}

void waitForAnyButtonPress()
{

  while (digitalRead(0) == HIGH && digitalRead(35) == HIGH)
  {
    delay(10); // Add a small delay to prevent excessive CPU usage
  }
}
void intToLcd(int number)
{
  lcd.print(number);
  lcd.display();
}
void textToLcd(const char *text)
{
  lcd.print(text);
  lcd.display();
}

void lcdSettings(int fontSize, int textColor, int cursorX, int cursorY)
{
  lcd.setCursor(cursorX, cursorY);
  lcd.setTextSize(fontSize);
  lcd.setTextColor(textColor);
}

double getAverage(int inputArray[], int arraySize)
{
  int i, sum = 0;
  double avg;

  for (i = 0; i < arraySize; ++i)
  {
    sum += inputArray[i];
  }
  avg = double(sum) / arraySize;

  return avg;
}

// background.createSprite(lcd_width, lcd_height);
// squirtGun.createSprite(squirtGunWidth, SquirtGunHeight);
// squirtGun.setSwapBytes(true);
// squirtGun.pushImage(0, 0, squirtGunWidth, SquirtGunHeight, watergun);
// cat1.createSprite(catWidth, catHeight);
// cat1.setSwapBytes(true);
// cat1.pushImage(0, 0, catWidth, catHeight, cat);

// int button1 = digitalRead(0);
// int button2 = digitalRead(35);
// lcd.setCursor(0, 0);

// Serial.println(EEPROM.read(0));
//  lcd.print("Hello World");

// distance = ultrasonic.read();
//  lcd.print(distance);
//  lcd.display();
// Serial.println(getAverage(readingsArray, readingsArraySize));

// lcd.clear();
//  for (int i = 3; i < 280; i++)
//  {

//   if (distance < 10)
//   {
//     digitalWrite(17, HIGH);

//   }
//   else
//   {
//     digitalWrite(17, LOW);
//   }
//   // Serial.println(distance);
//   //   // lcd.fillScreen(0x0000);
//   //   background.fillSprite(TFT_BLACK);
//   //   // cat1.pushSprite(&background, i, 0);
//   //   cat1.pushRotateZoom(&background, i, 60, i - 100, .8, .8, 0);
//   //   squirtGun.pushRotateZoom(&background, 70, 70, 0, 1, 1, 0);
//   //   background.pushSprite(&lcd, 0, 0);
//   //   lcd.display();
//   //   delay(30);

// // Example of a use
// int distances_from_cat_gun_while_calibrating[5] = ultrasonic.read();

// for (int i = 0; i < 5; i++)
// {
//   distances_from_cat_gun_while_calibrating[i] = ultrasonic.read();
// }
// average(distances_from_cat_gun_while_calibrating[]);
