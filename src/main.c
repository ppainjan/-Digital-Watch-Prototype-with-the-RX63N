/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <machine.h>
#include "platform.h"
//#include "iodefine.h"
#include "cmt_periodic_multi.h"
/*******************************************************************************
* Prototypes for variables and functions
*******************************************************************************/
volatile char Year1000, Year100, Year10, Year1, Mon10, Mon1, Day10, Day1,
Day, Hour10, Hour1, Min10, Min1, Sec10, Sec1;
volatile int flag = 0, label = 0, cmt_flag = 0, bl_flag = 0, bl_count = 0;
volatile int minute = 0,hour = 0,second = 0,alarm_hour10 = 0,alarm_hour1 = 0,alarm_min10 = 0,alarm_min1 = 0,alarm_sec10 = 0,alarm_sec1 = 0;
volatile int disp_hour10 = 0,disp_hour1 = 0,disp_min10 = 0,disp_min1 = 0,disp_sec10 = 0,disp_sec1 = 0;
volatile int tune1 = 0, tune2 = 0; 

#define CHANNEL_0   0
#define TIMER_COUNT_5S   468750

#define CHANNEL_1   1
#define TIMER_COUNT_1S  93750

void S12ADC_init(void);
void S12ADC_start (void);
uint16_t S12ADC_read (void);

void InitTimer(void);
void Displaytime(void);
void GetTime(void);
void display_time(void);
void stopwatch(void);
void alarm_mode(void);
void backlight_time_check(void);
void set_time_stopwatch_mode(void);
void set_time_display_mode(void);
void set_alarm_time_mode(void);
void delay(void);
void delay1(void);
void second_loop(void);
void backlight(void);
void disablePRD(void);
void EnablePRD(void);
void Excep_RTC_PRD(void);
void Initspeaker(void);
void alarmtime(void);
void Init_Alarm(void);  
void Excep_RTC_ALM(void);
void DAC_Init(void);

#pragma interrupt (Excep_RTC_PRD(vect=93))
#pragma interrupt (Excep_RTC_ALM(vect=92))






/******************************************************************************
* Function name: main
* Description  : Main program function
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{
    
    /* Initialize LCD */
    lcd_initialize();
    
    /* Clear LCD */
    lcd_clear();
    
    /* Display message on LCD */
    lcd_display(LCD_LINE1, "  Digital   ");
    lcd_display(LCD_LINE2, "   Watch  ");

	cmt_init(); /* Initialize the CMT unit for application timing tasks. */
    InitTimer(); /* Initialize the Real Time Clock */
	S12ADC_init (); /* Initialize the 12 bit Analog to digital converter */
	DAC_Init(); /* Initialize Digtial to Analog converter */
	cmt_callback_set(CHANNEL_1, &backlight);
    /* This is the main loop.  It does nothing but toggle LED4 periodically */
    while (1)
    {
		if(SW1 == 0)
		{
			delay1();
			if(SW1 == 0)
			{
				backlight_time_check(); // Function to check switch and clear 15 seconds counter for backlight off
				delay1();
				display_time();	 
			}				
    	}
	}
} /* End of function main() */

/******************************************************************************
Function name: display_time
* Description  : Displays RTC on the LCD.
* Arguments    : none
* Return value : none
******************************************************************************/

void display_time(void)
{
	//lcd_clear();
	lcd_display(LCD_LINE1, "Display_Mode");
	lcd_display(LCD_LINE2, "Time -");
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	while(SW1 == 1)
	{
		Displaytime();
		cmt_flag = 1;
		delay1();
		delay1();
		delay1();

	}	
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	if(SW1 == 0)
	{
		delay1();
		delay1();
		if(SW1 == 1)
		{
			label = 0;
			stopwatch(); // If key is pressed for shot time go to stopwatch mode 
		}
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		backlight_time_check();
		delay1();
		label = 1;	
		cmt_callback_set(CHANNEL_0, &second_loop); // Function assigned for cmt channel 1 callback 
		cmt_start(CHANNEL_0, TIMER_COUNT_5S); // Start cmt 5 second call back timer
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();	
		while(SW1 == 0){}
		if(cmt_flag == 1)
		{
			label = 0;
			stopwatch();
		}
	}			
}		

/******************************************************************************
Function name: stopwatch
* Description  : Displays 00:00:00 intially and start stopwatch with switch 2 press. 
				 Pressing switch again pauses stopwatch and pressing again unpause.
				 Pressing switch 3 reset stopwatch to 00:00:00   
* Arguments    : none
* Return value : none
******************************************************************************/				
	
