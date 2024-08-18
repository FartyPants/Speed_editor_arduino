#include <EncoderButton.h>
#include "Mouse.h"
#include "Keyboard.h"

// ports: 4,5,6,7,8,9,10,16,14,15
// LED: 18 - red
// LED: 19 - blue
// Play (spaceb bar) 15
// Split (Ctrl B) 14
// select all (Ctrl A) 9
// undo (Ctr Z) 8
// Backspace {backspce} 10
// delete [DEL] 16
// IN (i) 6 and OUT (o) 5
// CTRL) 7 
// (ALT) 4

// play (spacebar)

#define RED_LED 18
#define BLUE_LED 19

#define ENCODER_BUTTON 20

volatile bool bUseControlWithWheel = true;

volatile unsigned long encoderTime = 0;

volatile bool bSleep = false;

volatile int32_t  encoderPosition  = 0;
volatile int jogType = 0;

volatile int jog_mode = 0;

EncoderButton encoderBTN(2, 3);

EncoderButton BTN_4(4);
EncoderButton BTN_5(5);
EncoderButton BTN_6(6);
EncoderButton BTN_7(7);
EncoderButton BTN_8(8);
EncoderButton BTN_9(9);
EncoderButton BTN_10(10);
EncoderButton BTN_16(16);
EncoderButton BTN_14(14);
EncoderButton BTN_15(15);

EncoderButton BTN_20(ENCODER_BUTTON);

volatile bool controlPressed = false;
volatile bool altPressed = false;
volatile bool bMovingFast = false;
volatile bool bSpecialLED = false;
volatile bool bSpecialButton = false;

void blinkPurple()
{
  digitalWrite(BLUE_LED,HIGH);
  digitalWrite(RED_LED,LOW);
  delay(30);
  digitalWrite(RED_LED,HIGH);
  delay(30);
  digitalWrite(RED_LED,LOW);
  delay(30);
  digitalWrite(RED_LED,HIGH);
  delay(30);
  digitalWrite(RED_LED,LOW);

}

void blinkRed()
{
  digitalWrite(BLUE_LED,LOW);
  digitalWrite(RED_LED,LOW);
  delay(30);
  digitalWrite(RED_LED,HIGH);
  delay(30);
  digitalWrite(RED_LED,LOW);
  delay(30);
  digitalWrite(RED_LED,HIGH);
  delay(30);
  digitalWrite(RED_LED,LOW);
  delay(30);
  digitalWrite(RED_LED,HIGH);
  delay(30);  
  digitalWrite(RED_LED,LOW);
  digitalWrite(BLUE_LED,HIGH);

}

void blinkBlue()
{
  digitalWrite(RED_LED,LOW);
  digitalWrite(BLUE_LED,LOW);
  delay(30);
  digitalWrite(BLUE_LED,HIGH);
  delay(30);
  digitalWrite(BLUE_LED,LOW);
  delay(30);
  digitalWrite(BLUE_LED,HIGH);
  delay(30);
  digitalWrite(BLUE_LED,LOW);
  delay(30);
  digitalWrite(BLUE_LED,HIGH);


}


void keyPress(uint8_t c, bool bAlt=false, bool bControl=false, bool bShift=false)
{
  //Serial.println("key");
  if (bAlt)
    Keyboard.press(KEY_LEFT_ALT);

  if (bControl)
    Keyboard.press(KEY_LEFT_CTRL);

  if (bShift)
    Keyboard.press(KEY_LEFT_SHIFT);

  if (bSpecialLED)
    digitalWrite(BLUE_LED,LOW);

  digitalWrite(RED_LED,HIGH);
	Keyboard.press(c);	// Keydown
  delay(100);
	Keyboard.release(c);		// Keyup
	    
  
  if (bAlt)
    Keyboard.release(KEY_LEFT_ALT);

  if (bControl)
    Keyboard.release(KEY_LEFT_CTRL);

  if (bShift)
    Keyboard.release(KEY_LEFT_SHIFT);

  if (altPressed==false && controlPressed==false && bSpecialButton == false)
    digitalWrite(RED_LED,LOW);

  if (bSpecialLED)
  {
    bSpecialLED = false;
  }

  digitalWrite(BLUE_LED,HIGH);
   
}

