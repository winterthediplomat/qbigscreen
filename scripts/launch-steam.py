#!/usr/bin/env python
# This Python file uses the following encoding: utf-8

import subprocess
import argparse
import sys
import os
import psutil

def start_qjoypad_with_profile(profile="none"):
    try:
        subprocess.Popen(["/opt/qbigscreen/start-antimicrox.py", profile])
    except FileNotFoundError:
        subprocess.Popen([os.path.expanduser("~/.config/qbigscreen/scripts/start-antimicrox.py"), profile])

def create_lockfile(profile):
    try:
        subprocess.Popen(["/opt/qbigscreen/create-lockfile.sh", profile])
    except FileNotFoundError:
        subprocess.Popen([os.path.expanduser("~/.config/qbigscreen/scripts/create-lockfile.sh"), profile])

def delete_lockfile():
    try:
        subprocess.Popen(["/opt/qbigscreen/delete-lockfile.sh"])
    except FileNotFoundError:
        subprocess.Popen([os.path.expanduser("~/.config/qbigscreen/scripts/delete-lockfile.sh")])


def main(args):
    start_qjoypad_with_profile("none")

    p = subprocess.Popen("steam")
    create_lockfile("steam")
    returncode = p.wait()

    start_qjoypad_with_profile("none")
    delete_lockfile()

    sys.exit(100+returncode)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    # parser.add_argument("url")
    # parser.add_argument("prefsize")

    main(parser.parse_args())
