#ifndef ATMEGA_SNAKE_TIMER_H_
#define ATMEGA_SNAKE_TIMER_H_

#define TMR1_INTERRUPT ISR(TIMER1_COMPA_vect, ISR_BLOCK)

void tmr1_init(void);

#endif /* ATMEGA_SNAKE_TIMER_H_ */
