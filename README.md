# slate-tr1-i2c-slaves
This repository contains the default code for each I2C slave on the I2C bus on the Slate TR1 robot. These controllers enable decoupled communication between the main computer (NVIDIA Jetson TK1) and the microcontrollers (Elegoo Nano V3.0) that communicate directly with the motor controllers (L298n, A4988).

This repository is open-source and features an MIT license. You may modify the code running on the microcontrollers to better suit your needs, but be careful as you may accidently alter dependencies--such as I2C addresses.

Happy hacking!

Team @ Slate Robotics
