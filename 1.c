

#include <mega8.h>
#include <alcd.h>
#include <stdio.h>
#include <delay.h>

int second=52,minute=40,hour=22,second2,minute2,hour2;
char str[50],k=0,alarm=0,alarmset=0,x=0,i=1,set=0;

char keypad(void);

interrupt [EXT_INT0] void ext_int0_isr(void)
{
 k=keypad();   //read keypad
 set=1;        // read keypad done
 PORTB=0xF0;   //

}

interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// Reinitialize Timer1 value
TCNT1H=0x85EE >> 8;
TCNT1L=0x85EE & 0xff;
// Place your code here

 x++;
 second++;
     if(second>59)
     {
        second = 0;
        minute++;

         if(minute > 59)
            {
              minute=0;
              hour++;

              if(hour>23)
              hour=0;
            }
     }
}

void main(void)
{

DDRC=0xFF;

DDRB=0x0F;
PORTB=0xF0;

PORTD.2=1;

DDRD.0=1;
PORTD.0=0;


// Timer normal mode   period 1S
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
TCNT1H=0x85;
TCNT1L=0xEE;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

TIMSK= (1<<TOIE1) ;   // timer 0 overflow enable

//   int0 fallig mode enable
GICR|=  (1<<INT0);
MCUCR=(0<<ISC11) | (0<<ISC10) | (1<<ISC01) | (0<<ISC00);
GIFR= (1<<INTF0);

lcd_init(20);

#asm("sei")

        lcd_gotoxy(0,2);
        lcd_puts("To Set Alarm Click 'C'");

while (1)
      {

        sprintf(str,"%02d : %02d : %02d",hour,minute,second);    // show Time
        lcd_gotoxy(0,0);
        lcd_puts(str);

      if(alarmset)   //alarmset = 1
      {
        if( hour2 == hour & minute2 == minute & second2 == second)  // Alarm
        {
           alarm = 1;
           x=0;
        }

        if(alarm)
        {
          if(x<2);        //Ringing 3 Second
          {
              PORTD.0= ~PORTB.0;
              delay_ms(100);
              PORTD.0= ~PORTD.0;
              delay_ms(100);
          }

          if(x>2)       // alarm off
          {
            PORTD.0=0;
            alarm = 0;
            alarmset=0;
          }
        }

      }


        if(set)  // keypad pushed
        {
          switch(k)
          {
           case 12: // '-'
           i--;
           break;

           case 13: // '+'
           i++;
           break;

           case 14:  // '='
           alarmset=1;
           lcd_clear();

            sprintf(str,"%02d : %02d : %02d",hour,minute,second); //Convert int to string
            lcd_gotoxy(0,0);
            lcd_puts(str);

            sprintf(str,"%d%d : %d%d : %d%d",hour2/10,hour2%2,minute2/10,minute2%10,second2/10,second2%10);
            lcd_gotoxy(0,1);
            lcd_puts(str);

           lcd_gotoxy(0,2);
           lcd_puts("Alarm Set    ");
           break;

           case 15:
           set=1;
           lcd_clear();
           lcd_gotoxy(0,0);
           lcd_puts(str);
           lcd_gotoxy(0,3);
           lcd_puts("Enter Second   ");
           lcd_gotoxy(0,2);
           lcd_puts("Setting Alarm    ");
           break;


           default:
               switch(i++)
               {
                 case 1:

                 second2 = k; // first digit of second
                 break;

                 case 2:
                 second2 += (k*10);      // second digit of second
                 if(second2>59) second=59;
                 lcd_gotoxy(0,3);
                 lcd_puts("Enter Minute  ");
                 break;

                 case 3:
                 minute2 = k;    // first digit of minute
                 break;

                 case 4:
                 minute2 += (k*10);     // second digit of second
                 if(minute2>59) minute2=59;
                 lcd_gotoxy(0,3);
                 lcd_puts("Enter Hour  ");
                 break;

                 case 5:
                 hour2 = k;            // first digit of hour
                 break;

                 case 6:
                 hour2 += (k*10);        // secodn digit of second
                 if(hour2>23) hour2=0;
                 lcd_gotoxy(0,3);
                 lcd_puts("          ");
                 i=1;
                 break;
               }
         }
            sprintf(str,"%02d : %02d : %02d",hour,minute,second);
            lcd_gotoxy(0,0);
            lcd_puts(str);

            sprintf(str,"%d%d : %d%d : %d%d",hour2/10,hour2%10,minute2/10,minute2%10,second2/10,second2%10);
            lcd_gotoxy(0,1);
            lcd_puts(str);

          set=0;
         }




      }
}

char keypad(void)
{
PORTB=0xFE;

        if(PINB.4==0)
              {
                return 7;    // number 7
              while(PINB.4==0);
              }
        if(PINB.5==0)
              {
                return 8;    // number 8
              while(PINB.5==0);
              }
        if(PINB.6==0)
              {
                return 9;   // number 9
              while(PINB.6==0);
              }
        /*if(PINB.7==0)
              {
                return 10;   // '/'
              while(PINB.7==0);
              }     */

PORTB=0xFD;

        if(PINB.4==0)
              {
              return 4;  // number 4
              while(PINB.4==0);
              }
        if(PINB.5==0)
              {
              return 5;    // number 5
              while(PINB.5==0);
              }
        if(PINB.6==0)
              {
              return 6;      // number 6
              while(PINB.6==0);
              }
      /*  if(PINB.7==0)
              {
              return 11;     //  '*'
              while(PINB.7==0);
              }*/
PORTB=0xFB;
        if(PINB.4==0)
              {
              return 1;     // number 1
              while(PINB.4==0);
              }
        if(PINB.5==0)
              {
              return 2;     // number 2
              while(PINB.5==0);
              }
        if(PINB.6==0)
              {
              return 3;     // number 3
              while(PINB.6==0);
              }
        if(PINB.7==0)
              {
              return 12;    //  '-'
              while(PINB.7==0);
              }
PORTB=0xF7;

        if(PINB.4==0)
              {
              return 15;     // 'Clear'
              while(PINB.4==0);
              }
        if(PINB.5==0)
              {
              return 0;      // number 0
              while(PINB.5==0);
              }
        if(PINB.6==0)
              {
              return 14;    //  '=' >> ok
              while(PINB.6==0);
              }
        if(PINB.7==0)
              {
               return 13;   // '+'
               while(PINB.7==0);
              }

     PORTB=0xF0;

}