void stopwatch(void)
{
	volatile int i = 0;
	char buffer [30];
	lcd_clear();
	lcd_display(LCD_LINE2, "Stopwatch");
	set_time_stopwatch_mode();
	sprintf((char *)buffer, "0%d:0%d:0%d",hour,minute,second);  
	lcd_display(LCD_LINE3, buffer); //Display 00:00:00 on LCD screen
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	while (SW1 == 1 )
    {
		if(SW3 == 0)
		{
	    	delay1();
			if(SW3 == 0)
			{
				delay1();
				backlight_time_check();
				delay1();
				minute = 0;
				hour = 0;
				second = 0;
				sprintf((char *)buffer, "0%d:0%d:0%d",hour,minute,second);
				lcd_display(LCD_LINE3, buffer);
				//disablePRD();
				i = 0;
			}	
		}
		delay1();
		delay1();
		if(SW2 == 0)
		{
	    	delay1();
			if(SW2 == 0)
			{
				delay1();
				backlight_time_check();
				delay1();
				i  = i + 1;
			}
		}
		delay1();
		delay1();
		if((i % 2) == 1 )
		{
			EnablePRD(); // Start periodic counter  to start stopwatch
			i = 1;
		 			
		}	
		if ((i % 2) == 0 )
		{
			disablePRD(); // Stop periodic counter  to stop stopwatch
		}
	}
	 delay1();
	backlight_time_check();
	delay1();
	disablePRD();
	delay1();
	delay1();
	delay1();
	delay1();
	alarm_mode(); //Go to alarm mode if switch 3 is pressed
}

/******************************************************************************
Function name: alarm_mode
* Description  : Displays default alarm time intially and pressing switch 1 for short time sets default tiem as alarm time. 
				 Pressing switch 1 for long time enters set alarm time mode.  
				 Pressing switch 2 selects tune 1 for alarm and pressing switch 2 selects tune 2 for alarm.    
* Arguments    : none
* Return value : none
******************************************************************************/								
void alarm_mode(void)
{
	lcd_clear();
	lcd_display(LCD_LINE1, "Set Alarm");
	lcd_display(LCD_LINE2, "Set Time");
	Init_Alarm();
	alarmtime(); // Displays default alarm time
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	while(SW1 == 1){}
	if(SW1 == 0)
	{
		delay1();
		delay1();
		delay1();
		delay1();
		if(SW1 == 1)
		{
			delay1();
		}	 
		else
		{
			delay();
			delay();
			if(SW1 == 0)
			 set_alarm_time_mode(); //Function to set new alarm time
		}
	}
	lcd_display(LCD_LINE4, "SW2-> Tune1");
	lcd_display(LCD_LINE5, "SW3-> Tune2");
	while((SW2 == 1)&&(SW3 == 1)){}
	if(SW2 == 0)
		tune1 = 1; // Flag set to select tune 1
	if(SW3 == 0)
		tune2 = 1; // Flag set to select tune 1
	delay1();
	lcd_clear();
	backlight_time_check();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	lcd_display(LCD_LINE6, "Alarm Set");
	display_time();		 		
}	
		
/******************************************************************************
Function name: InitTimer
* Description  : This function sets the date, day and time of RTC.And start the RTC.      
* Arguments    : none
* Return value : none
******************************************************************************/								
void InitTimer(void)
{
	RTC.RCR4.BIT.RCKSEL = 1; //Set date and time on RTC
	RTC.RCR2.BIT.START = 0; //Write 0 to start bit in RCR2
	while(RTC.RCR2.BIT.START == 1){}//Wait for start bit to clear to 0
	RTC.RFRH.WORD = 0x0001;
	RTC.RFRL.WORD = 0xFFFF;
	RTC.RCR2.BIT.RESET = 1; //Write 1 to Reset bit in RCR2
	RTC.RCR3.BIT.RTCEN=0;
	RTC.RCR2.BIT.HR24 = 1; //RTC operates in 24 hour mode
	RTC.RYRCNT.WORD = 0x0016; //Set year, month, day of
	RTC.RMONCNT.BYTE = 0x01; //week, date, hour, minute,
	RTC.RDAYCNT.BYTE = 0x29; //second to 2016 Jan 29st
	RTC.RWKCNT.BYTE = 0x02; //Tue 11:59:30
	RTC.RHRCNT.BYTE = 0x11;
	RTC.RMINCNT.BYTE = 0x59;
	RTC.RSECCNT.BYTE = 0x30;
	RTC.RCR2.BIT.START = 1; //Set start bit to 1 in RCR2
	while(RTC.RCR2.BIT.START == 0){} //Wait for start bit to set to 1
	//Set carry interrupt for use later when reading time from RTC
	IEN(RTC,COUNTUP) = 0; //Disable carry interrupt in ICU
	RTC.RCR1.BIT.CIE = 1; //Enable carry interrupt in the RTC
}

/******************************************************************************
Function name: Displaytime
* Description  : Display time on LCD screen.     
* Arguments    : none
* Return value : none
******************************************************************************/								
void Displaytime(void)
{
	char buffer [30];
	GetTime();
	sprintf((char *)buffer, "%d%d:%d%d:%d%d",Hour10,Hour1,Min10,Min1,Sec10,Sec1);
	lcd_display(LCD_LINE4, buffer);
}

