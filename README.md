# ArdLink

*v0.1*

## What?
ArdLink is a runtime for Arduino IDE compatible microcontrollers and was created to allow:
- Easy access to microcontroller resources (via serial) such as digital IO from an external host. 
- A common bytecode that can be executed across different microcontrollers. Code can be executed directly from a host or from the microcontroller EEPROM.
- Direct interchangeability of similar microcontrollers in larger projects.


## Why?

I like working on hobby electronics, and as a result I have a few (to many) microcontrollers hanging around. Because of the variety of devices I had at hand, I wanted some common method of using them without any particular dependence on a build environment. Additionally, a significant portion of my application with these devices comes down to utilizing them for GPIO as controlled by a host computer. ArdLink is still in it's infancy, but I hope to allow more functionality to permit communication in standard protocols such as 1-Wire, I^2^C, etc as well as simplified programming from Python.


## How?

### Requirements

#### Python (requirements.txt)

```
pytest>=5.3.5
path.py>=12.5.0
pyserial>=3.5
```

Testing requires the use of the following packages:
- [PicSimLab](https://lcgamboa.github.io/)>=0.8.9
- [Arduino IDE](https://www.arduino.cc/en/software)>=1.8.19
- [com0com](https://sourceforge.net/projects/com0com/files/com0com/3.0.0.0/com0com-3.0.0.0-i386-and-x64-signed.zip/downloade)>=3.0.0.0

