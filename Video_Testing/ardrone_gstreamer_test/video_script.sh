export PATH=/opt/arm/gst/bin:$PATH
export DSP_PATH=/lib/dsp
gst-launch -v v4l2src device=/dev/video1 queue-size=2 always-copy=false !  videoscale ! video/x-raw-yuv, width=640, height=480 ! videorate ! video/x-raw-yuv, framerate=10/1 !dspmp4venc mode=1 ! rtpmp4vpay config-interval=2 mtu=64000 ! udpsink host=192.168.1.2 port=5920 sync=false &