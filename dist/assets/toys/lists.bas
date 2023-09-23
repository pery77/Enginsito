l1  = list()
for c = 0 to 9
    push(l1,c)
next

it = iterator(l1)

print  "First pass"
while move_next(it)
    print get(it)
wend

push(l1,321)

print  "Second pass"
for i = 0 to len(l1) - 1  
    print get(l1, i)
    if rnd (0,100) < 10 then push(l1,"x")
next


'print  "Third pass"
'for i in l1
'    print i
'next