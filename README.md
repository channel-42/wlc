# wlc - wled cli 
A simple commandline tool for controlling a wled host. 

**currently a WIP**

## Capabilities
wlc can control common settings of your wled device, such as:

- brightness
- hue and saturation
- effect type, speed and intensity
- color palette

These settings can also be only applied to a specified LED-segment.

## Configuration
Configuration is done through editing `config.h`, located in the `include` directory. The default IP address will most likely have to be edited to match your host's address.

## Installation
Clone this repo to your system and navigate into it's directory. Then: 
```
make && make install
```
This will install wlc into `~/.local/bin/`. Thus, make sure the directory is in your `$PATH` to be able to execute the programme in your shell. 

To install it globally, move or copy the binary to `/usr/bin/`. 
