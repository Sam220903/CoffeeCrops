# CoffeeCrops
Project for participation in LACCEI Hackaton 2024

For testing the ARDUINO code, you must have the required hardware components and conections, shown in the included diagram.
The MATLAB and the ARDUINO code must be run separately, you have to follow the following steps to run the complete project:
* Count with a hardware system similar to the one shown on the includen diagram
* Create a channel in ThingSpeak with at least 3 fields, naming them as TEMPERATURE, HUMIDITY and LIGHT
* Upload the ARDUINO Code to an Arduino UNO card
(At this point, your channel must have started recording data)
* Go to Apps > MATLAB Analysis
* Create a blank code
* Copy and paste the text in the .mat file
* Select the "Save" option
* Start monitoring the recorded data and sent alerts