/******************************************************************************
Function name: alarmtime
* Description  : Display alarm time on LCD screen.     
* Arguments    : none
* Return value : none
******************************************************************************/								
void alarmtime(void)
{
	char buffer [30];
	sprintf((char *)buffer, "%d%d:%d%d:%d%d",RTC.RHRAR.BIT.HR10,RTC.RHRAR.BIT.HR1,RTC.RMINAR.BIT.MIN10,RTC.RMINAR.BIT.MIN1,RTC.RSECAR.BIT.SEC10,RTC.RSECAR.BIT.SEC1);
	lcd_display(LCD_LINE3, buffer);
}

/******************************************************************************
Function name: GetTime
* Description  : Reads the register of hour, minute and seconds. And store the values of registers to variables.     
* Arguments    : none
* Return value : none
******************************************************************************/								
void GetTime(void)
{
	do
	{
		IR(RTC,COUNTUP) = 0; //Clear the Carry Flag
		Year10 = RTC.RYRCNT.BIT.YR10;
		Year1 = RTC.RYRCNT.BIT.YR1;
		Mon10 = RTC.RMONCNT.BIT.MON10;
		Mon1 = RTC.RMONCNT.BIT.MON1;
		Day10 = RTC.RDAYCNT.BIT.DATE10;
		Day1 = RTC.RDAYCNT.BIT.DATE1;
		Day = RTC.RWKCNT.BYTE;
		Hour10 = RTC.RHRCNT.BIT.HR10;
		Hour1 = RTC.RHRCNT.BIT.HR1;
		Min10 = RTC.RMINCNT.BIT.MIN10;
		Min1 = RTC.RMINCNT.BIT.MIN1;
		Sec10 = RTC.RSECCNT.BIT.SEC10;
		Sec1 = RTC.RSECCNT.BIT.SEC1;
	} while(IR(RTC,COUNTUP) == 1);
}


/*******************************************************************************
* Outline 		: Excep_RTC_ALM
* Description   : RTC alarm interrupt function generated when the time set in 
*				  the alarm registers matches with the current RTC time. 
* Argument  	: none
* Return value  : none
*******************************************************************************/
void Excep_RTC_ALM(void)
{
	int i,j;
	uint16_t k;
	/* Turn on LED1 to indicate that the alarm is triggered	*/
	LED8 = LED_ON;
	Initspeaker(); //intialise the speaker pins
	delay1();
	S12ADC_start(); // Start ADC conversion
	
	while((SW2 == 1) && (tune1 == 1)) // If tune 1 is selected
	{
		S12AD.ADCSR.BIT.ADST = 1; 
		delay1();
		delay1();
		k = S12ADC_read();
		k = k / 4;
		S12AD.ADCSR.BIT.ADST = 0;
		delay1();
		delay1();   
		DA.DADR1 = k ; 		
		delay1();
		delay1();
		PORT1.PODR.BIT.B7 = 0;
		for(i = 0;i < 600; i++)
			for(j = 0;j <100; j++);
		PORT1.PODR.BIT.B7 = 1;
		for(i = 0;i < 600; i++)
			for(j = 0;j <100; j++);	
		
	}
	tune1 = 0;
	while((SW2 == 1) && (tune2 == 1)) // If tune 2 is selected
	{
		S12AD.ADCSR.BIT.ADST = 1; 
		delay1();
		delay1();
		k = S12ADC_read();
		k = k / 4;
		S12AD.ADCSR.BIT.ADST = 0;
		delay1();
		delay1();   
		DA.DADR1 = k ; 		
		delay1();
		delay1();
		for(i = 0;i < 300; i++)
			for(j = 0;j< 100; j++);
		PORT1.PODR.BIT.B7 = 1;
		for(i = 0;i < 300; i++)
			for(j = 0;j <100; j++);	
	}
	tune2 = 0;
	delay1();
	backlight_time_check();
	delay1();
	delay1();
	delay1();		
	lcd_display(LCD_LINE6, "");
    IR(RTC,ALARM) = 0; /* Clear the interrupt flag */
	IEN(RTC,ALARM) = 0;
}

/*******************************************************************************
* Outline 		: set_time_stopwatch_mode
* Description   :  
* Argument  	: none
* Return value  : none
*******************************************************************************/
void set_time_stopwatch_mode(void)
{
	RTC.RCR1.BYTE = 0xE4;
	disablePRD();
}

/*******************************************************************************
* Outline 		: delay
* Description   : Long delay 
* Argument  	: none
* Return value  : none
*******************************************************************************/	 
void delay(void)
{
	int i,j;
	for (i = 1; i < 24000; i++)
	 for (j = 1; j< 100; j++ );
}	

/*******************************************************************************
* Outline 		: delay1
* Description   : Short delay 
* Argument  	: none
* Return value  : none
*******************************************************************************/	 
void delay1(void)
{
	int i, j ;
	for(i = 0 ; i < 10000; i++)
		for(j = 0 ; j < 100; j++);
}

