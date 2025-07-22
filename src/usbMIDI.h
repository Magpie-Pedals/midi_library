/*
 Copyright (c) 2020 lathoub

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#pragma once

#include "MIDI.h"
// #include <MIDIUSB.h>
#include "bsp/board.h"
#include "tusb.h"

#include <vector>

BEGIN_MIDI_NAMESPACE

// typedef struct
// {
//     uint8_t header;
//     uint8_t byte1;
//     uint8_t byte2;
//     uint8_t byte3;
// } midiEventPacket_t;

class usbMidiTransport
{
private:
    // byte mTxBuffer[4];
    // size_t mTxIndex;
    // MidiType mTxStatus;

    // byte mRxBuffer[4];
    // size_t mRxLength;
    // size_t mRxIndex;

    // uint8_t mPacket[4];
    uint8_t cableNumber;
    std::vector<uint8_t> txBuf;

public:
    usbMidiTransport(uint8_t cableNumber = 0)
    {
        this->cableNumber = cableNumber;
    };

public:
    static const bool thruActivated = false;

    void begin()
    {
        tud_init(BOARD_TUD_RHPORT);
    };

    bool beginTransmission(MidiType status) // not implemented still
    {
        txBuf.clear(); // clear the buffer before we start collecting messages
        return true;
    }

    void endTransmission()
    {
        tud_midi_stream_write(this->cableNumber, &txBuf[0], txBuf.size()); // send the whole thing in one midi packet
    }

    void end()
    {
    }

    void write(byte b)
    {
        txBuf.push_back(b); // write the byte to the transmit buffer
    };

    byte read()
    {
        uint8_t data;
        return (tud_midi_stream_read(&data, 1) ? data : (-1));
    };

    unsigned available()
    {
        return tud_midi_available();
    };
};

END_MIDI_NAMESPACE

/*! \brief
 */

#define USBMIDI_TYPE MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::usbMidiTransport>

#define MIDI_CREATE_INSTANCE(CableNr, Name)                \
    MIDI_NAMESPACE::usbMidiTransport __usb##Name(CableNr); \
    MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::usbMidiTransport> Name((MIDI_NAMESPACE::usbMidiTransport &)__usb##Name);

#define USBMIDI_CREATE_CUSTOM_INSTANCE(CableNr, Name, Settings) \
    USBMIDI_NAMESPACE::usbMidiTransport __usb##Name(CableNr);   \
    MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport, Settings> Name((MIDI_NAMESPACE::usbMidiTransport &)__usb##Name);

#define USBMIDI_CREATE_DEFAULT_INSTANCE() \
    USBMIDI_CREATE_INSTANCE(0, usbMIDI)