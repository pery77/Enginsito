REM Based in this video
REM https://www.youtube.com/watch?v=zH_omFPqMO4

ROWS = 20
COLS = 12
POSX = 20
POSY = 8

offsetX = POSX + 8
offsetY = POSY + 8

dim field(21, 13)
dim figures(7,4)

class point
    x = 0
    y = 0
endclass

drawed = 0
dx = 0
rotation = 0
color = 0

timer = 0
delay = 200
delayMove = 70

a = list()
b = list()
for c = 0 to 3
    push(a, new(point))
    push(b, new(point))
next

shapeData = list(    
    1,3,5,7, ' I    [0][1]  T > [-][-]
    2,4,5,7, ' Z    [2][3]      [-][3]
    3,5,4,6, ' S    [4][5]      [4][5]
    3,5,4,7, ' T    [6][7]      [-][7]
    2,3,5,7, ' L    
    3,5,7,6, ' J    
    2,3,4,5, ' O    
)
c = 0
for sd in shapeData
    figures(floor(c/4), c mod 4 ) = sd 
    c = c + 1
next

def drawDeco()
    sprite(1, POSX, POSY, 2)
    sprite(1, POSX, (ROWS + 2) * 8 + POSY, 2, 3)
    sprite(1, (COLS + 2) * 8 + POSX, POSY, 2, 1)
    sprite(1, (COLS + 2) * 8 + POSX, (ROWS + 2) * 8 + POSY, 2, 2)
    for r = 1 to ROWS + 1
        y = r * 8 
        sprite(2, POSX, y + POSY, 2, 3)
        sprite(2, (COLS + 1) * 8 + 8 + POSX, y + POSY,2 ,1)
    next
    for c = 1 to COLS + 1
        x = c * 8 
        sprite(2, x + POSX, POSY, 2)
        sprite(2, x + POSX, (ROWS + 1) * 8 + 8 + POSY, 2 ,2)
    next 
enddef

def drawBlock(x,y,c)
    rect(x+1, y+1, 6, 6, 0, c)
    sprite(0, x, y, 3)
enddef

def drawField()
    rect(POSX+8, POSY+8, (COLS + 1) * 8, (ROWS + 1) * 8,0 ,0)
    for r = 0 to ROWS
        y = r * 8 + 8
        for c = 0 to COLS
            x = c * 8 + 8
            if field(r, c) = 0 then
                sprite(3, x + POSX, y + POSY, 1)
            else
                drawBlock(c * 8 + offsetX, r * 8 + offsetY, field(r, c) + 8)
            endif
        next
    next    
enddef



def setFigure(f)
    color = f
    c = 0
    for i in a
        i.x = (figures(color,c) mod 2)
        i.y = floor(figures(color,c) / 2)
        c = c + 1
    next
enddef

def drawFigure()
    for i in a
        drawBlock(i.x * 8 + offsetX, i.y * 8 + offsetY, color + 8)
    next
enddef

def check()
    r = 1
    for i in a 
        if i.x < 0 or i.x > COLS-1 or i.y > ROWS-1 then 
            return 0
        elseif 
    next
    return 1
enddef

def init()
    setFigure(rnd(0,6))
enddef

def tick()
    timer = timer + delta
    moveTick = moveTick + delta

    if key_down(65) then dx =-1
    if key_down(68) then dx = 1
    if key_pressed(32) then rotate = 1

    'MOVE
    if moveTick > delayMove then
        c=0
        for i in a 
            set(b,c,i)
            i.x = i.x + dx
            c = c + 1
        next

        if not check() then
            c = 0
            for i in b 
                set(a,c,i)
                c = c + 1
            next
        endif
        moveTick = 0
    endif

    'ROTATION
    if rotate then
        p = get(a, 1)
        for i in a 
            x = i.y - p.y
            y = i.x - p.x
            i.x = p.x - x
            i.y = p.y + y
        next

        if not check() then
            c = 0
            for i in b 
                set(a,c,i)
                c = c + 1
            next
        endif
    endif

    'TICK
    if timer > delay then
        c=0
        for i in a 
            set(b,c,i)
            i.y = i.y + 1
            c = c + 1
        next
        if not check() then
            for i in b 
                field(i.y, i.x) = color
            next
            setFigure(rnd(0,6))
        endif
        timer = 0
    endif
    
    dx = 0; rotate = 0;
enddef


def draw()
    'Only draw deco and clear screen first time
    if not drawed then
        cls(1)
        drawDeco()
        drawed = 1
        line(160,0,160,200,1,9)
    endif

    drawField()
    drawFigure()

enddef