/*******************************************************************************
* Outline 		: second_loop
* Description   : Function to check switch 1 is pressed for 5 second and go to set new time function. 
* Argument  	: none
* Return value  : none
*******************************************************************************/	 
void second_loop(void)
{
	flag = 1;
	cmt_stop(CHANNEL_0);
	if(label == 1)
	{
		while(SW1 == 0){}
		delay1();
		delay1();
		set_time_display_mode();
	}
}

/*******************************************************************************
* Outline 		: backlight
* Description   : Increment the switch not pressed every 1 second. If count is equal to 15, turnoff backlight and clear count. 
* Argument  	: none
* Return value  : none
*******************************************************************************/	 
void backlight(void)
{
	bl_count = bl_count + 1;
	
	if(bl_count == 65)
	{
		BL_ENA = 0;
		bl_count = 0;
	}
}	

/*******************************************************************************
* Outline 		: set_time_display_mode
* Description   : Function to set new RTc time.First set hour value then minute value and then second value.
                  Pressing switch 2 increment the value and pressing switch 3 decrement the value.
* Argument  	: none
* Return value  : none
*******************************************************************************/	 
void set_time_display_mode(void)
{
	lcd_clear();
	cmt_stop(CHANNEL_0);
	disp_hour10 = RTC.RHRCNT.BIT.HR10;
	disp_hour1 = RTC.RHRCNT.BIT.HR1;
	disp_min10 = RTC.RMINCNT.BIT.MIN10;
	disp_min1 = RTC.RMINCNT.BIT.MIN1;
	disp_sec10 = RTC.RSECCNT.BIT.SEC10;
	disp_sec1 = RTC.RSECCNT.BIT.SEC1;
	int count, count1 = 0, count2 = 0; 
	char buffer [30];
	label = 0;
	RTC.RCR2.BIT.START = 0;
	while(RTC.RCR2.BIT.START == 1){}
	lcd_clear();
	count = 0;
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	lcd_display(LCD_LINE1, "Set Time");
	while((SW1 == 1) && (count == 0 ))
	{
		if(SW2 == 0)
		{
			delay1();
			backlight_time_check();
			delay1();
			if((disp_hour1 == 9) && (disp_hour10 != 2) )
			{
				disp_hour10 = disp_hour10 + 1;
				disp_hour1 = 0;
			}
			else if((disp_hour10 == 2 ) &&(disp_hour1 == 4 ))
			{
				disp_hour1 = 0;
				disp_hour10 = 0;		
		
			}
			else
				disp_hour1 = disp_hour1 + 1;
		}	
		delay1();
		if(SW3 == 0)
		{
			delay1();
			backlight_time_check();
			delay1();
			if((disp_hour1 == 0) &&((disp_hour10 < 3) && (disp_hour10 > 0)) )
			{
				disp_hour10 = disp_hour10 - 1;
				disp_hour1 = 9;
			}
			else if((disp_hour10 == 0 ) &&(disp_hour1 == 0 ))
			{
				disp_hour10 = 2;
				disp_hour1 = 3;		
		
			}
			else
				disp_hour1 = disp_hour1 - 1; 		
		}
		LED4 = ~LED4;
		sprintf((char *)buffer,"%d%d:%d%d:%d%d",disp_hour10,disp_hour1,disp_min10,disp_min1,disp_sec10,disp_sec1);
		lcd_display(LCD_LINE3, buffer);
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		sprintf((char *)buffer,"  :%d%d:%d%d",disp_min10,disp_min1,disp_sec10,disp_sec1);
		lcd_display(LCD_LINE3, buffer);
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();		
		
	}
	delay1();
	backlight_time_check();
	count1 = 1;
	count = 1;
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	while((SW1 == 1) && (count1 == 1 ))
	{
		delay1();
		LED5 = ~LED5;
		sprintf((char *)buffer,"%d%d:%d%d:%d%d",disp_hour10,disp_hour1,disp_min10,disp_min1,disp_sec10,disp_sec1);
		lcd_display(LCD_LINE3, buffer);
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		sprintf((char *)buffer,"%d%d:  :%d%d",disp_hour10,disp_hour1,disp_sec10,disp_sec1);
		lcd_display(LCD_LINE3, buffer);
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		if(SW2 == 0)
		{
			delay1();
			backlight_time_check();
			delay1();
			if((disp_min1 == 9) && (disp_min10 != 5)) 
			{
				disp_min10 = disp_min10 + 1;
				disp_min1 =0;
			}
			else if((disp_min10 == 5 ) &&(disp_min1 == 9 ))
			{
				disp_min1 = 0;
				disp_min10 = 0;		
			}
			else
				disp_min1 = disp_min1 + 1;	
		}
		if(SW3 == 0)
		{
			delay1();
			backlight_time_check();
	        delay1();	
			if((disp_min1 == 0) && (disp_min10 != 0)) 
			{
				disp_min10 = disp_min10 - 1;
				disp_min1 = 9 ;
			}
			else if((disp_min10 == 0 ) &&(disp_min1 == 0 ))
			{
				disp_min1= 9;
				disp_min10 = 5;		
		
			}
			else
				disp_min1 = disp_min1 - 1;
		}
	}
	delay1();
	backlight_time_check();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	count1 = 0;
	count2 = 1;
	while((SW1 == 1) && (count2 == 1))
    {
		LED6 = ~LED6;
		sprintf((char *)buffer,"%d%d:%d%d:%d%d",disp_hour10,disp_hour1,disp_min10,disp_min1,disp_sec10,disp_sec1);
		lcd_display(LCD_LINE3, buffer);
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		sprintf((char *)buffer,"%d%d:%d%d:  ",disp_hour10,disp_hour1,disp_min10,disp_min1);
		lcd_display(LCD_LINE3, buffer);
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		if(SW2 == 0)
		{
			delay1();
			backlight_time_check();
			delay1();
			if((disp_sec1 == 9) && (disp_sec10 != 5)) 
			{
				disp_sec10 = disp_sec10 + 1;
				disp_sec1 = 0;
			}
			else if((disp_sec10 == 5 ) &&(disp_sec1 == 9 ))
			{
				disp_sec1 = 0;
				disp_sec10 = 0;		
		
			}
			else
				disp_sec1 = disp_sec1 + 1;
		}
		if(SW3 == 0)
		{
			delay1();
			backlight_time_check();
			delay1();
			if((disp_sec1 == 0) && (disp_sec10 != 0)) 
			{
				disp_sec10 = disp_sec10 - 1;
				disp_sec1 = 9 ;
			}
			else if((disp_sec10 == 0 ) &&(disp_sec10 == 0 ))
			{
				disp_sec1 = 9;
				disp_sec10 = 5;		
		
			}
			else
				disp_sec1 = disp_sec1 - 1;		
		}
	}
	delay1(); 
	backlight_time_check();
	count2 = 0; 
	RTC.RHRCNT.BIT.HR10 = disp_hour10;
	RTC.RHRCNT.BIT.HR1 = disp_hour1;
	RTC.RMINCNT.BIT.MIN10 = disp_min10;
	RTC.RMINCNT.BIT.MIN1 = disp_min1;
	RTC.RSECCNT.BIT.SEC10 = disp_sec10;
	RTC.RSECCNT.BIT.SEC1 = disp_sec1;
	lcd_clear();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	RTC.RCR2.BIT.START = 1; 
	while(RTC.RCR2.BIT.START == 0){}
	while(SW1 == 0){}	
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	display_time();
	
}

