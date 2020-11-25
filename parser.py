import re
import csv
f = open('main.cmb')
line = f.readline()


calls = []


inlinevars = []
varcnt = 0

while line:
    splitted = [ '"{}"'.format(x) for x in list(csv.reader([line], delimiter=' ', quotechar='"'))[0] ]
    
    
    if(splitted[0] == '"init"'):
        if(splitted[1] == '"new"'):
            if(splitted[2] == '"var"'):
                for i in range(len(splitted) - 3):
                    varcnt+=1
                    inlinevars.append(splitted[i + 3])
                    calls.append("malloc 4") #4 bytes for 32 bit integer
                    calls.append("setptr " + str(inlinevars.index(splitted[i + 3])) + " v")
                    #print("DEBUG: init var " + splitted[i + 3])
    if(splitted[0] == '"set"'):
        if(splitted[1] == '"var"'):
            setvars = []
            index = 0
            for i in range(len(splitted) - 2):
                if(splitted[i + 2] == '"to"'):
                    index = i + 3
                    break
                setvars.append(inlinevars.index(splitted[i + 2]))
            
            print(splitted[index])
            
    line = f.readline()
f.close()
calls.insert(0, "ptrram "+ str(varcnt))
print(calls)