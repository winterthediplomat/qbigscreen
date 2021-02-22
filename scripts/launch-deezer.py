#!/usr/bin/env python
# This Python file uses the following encoding: utf-8

import subprocess
import argparse
import sys
import os
import psutil

def start_qjoypad_with_profile(profile=None):
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

    if profile:
        qjoypad_p = subprocess.Popen("qjoypad {}".format(profile).split(" "))

def main(args):
    start_qjoypad_with_profile("mouse")

    # start mpv
    p = subprocess.Popen("deezer")
    returncode = p.wait()

    # start_qjoypad_with_profile("mouse")

    sys.exit(100+returncode)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    # parser.add_argument("url")
    # parser.add_argument("prefsize")

    main(parser.parse_args())
