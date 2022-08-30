#include "./PSU.h"
#include "../refs.h"

psu_context_t* psu_init(int vd_r1, int vd_r2, float vref, float battery_min_voltage, float battery_max_voltage)
{
    psu_context_t* b = (psu_context_t*)malloc(sizeof(psu_context_t));

    b->battery_min_voltage = battery_min_voltage;
    b->battery_max_voltage = battery_max_voltage;
    b->vref = vref;
    
    //2.75 * 47000 / 69000 = 1.873
    float vd_min = (battery_min_voltage * (float)vd_r2) / (float)(vd_r1 + vd_r2);
    //4.2 * 47000 / 69000 = 2.860
    float vd_max = (battery_max_voltage * (float)vd_r2) / (float)(vd_r1 + vd_r2);

    //1.873 / 3.3 * 256 = 145
    b->adc_min = (ushort)((vd_min / vref) * 256.0f);
    //2.860 / 3.3 * 256 = 221
    b->adc_max = (ushort)((vd_max / vref) * 256.0f);

    return b;
}

float psu_calculate_voltage(psu_context_t* cxt, int adc_reading)
{
    return cxt->vref;
}

ushort psu_calculate_percent(psu_context_t* cxt, int adc_reading)
{
    ushort max = cxt->adc_max - cxt->adc_min;
    ushort val = adc_reading - cxt->adc_min;
    return (ushort)(((float)val / (float)max) * 100.0f);
}