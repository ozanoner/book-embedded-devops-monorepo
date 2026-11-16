#!/usr/bin/env python3
"""Build the matrix for the manual vault build.

Reads tools/vault-matrix.json, filters by the CHAPTERS and TARGETS environment variables,
and writes a GitHub Actions matrix to GITHUB_OUTPUT.

CHAPTERS is "all" or a comma separated list such as "ch02,ch04".
TARGETS is a comma separated list such as "esp32,esp32c3".
"""
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
MANIFEST = os.path.join(HERE, "vault-matrix.json")


def split(value):
    return [x.strip() for x in value.split(",") if x.strip()]


chapters = split(os.environ.get("CHAPTERS", "all"))
targets = split(os.environ.get("TARGETS", "esp32,esp32c3"))

manifest = json.load(open(MANIFEST))
include = []
skipped = []
for entry in manifest:
    if "all" not in chapters and entry["chapter"] not in chapters:
        skipped.append(entry["chapter"])
        continue
    for target in entry["targets"]:
        if target not in targets:
            continue
        include.append({"chapter": entry["chapter"], "app": entry["app"], "target": target})

out = json.dumps({"include": include})
print("selected %d builds, skipped %s" % (len(include), skipped or "none"), file=sys.stderr)
path = os.environ.get("GITHUB_OUTPUT")
if path:
    with open(path, "a") as f:
        f.write("matrix=%s\n" % out)
        f.write("count=%d\n" % len(include))
else:
    print(out)
