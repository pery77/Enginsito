dim grid(12,12)
dim sgrid(12,12)
w = 16
level = 10

def sp(x,y,t)
    rect(x*w, y*w, w, w, 0, 2)
    rect(x*w+2, y*w+2, w-2, w-2, 0, 1)
    g = grid (x,y)
    sprite(g,x*w+6,y*w+5,g+7)
enddef
for i = 1 to 10
    for j = 1 to 10
        g = 0
        if rnd(0,100) < level then g = 9
        grid(i, j)= g
    next
next
for i = 1 to 10
    for j = 1 to 10
        n = 0
        if grid(i, j) = 9 then next
        if grid(i+1, j) = 9 then n = n + 1
        if grid(i, j+1) = 9 then n = n + 1
        if grid(i-1, j) = 9 then n = n + 1
        if grid(i, j-1) = 9 then n = n + 1
        if grid(i+1, j+1) = 9 then n = n + 1
        if grid(i-1, j-1) = 9 then n = n + 1
        if grid(i-1, j+1) = 9 then n = n + 1
        if grid(i+1, j-1) = 9 then n = n + 1
        grid(i, j) = n
    next
next

def draw()
    cls(0) 
    for i = 1 to 10
        for j = 1 to 10
            sp(i, j, 0)
        next
    next
enddef