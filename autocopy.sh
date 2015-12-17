#!/bin/bash

make clean
make
sudo dd if=kern.elf of=/dev/mmcblk0p2 
sudo dd if=mbr.bin of=/dev/mmcblk0 
sync
