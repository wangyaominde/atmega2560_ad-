#include<avr/io.h>
int main(void)
{
	//PB1 PB2 设置为输出模式
	DDRB |= (1 << DDB1) | (1 << DDB2);
	//PWM周期计数器
	ICR1 = 300;
	//PB1占空比计数器，占空比50%
	OCR1A = 150;
	//PB2占空比计数器，占空比50%
	OCR1B = 150;
	//OC1A OC1B起始高电平电平
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	//OC1A OC1B 快速PWM，TOP值为ICR1，不分频，PWM启动
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10);
	while(1);
	return 0;
}