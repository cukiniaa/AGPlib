import pylab
import numpy as np
import sys

pylab.rcParams.update({'font.size': 40})
opt_res_filename = '../results/experiment-all-20-600-couto-new.out'


def to_int_array(arr):
    return list(map(lambda x: int(x), arr))


def avg(keys, values):
    d = {}
    for i, k in enumerate(keys):
        if k not in d:
            d[k] = []
        d[k].append(values[i])
    y = list(map(lambda k: np.mean(d[k]), sorted(d)))
    return (sorted(d), y)


def count(keys):
    d = {}
    for k in keys:
        if k not in d:
            d[k] = 0
        d[k] += 1
    return d


def instances_names(filename):
    instances_arr = []
    with open(filename) as f:
        for l in f:
            row = l.split(' ')
            instances_arr.append(row[0])
    return instances_arr


def error(val, opt):
    return abs(val - opt) / float(opt)


def all_errors(alg_res, opt_res):
    errors = []
    for fname, val in alg_res.items():
        opt = opt_res[fname]
        errors.append(error(val, opt))
    return errors


def size_guards(v, g, label=""):
    (x, y) = avg(v, g)
    pylab.plot(x, y, linewidth=8, label=label)
    size_guards_labels()


def size_time(v, t, label=""):
    (x, y) = avg(v, t)
    pylab.plot(x, y, linewidth=8, label=label)
    size_time_labels()


def size_error(i_n, v, g, opt_i_n, opt_g, label=""):
    alg_res = dict(zip(i_n, g))
    opt_res = dict(zip(opt_i_n, opt_g))
    errors = all_errors(alg_res, opt_res)
    (x, y) = avg(v, errors)
    pylab.plot(x, y, linewidth=8, label=label)
    size_error_labels()


def size_success(v, s_v):
    v_count = count(v)
    s_v_count = count(s_v)
    ratio = {}
    for k, c in v_count.items():
        if k not in s_v_count:
            ratio[k] = 0
            continue
        ratio[k] = s_v_count[k] / c * 100

    for ver, r in sorted(ratio.items()):
        print(ver, r)


def size_guards_labels():
    pylab.xlabel("Number of nodes", labelpad=20)
    pylab.ylabel("Mean number of guards", labelpad=20)


def size_time_labels():
    pylab.xlabel("Number of nodes", labelpad=20)
    pylab.ylabel("Mean time (s)", labelpad=20)


def size_error_labels():
    pylab.xlabel("Number of nodes", labelpad=20)
    pylab.ylabel("Mean approximation error", labelpad=20)


def process(option, i_n, v_org, g, t, label="", v_low_bound=0, v_up_bound=2000):
    v = [x for x in v_org if x >= v_low_bound and x <= v_up_bound]
    g = [x for i, x in enumerate(g) if v_org[i] >= v_low_bound and v_org[i] <= v_up_bound]
    if (option == 'size-guards'):
        size_guards(v, g, label)
    elif (option == 'size-time'):
        size_time(v, t, label)
    elif (option == 'size-error'):
        (opt_i_n, _, opt_g, _) = couto(opt_res_filename)
        size_error(i_n, v, g, opt_i_n, opt_g, label)
    elif (option == 'size-success'):
        (s_i_n, s_v, s_g, s_t) = baumgartner_success(filename)
        size_success(v, s_v)


def couto(filename):
    i_n = instances_names(filename)
    data = pylab.loadtxt(filename, usecols=[1, 2, 4])
    v = data[:, 0]
    g = data[:, 1]
    t = data[:, 2]
    return (i_n, v, g, t)


def fisk(filename):
    i_n = instances_names(filename)
    data = pylab.loadtxt(filename, usecols=[1, 2])
    v = data[:, 0]
    g = list(map(lambda n: np.floor(n/3), v))
    t = data[:, 1]
    return (i_n, v, g, t)


def ghosh(filename):
    i_n = instances_names(filename)
    data = pylab.loadtxt(filename, usecols=[1, 2, 3])
    v = data[:, 0]
    g = data[:, 1]
    t = data[:, 2]
    return (i_n, v, g, t)


def baumgartner(filename):
    i_n = instances_names(filename)
    data = pylab.loadtxt(filename, usecols=[1, 2, 5])
    v = data[:, 0]
    g_int = list(map(lambda x: int(x), data[:, 1]))
    g = [x if x > 0 else np.floor(v[i]/3) for i, x in enumerate(g_int)]
    t = [x if x < 120 else x + 200 for i, x in enumerate(data[:, 2])]
    return (i_n, v, g, t)


def baumgartner_success(filename):
    i_n = instances_names(filename)
    data = pylab.loadtxt(filename, usecols=[1, 2, 4, 5])
    s = to_int_array(data[:, 2])
    v = [x for i, x in enumerate(to_int_array(data[:, 0])) if s[i] > 0]
    g = [x for i, x in enumerate(to_int_array(data[:, 1])) if s[i] > 0]
    t = [x for i, x in enumerate(to_int_array(data[:, 3])) if s[i] > 0]
    i_n = [x for i, x in enumerate(i_n) if s[i] > 0]
    return (i_n, v, g, t)


labels = {
    "-f": "Fisk",
    "-g": "Ghosh",
    "-c": "Couto",
    "-b": "Baumgartner",
    "-bs": "Baumgartner (successful attempts)"
}

algo = {
    '-c': couto,
    '-f': fisk,
    '-g': ghosh,
    '-b': baumgartner,
    '-bs': baumgartner_success
}

if len(sys.argv) < 2:
    print("Too few arguments, 2 expected")
    exit(-1)

option = sys.argv[1]

i = 2
while i < len(sys.argv):
    algo_name = sys.argv[i]
    filename = sys.argv[i+1]
    label = labels[algo_name]
    (i_n, v, g, t) = algo[algo_name](filename)
    process(option, i_n, v, g, t, labels[algo_name], 0, 2000)
    i += 2

pylab.legend()
pylab.show()
