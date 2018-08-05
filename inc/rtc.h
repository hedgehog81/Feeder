#ifndef RTC_H
#define RTC_H


#ifdef __cplusplus
extern "C"
{
#endif


typedef struct
{
	unsigned char sec;
	unsigned char min;
	unsigned char hour; 	
	unsigned char day;
	unsigned char month;
	unsigned char year;
	
} TIME;

void RTC_Read(TIME* ptime);
void RTC_Write(const TIME* ptime);
void RTC_WriteTime(const TIME* ptime);



#ifdef __cplusplus
extern "C"
}
#endif


#endif
