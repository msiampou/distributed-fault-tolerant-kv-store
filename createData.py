#!/usr/bin/python

import sys, getopt
import string

def create_nested(fields, numlines, maxlevel, maxlength, maxkeys, maxk):
   for i in range(maxkeys):
      if (maxlevel == -1):
         print("b", end="")
         return
      if (i == 0):
         print("{", end="")
      print("a : ", end="")
      create_nested(fields, numlines, maxlevel-1, maxlength, maxkeys, maxk)
      if i != maxkeys-1:
         print("; ", end="")
   print("} ", end="")

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
   create_nested(fields, numlines, maxlevel, maxlength, maxkeys, maxlevel)
   print()