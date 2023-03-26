import "assets/lib/ui.bas"

'Variables
bullets = list()
rocks = list()
aliens = list()
score = 0
hiScore = 100000
lives = 3
maxbullets = 3

alienTick = 0
alienTickJump = 30

'Set sounds
sfx.render(5, 90)

'Set graphics
setSprite(0,15,31,63,127,255,255,255,255)    'rock square
setSprite(1,255,255,255,255,240,224,192,128) 'rock square down
setSprite(2,255,255,255,255,255,255,255,255) 'rock full
setSprite(3,171,221,162,173,75,151,201,229) 'rock damange 3
setSprite(4,52,28,163,8,64,26,73,28)         'rock damange 2
setSprite(5,48,4,34,0,0,10,72,16)            'rock damange 1

setSprite(6,4,2,7,13,31,47,36,2)    'alien 1a
setSprite(7,4,2,7,13,31,47,66,4)    'alien 1b

setSprite(16,16,9,0,84,0,9,16,0)    'explosion

renderSprites()

'Tools

def checkCollisionAABB(x1, y1, w1, h1, x2, y2, w2, h2)
    xOverlap = (x1 + w1 >= x2) AND (x2 + w2 >= x1)
    yOverlap = (y1 + h1 >= y2) AND (y2 + h2 >= y1)
    return xOverlap AND yOverlap
enddef

'Classes
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

    def isDead()
        return y < 14 
    enddef
endclass

class player
    x = 152
    y = 188
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
        'Check for player bullets collision
        for b in bullets
            'if b.x > x and b.x < x+8 and b.y > y and b.y < y+8 then
            if checkCollisionAABB(b.x,b.y,2,4,x,y,8,8) then
                state = state - 1
                remove(bullets, index_of(bullets, b))
            endif
        next
    enddef

    def draw()
        sp = 2
        flag = 0
        if shape = 1 then 
            sp = 0 
        endif
        if shape = 2 then 
            sp = 0 
            flag = 1
        endif
        if shape = 3 then 
            sp = 1 
        endif
        if shape = 4 then 
            sp = 1 
            flag = 8
        endif
        draw.sprite(sp,x,y,14,flag)
        if state < 4 then
            draw.sprite(state+2,x,y,0)
        endif
    enddef

    def isDead()
        return state < 1
    enddef

endclass

class alien
    x = 0
    y = 0
    shape = 0
    dead = 0
    deadCounter = 0

    def update()
        'Check for player bullets collision
        for b in bullets
            if checkCollisionAABB(b.x,b.y,2,4,x+2,y,11,8) then
                remove(bullets, index_of(bullets, b))
                dead = 1
                sfx.play(15,127)
            endif
        next

        if dead then
            deadCounter = deadCounter + 1
        endif
    enddef
    def draw()
        if dead then
            draw.sprite(16,x,y,3)
            draw.sprite(16,x+8,y,3,8)
        else
            draw.sprite(6,x,y,3)
            draw.sprite(6,x+8,y,3,8)
        endif
    enddef
    def isDead()
        return deadCounter > 10
    enddef
endclass

p = new (player)

def addRock(x,y,shape)
    r = new (rock)
    r.x = x * 8
    r.y = y * 8
    r.shape = shape
    push(rocks, r)
enddef

def addMetaRock(x,y)
    addRock(x,     y, 1)
    addRock(x+1,   y, 0)
    addRock(x+2,   y, 0)
    addRock(x+3,   y, 2)
    addRock(x,   y+1, 0)
    addRock(x+1, y+1, 3)
    addRock(x+2, y+1, 4)
    addRock(x+3, y+1, 0)
    addRock(x,   y+2, 0)
    addRock(x+3, y+2, 0)
enddef

addMetaRock(3,18)
addMetaRock(13,18)
addMetaRock(23,18)
addMetaRock(33,18)

def addAlien(x,y)
    a = new (alien)
    a.x = x * 20
    a.y = y * 8
    a.shape = 0
    push(aliens, a)

enddef
for n = 0 to 9
    addAlien(n,6)
    addAlien(n,8 )
next

'UI
def drawUI()
    draw.rect(0,0,320,13,0,7)
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
        if b.isDead() then
            remove(bullets, 0)
        endif
    endif

    'update rocks
    rc = 0
    for r in rocks
        r.update()
        rg = get(rocks, rc)
        if rg.isDead() then 
            remove(rocks, rc)
        endif
        rc = rc + 1
    next

    'update aliens
    ac = 0
    for a in aliens
        a.update()
        ag = get(aliens, ac)
        if ag.isDead() then
            remove(aliens, ac)
        endif
        ac = ac + 1    
    next

    alienTick = alienTick + 1
    if alienTick > alienTickJump then
        alienTick = 0

    endif

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

    for a in aliens
        a.draw()
    next


    IF mouse.down(2) THEN ui.drawPalette() ENDIF
    ui.drawmouse()
enddef