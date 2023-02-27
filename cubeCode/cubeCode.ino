#include <Adafruit_NeoPixel.h>
#include "train.h"
#include "comet.h"
#include "collide.h"
#include "explode.h"
#define LED_PIN    15
#define LED_COUNT 49

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ400);

Train train(&strip, 32);
Comet comet(&strip, 255,8);
Collide collide(&strip, 255,8);
Explode explode(&strip, 255,8,2);

volatile int buttonPushed = -1;
int stateRunnning = -1;
volatile unsigned long lastButtonPushTime = 0;

void setup() 
{
  randomSeed(analogRead(0));
  strip.begin();
  delay(10);
  strip.clear();
  strip.show(); 
  attachInterrupt(digitalPinToInterrupt(6), button0Pushed, RISING);
  attachInterrupt(digitalPinToInterrupt(7), button1Pushed, RISING);
  attachInterrupt(digitalPinToInterrupt(8), button2Pushed, RISING);
  attachInterrupt(digitalPinToInterrupt(9), button3Pushed, RISING);
}

void loop() 
{
  if (buttonPushed >= 0)
  {
    if (stateRunnning >= 0)
    {
      switch (stateRunnning) 
      {
        case 0:
          train.stop();
          break;
        case 1:
          comet.stop();
          break;
        case 2:
          collide.stop();
        case 3:
          explode.stop();
          break;
        default:
          break;
      }
      if (buttonPushed == stateRunnning) buttonPushed = -1;
      stateRunnning = -1;   
    }
    switch (buttonPushed) 
    {
      case 0:
        train.start();
        break;
      case 1:
        comet.start();
        break;
      case 2:
        collide.start();
        break;
      case 3:
        explode.start();
        break;
      default:
        break;
    }    
    stateRunnning = buttonPushed;
    buttonPushed = -1;
  }
  train.loop();
  comet.loop();
  collide.loop();
  explode.loop();
  delay(5);
}
boolean debounce()
{
  unsigned long now = millis();
  if ((now - lastButtonPushTime) < 1000) return false;
  if (buttonPushed >= 0) return false;
  lastButtonPushTime = now;
  return true;
}
void button0Pushed()
{
  if (!debounce()) return;
  buttonPushed = 0;
}
void button1Pushed()
{
  if (!debounce()) return;
  buttonPushed = 1;
}
void button2Pushed()
{
  if (!debounce()) return;
  buttonPushed = 2;
}
void button3Pushed()
{
  if (!debounce()) return;
  buttonPushed = 3;
}
