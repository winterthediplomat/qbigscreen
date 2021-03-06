#!/usr/bin/env bash
instant=$(date +%s)
echo "$1" > "/tmp/qbigscreen-lockfile-${instant}"
