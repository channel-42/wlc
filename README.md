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

## Use case examples

### setting a profile at system startup
This can be done by calling wlc from `.xinitrc`
```
# ...
wlc <flags and values> &
# ...
```

### synchronize LEDs with daylight
This can be done by running a hourly cronjob from wlc with the `-t` flag. wlc will match the light temperature to the system's time.

## How it works
wlc controls the wled host through the wled http api by making GET requests. The official wled wiki page on this [topic](https://github.com/Aircoookie/WLED/wiki/HTTP-request-API) explains the inner-workings in greater detail.
