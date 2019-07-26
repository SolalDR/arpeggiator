# arpeggiator
Arduino program to build an arpegiator

## Installation

Install `arduino-cli`<br>
https://github.com/arduino/arduino-cli

Install `emulino`<br>
https://github.com/juliendargelos/emulino

## Development

In `src/config.h` make sure macro `DEBUG` is set to `false` 

In project directory run 
```
$ ./build.sh
```

Maybe you should make the file executable
```
$ chmod +x build.sh
```


## Production

To run this program in a real arduino as a MIDI device, you need to enable the DFU.<br>
https://www.arduino.cc/en/Hacking/DFUProgramming8U2

And flash the arduino using this project.<br>
https://github.com/kuwatay/mocolufa

