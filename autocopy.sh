#!/bin/bash

make clean
make
sudo dd if=kern.elf of=/dev/mmcblk0p2 
sudo dd if=mbr.bin of=/dev/mmcblk0 count=446
sudo dd if=shell.bin of=/dev/mmcblk0p2 bs=1k seek=5000
sync
