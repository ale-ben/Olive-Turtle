/**
 * @file ledPWM.h
 * @author Benetton Alessandro (aleben98@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef LEDPWM_H
#define LEDPWM_H
/**
 * @brief Setup LED PWM
 *
 * @param channel PWM channel (0-15)
 * @param frequency PWM frequency (Hz)
 * @param resolution PWM resolution (bits)
 * @param pin GPIO pin
 */
void setupLedPWM(int pin, int channel, int frequency, int resolution = 8);

/**
 * @brief Set the Led PWM
 * 
 * @param value PWM value (0-255)
 */
void setLedPWM(int value);

/**
 * @brief Get the Led PWM value
 * 
 * @return int PWM value (0-255)
 */
int getLedPWM();
#endif