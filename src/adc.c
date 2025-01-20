#include "adc.h"

void
adc_set_voltage_reference(voltage_reference_t voltage_reference) {
    const uint8_t VOLTAGE_REFERENCE_MASK = (1 << REFS1) | (1 << REFS0);
    uint8_t temp_admux = ADMUX;
    temp_admux &= ~VOLTAGE_REFERENCE_MASK;
    temp_admux |= (voltage_reference & VOLTAGE_REFERENCE_MASK);
    ADMUX = temp_admux;
}

void
adc_set_clock_prescaler(clock_prescaler_t clock_prescaler) {
    const uint8_t CLOCK_PRESCALER_MASK = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    uint8_t temp_adcsra = ADCSRA;
    temp_adcsra &= ~CLOCK_PRESCALER_MASK;
    temp_adcsra |= (clock_prescaler & CLOCK_PRESCALER_MASK);
    ADCSRA = temp_adcsra;
}
