#!/usr/bin/env python
# This Python file uses the following encoding: utf-8

import subprocess
import psutil
import os
import datetime

def find_process(process_name):
    maybe_process = [p for p in psutil.process_iter() if process_name in p.name()]
    if maybe_process:
        return maybe_process[0]
    else:
        return None


def find_lockfile(pattern):
    walkres = [(path, dirs, files) for (path, dirs, files) in os.walk("/tmp") if any(filter(lambda fname: pattern in fname, files))]
    if walkres:
        # assume only one "qbigscreen-lockfile-(timestamp)" file
        (_, _, files) = walkres[0]
        fname = next(filter(lambda fn: pattern in fn, files))
        return fname
    else:
        return None

def process_name(fpath):
    with open(fpath) as src:
        return src.read().strip()

def create_visible_lockfile():
    now = (datetime.datetime.now().timestamp())
    with open("/tmp/qbigscreen-visible-"+now, "w") as dst:
        dst.write(str(now))

def delete_visible_lockfile(fpath):
    os.remove(fpath)

def main():

    p = find_process("qbigscreen")

    # qbigscreen is not running -> start qbigscreen
    if not p:
        subprocess.Popen(["/usr/local/bin/qbigscreen"])
        return

    # qbigscreen is running
    app_lockfile = find_lockfile("qbigscreen-lockfile")
    visible_lockfile = find_lockfile("qbigscreen-visible")

    # if qbigscreen is not visible, show qbigscreen
    if not visible_lockfile:
        create_visible_lockfile()
        subprocess.call(["/opt/qbigscreen/activate-window-of-current-instance.sh", str(p.pid)])
        return

    # if qbigscreen is visible, switch to the app if present
    if app_lockfile:
        app_name = process_name(app_lockfile)
        app_p = find_process(app_name)
        subprocess.call(["/opt/qbigscreen/activate-window-of-current-instance.sh", str(app_p.pid)])
        delete_visible_lockfile(visible_lockfile)
        return


if __name__ == "__main__":
    main()
