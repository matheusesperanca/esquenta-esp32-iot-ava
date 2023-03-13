/**
 * @file goniometer.c
 * @author Matheus Arcangelo Esperanca
 * @brief 
 * @version 0.1
 * @date 2023-03-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "goniometer.h"

/**
 * @brief Inicializacao do medidor de angulo - goniometro
 * 
 */
void initGoniometer(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
}

/**
 * @brief Método para leitura da posicao do potenciometro em angulo (goniometro)
 * 
 * @param angle endereco da variavel que recebe o valor da medicao
 * @return int 0 se ok, -1 - falha
 */
int readGoniometer(float *ang)
{   
    int raw = adc1_get_raw(channel);
    //*ang = (float)ANGLE_MAX * ( (float)adc1_get_raw(channel) / (float)ADC_WIDTH );
    *ang = (float)ANGLE_MAX * ( (float)raw / (float)ADC_WIDTH );
    if((*ang >= 0) && (*ang <= ANGLE_MAX))
    {
        return 0;
    }
    else
    {
        return -1; // erro de conversao, valores fora dos limites
    }
}
