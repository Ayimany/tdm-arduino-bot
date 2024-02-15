#!/usr/bin/bash

platformio device monitor -p /dev/ttyACM* -b 9600   \
    -f direct                                       \
    --raw                                           \
    -f send_on_enter                                \
    -f colorize                                     \
    -f nocontrol                                    \
    --echo
