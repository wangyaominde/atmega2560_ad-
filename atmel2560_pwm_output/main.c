#include<avr/io.h>
int main(void)
{
	//PB1 PB2 ����Ϊ���ģʽ
	DDRB |= (1 << DDB1) | (1 << DDB2);
	//PWM���ڼ�����
	ICR1 = 300;
	//PB1ռ�ձȼ�������ռ�ձ�50%
	OCR1A = 150;
	//PB2ռ�ձȼ�������ռ�ձ�50%
	OCR1B = 150;
	//OC1A OC1B��ʼ�ߵ�ƽ��ƽ
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	//OC1A OC1B ����PWM��TOPֵΪICR1������Ƶ��PWM����
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10);
	while(1);
	return 0;
}