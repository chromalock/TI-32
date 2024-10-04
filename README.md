# TI-32

A mod for TI-84 calculators with a link port (I/O) to give them internet access and add other features, like test mode breakout and camera support

![built pcb](./pcb/built.png)

## Fork differences

- Google api option for server

## Features to be Added

- Change Wi-Fi settings directly from calculator
- Watchdog when receiving items
- Support for color images
- Action text during waiting phase
- Support for multi-page response from GPT
- Support for chat history from GPT
- Support for bigger menu (320x240 resolution only)
- Support for lowercase text
- Documentation
- Basic Web Browsing
- HTTPS Encryption
- Email Send and Read
- Discord Access
- Get local weather
- Control computer wirelessly
- QR Code & Barcode scanner
- Video player

## Bug Fixes

- GPT Menu closes immediately when receiving response
- App transfer fails

## Known Issues

- Images don't tranfer: if you get this problem, one cause is the esp32 not having enough memory to store the picture or the picture is too large.

## Documentation

Read the DOCUMENTATION.md file

## Video
[![YouTube](http://i.ytimg.com/vi/Bicjxl4EcJg/hqdefault.jpg)](https://www.youtube.com/watch?v=Bicjxl4EcJg)
