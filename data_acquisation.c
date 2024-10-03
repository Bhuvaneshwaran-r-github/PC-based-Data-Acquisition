#include<lpc21xx.h>
#include"lcd_fourbit.h"	
#include"stdio.h"	
typedef unsigned char u8;

/*============================================I2C==========================================================*/

#define am 0<<5;
#define pm 1<<5;
#define hr_12 1<<6;
#define hr_24 0<<6;

#define SI_BIT 3
#define START 1<<5
#define STOP 1<<4
#define I2EN 1<<6

#define CCLK 60000000
#define PCLK CCLK/4
#define I2C_SPEED 100000
#define LOADVAL ((PCLK/I2C_SPEED)/2)

void i2c_init(void);
void i2c_start(void);
void i2c_restart(void);
void i2c_write(u8);
void i2c_stop(void);
u8 i2c_nack(void);

void eeprom_write(u8,u8,u8);
u8 eeprom_read(u8,u8);
void rtc(void);

/*============================================SPI==========================================================*/
//Operation
#define SET(WORD,POS) (WORD|=(1<<POS))
#define CLR(WORD,POS) (WORD&=~(1<<POS))
#define READ(WORD,POS) ((WORD>>POS)&1)

//S0SPCR Register bits
#define MSTR 5	//master bit enable
#define SPIF 7	//SPI flag

//Mode Selection
#define MODE0 0x00
#define MODE1 0x08
#define MODE2 0x10
#define MODE3 0x18

//Port Pin
#define CS 7//port pin 0.7

void spi_init(void);
float read_adc_mcp_3204(u8);
u8 SPI0(u8);
 
/*============================================UART=========================================================*/

typedef unsigned char u8;
		
void UART0_CONFIG(void);
void UART0_TX(char);
u8 UART0_RX(void); 
void UART0_STR(u8*);
void UART0_LCD(void);
void VECT_CONFIG(void);	
void UART0_INTEGER(int); 
void UART0_FLOAT(float);

/*============================================MAIN=========================================================*/

int main()
{					
	u8 hr,min,sec;
	float f;
 	lcd_init();
	UART0_CONFIG();
	i2c_init();
	spi_init();
			   
	UART0_STR("PC Based Data Acquisition\r\n");//Project Title Display	 
	lcd_str("PC Based Data Acquisition");   
   	delay_ms(500);
				
	eeprom_write(0x68,0x00,0x03);//SET SEC from 3
	eeprom_write(0x68,0x01,0x24);//SET MIN from 24	
	eeprom_write(0x68,0x02,0x03);//SET HOUR from 3										  6

	while(1)
	{	
   			lcd_command(0x01);//clear  
			lcd_command(0x80);//starting address
		
			hr=eeprom_read(0x68,0x02); 
			min=eeprom_read(0x68,0x01);
			sec=eeprom_read(0x68,0x00);	

			//display in lcd
		  	lcd_data((hr/16)+48); //Hr in BCD format if 24(decimal) => 0010 0100 => 36(BCD) 		   
		  	lcd_data((hr%16)+48);
		  	lcd_data(':');	 	   
		  	lcd_data((min/16)+48);			   
		  	lcd_data((min%16)+48);
		  	lcd_data(':');	 	   
		  	lcd_data((sec/16)+48);			   
		  	lcd_data((sec%16)+48);
						 
			//display in PC
			UART0_TX((hr/16)+48);  
			UART0_TX((hr%16)+48);
			UART0_TX(':');
			UART0_TX((min/16)+48);
			UART0_TX((min%16)+48);
			UART0_TX(':');
			UART0_TX((sec/16)+48);
			UART0_TX((sec%16)+48);
			UART0_STR("\r\n");
			lcd_command(0xc0);
			
			//temp
			f=read_adc_mcp_3204(0);	   
			//display in lcd
			lcd_str("Temp: ");
			lcd_float(f);
			lcd_str(" C");  
			
			//display in PC
			UART0_STR("Temp: ");
			UART0_FLOAT(f);
			UART0_STR(" C");	
			UART0_STR("\r\n");
			delay_ms(1000);		
	}	
}