/*******************************************************************************
* Outline 		: Excep_RTC_PRD
* Description   : RTC period interrupt function generated every 1 second. It display updated stopwatch time.  
* Argument  	: none
* Return value  : none
*******************************************************************************/
void Excep_RTC_PRD(void)
{
	char buffer [30];
	if((second < 10) &&(minute < 10) &&(hour < 10))
		sprintf((char *)buffer, "0%d:0%d:0%d",hour,minute,second);
	else if	((second < 10) &&(minute < 10))
		sprintf((char *)buffer, "%d:0%d:0%d",hour,minute,second);
	else if	((second < 10) &&( hour < 10))
		sprintf((char *)buffer, "0%d:%d:0%d",hour,minute,second);
	else if	((hour < 10) &&(minute < 10))
		sprintf((char *)buffer, "0%d:0%d:%d",hour,minute,second);
	else if	(second < 10)
		sprintf((char *)buffer, "%d:%d:0%d",hour,minute,second);
	else if	(minute < 10)
		sprintf((char *)buffer, "%d:0%d:%d",hour,minute,second);
	else if	(hour < 10)
		sprintf((char *)buffer, "%0d:%d:%d",hour,minute,second);						
	lcd_display(LCD_LINE3, buffer);
	LED4 = ~ LED4;
	second = second + 1;
	if(second == 60)
	{
	  second = 0;	
	  minute = minute + 1;
	} 
	if (minute == 60)
	{
		minute = 0;
		hour = hour + 1;
	}	
	if(hour == 23)
	hour = 0;  
	
}

/*******************************************************************************
* Outline 		: EnablePRD 
* Description   : Enable the periodic interrupt 
* Argument  	: none
* Return value  : none
*******************************************************************************/	 
void EnablePRD(void) 
{
	IPR(RTC,PRD) = 0x07;
	IR(RTC,PRD)  = 0;
	IEN(RTC,PRD) = 1;
}

/*******************************************************************************
* Outline 		: disablePRD
* Description   : Disable the periodic interrupt 
* Argument  	: none
* Return value  : none
*******************************************************************************/	 
void disablePRD(void) 
{
	IPR(RTC,PRD) = 0x07;
	IR(RTC,PRD)  = 0;
	IEN(RTC,PRD) = 0;
}

