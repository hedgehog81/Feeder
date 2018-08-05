#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "shell.h"
#include <stdint.h>
#include "timer.h"
#include "uart.h"
#include "hardware.h"
#include "rtc.h"
#include "servo.h"
#include "i2c_impl.h"
#include "settings.h"
#include "feeder.h"

#define UNUSED(a) (void)(a)
 

typedef void (*CmdHandler)(char argc,char** argv);


typedef struct
{
	const char* szDesc;
	const char* szCommand;
	char  MinArgCount;
	CmdHandler Handler;

} COMMAND;








static void CmdRtcRead(char argc,char** argv)
{
	TIME time = {0,};	

	UNUSED(argc);
	UNUSED(argv);
	

	RTC_Read(&time);

	printf_P(PSTR("RTC %02X/%02X/%02X %02X:%02X:%02X\r\n"),time.day, time.month, time.year, time.hour, time.min, time.sec);
	
	
}

static void CmdRtcWrite(char argc,char** argv)
{
	
	unsigned int temp[5];	
	TIME time ={0,};


	if (argc != 3)
	{
		printf_P(PSTR("Please provide time and date\r\n"));
		return;
	}
		
	
	if (3 != sscanf(argv[1],"%02X/%02X/%02X",&temp[0],&temp[1],&temp[2]))
	{
		printf_P(PSTR("Invalid date format\r\n"));
		return;
	}

	if (2 != sscanf(argv[2],"%02X:%02X",&temp[3],&temp[4]))
	{
		printf_P(PSTR("Invalid time format\r\n"));
		return;
	}

	time.day 	= temp[0];
	time.month 	= temp[1];
	time.year  	= temp[2];
	time.hour  	= temp[3];
	time.min   	= temp[4];

	RTC_Write(&time);
	
}



static void CmdServoSetPosition(char argc,char** argv)
{
	uint8_t position;	

	if (argc != 2)
	{
		printf_P(PSTR("Please provide position\r\n"));
		return;
	}
		
	position = atoi(argv[1]);
	
	if (position > 180)
	{
		printf_P(PSTR("Position cant be greater than 180\r\n"));
		return;
	}	
	
	Servo_SetPosition(position);
	
	
}

static void CmdServoSetPositionRaw(char argc,char** argv)
{
	uint8_t position;	

	if (argc != 2)
	{
		printf_P(PSTR("Please provide position\r\n"));
		return;
	}
		
	position = atoi(argv[1]);
	
	if (position > 256)
	{
		printf_P(PSTR("Position cant be greater than 256\r\n"));
		return;
	}	
	
	Servo_SetPositionRaw(position);
	
	
}




static void CmdServoSetPositionTimed(char argc,char** argv)
{
	uint8_t position;	
	uint16_t duration;	
	
	if (argc != 3)
	{
		printf_P(PSTR("Please provide position & duration\r\n"));
		return;
	}
		
	position = atoi(argv[1]);
	duration = atoi(argv[2]);
	
	if (position > 180)
	{
		printf_P(PSTR("Position cant be greater than 256\r\n"));
		return;
	}	
	
	Servo_SetPositionTimed(position,duration);
	
	
}


static void CmdServoTwiggle(char argc,char** argv)
{

	UNUSED(argc);
	UNUSED(argv);
	
	FeedCycle();

}



static void CmdAlarmSet(char argc,char** argv)
{
	unsigned int hh;
	unsigned int mm;	

	if (argc != 2)
	{
		printf_P(PSTR("Please provide trigger time\r\n"));
		return;
	}

	if (2 != sscanf(argv[1],"%02X:%02X",&hh,&mm))
	{
		printf_P(PSTR("Invalid time format\r\n"));
		return;
	}

	g_settings.triggertime = ((uint16_t)hh << 8) | ((uint16_t)mm & 0xFF); 
	
	Settings_Flush();
}


static void CmdAlarmGet(char argc,char** argv)
{

	UNUSED(argc);
	UNUSED(argv);

	printf_P(PSTR("Trigger time: %02X:%02X\r\n"),(g_settings.triggertime >> 8), (g_settings.triggertime & 0xFF));

}


static void CmdCycleGet(char argc,char** argv)
{

	UNUSED(argc);
	UNUSED(argv);

	printf_P(PSTR("Twig cycle: %02X\r\n"),g_settings.twigcycle);

}

static void CmdCycleSet(char argc,char** argv)
{
	unsigned int cycles;
		

	if (argc != 2)
	{
		printf_P(PSTR("Please provide number of cycles\r\n"));
		return;
	}

	if (1 != sscanf(argv[1],"%d",&cycles) || (cycles <= 0) || (cycles > 50))
	{
		printf_P(PSTR("Invalid cycle format\r\n"));
		return;
	}
	



	g_settings.twigcycle = cycles; 
	
	Settings_Flush();

}


