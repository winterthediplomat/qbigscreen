#!/usr/bin/env python
# This Python file uses the following encoding: utf-8

import subprocess
import argparse
import streamlink
from pprint import pprint
import sys
import os

def main(args):
    s = streamlink.Streamlink()
    streams = s.streams(args.url)
    available_quality = list(streams.keys())
    quality_to_request = "best" if args.prefsize not in available_quality else args.prefsize

    mpv_script_to_use = "/opt/qbigscreen/start-mpv.sh"
    if not os.path.exists(mpv_script_to_use):
        mpv_script_to_use = os.path.expanduser("~/.config/qbigscreen/scripts/start-mpv.sh")

    p = subprocess.Popen("streamlink --player {} {} {}".format(
        mpv_script_to_use,
        args.url,
        quality_to_request).split(" "))
    returncode = p.wait()
    sys.exit(returncode)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("url")
    parser.add_argument("prefsize")

    main(parser.parse_args())