/*******************************************************************************
* Outline 		: backlight_time_check
* Description   : Function to check no switch pressed count and reset the count.
* Argument  	: none
* Return value  : none
*******************************************************************************/	 
void backlight_time_check(void)
{
	if(bl_count > 0)
	{
		BL_ENA = 1;
		bl_count = 0;
		cmt_stop(CHANNEL_1);
	}
	
	if (bl_count == 0)
	{
		cmt_stop(CHANNEL_1); 
		cmt_start(CHANNEL_1, TIMER_COUNT_1S);
	}
}	

/*******************************************************************************
* Outline 		: Initspeaker
* Description   : Initialise the speaker pins 
* Argument  	: none
* Return value  : none
*******************************************************************************/	 
void Initspeaker(void)
{
	PORT1.PDR.BIT.B7 = 1; //Amplifier
	PORT1.PMR.BIT.B7 = 0; //Amplifier
	
	PORTA.PDR.BIT.B2 = 1;
	PORTA.PMR.BIT.B2 = 0;
	PORTA.PODR.BIT.B2 = 1; //Amplifier
	
	PORTC.PDR.BIT.B1 = 1;
	PORTC.PMR.BIT.B1 = 0;
	PORTC.PODR.BIT.B1 = 1; //Speaker
}

/*******************************************************************************
* Outline 		: set_alarm_time_mode
* Description   : Function to set new alarm time.First set hour value then minute value and then second value.
                  Pressing switch 2 increment the value and pressing switch 3 decrement the value.
* Argument  	: none
* Return value  : none
*******************************************************************************/	 
void set_alarm_time_mode(void)
{
	lcd_clear();
	alarm_hour10 = RTC.RHRAR.BIT.HR10;
	alarm_hour1 = RTC.RHRAR.BIT.HR1;
	alarm_min10 = RTC.RMINAR.BIT.MIN10;
	alarm_min1 = RTC.RMINAR.BIT.MIN1;
	alarm_sec10 = RTC.RSECAR.BIT.SEC10;
	alarm_sec1 = RTC.RSECAR.BIT.SEC1;
	RTC.RCR1.BIT.AIE = 0;
	while(RTC.RCR1.BIT.AIE == 1);
	int count, count1 = 0, count2 = 0; 
	char buffer [30];
	lcd_clear();
	count = 0;
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	while(SW1 == 0){}
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	lcd_display(LCD_LINE1, "New Alarm");
	while((SW1 == 1) && (count == 0 ))
	{
		if(SW2 == 0)
		{
			delay1();
			backlight_time_check();
			delay1();
			if((alarm_hour1 == 9) && (alarm_hour10 != 2) )
			{
				alarm_hour10 = alarm_hour10 + 1;
				alarm_hour1 = 0;
			}
			else if((alarm_hour10 == 2 ) &&(alarm_hour1 == 4 ))
			{
				alarm_hour1 = 0;
				alarm_hour10 = 0;		
		
			}
			else
				alarm_hour1 = alarm_hour1 + 1;
		}	
		delay1();
		if(SW3 == 0)
		{
			delay1();
			backlight_time_check();
			delay1();
			if((alarm_hour1 == 0) && ((alarm_hour10 < 3) && (alarm_hour10 > 0)))
			{
				alarm_hour10 = alarm_hour10 - 1;
				alarm_hour1 = 9;
			}
			else if((alarm_hour10 == 0 ) &&(alarm_hour1 == 0 ))
			{
				alarm_hour10 = 2;
				alarm_hour1 = 3;		
		
			}
			else
				alarm_hour1 = alarm_hour1 - 1; 		
		}
		LED4 = ~LED4;
		sprintf((char *)buffer,"%d%d:%d%d:%d%d",alarm_hour10,alarm_hour1,alarm_min10,alarm_min1,alarm_sec10,alarm_sec1);
		lcd_display(LCD_LINE3, buffer);
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		sprintf((char *)buffer, "  :%d%d:%d%d",alarm_min10,alarm_min1,alarm_sec10,alarm_sec1);
		lcd_display(LCD_LINE3, buffer);
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();		
		
	}
	delay1();
	backlight_time_check();
	count1 = 1;
	count = 1;
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	while((SW1 == 1) && (count1 == 1 ))
	{
		delay1();
		LED5 = ~LED5;
		sprintf((char *)buffer, "%d%d:%d%d:%d%d",alarm_hour10,alarm_hour1,alarm_min10,alarm_min1,alarm_sec10,alarm_sec1);
		lcd_display(LCD_LINE3, buffer);
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		sprintf((char *)buffer, "%d%d:  :%d%d",alarm_hour10,alarm_hour1,alarm_sec10,alarm_sec1);
		lcd_display(LCD_LINE3, buffer);
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		if(SW2 == 0)
		{
			delay1();
			backlight_time_check();
			delay1();
			if((alarm_min1 == 9) && (alarm_min10 != 5)) 
			{
				alarm_min10 = alarm_min10 + 1;
				alarm_min1 = 0;
			}
			else if((alarm_min10 == 5 ) &&(alarm_min10 == 9 ))
			{
				alarm_min10 = 0;
				alarm_min1 = 0;		
			}
			else
				alarm_min1 = alarm_min1 + 1;	
		}
		if(SW3 == 0)
		{
			delay1();
			backlight_time_check();
	        delay1();	
			if((alarm_min1 == 0) && (alarm_min10 != 0)) 
			{
				alarm_min10 = alarm_min10 - 1;
				alarm_min1 = 9 ;
			}
			else if((alarm_min10 == 0 ) &&(alarm_min1 == 0 ))
			{
				alarm_min1 = 9;
				alarm_min10 = 5;		
		
			}
			else
				alarm_min1 = alarm_min1 - 1;
		}
	}
	delay1();
	backlight_time_check();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	count1 = 0;
	count2 = 1;
	while((SW1 == 1) && (count2 == 1))
    {
		LED6 = ~LED6;
		sprintf((char *)buffer, "%d%d:%d%d:%d%d",alarm_hour10,alarm_hour1,alarm_min10,alarm_min1,alarm_sec10,alarm_sec1);
		lcd_display(LCD_LINE3, buffer);
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		sprintf((char *)buffer, "%d%d:%d%d:  ",alarm_hour10,alarm_hour1,alarm_min10,alarm_min1);
		lcd_display(LCD_LINE3, buffer);
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		delay1();
		if(SW2 == 0)
		{
			delay1();
			backlight_time_check();
			delay1();
			if((alarm_sec1 == 9) && (alarm_sec10 != 5)) 
			{
				alarm_sec10 = alarm_sec10 + 1;
				alarm_sec1 =0;
			}
			else if((alarm_sec10 == 5 ) && (alarm_sec1 == 9 ))
			{
				alarm_sec1 = 0;
				alarm_sec10 = 0;		
			}
			else
				alarm_sec1 = alarm_sec1 + 1;
		}
		if(SW3 == 0)
		{
			delay1();
			backlight_time_check();
			delay1();
			if((alarm_sec1 == 0) && (alarm_sec10 != 0)) 
			{
				alarm_sec10 = alarm_sec10 - 1;
				alarm_sec1 = 9 ;
			}
			else if((alarm_sec10 == 0 ) &&(alarm_sec1 == 0 ))
			{
				alarm_sec1 = 9;
				alarm_sec10 = 5;		
		
			}
			else
				alarm_sec1 = alarm_sec1 - 1;		
		}
	}
	delay1(); 
	backlight_time_check(); 
	count2 = 0;
	RTC.RCR1.BIT.AIE = 1;
	while(RTC.RCR1.BIT.AIE == 0);
	RTC.RHRAR.BIT.HR10 = alarm_hour10;
	RTC.RHRAR.BIT.HR1 = alarm_hour1;
	RTC.RMINAR.BIT.MIN10 = alarm_min10;
	RTC.RMINAR.BIT.MIN1 = alarm_min1;
	RTC.RSECAR.BIT.SEC10 = alarm_sec10;
	RTC.RSECAR.BIT.SEC1 = alarm_sec1; 
	lcd_clear();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	while(SW1 == 0){}	
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
	delay1();
}

