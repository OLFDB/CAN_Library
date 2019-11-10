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


#include "CAN.h"



//    // Initializes CAN communications.
//     void CANClass::begin(uint32_t bitrate){
//
//     }; //OK
//    // Finishes CAN communications
//     void CANClass::end();  //OK
//    // Check if message has been received on any of the buffers
//     uint8_t CANClass::available(); //OK
//    // Receive CAN message and allows use of the message structure for easier message handling
//     CAN_Frame read(); //OK
//
//     void CANClass::flush();  //OK
//    // Load and send CAN message.
//     uint8_t CANClass::write(const CAN_Frame&); // OK
//
//    //CAN_Frame& operator=(const CAN_Frame&);
//
//    // Experimental
//    //
//     void CANClass::setMask(uint8_t maskID, CAN_Filter mask); //OK
//    //
//     void CANClass::setMask(CAN_Filter mask);  //inline
//    //
//     void CANClass::clearMask(uint8_t maskID = 0);	//OK
//    //
//     void CANClass::setFilter(uint8_t filterID, CAN_Filter filter); //OK
//    //
//     void CANClass::setFilter(CAN_Filter filter); //OK
//    //
//     void CANClass::clearFilter(uint8_t filterID = 0); //OK
//    //
//     void CANClass::enableRXInterrupt();  //OK
//    //
//     void CANClass::disableRXInterrupt();  //OK


//CAN_Frame& CANClass::operator=(const CAN_Frame&)
//{
//}