void keyPress_sleep(uint8_t c, bool bAlt=false, bool bControl=false, bool bShift=false)
{
  //Serial.println("key");
  if (bAlt)
    Keyboard.press(KEY_LEFT_ALT);

  if (bControl)
    Keyboard.press(KEY_LEFT_CTRL);

  if (bShift)
    Keyboard.press(KEY_LEFT_SHIFT);

  digitalWrite(RED_LED,HIGH);
	Keyboard.press(c);	// Keydown
  delay(20);
	Keyboard.release(c);		// Keyup
	    
  
  if (bAlt)
    Keyboard.release(KEY_LEFT_ALT);

  if (bControl)
    Keyboard.release(KEY_LEFT_CTRL);

  if (bShift)
    Keyboard.release(KEY_LEFT_SHIFT);

  digitalWrite(RED_LED,LOW);

}

// alt
void on_ALT_pressed(EncoderButton& eb) 
{
  if (bSleep)
  {
    // paste in sleep mode
    keyPress_sleep('v',false,true);
    return;
  }

  digitalWrite(RED_LED,HIGH);

  if (bSpecialButton)
  {
    keyPress(KEY_DOWN_ARROW);
    return;
  }

  altPressed =  true;
  Keyboard.press(KEY_LEFT_ALT);

}

void on_ALT_released(EncoderButton& eb) {
  
  if (bSleep) return;

  altPressed = false;
  Keyboard.release(KEY_LEFT_ALT);
  digitalWrite(RED_LED,LOW);
}

void on_ALT_triple(EncoderButton& eb) 
{
  
 
}

// CTRL
void on_CTRL_pressed(EncoderButton& eb) 
{
   if (bSleep) 
   {
      // copy in sleep mode
      keyPress_sleep('c',false,true);
      return;
   }
  //Serial.println("CTRL");
  digitalWrite(RED_LED,HIGH);

  if (bSpecialButton)
  {
    keyPress(KEY_UP_ARROW);
    return;
  }

  controlPressed =  true;
  Keyboard.press(KEY_LEFT_CTRL);

}

void on_CTRL_released(EncoderButton& eb) {
  
  if (bSleep) return;

  controlPressed = false;
    Keyboard.release(KEY_LEFT_CTRL);
    digitalWrite(RED_LED,LOW);
}

void on_CTRL_triple(EncoderButton& eb) {
  
  if (bSleep) return;

  bUseControlWithWheel = !bUseControlWithWheel;

  digitalWrite(BLUE_LED,LOW);
  digitalWrite(RED_LED,LOW);

  if (bUseControlWithWheel)
  {
    delay(100);
    digitalWrite(RED_LED,HIGH);
    delay(100);
    digitalWrite(RED_LED,LOW);
    delay(100);
    digitalWrite(RED_LED,HIGH);
    delay(100);
    digitalWrite(RED_LED,LOW);
    delay(100);
    digitalWrite(RED_LED,HIGH);
    delay(100);
    digitalWrite(RED_LED,LOW);  
    delay(100);  
  }
  else
  {
    delay(100);
    digitalWrite(BLUE_LED,HIGH);
    delay(100);
    digitalWrite(BLUE_LED,LOW);
    delay(100);
    digitalWrite(BLUE_LED,HIGH);
    delay(100);
    digitalWrite(BLUE_LED,LOW);
    delay(100);
    digitalWrite(BLUE_LED,HIGH);
    delay(100);
    digitalWrite(BLUE_LED,LOW);    
    delay(100);

  }

   digitalWrite(BLUE_LED,HIGH);
} 

void on_spacebar_pressed(EncoderButton& eb)
{
  if (bSleep) return;

  if(controlPressed)
  {
    Keyboard.release(KEY_LEFT_CTRL);
    bSpecialLED = true;
    keyPress(KEY_UP_ARROW);
    Keyboard.press(KEY_LEFT_CTRL);
  }
  else if(altPressed)
    {
      Keyboard.release(KEY_LEFT_ALT);
      bSpecialLED = true; 
      keyPress(KEY_DOWN_ARROW);
      Keyboard.press(KEY_LEFT_ALT);
    }
  else  
   keyPress(' ');

}


void on_spacebar_longpressed(EncoderButton& eb)
{

}


