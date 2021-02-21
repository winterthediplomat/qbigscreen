#!/usr/bin/env python
# This Python file uses the following encoding: utf-8

import subprocess
import argparse
import streamlink
from pprint import pprint
import sys
import os
import psutil

def start_qjoypad_with_profile(profile):
    if not os.path.exists("/usr/bin/qjoypad"):
        return

    def unused_callback(proc):
        pass

    maybe_p = list(filter(lambda p: "qjoypad" in p.name(), psutil.process_iter()))
    if len(maybe_p):
        maybe_p = maybe_p[0]
        maybe_p.terminate()
        gone, alive = psutil.wait_procs([maybe_p], timeout=3, callback=unused_callback)
        if maybe_p in alive:
            maybe_p.kill()
    qjoypad_p = subprocess.Popen("qjoypad {}".format(profile))

def main(args):
    s = streamlink.Streamlink()
    streams = s.streams(args.url)

    if not streams:
        sys.exit(1)

    available_quality = list(streams.keys())
    quality_to_request = "best" if args.prefsize not in available_quality else args.prefsize

    # start qjoypad if available_quality
    start_qjoypad_with_profile("mpv")

    # start mpv
    mpv_script_to_use = "/opt/qbigscreen/start-mpv.sh"
    if not os.path.exists(mpv_script_to_use):
        mpv_script_to_use = os.path.expanduser("~/.config/qbigscreen/scripts/start-mpv.sh")

    p = subprocess.Popen("streamlink --player {} {} {}".format(
        mpv_script_to_use,
        args.url,
        quality_to_request).split(" "))
    returncode = p.wait()

    start_qjoypad_with_profile("mouse")

    sys.exit(100+returncode)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("url")
    parser.add_argument("prefsize")

    main(parser.parse_args())
