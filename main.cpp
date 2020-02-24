#include "mbed.h"
#include "ultrasonic.h"
#include "Servo.h"
#include "C12832.h"

void dist(int distance)
{
    
    
}

Serial pc(USBTX, USBRX);
C12832 lcd(p5, p7, p6, p8, p11);

int dis;
DigitalIn reset(p14);
DigitalOut sound(p13);
Timer timer1; 
Timer timer2; 
bool first=true;
ultrasonic mu(p12, p10, .1, 1, &dist);
Servo servo(p21);
float eat;
int numturn=0;



void turnservo(){  

    servo=0;
    wait(2.2);
    servo=0.5;
    
    printf("food out\r\n"); 
    timer1.reset();
    timer1.start();
    printf("timer started again\r\n"); 
    numturn++;
    
}
void soundon(){
            sound=1;
            wait(0.5);
            sound=0;
            wait(0.5);
    }

int main() {
    
    mu.startUpdates();//start measuring the distance
    while(1)
    {
        
        
        mu.checkDistance();
       
        if(numturn<4){
            timer2.reset();
            dis=mu.getCurrentDistance();
            
            if ((first)&&(dis<200)){
                        timer1.start();
                        first=false;
                        turnservo();
                    }
            else if ((timer1.read()>10800)&&(dis<200)){
                        
                        turnservo();
                    }
                    
            //print time on lcd        
            eat=(timer1.read());
            if (eat>0){
                
                lcd.cls();
                lcd.locate(0,3);
                lcd.printf("TIME SINCE LAST MEAL:");
                lcd.locate(0,15);
                lcd.printf("%.0f sec",eat);
                wait(0.5);
                }        
                    
            }
                    
            //food container is empty                    
            else if(numturn==4){
                lcd.cls();
                lcd.locate(0,3);
                lcd.printf("REFILL CONTAINER!");
                wait(0.5);
                first=true;
                timer2.start();
                if(timer2.read()<8){
                    soundon();
                    }
                
                else if (timer2.read()>8){
                  sound=0;
                  timer2.stop();
                  }
                  
                if(reset){
                  
                  numturn=0;
                  
                  }
                }
      
        
    }
}      



   