// in and out

void on_in_pressed(EncoderButton& eb)
{
  if (bSleep) return;
   keyPress('i');
}

void on_in_longpressed(EncoderButton& eb)
{
  if (bSleep) return;
  bSpecialLED = true;
  keyPress('i',true);
}

void on_out_pressed(EncoderButton& eb)
{
  if (bSleep) return;
  
  if (bSpecialButton)
  {
    keyPress(KEY_F9);
    return;
  }  
  keyPress('o');
}

void on_out_longpressed(EncoderButton& eb)
{
  if (bSleep) return;
  bSpecialLED = true;
   keyPress('o',true);
}

// undo redo

void on_undo(EncoderButton& eb)
{
  if (bSleep) return;
   keyPress('z',false,true);
}

void on_redo_long(EncoderButton& eb)
{
  if (bSleep) return;

  bSpecialLED = true;
  keyPress('z',false,true,true);
  blinkRed();
}

// delete 

void on_delete(EncoderButton& eb)
{
   if (bSleep) return;
     
   keyPress(KEY_DELETE);
}

void on_backspace(EncoderButton& eb)
{
  if (bSleep) return;

  keyPress(KEY_BACKSPACE);
}

void do_sleep()
{
    bSleep = true;
      
    digitalWrite(BLUE_LED,LOW);
    digitalWrite(RED_LED,HIGH);
    Keyboard.releaseAll();
    controlPressed = false;
    altPressed = false;
    bMovingFast = false;
    bSpecialButton = false;  
    encoderPosition = 0;  
}

// special
void on_specialButton(EncoderButton& eb)
{
  if (bSleep)
  {
    bSleep = false;
    digitalWrite(BLUE_LED,HIGH);
    digitalWrite(RED_LED,LOW);
    controlPressed = false;
    altPressed = false;    
    bMovingFast = false;
    bSpecialButton = false;
    encoderPosition = 0;
    return;
  }

  digitalWrite(RED_LED,HIGH);
  bSpecialButton = true;
  encoderPosition = 0;


}

void on_specialButtonRelease(EncoderButton& eb)
{
  if (bSleep)
    digitalWrite(RED_LED,HIGH);
  else 
    digitalWrite(RED_LED,LOW);

  bSpecialButton = false;
  encoderPosition = 0;
  if (jogType!=0)
  {
    keyPress('k');
    jogType = 0;

  }

}

void on_sleep(EncoderButton& eb)
{
   do_sleep();
}

void on_cut(EncoderButton& eb)
{
   if (bSleep) return;

  if (bSpecialButton)
  {
    keyPress('a',false,true);
  }

  keyPress('b',false,true);
  
  if (bSpecialButton)
  {
    keyPress('a',false,true,true);
  }
}

void encoder_button_down(EncoderButton& eb) {

  jog_mode = jog_mode+1;
  if (jog_mode>1)
    jog_mode = 0;

  if (jog_mode==0)
    blinkBlue();
  else
    blinkRed();
      
}

