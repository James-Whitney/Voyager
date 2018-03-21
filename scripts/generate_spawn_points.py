#!/usr/bin/env python

import json

points = []
for x in range(0, 200, 20):
    for y in range(0, 2000, 20):
        points.append([x, 0, y])

print(json.dumps(points))
