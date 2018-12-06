# PoolTV

Local network cam streamer + recorder.

Name - initial use case, to transmit ongoing pool games from garage.

### How to build

Get buildroot - version I used: buildroot-2018.08.3

extract to build dir and go there.

```
make BR2_EXTERNAL=../../buildroot-external-tree pooltv_defconfig
```

BR2_EXTERNAL is path to buildroot-external-tree in this repository.

```
make sender all
```

After this, output stuff is in output/images

### How to install
```
sudo dd if=output/images/sdcard.img of=/dev/mmcblk0 bs=4M status=progress
```
### Rasp camera caps (from gstreamer)
```
video/x-raw, format=(string)YUY2, width=(int)[ 32, 2592, 2 ], height=(int)[ 32, 1944, 2 ], framerate=(fraction)[ 1/1, 90/1 ];
video/x-raw, format=(string)UYVY, width=(int)[ 32, 2592, 2 ], height=(int)[ 32, 1944, 2 ], framerate=(fraction)[ 1/1, 90/1 ];
video/x-raw, format=(string)I420, width=(int)[ 32, 2592, 2 ], height=(int)[ 32, 1944, 2 ], framerate=(fraction)[ 1/1, 90/1 ];
video/x-raw, format=(string)YV12, width=(int)[ 32, 2592, 2 ], height=(int)[ 32, 1944, 2 ], framerate=(fraction)[ 1/1, 90/1 ];

image/jpeg, width=(int)[ 32, 2592, 2 ], height=(int)[ 32, 1944, 2 ], framerate=(fraction)[ 1/1, 90/1 ];
image/jpeg, width=(int)[ 32, 2592, 2 ], height=(int)[ 32, 1944, 2 ], framerate=(fraction)[ 1/1, 90/1 ];

video/x-raw, format=(string)BGRx, width=(int)[ 32, 2592, 2 ], height=(int)[ 32, 1944, 2 ], framerate=(fraction)[ 1/1, 90/1 ];
video/x-raw, format=(string)BGR, width=(int)[ 32, 2592, 2 ], height=(int)[ 32, 1944, 2 ], framerate=(fraction)[ 1/1, 90/1 ];
video/x-raw, format=(string)RGB, width=(int)[ 32, 2592, 2 ], height=(int)[ 32, 1944, 2 ], framerate=(fraction)[ 1/1, 90/1 ];
video/x-raw, format=(string)NV21, width=(int)[ 32, 2592, 2 ], height=(int)[ 32, 1944, 2 ], framerate=(fraction)[ 1/1, 90/1 ];
video/x-raw, format=(string)NV12, width=(int)[ 32, 2592, 2 ], height=(int)[ 32, 1944, 2 ], framerate=(fraction)[ 1/1, 90/1 ];
video/x-raw, format=(string)YVYU, width=(int)[ 32, 2592, 2 ], height=(int)[ 32, 1944, 2 ], framerate=(fraction)[ 1/1, 90/1 ];

video/x-h264, stream-format=(string)byte-stream, alignment=(string)au, width=(int)[ 32, 2592, 2 ], height=(int)[ 32, 1944, 2 ], framerate=(fraction)[ 1/1, 90/1 ]
```
### Raspivid based pipeline
```
raspivid -b 25000000 -o - -t 0 -fps 30 -g 5 -pf high -ih -qp 28 -fl | gst-launch-1.0 rtpbin name=rtpbin do-retransmission=true rtp-profile=avpf fdsrc ! h264parse ! rtph264pay ! rtpbin.send_rtp_sink_0 rtpbin.send_rtp_src_0 ! udpsink host=224.100.100.100 port=4000 multicast-iface=eth0 rtpbin.send_rtcp_src_0 ! udpsink name=sink_rtcp host=224.100.100.100 port=5001 multicast-iface=eth0 sync=false async=false udpsrc port=5002 address=224.100.100.100 multicast-iface=eth0 ! rtpbin.recv_rtcp_sink_0
```
### To receive
```
./gst-launch-1.0 rtpbin rtp-profile=avpf name=rtpbin udpsrc multicast-iface=enx000cf6bee4db port=4000 address=224.100.100.100 caps="application/x-rtp,media=video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96" ! rtpbin.recv_rtp_sink_0 rtpbin. ! rtph264depay ! h264parse ! vaapih264dec ! vaapisink udpsrc port=5001 address=224.100.100.100 multicast-iface=enx000cf6bee4db ! rtpbin.recv_rtcp_sink_0 rtpbin.send_rtcp_src_0 ! udpsink host=224.100.100.100 port=5002 multicast-iface=enx000cf6bee4db sync=false async=false
```
