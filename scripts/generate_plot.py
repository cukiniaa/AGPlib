#!/usr/bin/python3
import pylab
import sys

true = ['ann', 'yes']


def rgb_color(rgb_str_arr):
    return map(lambda a: float(a)/255.0, rgb_str_arr)


def read_polygon(line, f):
    n = int(line[0])
    annotate = False if len(line) < 2 else line[1] in true
    polygon = []
    for _ in range(n):
        x, y = map(lambda x: float(x), f.readline().split())
        polygon.append((x, y))
    polygon.append(polygon[0])
    return (polygon, annotate)


def read_point(line, f):
    x, y = float(line[0]), float(line[1])
    r, g, b = rgb_color(line[2:])
    return (x, y, r, g, b)


def read_guard(line, f):
    x, y = map(lambda p: float(p), line)
    return (x, y)


def read_hole(line, f):
    m = int(line[0])
    annotate = False if len(line) < 2 else line[1] in true
    hole = []
    for _ in range(m):
        x, y = map(lambda x: float(x), f.readline().split())
        hole.append((x, y))
    return (hole, annotate)


def read_segment(line, f):
    x1, y1, x2, y2 = map(lambda x: float(x), line)
    return (x1, y1, x2, y2)


def read_region(line, f):
    return read_polygon(line, f)


def draw_polygon(data):
    polygon, annotate = data

    x, y = zip(*polygon)
    pylab.plot(x, y, color='black', linestyle='-', linewidth='2.5', marker='o')
    pylab.fill(x, y, color='grey', alpha=0.1)

    if annotate:
        for xy in polygon:
            pylab.annotate('(%s, %s)' % xy, xy=xy, textcoords='data')


def draw_region(data):
    polygon, annotate = data
    x, y = zip(*polygon)
    pylab.plot(x, y, color='blue', linestyle='-', linewidth='1')
    pylab.fill(x, y, color='blue', alpha=0.3)

    if annotate:
        for xy in polygon:
            pylab.annotate('(%s, %s)' % xy, xy=xy, textcoords='data')


def draw_point(data):
    x, y, r, g, b = data
    pylab.plot(x, y, color=(r, g, b), marker='o', ms='11.0')


def draw_guard(data):
    x, y = data
    pylab.plot(x, y, color='orange', marker='X', ms='30')


def draw_hole(data):
    hole, annotate = data

    x, y = zip(*hole)
    # pylab.fill(x, y, color='black', alpha=0.2)
    pylab.fill(x, y, color='red', alpha=0.85)


def draw_segment(data):
    x1, y1, x2, y2 = data
    pylab.plot([x1, x2], [y1, y2], linestyle='-', color='grey')


def draw(items, drawing_functions, priority):
    for item_type in priority:
        to_draw = [item[1] for item in items if item[0] == item_type]
        for item in to_draw:
            drawing_functions[item_type](item)

    pylab.axis('off')
    pylab.show()


def instructions():

    print("-----------------------------------------------------------")
    print("""This is a script to generate drawings from files for AGPlib.

Use the plot module to generate data and save them in a file.
The following command runs the script:
    python3 generate_plot.py [filename]
Args:
    filename - specifies a path to the file

Following options are available:

    1) Plot a simple polygon
    pol n [ann]
              n - the number of vertices of the polygon
              n points' coordinates separated by space ("x y") are expected after
              ann - optional, if provided points' coordinates will be provided
    2) Plot a point
    pnt x y r g b
              x, y - point's coordinates
              r, g, b - colour
    3) Plot a guards
    grd x y
              x, y - guard's coordinates
    4) Plot a region (simple polygon)
    rgn n [ann]
              n - the number of vertices of the region
              n points' coordinates separated by space ("x y") are expected after
              ann - optional, if provided points' coordinates will be provided
    5) Plot a segment
    sgmnt x1 y1 x2 y2
              x1, y1, x2, y2 - coordinates of end points
    6) Plot a hole (simple polygon)
    hole n [ann]
              n - the number of vertices of the hole
              n points' coordinates separated by space ("x y") are expected after
              ann - optional, if provided points' coordinates will be provided
          """)


if __name__ == "__main__":
    if (len(sys.argv) < 2):
        instructions()
        sys.exit(-1)
    filename = sys.argv[1]
    f = open(filename)

    read_options = {
        'pol': read_polygon,
        'pnt': read_point,
        'grd': read_guard,
        'hole': read_hole,
        'sgmnt': read_segment,
        'rgn': read_region
    }

    options = {
        'pol': draw_polygon,
        'pnt': draw_point,
        'grd': draw_guard,
        'hole': draw_hole,
        'sgmnt': draw_segment,
        'rgn': draw_region
    }

    items = []

    line = f.readline()
    while line != '':
        words = line.strip('\n').strip().split(' ')
        option = words[0]
        if option not in options:
            print("ERROR: no such option", option)
            instructions()
            exit(-1)
        if option not in []:
            items.append([option, read_options[option](words[1:], f)])
        else:
            read_options[option](words[1:], f)
        line = f.readline()
        while line == '\n':
            line = f.readline()

    f.close()

    priority = ['pol', 'hole', 'rgn', 'sgmnt', 'pnt', 'grd']
    draw(items, options, priority)
