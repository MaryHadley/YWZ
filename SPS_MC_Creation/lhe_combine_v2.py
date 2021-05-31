#!/usr/bin/env python
print "GOt here 1"
#Credit to Hua-Sheng Shao, with a few tweaks by Mary Hadley 
import sys
import os


files=[arg for arg in sys.argv[1:] if arg.startswith('--files=' or  ' ')]
if not files:
    raise Exception, "The usage of the script should be e.g., ./lhe_combine --files=</PATH/TO/file1.lhe>,</PATH/TO/file2.lhe>,</PATH/TO/file3.lhe> --out=combine.lhe Do NOT put spaces between the inputs to --files, separate them with commas only as in the example. combine.lhe will be the default output name of the --out file if the --out argument is omitted"
print "files", files
files=files[0]
print "files:", files
files=files.replace('--files=','')
print files
#files=[file.lower() for file in files.split(',')]
files=[file for file in files.split(',')]

outfile=[arg for arg in sys.argv[1:] if arg.startswith('--out=')]
print outfile
if outfile:
    outfile=outfile[0]
    print outfile
    outfile=outfile.replace('--out=', '')
    print outfile
if not outfile:
    outfile=['combine.lhe']
    outfile=outfile[0]

#outfile=outfile[0]

currentdir=os.getcwd()

offset=100

headers=[]
inits=[]
events=[]
firstinit=""

ilil=0
for i,file in enumerate(files):
    stream=open(file,'r')
    headQ=True
    initQ=False
    iinit=-1
    ievent=-1
    eventQ=False
    procid=None
    proc_dict={}
    for line in stream:
        sline=line.replace('\n','')
        if "<init>" in line or "<init " in line:
            initQ=True
            headQ=False
            iinit=iinit+1
            if i==0: inits.append(sline)
        elif headQ and i == 0:
            headers.append(sline)
        elif "</init>" in line or "</init " in line:
            initQ=False
            iinit=-1
            if i==0: inits.append(sline)
        elif initQ:
            iinit=iinit+1
            if iinit == 1:
                if i == 0:
                    firstinit=sline
                    inits.append(sline)
                else:
                    if not sline == firstinit:
                        raise Exception, "the beam information of the LHE files is not identical"
            elif iinit >= 2:
                procid=sline.split()[-1]
                procpos=sline.index(' '+procid)
                ilil=ilil+1
                sline=sline[:procpos]+(' %d'%(offset+ilil))
                proc_dict[procid]=offset+ilil
                if i == 0:
                    inits.append(sline)
                else:
                    inits.insert(-1,sline)
            else:
                raise Exception, "should not reach here. Do not understand the <init> block"
        elif "<event>" in line or "<event " in line:
            eventQ=True
            ievent=ievent+1
            events.append(sline)
        elif "</event>" in line or "</event " in line:
            eventQ=False
            ievent=-1
            events.append(sline)
        elif eventQ:
            ievent=ievent+1
            if ievent == 1:
                found=False
                for procid,new_procid in proc_dict.items():
                    if ' '+procid+' ' not in sline: continue
                    procpos=sline.index(' '+procid+' ')
                    found=True
                    sline=sline[:procpos]+(' %d'%(new_procid))+sline[procpos+len(' '+procid):]
                    break
                if not found: raise Exception, "do not find the correct proc id !"
                #sline=sline[:procpos]+(' %d'%(new_procid))+sline[procpos+len(' '+procid):]
            events.append(sline)
    stream.close()

text='\n'.join(headers)+'\n'
text=text+'\n'.join(inits)+'\n'
text=text+'\n'.join(events)

stream=open(outfile,'w')
stream.write(text)
stream.close()
print "The final merged lhe files is %s"%outfile
