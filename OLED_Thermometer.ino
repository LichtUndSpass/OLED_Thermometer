#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

#define sclk 13
#define miso 12
#define mosi 11
#define dc   7
#define oledcs   10
#define rst  9

//Farben
#define	BLACK           0b0000000000000000
#define	BLUE            0b0000000000011111
#define	RED             0b1111100000000000
#define YELLOW          0b1111111111100000
#define WHITE           0b1111111111111111
#define ORANGE          0b1111110001000000

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_SSD1351 tft = Adafruit_SSD1351(oledcs, dc, rst);

void setup()
{
  mlx.begin();
  tft.begin();
  tft.setRotation(2);//Dreht Bild um 180 Grad
  tft.fillScreen(BLACK);
}


int counter = 0;
float maxValue = 40;
float lastTemp = 0;
float currentTemp = 0;
int color = BLACK;

void loop()
{
  currentTemp =   mlx.readObjectTempC();

  if (currentTemp > maxValue)
  {
    maxValue = currentTemp;
  }

  if (currentTemp > 30)
  {
    color = RED;
  }
  else if (currentTemp > 20)
  {
    color = ORANGE;
  }
  else if (currentTemp > 15)
  {
    color = YELLOW;
  }
  else
  {
    color = BLUE;
  }

  float f = currentTemp / maxValue;
  tft.fillRect(2 * counter, 128 - f * 118, 2, f * 118,  color); //x,y,w,h,farbe
  tft.setTextColor(color);


  if (abs(currentTemp - lastTemp) > 0.15)//Hysterese - vermeidet zu starkes Zappeln
  {
    lastTemp = currentTemp;

    //alten Text löschen
    if (currentTemp > 100)
    {
      tft.fillRect(50, 0, 64, 8, BLACK);
    }
    else
    {
      tft.fillRect(50, 0, 32, 8, BLACK);
    }

    //Text in die Mitte der Bildschirms setzen
    tft.setCursor(50, 0);
    tft.print(currentTemp);
  }

  delay(50);

  counter++;
  if (counter > 63)
  {
    counter = 0;
    //Graph löschen
    tft.fillRect(0, 10, 128, 118,  BLACK);//x,y,w,farbe
  }
}



