# Gstreamer AR Drone 2.0

This assumes you already installed ardrone_gstreamer. If not refer to its [README](https://github.com/CatarinaSilva/ARDrone2.0/tree/master/ARDrone2_Gstreamer).

##Testing

Testing from inside Matlab:
* Open Matlab and go into the **ardrone_gstreamer_test** folder
* Run **Startup_Script.m**
* Build, Connect and Run the dummy model inside this folder, **runVideo.slx**
* Check if you are receiving the video in your computer:
	* [With Gstreamer] Open a command line and run the gstreamer line described in **GstreamerWindows10.txt* 
	* [With openCV inside Matlab] Run the matlab script **receiveVideo.m** (to be uploaded)
		
You should be able to see the video in a window created by gstreamer in your host machine (this should happen already after build, but if you also connect and run you should see the LEDs turn green and this would confirm that the code was deployed correctly)



##Debug

If you can't see the video check for:

##### **video_script.sh** missing

Add the file **video_script.sh** to the update folder using FTP:
* Connect with FTP protocol with WinSCP to the drone through port 5551 (this port refers to the /update folder) 
* Upload **video_script.sh**
* Close session
	

	