/*******************************************************************************
* Outline 		: Init_Alarm
* Description   : Initialise alarm registers and seet default alarm time.  
* Argument  	: none
* Return value  : none
*******************************************************************************/	 
void Init_Alarm(void)
{
	//Check clock running
	if(RTC.RCR2.BIT.START == 1){
	//Disable ICU alarm interrupt
	IEN(RTC,ALARM) = 0;
	//Set alarm
	RTC.RSECAR.BYTE = 0x00;
	RTC.RMINAR.BYTE = 0x00;
	RTC.RHRAR.BYTE = 0x12;
	RTC.RWKAR.BYTE = 0x02;
	RTC.RDAYAR.BYTE = 0x29;
	RTC.RMONAR.BYTE = 0x01;
	RTC.RYRAR.WORD = 0x16;
	RTC.RSECAR.BIT.ENB = 0x01;
	RTC.RMINAR.BIT.ENB = 0x01;
	RTC.RHRAR.BIT.ENB = 0x01;
	RTC.RWKAR.BIT.ENB = 0x01;
	RTC.RDAYAR.BIT.ENB = 0x01;
	RTC.RMONAR.BIT.ENB = 0x01;
	RTC.RYRAREN.BIT.ENB = 0x01;
	//Enable RTC alarm
	RTC.RCR1.BIT.AIE = 1;
	while(RTC.RCR1.BIT.AIE == 0); //Wait for it to be written before continuing
	//Use periodic interrupt to wait 1/64 second
	IR(RTC,ALARM) = 0;
	//Enable ICU alarm interrupt
	IEN(RTC,ALARM) = 1;
	IPR(RTC,ALARM) = 3;
	}
	else {
	//error handling code	
	}
}	

