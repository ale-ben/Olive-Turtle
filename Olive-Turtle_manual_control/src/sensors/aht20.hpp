/**
 * @file aht20.hpp
 * @author Benetton Alessandro (aleben98@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ATH20_HPP
#define AHT20_HPP

// #define DISABLE_AHT20

/**
 * @brief Initializes sensor communication
 *
 * @return true If sensor initialization succeded
 * @return false If sensor initialization failed
 */
bool aht20Setup();

/**
 * @brief Returns the last read temperature
 *
 * @return float The temperature in °C
 */
float aht20GetTemperature();

/**
 * @brief Returns the last read hummidity
 *
 * @return float The hummidity in % RH
 */
float aht20GetHumidity();

/**
 * @brief Prints the last read data to the serial port
 *
 */
void aht20PrintData();
#endif