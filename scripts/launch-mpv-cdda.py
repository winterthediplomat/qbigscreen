#!/usr/bin/env python
# This Python file uses the following encoding: utf-8

import subprocess
import argparse
import streamlink
from pprint import pprint
import sys
import os
import psutil

def start_qjoypad_with_profile(profile="none"):
    try:
        subprocess.Popen(["/opt/qbigscreen/start-antimicrox.py", profile])
    except FileNotFoundError:
        subprocess.Popen([os.path.expanduser("~/.config/qbigscreen/scripts/start-antimicrox.py"), profile])


def main(args):
    start_qjoypad_with_profile("vlc")

    p = subprocess.Popen("vlc cdda:///dev/sr0".split(" "))
    returncode = p.wait()

    start_qjoypad_with_profile("none")

    sys.exit(100+returncode)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    main(parser.parse_args())