void onEncoder(EncoderButton& eb) {
  
  if (bSleep)
  {
    return;
  }
  int enc = -eb.increment();
  bool fast = false;
  encoderPosition = encoderPosition-enc;

  int maxshots = 0;

  if (controlPressed)
  {
    Keyboard.release(KEY_LEFT_CTRL);
    
      if (enc>0)
        keyPress(KEY_LEFT_ARROW);
      if (enc<0)
        keyPress(KEY_RIGHT_ARROW);
    
    Keyboard.press(KEY_LEFT_CTRL);
    return;
  }

  if (bSpecialButton || jog_mode==1)
  {
    int prevJog = jogType;

    int nOff = 2;
    
    if (encoderPosition > nOff)
    {
      jogType+=1;
      if (jogType>1) jogType = 1;
      encoderPosition = 0;
      
    }
    if (encoderPosition < -nOff)
    {
      jogType-=1;
      if (jogType<-1) jogType = -1;
      encoderPosition = 0;
      
    }

    if (prevJog!=jogType)
    {
      if (jogType==-1)
        {
          keyPress('k');
          keyPress('j');
        }
      if (jogType==-2)
        {
          keyPress('k');
          keyPress('j');
          keyPress('j');
        }        
      if (jogType==0)
        {
          keyPress('k');
        }

      if (jogType==1)
        {
          keyPress('k');
          keyPress('l');
        }
      if (jogType==2)
      {
        keyPress('k');
        keyPress('l'); 
        keyPress('l');
      }

    }

    return;

  }
/*
  if(bSpecialButton)
  {
    if (enc>0)
    {
      bSpecialLED = true;
      keyPress(KEY_UP_ARROW);
    }
    if (enc<0)
    {
      bSpecialLED = true;
      keyPress(KEY_DOWN_ARROW);
    }

    return;
  }
*/

  if (altPressed)
  {
    enc = -enc;
  }
  else
  {

    if ((millis() - encoderTime)<50)
      fast = true;

    if (bUseControlWithWheel)
    {
      Keyboard.press(KEY_RIGHT_CTRL);
    }
  }


  Mouse.move(0, 0, enc);

  if (fast)
  {
     //Serial.println("fast");
     digitalWrite(RED_LED,HIGH);
     bMovingFast = true;
     delay(10);
     Mouse.move(0, 0, enc);
     delay(10);
     Mouse.move(0, 0, enc);
     delay(10);
     Mouse.move(0, 0, enc);     

  }
  else
  {
  }

  if (bUseControlWithWheel)
  {
    Keyboard.release(KEY_RIGHT_CTRL);
  }
  
  encoderTime = millis();
}

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  delay(100);
  //Serial.println("EncoderButton Basic Example");

  pinMode(BLUE_LED,OUTPUT);
  pinMode(RED_LED,OUTPUT);

  pinMode(ENCODER_BUTTON,INPUT_PULLUP);

  digitalWrite(RED_LED,LOW);
  digitalWrite(BLUE_LED,LOW);
  delay(50);
  digitalWrite(RED_LED,HIGH);
  delay(50);
  digitalWrite(RED_LED,LOW);
  delay(50);
  digitalWrite(RED_LED,HIGH);
  delay(50);
  digitalWrite(RED_LED,LOW);

  encoderBTN.setEncoderHandler(onEncoder);  

  //CONTROL 
  BTN_16.setPressedHandler(on_CTRL_pressed);
  BTN_16.setReleasedHandler(on_CTRL_released);
  BTN_16.setTripleClickHandler(on_CTRL_triple);

  // ALT KEY
  BTN_10.setPressedHandler(on_ALT_pressed);
  BTN_10.setReleasedHandler(on_ALT_released);
  //BTN_4.setTripleClickHandler(on_ALT_triple);

  // spacebar
  BTN_15.setPressedHandler(on_spacebar_pressed);
  //BTN_15.setLongPressHandler(on_spacebar_longpressed);

  BTN_7.setPressedHandler(on_in_pressed);
  BTN_7.setLongPressHandler(on_in_longpressed);
  BTN_4.setPressedHandler(on_out_pressed);
  BTN_4.setLongPressHandler(on_out_longpressed);

// undo redo
  //BTN_8.setPressedHandler(on_undo_press);
  BTN_8.setClickHandler(on_undo);
  BTN_8.setLongPressHandler(on_redo_long);

// delete and backspace (16, 10 or 7,4)
  BTN_6.setPressedHandler(on_delete);
  BTN_5.setPressedHandler(on_backspace);

// special button
  BTN_9.setPressedHandler(on_specialButton);
  BTN_9.setReleasedHandler(on_specialButtonRelease);
  BTN_9.setTripleClickHandler(on_sleep);
  //BTN_9.setLongPressHandler(on_sleep);

//cut
  BTN_14.setPressedHandler(on_cut);

  BTN_20.setPressedHandler(encoder_button_down);


  Mouse.begin();
  Keyboard.begin();

  digitalWrite(BLUE_LED,HIGH);
}

void loop() { 
 
 if (bMovingFast && altPressed==false && controlPressed==false && bSpecialButton==false)
    digitalWrite(RED_LED,LOW);

  encoderBTN.update();
  BTN_4.update();
  BTN_5.update();
  BTN_6.update();
  BTN_7.update();
  BTN_8.update();
  BTN_9.update();
  BTN_10.update();
  BTN_16.update();
  BTN_14.update();
  BTN_15.update();
  BTN_20.update();
  
}