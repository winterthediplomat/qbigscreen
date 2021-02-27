#!/usr/bin/env python
# This Python file uses the following encoding: utf-8

import subprocess
import psutil

def main():
    maybe_qbigscreen = [p for p in psutil.process_iter() if "qbigscreen" in p.name()]
    if maybe_qbigscreen:
        p = maybe_qbigscreen[0]
        subprocess.call(["/opt/qbigscreen/activate-window-of-current-instance.sh", str(p.pid)])
    else:
        subprocess.Popen(["/usr/local/bin/qbigscreen"])

if __name__ == "__main__":
    main()
