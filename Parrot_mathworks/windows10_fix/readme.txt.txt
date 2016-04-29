
##### Uploading the compiled code automatically to the drone using Windows 10


In the past, we experienced some problems with uploading the files to the drone using Windows 10. We found a solution to this using WinSCP.


For this, you need to perform three steps:

1. Install the WinSCP ftp client (https://winscp.net/eng/download.php, or use "winscp575setup.exe").
2. Overwrite the "ssh_download.bat" file in the AR_Drone root directory. Be sure to make a copy of the original!!
3. Check if "WinSCP.com" is installed in the "C:\Program Files (x86)\WinSCP\" directory, else edit the "ssh_download.bat" file to point towards the correct directory (Line 41).

Now the code will be uploaded via WinSCP instead of the problematic Windows FTP program.


If you have questions, email Pepijn (p.b.cox@tue.nl)