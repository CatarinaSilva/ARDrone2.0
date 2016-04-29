# ARDrone2.0

The goal of the project wwas to implement vision based control on a low cost quadrotor, namely Parrot 2.0, to tackle indoor localization problems and test them with the prepared platform.


## Installation

I am running Windows 10 on my computer, and this repository even included a fix for AR_Drone_Target because of this. This combination with the rest of the instaltion works. To be able to run the many parts of the program you need to install the following:

* Simple attitude and altitude control:
  * Install Matlab R2014b (preferably with toolboxes)
  * Install AR Drone Target -> read the README file on [AR Drone Target](https://github.com/CatarinaSilva/ARDrone2.0/blob/master/Parrot_mathworks/README.md)
* Video based control:
  * Install video library blocks ->  read the README file on [AR Drone Target](https://github.com/CatarinaSilva/ARDrone2.0/blob/master/Parrot_mathworks/README.md) (optional, only if you want to test these blocks, since the performance is worse than Gstremer)
  * Install gstreamer on the quadcopter -> read the README file on [ARDrone2_Gstreamer](https://github.com/CatarinaSilva/ARDrone2.0/blob/master/ARDrone2_Gstreamer/README.md)
  * Install [Gstreamer > 1.0](https://gstreamer.freedesktop.org/download/) on your computer (optional - however, it's good for testing purposes)
  * Install [openCV 3.0](http://opencv.org/opencv-3-0.html) on your computer
  * Install [mexopencv](https://github.com/kyamagu/mexopencv) for Matlab
  * Install [WinSCP](https://winscp.net/eng/download.php)
 
