#!/bin/bash
# fsuae-test.sh MODEL ROM ADF HOSTDIR SECONDS SHOT.png
# Boots ADF on an emulated Amiga with HOSTDIR mounted as DH1:, waits, and
# takes a screenshot. Programs write results to DH1:.
set -e
MODEL=$1 ROM=$2 ADF=$3 HOST=$4 SECS=${5:-40} SHOT=${6:-shot.png}
CFG=$(mktemp --suffix=.fs-uae)
cat > $CFG <<CFGEOF
[fs-uae]
amiga_model = $MODEL
kickstart_file = $ROM
floppy_drive_0 = $ADF
hard_drive_1 = $HOST
hard_drive_1_label = DH1
floppy_drive_speed = 0
automatic_input_grab = 0
fullscreen = 0
window_width = 720
window_height = 568
audio_driver = null
video_sync = 0
warp_mode = 1
uae_cpu_speed = max
CFGEOF
export DISPLAY=:77
Xvfb :77 -screen 0 800x600x24 >/dev/null 2>&1 &
XPID=$!
sleep 1
HOME=$(mktemp -d) fs-uae $CFG >/tmp/fsuae.log 2>&1 &
FPID=$!
sleep $SECS
import -window root $SHOT || true
kill $FPID 2>/dev/null; sleep 1; kill -9 $FPID 2>/dev/null || true
kill $XPID 2>/dev/null || true
rm -f $CFG
