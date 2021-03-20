#!/usr/bin/python

import sys, getopt
import random
import string
import sys
import math

def create_random_value(type, maxlength):
   if type == "int":
      start = 10**(maxlength-1)
      end = (10**maxlength)-1
      return random.randint(start, end)
   elif type == "string":
      return ''.join(random.choice(string.ascii_lowercase) for i in range(maxlength))
   elif type == "float" or type == "double" :
      start = 10**(maxlength-1)
      end = (10**maxlength)-1
      return (random.randint(start, end)) / (10** math.ceil(maxlength/2))
   else:
      raise TypeError("Type " + type + " is not supported")


def create_nested(fields, maxlevel, maxlength, maxkeys, value):
   if (maxkeys == 0):
      print("{}", end="")
      return
   for i in range(maxkeys):
      key, type = random.choice(list(fields.items()))
      if maxlevel == 0:
         print(value, end="")
         return
      if i == 0:
         print("{ ", end="")
      print(key + " : ", end="")
      create_nested(fields, maxlevel-1, maxlength, maxkeys, create_random_value(type, maxlength))
      if i != maxkeys-1:
         print("; ", end="")
   print(" }", end="")

def generate_random_depth(maxlevel, maxkeys):
   depth = []
   for i in range(maxkeys):
      depth.append(random.randint(0,maxlevel))
   return depth

def generate_keys(fields, maxlevel, maxlength, maxkeys):
   depth = generate_random_depth(maxlevel,maxkeys)
   for i in range(0,maxkeys):
      if i==0:
         print("{ ", end="")
      print("key" + str(i) + ": ", end="")
      create_nested(fields, depth[i], maxlength, random.randint(0,maxkeys), create_random_value("int", maxlength))
      if i < maxkeys-1:
         print("; ", end="")
      else:   
         print(" }", end="")
   if maxkeys == 0:
      print("{}", end="")

def generate_dataset(fields, numlines, maxlevel, maxlength, maxkeys):
   for i in range(0,numlines):
      print("person" + str(i+1) + ": ", end="")
      generate_keys(fields, maxlevel,maxlength,maxkeys)
      print()

def create_outfile(fields, numlines, maxlevel, maxlength, maxkeys):
   with open('testfile.txt', 'w') as f:
      sys.stdout = f
      generate_dataset(fields, numlines, maxlevel, maxlength, maxkeys)

def parse_file(file):
   fields = {}
   with open(file) as f:
      for line in f:
         key, value = line.split()
         fields[key] = value
   return fields

def parse_args(argv):
   try:
      opts, args = getopt.getopt(argv,"k:n:d:l:m:")
   except getopt.GetoptError:
      print('createData.py -k <keyfile> -n <numlines> -d <maxlevel> -l <maxlength> -m <maxkeys>')
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print('createData.py -k <keyfile> -n <numlines> -d <maxlevel> -l <maxlength> -m <maxkeys>')
         sys.exit()
      elif opt in ("-k"):
         keyfile = arg
      elif opt in ("-n"):
         numlines = int(arg)
      elif opt in ("-d"):
         maxlevel = int(arg)
      elif opt in ("-l"):
         maxlength = int(arg)
      elif opt in ("-m"):
         maxkeys = int(arg)
      else:
         print('createData.py -k <keyfile> -n <numlines> -d <maxlevel> -l <maxlength> -m <maxkeys>')
   return [keyfile,numlines,maxlevel,maxlength,maxkeys]

if __name__ == "__main__":
   keyfile, numlines, maxlevel, maxlength, maxkeys = parse_args(sys.argv[1:])
   fields = parse_file(keyfile)
   create_outfile(fields, numlines, maxlevel, maxlength, maxkeys)
   
