#ifndef ATMEGA_SNAKE_ADC_H_
#define ATMEGA_SNAKE_ADC_H_

#include <avr/io.h>
#include "avr-utils/common/mem.h"

#define ADC_MAX_RAW_VALUE 0x3FF /* 10bits, 0b1111111111 = 1023 */
#define ADC_MAX_VALUE     5     /* 5V */

/**
 * \brief           ADC voltage reference enumeration
 */
typedef enum {
    AREF_PIN = (0 << REFS1) | (0 << REFS0), /*!< AREF, internal Vref turned off */
    AVCC_PIN = (0 << REFS1) | (1 << REFS0), /*!< AVcc with external capacitor at AREF pin */
    INTERNAL = (1 << REFS1) | (1 << REFS0), /*!< Internal 1.1V voltage reference with external capacitor at AREF pin */
} voltage_reference_t;

typedef enum {
    ADC0 = (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0),
    ADC1 = (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (1 << MUX0),
    ADC2 = (0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (0 << MUX0),
    ADC3 = (0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (1 << MUX0),
    ADC4 = (0 << MUX3) | (1 << MUX2) | (0 << MUX1) | (0 << MUX0),
    ADC5 = (0 << MUX3) | (1 << MUX2) | (0 << MUX1) | (1 << MUX0),
    ADC6 = (0 << MUX3) | (1 << MUX2) | (1 << MUX1) | (0 << MUX0),
    ADC7 = (0 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0),
    ADC8_TEMPERATURE_SENSOR = (1 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0),
    BANDGAP_VOLTAGE_1_1V = (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (0 << MUX0),
    GROUND_0V = (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0),
} analogue_source_t;

typedef enum {
    DIV_BY_2 = (0 << ADPS2) | (0 << ADPS1) | (1 << ADPS0),
    DIV_BY_4 = (0 << ADPS2) | (1 << ADPS1) | (0 << ADPS0),
    DIV_BY_8 = (0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0),
    DIV_BY_16 = (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0),
    DIV_BY_32 = (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0),
    DIV_BY_64 = (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0),
    DIV_BY_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0),
} clock_prescaler_t;

/**
 * \note            ADC does not consume power when disabled
 */
static REAL_INLINE void
adc_set_enabled(bool enable) {
    if (enable) {
        ADCSRA |= (1 << ADEN);
    } else {
        ADCSRA &= ~(1 << ADEN);
    }
}

void adc_set_voltage_reference(voltage_reference_t voltage_reference);

static REAL_INLINE void
adc_set_analogue_source(analogue_source_t analogue_source) {
    const uint8_t ADC_MUX_MASK = (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0);
    uint8_t temp_admux = ADMUX;
    temp_admux &= ~ADC_MUX_MASK;
    temp_admux |= (analogue_source & ADC_MUX_MASK);
    ADMUX = temp_admux;
}

void adc_set_clock_prescaler(clock_prescaler_t clock_prescaler);

static REAL_INLINE uint16_t
adc_wait_and_get_raw_value(void) {
    ADCSRA |= (1 << ADSC);
    while (!mem_get_bit(&ADCSRA, ADIF)) {}
    return ADC;
}

#endif /* ATMEGA_SNAKE_ADC_H_ */
