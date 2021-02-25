#!/usr/bin/env python
# This Python file uses the following encoding: utf-8

import argparse
import os
import subprocess
import psutil

PROGRAM_PATH="/usr/bin/antimicrox"
GLOBAL_LAYOUT_FPATH = "/opt/qbigscreen/{}.gamecontroller.amgp"
LOCAL_LAYOUT_FPATH = "~/.config/qbigscreen/layouts/{}.gamecontroller.amgp"

def profile_path(name):
    if os.path.exists(GLOBAL_LAYOUT_FPATH.format(name)):
        return GLOBAL_LAYOUT_FPATH.format(name)
    if os.path.exists(os.path.expanduser(LOCAL_LAYOUT_FPATH.format(name))):
        return LOCAL_LAYOUT_FPATH.format(name)
    raise ValueError("gamepad profile not found")

def unused_callback(i):
    pass

def main(args):
    if not os.path.exists(PROGRAM_PATH):
        raise ValueError("antimicrox is not installed in the default location")

    # kill pre-existing
    maybe_p = list(filter(lambda p: "antimicrox" in p.name(), psutil.process_iter()))
    if len(maybe_p):
        maybe_p = maybe_p[0]
        maybe_p.terminate()
        gone, alive = psutil.wait_procs([maybe_p], timeout=3, callback=unused_callback)
        if maybe_p in alive:
            maybe_p.kill()

    # start new
    if args.profile != "none":
        executed = "{} --hidden --profile {}".format(
            PROGRAM_PATH, profile_path(args.profile)).split(" ")
        print("[start-antimicrox] executed = ", executed)
        subprocess.Popen(executed)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("profile")

    main(parser.parse_args())
