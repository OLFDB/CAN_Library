/*
 Copyright © 2007-2015 Fabian Greif, David Harding, Kyle Crockett,
 Nuno Alves, Stevenh, Collin Kidder, Daniel Kasamis, Cory Fowler, teachop,
 Pedro Cevallos, Neil McNeight

 This file is part of CAN_Library.

 CAN_Library is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 2.1 of the License, or
 (at your option) any later version.

 CAN_Library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 Acknowledgements:
 Fabian Greif for the initial libraries for MCP2515, SJA1000 and AT90CAN
 http://www.kreatives-chaos.com/artikel/universelle-can-bibliothek
 as well as his updates at https://github.com/dergraaf/avr-can-lib
 David Harding for his version of the MCP2515 library
 http://forum.arduino.cc/index.php/topic,8730.0.html
 Kyle Crockett CANduino library with 16Mhz oscillator
 http://code.google.com/p/canduino/
 Nuno Alves for the help on Extended ID messaging
 Stevenh for his work on library and all of the MCP research/work
 http://modelrail.otenko.com/arduino/arduino-controller-area-network-can
 Collin Kidder (collin80) for his work on the Arduino Due CAN interface
 https://github.com/collin80/due_can
 Daniel Kasamis (togglebit) both for his code at
 https://github.com/togglebit/ArduinoDUE_OBD_FreeRunningCAN as well as his
 DUE CANshield http://togglebit.net/product/arduino-due-can-shield/
 Cory Fowler (coryjfowler) for 16 MHz bitrate timing information
 https://github.com/coryjfowler/MCP2515_lib
 teachop for the FlexCAN library for the Teensy 3.1
 https://github.com/teachop/FlexCAN_Library

 -------------------------------------------------------------------------------
 Change Log

 DATE      VER   WHO   WHAT
 07/07/13  0.1   PC    Modified and merge all MCP2515 libraries found. Stripped
 away most unused functions and corrected MCP2515 defs
 09/12/13  0.2   PC    Added selectable CS SPI for CAN controller to use 1 IC
 to control several mcp2515
 02/05/14  0.3   PC    Added filter and mask controls
 05/01/14  0.4   PC    Cleaned up functions, variables and added message
 structures for J1939, CANopen and CAN.
 05/07/14  1.0   PC    Released Library to the public through GitHub
 06/18/14  1.5   NEM   Preparing a unified CAN library across three different
 CAN controllers
 06/14/15  1.6.0 NEM   Code cleanup and compatibility with Arduino 1.6.*
 -------------------------------------------------------------------------------

 */

#if defined(ARDUINO_ARCH_AVR) && !defined(__MK20DX256__)

#ifdef NORDIC_CAN

#include "SPI_nordic.h"
#include "nrf_log.h"

#define SPI_INSTANCE  0                                           /**< SPI instance index. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE); /**< SPI instance. */
static volatile bool spi_xfer_done; /**< Flag used to indicate that SPI instance completed the transfer. */

static uint8_t m_tx_buf[] = "0123456789"; /**< TX buffer. */
static uint8_t m_rx_buf[sizeof(m_tx_buf) + 1]; /**< RX buffer. */
static const uint8_t m_length = sizeof(m_tx_buf); /**< Transfer length. */

void spi_event_handler(nrf_drv_spi_evt_t const *p_event, void *p_context)
{
	spi_xfer_done = true;

	//Log only with SPI_CONFIG_LOG_ENABLED enabled in sdk_config.h
	if (SPI_CONFIG_LOG_ENABLED)
	{
		NRF_LOG_DEBUG("Transfer completed.");
		if (m_rx_buf[0] != 0)
		{
			NRF_LOG_DEBUG(" Received:");
			NRF_LOG_HEXDUMP_DEBUG(m_rx_buf, strlen((const char* )m_rx_buf));
		}
	}
}

void pinMode(uint8_t pin, uint8_t mode)
{
	if (mode == 1)
	{
		nrf_gpio_cfg_output(pin);
	}
	else
	{
		nrf_gpio_cfg_input(pin, NRF_GPIO_PIN_PULLUP);
	}

}

uint8_t bitRead(uint8_t value, uint8_t bitnum)
{
	uint8_t result;
	return value &= (1 << bitnum);
}

uint8_t bitSet(uint8_t &value, uint8_t bitnum)
{
	return value |= (1 << bitnum);
}

void SPIClass::begin()
{
	nrf_gpio_cfg_output(SPI_SS_PIN);

	nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
	spi_config.frequency = NRF_DRV_SPI_FREQ_250K;
	spi_config.ss_pin = 0xFF;
	spi_config.miso_pin = SPI_MISO_PIN;
	spi_config.mosi_pin = SPI_MOSI_PIN;
	spi_config.sck_pin = SPI_SCK_PIN;
	APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler, NULL));
}

void SPIClass::end()
{
	nrf_drv_spi_uninit(&spi);
}

uint8_t SPIClass::transfer(uint8_t data)
{

	// Reset rx buffer and transfer done flag
	memset(m_rx_buf, 0, m_length);

	spi_xfer_done = false;

	m_tx_buf[0] = data;

	APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, m_tx_buf, 1, m_rx_buf, 1));  //Transferlenghth needs to be 2!

	while (!spi_xfer_done)
	{
		__WFE();
	}

	return m_rx_buf[0];
}

void SerialClass::begin(void)
{

}

void SerialClass::print(const char *x)
{
	NRF_LOG_INFO("%s", x);
}

void SerialClass::print(const char x)
{
	NRF_LOG_INFO("%i", x);
}

void SerialClass::println(void)
{
	NRF_LOG_INFO("");
}

void SerialClass::println(uint32_t x)
{
	NRF_LOG_INFO("%i", x);
}

void SerialClass::print(const char x, uint8_t mode)
{
	if (mode)
	{
		NRF_LOG_INFO("%x", x);
	}
	else
	{
		NRF_LOG_INFO("%c", x);
	}
}

#endif // NORDIC_CAN

#endif //defined(ARDUINO_ARCH_AVR) && !defined(__MK20DX256__)
