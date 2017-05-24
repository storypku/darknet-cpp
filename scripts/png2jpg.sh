#!/bin/bash
# Usage: copy this shell towards your data folder and execute
# Goal: will clone all the png files in your folder and make a jpg version of them
# Needed for detector RECALL processing
ls -1 *.png | xargs -n 1 bash -c 'convert "$0" "${0%.png}.jpg"'
