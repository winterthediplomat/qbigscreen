# This Python file uses the following encoding: utf-8

import subprocess
import psutil

def unused_callback(i):
    pass

def main():
    maybe_p = list(filter(lambda p: "qbigscreen" in p.name(), psutil.process_iter()))
    if len(maybe_p):
        maybe_p = maybe_p[0]
        maybe_p.terminate()
        gone, alive = psutil.wait_procs([maybe_p], timeout=3, callback=unused_callback)
        if maybe_p in alive:
            maybe_p.kill()

    subprocess.Popen("/usr/bin/qbigscreen")

if __name__ == "__main__":
    main()
