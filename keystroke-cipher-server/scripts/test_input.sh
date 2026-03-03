#!/bin/bash
# test_input.sh - simulate keystroke input without a physical keyboard
# use this during development before hid_intercept.c is wired up

MESSAGE=${1:-"hello world"}
echo "[*] Simulating input: '$MESSAGE'"
echo "$MESSAGE" > /dev/keycipher_out
echo "[✓] Written to /dev/keycipher_out - check /proc/keycipher/stats"
