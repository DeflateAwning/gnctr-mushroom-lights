# gnctr-mushroom-lights
A quick-and-dirty flashing lights repo for the 2023 GNCTR flashing mushroom sculpture

## Features
* 5 different good modes (and a few that didn't work so well)
* Manually-implemented PWM for brightness control on low-refresh-rate/non-linear response solid state relays
* Some modes use RNG for max effect (like the lightning mode)
* Auto cycling through modes
* Manual cycling through modes by button press

The `loop_lightning()` function is a fantastic reference for a reasonably-convincing stormy lightning simulator. 
