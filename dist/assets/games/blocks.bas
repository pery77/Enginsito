REM Based in this video
REM https://www.youtube.com/watch?v=zH_omFPqMO4

let ROWS = 20
let COLS = 12
let POSX = 20
let POSY = 8

let DELAY_DOWN = 400

let MENU = 0
let PLAY = 1
let GAMEOVER = 2

let state = MENU

let offsetX = POSX + 8
let offsetY = POSY + 8

dim field(21, 13)
dim figures(7,4)

class point
    var x = 0
    var y = 0
endclass

let drawed = 0
let dx = 0
let rotation = 0
let color = 0

let timer = 0
let delayFall = DELAY_DOWN
let delayMove = 70

let a = list()
let b = list()

for c = 0 to 3
    push(a, new (point))
    push(b, new (point))
next
let shapeData = list(    
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
                drawBlock(c * 8 + offsetX, r * 8 + offsetY, field(r, c))
            endif
        next
    next    
enddef

def setFigure()
    f = rnd(0,6)
    color = f + 8
    c = 0
    for i in a
        i.x = (figures(f,c) mod 2) + floor(COLS / 2) - 1
        i.y = floor(figures(f,c) / 2) - 4
        c = c + 1
    next
enddef

def drawFigure()
    for i in a
        if i.y >= 0 then 
            drawBlock(i.x * 8 + offsetX, i.y * 8 + offsetY, color)
    endif
    next
enddef

def check()
    for i in a
        fy = i.y 
        if fy < 0 then fy = 0
        if i.x < 0 or i.x >= COLS + 1 or i.y >= ROWS+1 then 
            return 0
        elif field(fy, i.x) then
            return 0
        endif
    next
    return 1
enddef

def init()
    setFigure()
enddef

def tick()

    timer = timer + delta * 1000
    moveTick = moveTick + delta * 1000

    if keydown(65) then dx =-1
    if keydown(68) then dx = 1
    if keydown(83) then delayFall = 20
    if keypressed(32) then rotate = 1

    'MOVE
    if moveTick > delayMove then
     
        for i = 0 to 3
            _a = get(a, i)
            _a2 = new(point)
            _a2.x = _a.x 
            _a2.y = _a.y
            set(b, i, _a2)
            _a.x = _a.x + dx
        next

        if not check() then
            for i = 0 to 3
                _b = get(b, i)
                _b2 = new(point)
                _b2.x = _b.x 
                _b2.y = _b.y
                set(a, i, _b2)
            next
        endif
        moveTick = 0
    endif

    'ROTATION
    if rotate then
        p = get(a, 1)
        for i = 0 to 3
            _a = get(a, i)
            x = _a.y - p.y
            y = _a.x - p.x
            _a.x = p.x - x
            _a.y = p.y + y
        next

        if not check() then
            for i = 0 to 3
                _b = get(b, i)
                _b2 = new(point)
                _b2.x = _b.x 
                _b2.y = _b.y
                set(a, i, _b2)              
            next
        endif
    endif

    'TICK
    if timer > delayFall then
        for i = 0 to 3
            _a = get(a, i)
            _a2 = new(point)
            _a2.x = _a.x 
            _a2.y = _a.y
            set(b, i, _a2)
            _a.y = _a.y + 1
        next
        if not check() then
            for i in b
            	if i.y >= 0 then 
                	field(i.y, i.x) = color
                else
                	state = GAMEOVER
                endif
            next
           setFigure()
        endif
        timer = 0
    endif
    'CHECK LINE
    k=ROWS;
    for i= ROWS to 0 step -1
        count=0
        for j = 0 to COLS 
            if field(i,j) then count = count + 1
            field(k,j)=field(i,j)
        next
        if count <= COLS then k = k - 1
    next
    dx = 0; rotate = 0; delayFall = DELAY_DOWN
enddef

def draw()
	'Only draw deco and clear screen first time
	if not drawed then
		cls(1)
        drawDeco()
        drawed = 1
        'line(160,0,160,200,1,9)
        'line(0,100,320,100,1,9)
    endif

    drawField()
    drawFigure()
    
    if state = GAMEOVER then cls(1)
enddef