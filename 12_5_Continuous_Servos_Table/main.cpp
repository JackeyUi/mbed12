#include "mbed.h"

#define CENTER_BASE 1500

BufferedSerial pc(USBTX, USBRX);
DigitalIn encoder(D10);
PwmOut servo(D11);

Thread s;
Timer t;
Ticker encoder_ticker;

volatile int steps;
volatile int last;

void servo_control(int speed) {
   if (speed > 200)       speed = 200;
   else if (speed < -200) speed = -200;

   servo = (CENTER_BASE - speed) / 20000.0f;
}

void speedcontrol() {
      servo_control(10);
      ThisThread::sleep_for(6000ms);
      servo_control(16);
}


void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}

int main() {

   pc.set_baud(9600);

   encoder_ticker.attach(&encoder_control, .01);

   //s.start(speedcontrol);

   int i = 0;
   while (i < 150) {
      if (i < 75)
      servo_control(30);
      else 
      servo_control(-38);

      steps = 0;
      t.reset();
      t.start();

      ThisThread::sleep_for(2000ms);

      float time = t.read();

      printf("%1.3f\r\n", (float)steps * 6.5 * 3.14 / 32 / time);

      i += 15;
   }
   servo_control(0);

   while(1);
}