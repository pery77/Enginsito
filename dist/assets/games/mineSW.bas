REM Based in this video
REM https://www.youtube.com/watch?v=c8wswUEfnrQ

dim grid(12,12)
dim sgrid(12,12)
w = 16
level = 10

PLAY = 0
OVER = 1

state = PLAY

def sp(x,y,t)
    xw = x*w
    yw = y*w
    rect(xw, yw, w, w, 0, 2)
    rect(xw+2, yw+2, w-2, w-2, 0, 1)
    
    hover = mouse_X > xw AND mouse_Y > yw AND mouse_X < xw + w AND mouse_Y < yw + w
    if hover then
        toogleFlag = 0
        if mouse_released(0) then sgrid (x,y) = grid(x,y)
        if mouse_released(1) then toogleFlag = 1
        if toogleFlag and sgrid (x,y) > 9 then
            if sgrid (x,y) = 11 then sgrid (x,y) = 10 else sgrid (x,y) = 11
        endif
    endif

    g = sgrid (x,y)
    if g = 10 then
        rect(xw, yw, w-2, w-2, 0, 4)
        meta(0,xw,yw)
    elseif g = 11 then
        rect(xw+2, yw+2, w-4, w-4, 0, 4)
        meta(1,xw,yw+2)
    else
        sprite(g,xw+6,yw+5,g+7)
    endif
    if g = 9 then 
        state = OVER
    endif
enddef

def create()
    for i = 1 to 10
        for j = 1 to 10
            g = 0
            if rnd(0,100) < level then g = 9
            grid(i, j) = g
            sgrid(i, j)= 10
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

enddef

create()

def draw()
    cls(0)
    rect(16,16,162,162,0,2)
    for i = 1 to 10
        for j = 1 to 10
            if state = OVER then sgrid (i,j) = grid(i,j)
            sp(i, j, 0)
        next
    next
    meta(2, mouse_x, mouse_y,3)
enddef