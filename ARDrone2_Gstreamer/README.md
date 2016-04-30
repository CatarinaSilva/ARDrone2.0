ARDrone2 gstreamer
==================

The folder contains arm-libraries for ardrone gstreamer, obtained from 
https://github.com/tudelft/ardrone2_gstreamer, under a GNU general public license 


## Description



## Installation

1.	Connect with FTP protocol with WinSCP to the drone through port 21
(this port refers to the /data/video folder)
![FTP connection to drone 21](https://github.com/CatarinaSilva/ARDrone2.0/blob/master/ARDrone2_Gstreamer/images/FTPconnection21.png)


2.	Upload the archive **arm_light.tgz**
     *IMPORTANT:	If you have a bunch of files with names of form **pic_000124477966_w640_h360.y** you may not have space for installing. Delete them (if you want to keep them download them to your computer but keep in mind they are useless)
![FTP connection to drone](https://github.com/CatarinaSilva/ARDrone2.0/blob/master/ARDrone2_Gstreamer/uploading.png)

3.	Close the session


4.	Telnet to the drone from a command line (this is done using matlab):
	* `"C:\Program Files\MATLAB\R2014a\toolbox\idelink\foundation\hostapps\plink.exe" -telnet -P 23 192.168.1.1`
	* NOTE: if you choose to use some other telnet version it is OK, the important part is basically the port `-P 23` and the IP `192.168.1.1`
	You should now be inside the drone and see the following
![Drone terminal](https://github.com/CatarinaSilva/ARDrone2.0/blob/master/ARDrone2_Gstreamer/images/droneTerminal.png)

5. Run the following commands inside the drone:
	* `cd /data/video`
	* `tar -xzf arm_light.tgz`
	* `rm -rf /data/video/arm_light.tgz`
	* `ls –l | grep drw`

You should have the following output, with opt folder:
![Gstreamer upacked](https://github.com/CatarinaSilva/ARDrone2.0/blob/master/ARDrone2_Gstreamer/images/gstreamerUnpack.png)

6. Place **basescript.sh** inside the drone's **bin** folder: it is a file that initializes the DSP “module” and mounts the necessary directories if not previously mounted
	* Connect with FTP protocol with WinSCP to the drone through port 5551 (this port refers to the /update folder) and upload **basescript.sh**, then close the session
![FTP connection to drone 5551](https://github.com/CatarinaSilva/ARDrone2.0/blob/master/ARDrone2_Gstreamer/images/FTPconnection5551.png)

	

7.	Telnet to the drone again from a command line:
	* `"C:\Program Files\MATLAB\R2014a\toolbox\idelink\foundation\hostapps\plink.exe" -telnet -P 23 192.168.1.1`

8. Run the following commands:
	* `cd /update`
	* `ls –l | grep ‘\-rw’`
	(you should see a list containing **basescript.sh**)
	* `mv basescript.sh /bin/basescript.sh`
	* `cd /bin`
	* `chmod 777 basescript.sh`
	* `ls –l | grep ‘\-rw’`
![Bin list](https://github.com/CatarinaSilva/ARDrone2.0/blob/master/ARDrone2_Gstreamer/images/listBin.png)

9. Try to run **basescript.sh**:
	*`./basescript.sh`
![Bin list](https://github.com/CatarinaSilva/ARDrone2.0/blob/master/ARDrone2_Gstreamer/images/basescript



## Change AR Drone Target to use the gstreamer video
1. Substitute **ssh_download.bat** inside AR_Drone_Target by **ssh_download_new.bat** (keep the other one somewhere for safekeeping) and change its name to **ssh_download.bat**
2. Add the file **video_script.sh** to the update folder using FTP:
	* Connect with FTP protocol with WinSCP to the drone through port 5551 (this port refers to the /update folder) and upload **video_script.sh**, then close the session

	
	
## Gstreamer installation on Windows 10 (for eventual testing)

* Go to  https://gstreamer.freedesktop.org/data/pkg/windows/1.7.91/ 
* Download gstreamer-1.0-x86_64-1.7.91.msi 
* Install Gstreamer 1.0 on your computer, with a custom installation where you install all plugins. 
