import "assets/lib/ui.bas"


'Variables
bullets = list()
rocks = list()
score = 0
hiScore = 100000
lives = 3
maxbullets = 3

'Set sounds
sfx.render(5, 90)

'Set graphics

'Clases
class bullet
    x = 0
    y = 0
    speed = 0

    def draw()
        draw.rect(x,y,2,4,0,10)
    enddef

    def update()
        y = y + speed
    enddef

    def isLive()
        return y < 14 
    enddef


endclass

class player
    x = 152
    y = 186
    speed = 2
    def draw()
        draw.rect(x,y,16,5,0,13)
        draw.rect(x+1,y-1,14,1,0,13)
        draw.rect(x+6,y-3,4,2,0,13)
        draw.rect(x+7,y-4,2,1,0,13)
    enddef
    def update()
        if key.down(65) then ' key A move left
            x = x - speed
            if x < 0 then x = 0 endif
        endif
        if key.down(68) then ' key D move right
            x = x + speed
            if x > 303 then x = 303 endif
        endif
        if key.pressed(32) then ' key Space shot
            if len(bullets) < maxbullets then
                b = new (bullet)
                b.x = x + 7 
                b.y = y - 8
                b.speed = -1
                push(bullets, b)
                sfx.play(5, 127)
            endif
        endif
    enddef
endclass

class rock
    x = 0
    y = 0
    shape = 0
    state = 4

    def update()
        for b in bullets
            if b.x > x and b.x < x+8 and b.y > y and b.y < y+8 then
                state = state - 1
                b.y = - 9
            endif
        next
    enddef

    def draw()
        draw.text(inttotext("%i",state),x,y,1,13)
        draw.rect(x,y,8,8,1,5)
    enddef

    def isLive()
        return state < 1
    enddef

endclass

p = new (player)

def addRock(x,y)
    r = new (rock)
    r.x = x * 8
    r.y = y * 8
    push(rocks, r)
enddef

def addMetaRock(x,y)
    addRock(x+1,y)
    addRock(x+2,y)
    addRock(x,y+1)
    addRock(x+1,y+1)
    addRock(x+2,y+1)
    addRock(x+3,y+1)
    addRock(x,y+2)
    addRock(x+3,y+2)
enddef

addMetaRock(3,18)
addMetaRock(13,18)
addMetaRock(23,18)
addMetaRock(33,18)

'UI
def drawUI()
    draw.rect(0,0,320,13,0,1)
    draw.line(0,12,320,12,1,4)
    draw.line(0,196,320,196,2,15)
    draw.text(intToText("SCORE %06i",score), 3,3,1,3)
    draw.text(intToText("LIVES %i",lives), 128,3,1,3)
    draw.text(intToText("HIGH %06i",hiScore), 224,3,1,3)
enddef

'Main update
def tick()
    'update player
    p.update()

    'update all player bullets
    for b in bullets 
        b.update()
    next
    
    if len(bullets) then 
        b = get(bullets, 0) 
        if b.isLive() then
            remove(bullets, 0)
        endif
    endif

    'update rocks
    rc = 0
    for r in rocks
        r.update()
        rg = get(rocks, rc)
        if rg.isLive() then 
            remove(rocks, rc)
        endif
        rc = rc + 1
    next

enddef

'Main draw
def draw()
    cls(0)
    drawUI()

    p.draw()

    for b in bullets 
        b.draw() 
    next

    for r in rocks
        r.draw()
    next


    IF mouse.down(2) THEN ui.drawPalette() ENDIF
    ui.drawmouse()
enddef