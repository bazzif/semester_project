## Seeed XIAO BLE Code
The code for the Seeed XIAO BLE can be found in firmware it contains
- Calibration : Calculates the average of the DFT coefficients and visualizes them. Used to calibrate the ring for the carrier
- BLEMouseController : Makes connection to a second device and receives a pinch state and executes a mouse click on the computer 
- BLEDataSendToPeripheral : Make connection to a second device and detectes a pinch and send it
## Python Code 
A Python script to visualize measurements by recording the serial output of the Seeed XIAO BLE with Putty. There are 3 scripts
- Pinch_measurment.py : Visualization of the FFT coefficients for pinch and no pinch
- SNR.py : Visualization of the SNR for all three test subjects as a box plot
- Latency_measurement.py: Visualization of the Latency as a box plot
This folder also contains all the measurements that were performed with the electroring
## CAD
Contains all CAD files as STEP files and STL files to make the ring. There is also an additional gcode file that can be used directly to make a 3d print with the Ultimaker2+
## PCB
All flies to creat the PCB and generate a Part list as well as the schemas that are used to correctly assign the components to the PCB


