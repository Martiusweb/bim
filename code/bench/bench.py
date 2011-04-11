#!/usr/bin/python

import matplotlib.pyplot as plt
import subprocess
from matplotlib import rc

rc('font',**{'family':'serif','sans-serif':['Helvetica']})
## for Palatino and other serif fonts use:
#rc('font',**{'family':'serif','serif':['Palatino']))
rc('text', usetex=True)

colors_data=["blue", "green", "red", "cyan", "magenta", "yellow", "black", "white"]
total_req=10000
concurrency=100



def get_data(file):
  k = list()
  v = list()
  f = open(file, 'r')
  lines = f.readlines()

  lines = lines[1:-1]

  for i in lines:
    i = i[0:-1]
    i = i.split(',')
    k.append(i[0])
    v.append(float(i[1]))
  return [k,v]


def bench(name, url):
  command = '/usr/bin/ab -n %d -c %d -k -e %s.csv %s' % (total_req, concurrency, name, url)
  command = command.split()
  print command
  ret = subprocess.call(command)

f = open("urls.txt",'r')

servers = dict()

for i in f.readlines():
  i = i.split()
  servers[i[0]] = i[1]
  bench(i[0], i[1])

data = dict()
color_index=0
for i in servers.keys():
  data[i] = get_data(i+".csv")
  plt.plot(data[i][0],data[i][1], 'k-', color=colors_data[color_index], label=i)
  color_index=color_index +1

plt.legend(data.keys(), loc=2)

zero = [0 for i in range(len(data[i][0]))]

color_index=0
for i in servers.keys():
  plt.fill_between(data[i][0], zero, data[i][1], facecolor=colors_data[color_index], alpha=0.3)
  color_index=color_index+1

plt.title('Response time, repartition. \\ ~  \small %d request, concurrency level : %d ' % (total_req, concurrency))
plt.xlabel('Percentage of time')
plt.ylabel('Time in $\mu{}s$')
plt.savefig("output.png")
