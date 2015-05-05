
#include <SoftwareSerial.h>

#define PIN_RX 0
#define PIN_TX 1

#define DIRECTION_TOP 't'
#define DIRECTION_RIGHT 'r'
#define DIRECTION_BOTTOM 'b'
#define DIRECTION_LEFT 'l'
#define DIRECTION_COMMAND 'd'

#define BAUD_RATE 9600


SoftwareSerial hc06(PIN_RX, PIN_TX);


void setup()
{
  // init bluetooth connection
  hc06.begin(BAUD_RATE);
  delay(1000);

  // activate LED to show initialized state
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}


char readValue;

void loop()
{
  if (hc06.available() > 0)
  {
    readValue = hc06.read(); 

    if (readValue == DIRECTION_COMMAND) {

      while (hc06.available() <= 0) {}
      readValue = hc06.read();

      if (readValue != -1) {
        reactToDirection(readValue);
      } else {
        hc06.println("Error");
      }
    } else {
      hc06.print("I received: ");
      hc06.println(readValue, DEC);
    }
  }
}

void reactToDirection(char d) {
  switch (d) {
    case DIRECTION_TOP:
      hc06.println("Top");
      break;
    case DIRECTION_RIGHT:
      hc06.println("Right");
      break;
    case DIRECTION_BOTTOM:
      hc06.println("Bottom");
      break;
    case DIRECTION_LEFT:
      hc06.println("Left");
      break;
    default:
      hc06.println("Unknown Direction");
      break;
  }
}
