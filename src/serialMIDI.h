/*!
 *  @file       serialMIDI.h
 *  Project     Arduino MIDI Library
 *  @brief      MIDI Library for the Arduino - Platform
 *  @license    MIT - Copyright (c) 2015 Francois Best
 *  @author     lathoub, Francois Best
 *  @date       22/03/20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once

#include "midi_Namespace.h"
#include <pico/stdlib.h>
#include <hardware/uart.h>

BEGIN_MIDI_NAMESPACE

struct DefaultSerialSettings
{
    /*! Override the default MIDI baudrate to transmit over USB serial, to
    a decoding program such as Hairless MIDI (set baudrate to 115200)\n
    http://projectgus.github.io/hairless-midiserial/
    */
    static const long BaudRate = 31250;
    static const uint8_t TxPin = 0;
    static const uint8_t RxPin = 1;
    static const bool TxFunctionAux = false;
    static const bool RxFunctionAux = false;
};

template <class _Settings = DefaultSerialSettings>
class SerialMIDI
{
    typedef _Settings Settings;

public:
    SerialMIDI(uart_inst_t *inSerial)
        : mSerial(inSerial) {};

public:
    static const bool thruActivated = true;

    void begin()
    {
        // // Initialise the Serial port
        // #if defined(AVR_CAKE)
        //         mSerial.template open<Settings::BaudRate>();
        // #else
        //         mSerial.begin(Settings::BaudRate);
        // #endif
        uart_init(mSerial, Settings::BaudRate);
        gpio_set_function(Settings::TxPin, (Settings::TxFunctionAux) ? GPIO_FUNC_UART_AUX : GPIO_FUNC_UART);
        gpio_set_function(Settings::RxPin, (Settings::RxFunctionAux) ? GPIO_FUNC_UART_AUX : GPIO_FUNC_UART);
    }

    void end()
    {
        uart_deinit(mSerial);
    }

    bool beginTransmission(MidiType)
    {
        return true;
    };

    void write(byte value)
    {
        // mSerial.write(value);
        uart_putc_raw(mSerial, value);
    };

    void endTransmission() {};

    byte read()
    {
        return uart_getc(mSerial);
    };

    unsigned available()
    {
        return uart_is_readable(mSerial);
    };

private:
    uart_inst_t *mSerial;
};

END_MIDI_NAMESPACE

/*! \brief Create an instance of the library attached to a serial port.
 You can use HardwareSerial or SoftwareSerial for the serial port.
 Example: MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, midi2);
 Then call midi2.begin(), midi2.read() etc..
 */
#define MIDI_CREATE_INSTANCE(SerialPort, Name)           \
    MIDI_NAMESPACE::SerialMIDI serial##Name(SerialPort); \
    MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI> Name((MIDI_NAMESPACE::SerialMIDI &)serial##Name);

/*! \brief Create an instance of the library attached to a serial port with
custom settings.
@see DefaultSettings
@see MIDI_CREATE_INSTANCE
*/
#define MIDI_CREATE_CUSTOM_INSTANCE(SerialPort, Name, Settings) \
    MIDI_NAMESPACE::SerialMIDI serial##Name(SerialPort);        \
    MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI, Settings> Name((MIDI_NAMESPACE::SerialMIDI &)serial##Name);

#define MIDI_CREATE_DEFAULT_INSTANCE() MIDI_CREATE_INSTANCE(uart0, uartMidi);