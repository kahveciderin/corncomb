#!/usr/bin/python3
import sys

class label:
  def __init__(self, name, address):
    self.name = name
    self.address = address


labels = []

def retreguse(opcode, *args):
    reguse = []
    argvals = []
    for arg in args:
        a = 0

        if arg[0] == '.':
            a = [x for x in labels if x.name == arg[1:len(arg)]][0].address
            reguse.insert(0,4) # 4 for virtual address
        elif len(arg) > 2 and arg[0] == '0' and arg[1] == 'x':
            a = int(arg, 16)
            reguse.insert(0,1) # 1 for value
        elif arg.isdecimal():
            a = int(arg)
            reguse.insert(0,1) 
        elif arg[0] == 'v':
            a = 0
            reguse.insert(0,2) # 2 for previous return
        else:
            a = int(arg[1])
            reguse.insert(0,3) # 3 for register
            if arg[0] != 'r':
                print("Warning: I am pretty sure there isn't a register called \""+arg+"\", because there isn't a register group named \""+arg[0]+"\"")
        
        argvals.append(a)


    regused = ''.join(str(e) for e in reguse)
    values = ' '.join("{0:0{1}x}".format(int(e),8) for e in argvals)
    
    return "{0:0{1}x}".format(opcode, 8)+" "+"{0:0{1}}".format(int(regused),8)+" "+values


with open(sys.argv[1]) as f:
    data = f.readlines()

addr = 0

instrs = ['dealloc', 'malloc', 'ptrram', 'setptr', 'getptr', 'set', 'get', 'print', 'scan', 'add', 'sub', 'mul', 'div', 'setreg', 'getreg', 'joz', 'call', 'ret', 'halt', 'push', 'pop', 'mod']
argcounts = [1,1,1,2,1,2,1,1,1,2,2,2,2,2,1,2,1,1,1,1,1,2]
data = [x.split(' ') for x in data]
for i in range(len(data)):
    data[i][-1] = data[i][-1].replace("\n", "")
for splitted in data:
    if len(splitted[0]) == 0: continue 
    if splitted[0][0] == ':':
        labels.append(label(splitted[0][1:len(splitted[0])], addr))
        continue
    if splitted[0] not in instrs:
        print("Error: haha no: {}".format(splitted))
        exit(1)
    addr += argcounts[instrs.index(splitted[0])] + 2

for splitted in data:
    if len(splitted[0]) == 0: continue
    if splitted[0][0] == ":":
        continue
    if splitted[0] in ["ret", "halt", "scan", "pop"]:
        a1 = "0"
    else:
        a1 = splitted[1]
    i = instrs.index(splitted[0])
    ac = argcounts[i]
    if ac == 2:
        args = (a1, splitted[2])
    else:
        args = (a1,)
    print(retreguse(i, *args))