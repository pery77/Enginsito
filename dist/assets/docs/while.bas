i = 10
def addi()
    i=i+1
enddef
while i >= 0 
    print i
    i = i - 1
    if rnd(0,10)<5 then call addi()
wend