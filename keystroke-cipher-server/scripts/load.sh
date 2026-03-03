#!/bin/bash
# load.sh - insert the kernel module and set up device files

set -e

echo "[*] Building kernel module..."
make -C ../kernel

echo "[*] Loading module..."
sudo insmod ../kernel/keycipher.ko

echo "[*] Creating device files..."
sudo mknod /dev/keycipher_in  c $(grep keycipher_in  /proc/devices | awk '{print $1}') 0
sudo mknod /dev/keycipher_out c $(grep keycipher_out /proc/devices | awk '{print $1}') 0

echo "[*] Setting permissions..."
sudo chmod 666 /dev/keycipher_in
sudo chmod 666 /dev/keycipher_out

echo "[*] Starting userspace daemon..."
../userspace/keycipher_daemon &

echo "[✓] KeyCipher loaded. Check dmesg for kernel logs."
echo "[✓] Stats: cat /proc/keycipher/stats"