static const char MENU_TEXT_1[] PROGMEM = "Read Rtc: rtcr \r\n";
static const char MENU_TEXT_2[] PROGMEM = "Write Rtc: rtcw dd/mm/yy hh:mm\r\n";
static const char MENU_TEXT_3[] PROGMEM = "Set position: pos \r\n";
static const char MENU_TEXT_4[] PROGMEM = "Set position (raw): rpos \r\n";
static const char MENU_TEXT_5[] PROGMEM = "Set position & duration: posex \r\n";
static const char MENU_TEXT_6[] PROGMEM = "Twiggle: twig \r\n";
static const char MENU_TEXT_7[] PROGMEM = "Set trigger time: alrmw hh:mm \r\n";
static const char MENU_TEXT_8[] PROGMEM = "Read trigger time: alrmr\r\n";
static const char MENU_TEXT_9[] PROGMEM = "Get twig cycle: cyclr\r\n";
static const char MENU_TEXT_10[] PROGMEM = "Set twig cycle: cyclw (cycle)\r\n";


static const char MENU_CMD_1[] PROGMEM = "rtcr";
static const char MENU_CMD_2[] PROGMEM = "rtcw";
static const char MENU_CMD_3[] PROGMEM = "pos";
static const char MENU_CMD_4[] PROGMEM = "rpos";
static const char MENU_CMD_5[] PROGMEM = "posex";
static const char MENU_CMD_6[] PROGMEM = "twig";
static const char MENU_CMD_7[] PROGMEM = "alrmw";
static const char MENU_CMD_8[] PROGMEM = "alrmr";
static const char MENU_CMD_9[] PROGMEM = "cyclr";
static const char MENU_CMD_10[] PROGMEM = "cyclw";



static const PROGMEM COMMAND cmd[] =
{
 
	{MENU_TEXT_1,MENU_CMD_1,	0,CmdRtcRead},
	{MENU_TEXT_2,MENU_CMD_2,	2,CmdRtcWrite},
	{MENU_TEXT_3,MENU_CMD_3,	1,CmdServoSetPosition},
	{MENU_TEXT_4,MENU_CMD_4,	1,CmdServoSetPositionRaw},
	{MENU_TEXT_5,MENU_CMD_5,	2,CmdServoSetPositionTimed},
	{MENU_TEXT_6,MENU_CMD_6,	0,CmdServoTwiggle},
	{MENU_TEXT_7,MENU_CMD_7,	1,CmdAlarmSet},
	{MENU_TEXT_8,MENU_CMD_8,	0,CmdAlarmGet},
	{MENU_TEXT_9,MENU_CMD_9,	0,CmdCycleGet},
	{MENU_TEXT_10,MENU_CMD_10,	1,CmdCycleSet},
};

static const unsigned char icmd = sizeof(cmd)/sizeof(cmd[0]);



static int uart_putchar(char cg, FILE *stream)
{
   if (cg == '\n')
        UART_Tx('\r');
      
   UART_Tx(cg);

   return 0;
} 



static char mygetchar(void)
{		
	 char ch;

	 while(1)
	 {
		if (UART_Rx(&ch))
		{
			UART_Tx(ch);
			return ch;
		}			
	 }
}


static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE); 	



void Shell_Init(void)
{
	stdout = &mystdout;
}


static int s_brun;

void Shell_Run(void)
{
		static char buffer[30];
		char ch;
		
		unsigned char cbbuffer;
		
		char* tokens[4];
		unsigned char tokencount;
		
		int  havetoken;
		
		unsigned char i;		
		
		s_brun = 1;

		while (s_brun)
		{
			cbbuffer = 0;
			tokencount = 0;
			havetoken = 0;
			
			printf_P(PSTR("\r\nCommand list:\r\n"));
			
		
			for (i = 0; i < icmd; ++i)
			{
				putchar(' ');
				
				printf_P((char*)pgm_read_word(&cmd[i].szDesc));
			}
			
			printf("**************************\r\n>");			
			
			while (cbbuffer < sizeof(buffer)-1)
			{
			 	ch = mygetchar();
				
				if (ch == '\r')
					break;				

				if (havetoken)
				{
					if ( ch == ' ')
					{
						havetoken = 0;
						buffer[cbbuffer++] = 0;
					}
					else
					{
						buffer[cbbuffer++] = ch;	
					}
				}
				else
				{
				 	if (ch != ' ')
					{
					 	havetoken = 1;
						tokens[tokencount++] = &buffer[cbbuffer];
						buffer[cbbuffer++] = ch;
					}
				}
				
				
			}

			buffer[cbbuffer] = 0;
			printf_P(PSTR("\r\n"));
			
			if (tokencount)
			{	
				

			  	for (i = 0; i < icmd; ++i)
				{
					const COMMAND* pCmd = &cmd[i];
					
					if (strcmp_P(tokens[0],(char*)pgm_read_word(&(pCmd->szCommand))) == 0)
					{
					  	if (pgm_read_byte(&(pCmd->MinArgCount))+1 >= tokencount)
						{
							(*(CmdHandler)pgm_read_word(&(pCmd->Handler)))(tokencount,tokens);

							printf_P(PSTR("\r\n"));						
						}		
						else
							printf_P(PSTR("Too few arguments\r\n"));						

						break;	
					}
				}

				if (i == icmd)
					printf_P(PSTR("Unknown command\r\n"));
 
			}

						
		}
		
	
	
}


