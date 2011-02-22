#! /usr/bin/python2.6

import sys
import CairoPlot

def get_program_name(progname):
    progname = progname.split('.')
    print progname
    return progname[-2].split('/')[1]

def parse_file(filename):
  print filename
  file = open(filename, 'r')
  sum = 0
  count = 0
  for i in file:
    count = count + 1
    sum = sum + float(i.split(' ')[0])
  return sum/count

def generate_v_labels(bottom, top, number):
  PRECISION = 1000
  top = top * PRECISION
  bottom = bottom * PRECISION
  increment = (top - bottom) / number 
  vlabels = list()
  for i in xrange(int(bottom), int(top), int((top - bottom)/number)):
    vlabels.append(str(float(i) / PRECISION))
  return vlabels

def draw_charts(data):
  vlabels = generate_v_labels(0, max(data.values()), 10)
  print vlabels
  CairoPlot.bar_plot ('bench.svg', data.values(), 240, 480, background = None, border = 20, grid = True, rounded_corners = True, h_labels = data.keys(), v_labels = vlabels)



# __ MAIN __
results = dict()
for i in range(len(sys.argv)):
    if i == 0:
        continue
    results[get_program_name(sys.argv[i])] = parse_file(sys.argv[i])

print results

draw_charts(results)

