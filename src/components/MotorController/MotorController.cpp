#include <Ticker.h> // used for the Vesc.
#include <Arduino.h>
#include "VescUtility.h" // There is a TODO:: note in file.
#include "MotorController.h"
#include "packet.h"
#include "packet.c"
#include "buffer.h"
#include "buffer.c"
#include "crc.h"
#include "crc.c"
#include "bldc_interface.h"
#include "bldc_interface.c"
#include "bldc_interface_uart.h"
#include "bldc_interface_uart.c"
#include "datatypes.h"


// Settings NOT SURE IF NEEDED
//#define PACKET_HANDLER

MotorController::MotorController()
{

}

// TODO:: See if this can just be put in the constructor
// Setup the MotorController Object.
void MotorController::setup()
{
	// Initialize MotorController Communication Ticker
	Ticker motorControllerTicker;
  motorControllerTicker.attach_ms(1, updateMotorController, this);

	// Initialize Uart Communication
	MotorController::uartInit();
}

/**
 * Initialize the UART interface communication.
 *
 */
//void MotorController::uartInit(void(*s_func)(unsigned char *data, unsigned int len), void(*p_func)(unsigned char b), void(*v_func)(mc_values *val))
void MotorController::uartInit()
{
	bldc_interface_uart_init(sendUartData);
	bldc_interface_set_rx_value_func(printValues);
}

/**
 * Processesing the bytes received from the VESC.
 * @param b incoming byte
 */
void MotorController::processUartByte(unsigned char b)
{
	bldc_interface_uart_process_byte(b);
}

void MotorController::update()
{
	bldc_interface_uart_run_timer();
}

void MotorController::set_current(float current)
{
	bldc_interface_set_current(current);
}

void MotorController::set_current_brake(float current)
{
	bldc_interface_set_current_brake(current);
}

void MotorController::get_values()
{
	bldc_interface_get_values();
}
