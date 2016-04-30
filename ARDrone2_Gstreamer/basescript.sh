echo Video Initialization

mkdir -p /opt/arm
mkdir -p /lib/dsp


mount --bind /data/video/opt /opt
mount --bind /data/video/opt/arm /opt/arm
mount --bind /data/video/opt/arm/lib/dsp /lib/dsp

export PATH=/opt/arm/gst/bin:$PATH
export DSP_PATH=/opt/arm/tidsp-binaries-23.i3.8/

/bin/dspbridge/cexec.out -T /opt/arm/tidsp-binaries-23.i3.8/baseimage.dof -v
/bin/dspbridge/dynreg.out -r /opt/arm/tidsp-binaries-23.i3.8/m4venc_sn.dll64P -v