/*******************************************************************************
* Outline 		: S12ADC_read
* Description   : Read ADC output
* Argument  	: none
* Return value  : none
*******************************************************************************/	 	
uint16_t S12ADC_read (void)
{
    uint16_t adc_result;
    
    adc_result = S12AD.ADDR2;        /* Read the result register for AN2 */
    
    return adc_result;
        
} /* End of function S12ADC_read() */

/*******************************************************************************
* Outline 		: S12ADC_start
* Description   : Start ADC
* Argument  	: none
* Return value  : none
*******************************************************************************/	 
void S12ADC_start (void)
{
    /* Start the A/D converter */
    S12AD.ADCSR.BIT.ADST = 1;
    
} /* End of function S12ADC_start() */

/*******************************************************************************
* Outline 		: S12ADC_init
* Description   : Initialise 12 bit ADC
* Argument  	: none
* Return value  : none
*******************************************************************************/	 
void S12ADC_init (void)
{
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
#endif

    /* Power up the S12ADC */
    MSTP(S12AD) = 0;
    
    /* Set up the I/O pin that will be used as analog input source. In this 
       demo the potentiometer will be used and is connected to port 42. */
    PORT4.PODR.BIT.B2 = 0;    /* Clear I/O pin data register to low output. */
    PORT4.PDR.BIT.B2  = 0;    /* Set I/O pin direction to input. */
    PORT4.PMR.BIT.B2  = 0;    /* First set I/O pin mode register to GPIO mode. */    
    MPC.P42PFS.BYTE = 0x80;   /* Set port function register to analog input, no interrupt. */    
        
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */
#endif  
    
    /* ADCSR: A/D Control Register  
    b7    ADST     0 a/d conversion start, Stop a scan conversion process
    b6    ADCS     1 Scan mode select, Continuous scan mode
    b5    Reserved 0 This bit is always read as 0. The write value should always be 0.
    b4    ADIE     0 Disables conversion complete IRQ to ICU
    b3:b2 CKS      0 A/D conversion clock select = PCLK/8
    b1    TRGE     0 Disables conversion to start w/ trigger
    b0    EXTRG    0 Trigger select, Scan conversion start by a timer source or software
	*/
    S12AD.ADCSR.BYTE = 0x40;

    /* ADANS0: A/D Channel Select Register 0
    b15:b0  ANS0: Selects analog inputs of the channels AN000 to AN015 that are 
    subjected to A/D conversion
    */
    S12AD.ADANS0.WORD = 0x0004; /* Read AN002, which is connected to the potentiometer */

    /* ADANS1: A/D Channel Select Register 1
	b15:b5  Reserved: These bits are always read as 0. 
                      The write value should always be 0.
    b4:b0   ANS1:     Selects analog inputs of the channels AN016 to AN020 
                      that are subjected to A/D conversion
    */
    S12AD.ADANS1.WORD = 0x0000;

    /* ADADS0: A/D-converted Value Addition Mode Select Register 0
    b15:b0  ADS0: A/D-Converted Value Addition Channel Select for AN000 to AN015.
    */
    S12AD.ADADS0.WORD = 0x0000;

    /* ADADS1: A/D-converted Value Addition Mode Select Register 1
	b15:b5  Reserved: These bits are always read as 0. The write value should always be 0.
    b4:b0   ADS1: A/D-Converted Value Addition Channel Select for AN016 to AN020.
    */
    S12AD.ADADS1.WORD = 0x0000;

    /* ADADC: A/D-Converted Value Addition Count Select Register
    b1:b0   ADC: 00 = 1 time conversion (same as normal conversion)
    */
    S12AD.ADADC.BYTE = 0x00;   /* 1-time conversion */

    /* ADCER: A/D Control Extended Register
    b15     ADRFMT:0  Right align the data in the result registers
    b5      ACE:0 Disables automatic clearing of ADDRn after it is read
    */
    S12AD.ADCER.WORD = 0x0000;   /* Right align data, automatic clearing off. */

    /* ADSTRGR: A/D Start Triggger Select Register
    b7:b4   Reserved. Always read/write 0.
    b3:b0   ADSTRS: 0, Software trigger or ADTRG0#
    */
    S12AD.ADSTRGR.BYTE = 0x00;
    
} /* End of function S12ADC_init() */


/*******************************************************************************
* Outline 		: DAC_Init
* Description   : Initialise DAC 
* Argument  	: none
* Return value  : none
*******************************************************************************/	 
void DAC_Init()
{
	PORT0.PDR.BIT.B5 = 1;
	PORT0.PMR.BIT.B5 = 0;
	//PORT0.ICR.BIT.B5 = 0;
	SYSTEM.MSTPCRA.BIT.MSTPA19 = 0;
	MPC.P03PFS.BYTE = 0x80;
	MPC.P05PFS.BYTE = 0x80;
    DA.DACR.BYTE = 0x8F;
}		