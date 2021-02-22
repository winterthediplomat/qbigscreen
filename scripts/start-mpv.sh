#!/usr/bin/env bash
echo "start-mpv.sh" $@
mpv --window-maximized=yes $1
exit $?
