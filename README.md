# JARViE Battery Saver Board
The Battery Saver Board allows your project to get the maximum life out of its battery.  The board provides power on/off control using the on-board push-button and or an external microcontroller (MCU) used in your project. On/off control is achieved by driving two digital signals, /ON and /OFF. Which allows you to automatically turn-off power to your projects after a desired amount of time.  Example, turn off the power to your project if a user does not press any buttons for 5 minutes.  Another example, using a proximity sensor to determine when a user is close and then waking up your project from sleep.

The Battery Saver Board features the MAXIM MAX1614, which is a high-side NMOSFET switch driver.  The module's size makes it very useful in portable battery-powered applications but it can be used with continuous DC power sources or voltage regulators as well. The module allows for push-button on/off control with very low current consumption. Additionally, **the module has an accurate low-battery detector that can be used to,**

- Indicate a low-battery condition 
- Provide an early power-fail warning to the system microprocessor
- Disconnect the battery from the load preventing deep discharge and battery damage

![](https://raw.githubusercontent.com/DudeYarvie/JARViE_Power_Ranger/master/Reference%20Docs/Pics/Tindie_CCA_Annotated.jpg)

# **Features & Specifications**
- Input/output voltage: +5VDC to +26VDC 
- Input/output current: 3A max
- PCB size: 1.6 x 0.9 in. 
- Module signals (ON, OFF, /LBO) can be driven or read using +3.3V and +5V logic devices
- Low power consumption extends the life of battery-powered projects (25µA max Quiescent Current, 6µA max Shutdown Current)
- Reverse current protection
- Adjustable under-voltage lockout that prevents batteries from discharging to unsafe voltage levels
- *System Power Too Low* alert allowing for memory backups and safe power-off housekeeping (/LBO is asserted LOW) 
- MCU control of system power on and shutdown via /ON and /OFF latched input pins (see truth table on schematic and Github)
- Adjustable soft-start output to minimize system in-rush current during power-on
- Attaches to 0.1" spaced solderless and perforated breadboards (e.g. Arduino Proto Shield)

## How to Order
[Purchase Battery Saver Board](https://www.tindie.com/products/jarvie/power-ranger-module/)

<a href="https://www.tindie.com/stores/jarvie/?ref=offsite_badges&utm_source=sellers_JARViE&utm_medium=badges&utm_campaign=badge_large"><img src="https://d2ss6ovg47m0r5.cloudfront.net/badges/tindie-larges.png" alt="I sell on Tindie" width="200" height="104"></a>

# **Technical Resources**
### ON/OFF Truth Table
The following truth table shows the Battery Saver Board control signal logic required to turn on and off power at its output power pin (OUT+):
![Truth_table](https://raw.githubusercontent.com/DudeYarvie/JARViE_Power_Ranger/master/Reference%20Docs/Pics/MAX1614_Truth_Table.png "Truth Table")

### Debouncing Module's Push-Button
**Debouncing of the module's tactile switch must be performed via software.  Reference code may be found in the *manual_shutdown_init* function of *BSP.cpp*.** There is no switch debouncing hardware on the modules because the module push-button is connected directly to the */ON* signal of the MAX1614. The signal must be pulled up to an internal +8V rail when the part receives power at its input.  This process may require that specific timing requirements internal to the device must be met in order for the MAX1614 to turn on properly.  Additionally, the */ON* pin of the MAX1614 is meant to source minimal current; overloading this pin may cause the part to malfunction. Switch debouncing hardware connected to the */ON* signal of MAX1614 may affect critical timing requirements and or overload the device, therefore debouncing hardware was not included.
