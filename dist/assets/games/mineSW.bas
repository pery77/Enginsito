REM Based in this video
REM https://www.youtube.com/watch?v=c8wswUEfnrQ

dim grid(12,12)
dim sgrid(12,12)
w = 16
level = 5
bombs = 0
clean = 0

PLAY = 0
OVER = 1
WIN = 2

state = PLAY
'peryEngine is realy bad in recursive :(
def floodfill(x,y)
    if grid(x,y)<>0 then return
    i = x
    while i <= 10 
        sgrid(i,y) = 0
        i = i + 1
        if grid(i,y) <> 0 then i = 11
    wend
    i = x
    while i > 0 
        sgrid(i,y) = 0
        i = i - 1
        if grid(i,y) <> 0 then i = 0
    wend
    if y+1 <= 10 then
        if grid(x,y+1) = 0 then
            CALL floodfill(x,y+1)
        endif
    endif
    if y-1 >= 1 then
        if grid(x,y-1) = 0 then
           ' CALL floodfill(x,y-1) 'crash :too much recursive
        endif
    endif
enddef

def sp(x,y,t)
    xw = x*w
    yw = y*w
    rect(xw, yw, w, w, 0, 2)
    rect(xw+2, yw+2, w-2, w-2, 0, 1)
    
    hover = mouse_X > xw AND mouse_Y > yw AND mouse_X < xw + w AND mouse_Y < yw + w
    if hover then
        toogleFlag = 0
        if mouse_released(0) then 
            sgrid(x,y) = grid(x,y)
            floodfill(x,y)
        endif
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
    if sgrid (x,y) < 10 then clean = clean + 1

enddef

def create()
    bombs = 0
    for i = 1 to 10
        for j = 1 to 10
            g = 0
            if rnd(0,100) < level then 
                g = 9
                bombs = bombs + 1
            endif
            grid(i, j) = g
            sgrid(i, j) = 10
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

def tick()
    if key_down(32) then 
        create()
        state = PLAY
    endif
enddef

def draw()
    cls(5)
    IF STATE = WIN THEN CLS(0)
    rect(16,16,162,162,0,2)

    clean = 0
    for i = 1 to 10
        for j = 1 to 10
            if state = OVER then sgrid (i,j) = grid(i,j)
            sp(i, j, 0)
        next
    next

    levelstate = 100 - clean - bombs
    if levelstate = 0 then state = WIN

    text(formatText("%i", levelstate),200,16,1,3)

    meta(3, mouse_x, mouse_y)
    meta(2, mouse_x, mouse_y)
enddef