/*============================================I2C==========================================================*/

void eeprom_write(u8 slaveAddr,u8 wBuffAddr,u8 dat)
{	
    i2c_start();	
    i2c_write(slaveAddr<<1|0); 
	i2c_write(wBuffAddr);   
	i2c_write(dat);    
	i2c_stop();
	delay_ms(10);  
}

u8 eeprom_read(u8 slaveAddr,u8 rBuffAddr)
{
	u8 dat;		
	i2c_start();
    i2c_write(slaveAddr<<1|0); //slaveAddr + w
	i2c_write(rBuffAddr);    //rBuffAddr
	i2c_restart();	
	i2c_write(slaveAddr<<1|1); //slaveAddr + r  
    dat=i2c_nack();	
	i2c_stop();
	return dat;

}
void i2c_init(void)
{
	PINSEL0|=0X50;
	I2SCLL=LOADVAL;
	I2SCLH=LOADVAL;
	I2CONSET=I2EN;
}

void i2c_start(void )
{
	I2CONSET=START;
	while(((I2CONSET>>SI_BIT)&1)==0);//wait until SI flag is cleared(transmission is to complete)
	I2CONCLR=START;
}

void i2c_restart(void)
{
	I2CONSET=START;
	I2CONCLR=1<<SI_BIT;
	while(((I2CONSET>>SI_BIT)&1)==0);
	I2CONCLR=START;
}

void i2c_write(u8 data)
{
	I2DAT=data;
	I2CONCLR=1<<SI_BIT;
	while(((I2CONSET>>SI_BIT)&1)==0);
}

void i2c_stop()
{
	I2CONSET=STOP;
	I2CONCLR=1<<SI_BIT;
}

u8 i2c_nack(void)
{
	I2CONCLR=1<<SI_BIT;		   
	while(((I2CONSET>>SI_BIT)&1)==0);
	return I2DAT;
}

/*============================================SPI==========================================================*/
 
void spi_init(void)
{
	PINSEL0|=0X1500; 	//pinsel selection for scl,mosi,miso
	S0SPCCR=150; 			//set baud rate = 15M Hz/100 kbps
	S0SPCR=(1<<MSTR|MODE3);	//Enable the spi interface
	IODIR0|=1<<7;		//selecting P0.7 as output pin(slave selection)
}

float read_adc_mcp_3204(u8 channel)
{
	u8 lbyte,hbyte;
	int adcVal=0;
	CLR(IOPIN0,CS);//clear output pin
	
	SPI0(0X06);			//start,dif/sig,D2
	hbyte=SPI0(channel<<6);//D1,D0 receive higher 6 bits
	lbyte=SPI0(0x00);	//receive lower 8bits 
	
	SET(IOSET0,CS);
	adcVal=((hbyte&0x0f)<<8)|lbyte;
	return (((adcVal*3.3)/4096)*100); //mul by 100 because 1500mV=150deg cel
}

u8 SPI0(u8 data)
{
	u8 stat;
	stat=S0SPSR;
	S0SPDR=data;
	while(READ(S0SPSR,SPIF)==0);
	return S0SPDR;
}
 
/*============================================UART=========================================================*/

void UART0_CONFIG(void)
{
	PINSEL0|=0X00000005;	//P0.0 as RXD and P0.1 as TXD
	U0LCR=0X83;	
	U0DLL=97;	//set baud rate
	U0LCR=0X03;
}

void UART0_TX(char data)
{
	U0THR=data;	//Transmit data
	while(((U0LSR>>5)&1)==0);//waiting for the data transfer is complete by checking status register
}

u8 UART0_RX(void)
{
	while((U0LSR&1)==0);	//waiting to receive full data
	return U0RBR;
}

void UART0_STR(u8* s)
{
 	while(*s)
		UART0_TX(*s++);
}
				   
void UART0_INTEGER(int num)
{
	char s[50];
	sprintf(s,"%d",num);	   
	UART0_STR((u8*)s);
}

void UART0_FLOAT(float f)
{
	char s[50];
 	sprintf(s,"%f",f);
	UART0_STR((u8*)s);
}





