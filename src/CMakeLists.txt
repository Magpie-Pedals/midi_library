add_library(midi STATIC
    midi_Namespace.h
    midi_Defs.h
    midi_Message.h
    midi_Platform.h
    midi_Settings.h
    MIDI.cpp
    MIDI.hpp
    MIDI.h
    serialMIDI.h
    usbMIDI.h
)

target_link_libraries(midi
    PUBLIC
    pico_stdlib
    hardware_uart
    hardware_gpio
    tinyusb_device
    tinyusb_board
)
