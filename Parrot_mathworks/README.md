AR Drone 2.0 - Simulink AR Drone Target
=======================================

This is a part of Daren Lee's repository for the AR Drone 2.0, available on
https://github.com/darenlee/SimulinkARDroneTarget, under a Revised BSD License.

It is the starting point for this project on flight control and localization with Parrot's AR Drone 2.0, allowing
code deployment from Matlab to the Drone with the necessary blocks that retrieve navigation data and send actuation
inputs to the drone.

After installing AR_Drone_Target, the blocks are available in Simulink's library browser under **AR Drone 2 Target**

Contents:
+ AR_Drone_Target
+ ~~AR_Drone_Models~~
+ ~~AR_Drone_Simulations~~

Inside **AR_Drone_Target/blocks/videolib** there is a script, **install_videolib.m**, that allows the instalation
of video retrieving and sending blocks, that will be available in Simulink's library browser under **AR Drone Video**


Simulink AR Drone Target
========================

Deploy MATLAB Simulink model automatically to Parrot AR Drone with Embedded Coder C code generation. These set of files contain a Simulink model for navigation and control which will allow the AR Drone 2.0 to hover. WiFi communication between a Simulink model running on a PC can receive telemetry and send commands to the drone for take-off and hover mode.

Tested in MATLAB R2014a and R2014b in Windows 7 64-bit

To get started:

1. install CodeSourcery toolchain
	Register here: http://www.mentor.com/
	Download Sourcery CodeBench Lite for ARM GNU/Linux here: 
	https://sourcery.mentor.com/sgpp/lite/arm/portal/subscription?@template=lite

2. cd in MATLAB into AR_Drone_Target folder and run install_script.m

3. Pick ~\MentorGraphics\Sourcery_CodeBench_Lite_for_ARM_GNU_Linux\bin folder


We would also like to thank:

- Egoitz Martinez, Aimar Cordero Garcia and Asier Urcelay for providing us with the 3D VRML model of the AR Drone. Please contact this group on any questions related to the 3D model of the drone (Contact: egoitzml@gmail.com)

- The Paparazzi community for developing and providing source code for drivers of the AR Drone 2.0.

- Omar Osorio for providing us a CAD model for the AR Drone 2.0 for Sim-Mechanics Import
http://grabcad.com/library/drone-2-0/

NOTE:
If you have issues downloading the compiler, try this link 
https://sourcery.mentor.com/sgpp/lite/arm/portal/package8738/public/arm-none-linux-gnueabi/arm-2011.03-41-arm-none-linux-gnueabi.exe

