#!/bin/bash
# unload.sh - cleanly remove the kernel module

echo "[*] Stopping userspace daemon..."
pkill -f keycipher_daemon || true

echo "[*] Removing device files..."
sudo rm -f /dev/keycipher_in /dev/keycipher_out

echo "[*] Unloading module..."
sudo rmmod keycipher

echo "[✓] KeyCipher unloaded."
