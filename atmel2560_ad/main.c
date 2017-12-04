#define F_CPU 16000000UL

#include <asf.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>
#include <avr/io.h>

#ifndef BIT
#define BIT(x)(1<<(x))
#endif		//�õ�λ

#define uchar unsigned char
#define uint unsigned int

//1602ʵ��
#define RS_CLR PORTB&=~BIT(0)//����1602��rs���Ž��ж��壬PORTB&=~BIT(0)������0λ�õ͵�ƽ
#define RS_SET PORTB|=BIT(0)//PORTB|=BIT(0)����0λ�øߵ�ƽ
#define RS_OUT DDRB|=BIT(0)//��PB0����Ϊ���

#define RW_CLR PORTB&=~BIT(1)
#define RW_SET PORTB|=BIT(1)
#define RW_OUT DDRB|=BIT(1)

#define E_CLR PORTB&=~BIT(2)
#define E_SET PORTB|=BIT(2)
#define E_OUT DDRB|=BIT(2)
//1602ʵ��

#pragma data:code
const uchar table[]="0123456789"; //ֻ��
volatile uchar AD_flag;  //��AD_flag�Ž�ram�У�ÿ�ζ�ȡ���У���ֹ���������Ż�



//ADת����ʼ������
void AD_Init()
{
	//ADMUX=0x40;
	ACSR |= (1<<ADC);
	ADMUX |= (1<<REFS0);		//ADMUX=0x40;
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1<<ADPS0)|(1<<ADIE); //ʹ��ADת����ADCʱ��128��Ƶ
	ADCSRB |= 0x00; //����ת��ģʽ
	sei();
}

//ADת����
/*
uint AD_GetData()
{
	ADCSRA |= (1 << ADSC)|(1<<ADEN);     //��ʼADת��
	return ADC;        //����ADCֵ
}
*/
void timer0_init()
{	
	TCCR0B = 0x11;
	TCNT0 = 0x00;
	OCR0A = 0x00;
	TIMSK0 = 0x02;
	TCCR1A  |=(1<<WGM11); 
	TCCR1B =0x0e;
	OCR1A = 0x00;
	TIMSK1 = 0x02;
	sei();
}

void write_com(uchar com)
{
	RS_SET;
	RW_SET;
	E_CLR;
	RS_CLR;
	RW_CLR;
	PORTC=com;
	_delay_ms(1);
	E_SET;
	_delay_ms(1);
	E_CLR;
}

void write_data(uchar data)
{
	RS_CLR;
	RW_SET;
	E_CLR;
	RS_SET;
	RW_CLR;
	PORTC=data;
	_delay_ms(1);
	E_SET;
	_delay_ms(1);
	E_CLR;
}

void init(void)
{
	DDRC = 0xff;
	PORTC = 0;
	
	RS_OUT;
	E_OUT;
	RW_OUT;
	
	//_delay_ms(1);
	write_com(0x38);
	//_delay_ms(1);
	write_com(0x38);
	//_delay_ms(1);
	write_com(0x38);
	write_com(0x38);
	write_com(0x08);
	write_com(0x01);
	write_com(0x06);
	write_com(0x0c);
}

void prstring(uchar da[])
{	
	int d = strlen(da);
	for (int i=0;i<d;i++)
	{
		write_data(da[i]);
	}
}

void prchar(uchar x,uchar y,uchar data)
{
	switch(y)
	{     
		case 0:
		write_com(0x80+0x00+x);
		break;
		case 1:
		write_com(0x80+0x40+x);
		break;
	}
	write_data(data);
}


int main (void)
{	
	init();
	AD_Init();
	timer0_init();
	uint top=0,down=0,i=0,ad[200],ada,c=0;
	while(1)
	{
		
		write_com(0x80);
	if (AD_flag==1)
	{
		ad[c++]=ADC;
		AD_flag=0;
	}
	if (c==20)
	{
		while(c>0)
		{
			if (c>500)
			{
				top++;
			}
			else if (c<500)
			{
				down++;
			}
			c--;
		}
		c=0;
		for (i=0;i<4;i++)
		{
			ad[3-i]=top%10;
			top=top/10;
		}
		for (i=0;i<4;i++)
		{
			prchar(i,0,table[ad[i]]);
			_delay_us(20);
		}
		top=1,down=1;
	}
	}
}
	/*
	for (i=0;i<4;i++)
	{
		ad[3-i]=top%10;
		top=top/10;
	}
	for (i=0;i<4;i++)
	{
		prchar(i,0,table[ad[i]]);
		_delay_us(20);
	}
	top=1,down=1;



	write_com(0x80);
	prstring("Hello");
	write_com(0x80+0x40);
	prstring("IOT");
	_delay_ms(1000);
	write_com(0x01); 
	*/
	ISR(TIMER0_COMPA_vect)
	{
		ADCSRA |= (1 << ADSC)|(1<<ADEN);
		//prstring("l");
	}

	ISR(ADC_vect)
	{
		AD_flag=1;
		
	}
