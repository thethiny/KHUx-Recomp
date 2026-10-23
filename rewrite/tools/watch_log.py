"""Follow a session log and print the lines worth an event, one at a time (for the Monitor tool).

    python rewrite/tools/watch_log.py out/host/rwtest_X.log [--until REGEX] [--width 140] [--from-start]

Prints screen changes, RW FAULT / RW MISMATCH, crashes and fatal errors, each flushed at once. A shell pipe
(tail -F | grep --line-buffered | cut) holds its output in cut's block buffer, so a watcher built that way never
sends an event. Waits for the file to appear and starts at its end (a log reused by a new run is followed from the
point the new run writes, or from the start when it is truncated). --until ends the watch on a matching line.
"""
import argparse
import os
import re
import sys
import time

DEFAULT = r'^>>> screen|^RW FAULT|^RW MISMATCH|^!! CRASH|^!! FATAL|C\+\+ throw'


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('log')
    ap.add_argument('--match', default=DEFAULT, help='regex of lines to print')
    ap.add_argument('--until', help='stop after a line matching this regex')
    ap.add_argument('--width', type=int, default=140)
    ap.add_argument('--from-start', action='store_true', help='also print the matching lines already in the file')
    a = ap.parse_args()
    match = re.compile(a.match)
    until = re.compile(a.until) if a.until else None

    while not os.path.exists(a.log):
        time.sleep(1)
    f = open(a.log, 'r', encoding='utf-8', errors='replace')
    if not a.from_start:
        f.seek(0, os.SEEK_END)
    buf = ''
    while True:
        chunk = f.read()
        if not chunk:
            try:
                if os.path.getsize(a.log) < f.tell():   # truncated: a new run took the file over
                    f.seek(0)
                    buf = ''
            except OSError:
                pass
            time.sleep(0.5)
            continue
        buf += chunk
        *lines, buf = buf.split('\n')
        for line in lines:
            line = line.rstrip('\r')
            hit_until = until is not None and until.search(line)
            if match.search(line) or hit_until:
                sys.stdout.write(line[:a.width] + '\n')
                sys.stdout.flush()
            if hit_until:
                return


if __name__ == '__main__':
    main()
