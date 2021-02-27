#!/usr/bin/env python
# This Python file uses the following encoding: utf-8

import subprocess

def start_qjoypad_with_profile(profile="none"):
    try:
        return subprocess.Popen(["/opt/qbigscreen/start-antimicrox.py", profile])
    except FileNotFoundError:
        return subprocess.Popen([os.path.expanduser("~/.config/qbigscreen/scripts/start-antimicrox.py"), profile])

def main():
    proc = start_qjoypad_with_profile("mouse")
    proc.wait()

if __name__ == "__main__":
    main()
