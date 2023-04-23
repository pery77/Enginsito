' First program for testing the engine
' 
import "assets/games/invaders_data.dat"
'Global Variables
bullets = list()
rocks = list()
aliens = list()
aliensBullets = list()

' hiScore = readed from invader_data.dat
score = nil ' overwrited in startGame()
lives = nil ' overwrited in startGame()
maxbullets = 10
maxAlienBullets = 3

alienTick = 0
alienStep = 0
alienTickJump = 30
alienDir = 4
alienDown = 8

wave = 0
alienShipTime = 0
recoveryPlayerTick = 0
alienShip = nil

'Game states
MENU      = 0
PLAYING   = 1
GAME_OVER = 2
PLAYER_DEAD = 3
WAVE_INFO = 4

gameState = MENU
gameOverTime = 0
menuTime = 0
waveInfoTime = 0

'Set sounds
'shot
sfx.env(0, 0, 61, 0, 44)
sfx.freq(0, 16, -64, 0, 0)
sfx.tone(0, 0, 0, 0, 0)
sfx.repeat(0, 0, 0, 0)
sfx.filter(0, 125, 127, 0, 106, -55)
sfx.wave(0, 2)
sfx.render(0, 88)
'explosion
sfx.env(1, 0, 35, 53, 138)
sfx.freq(1, 5, -61, 51, 240)
sfx.tone(1, 0, 0, 0, 0)
sfx.repeat(1, 0, 0, 0)
sfx.filter(1, 139, -4, 0, 0, 0)
sfx.wave(1, 3)
sfx.render(1, 26)
'alien steps
sfx.env(2, 0, 40, 0, 27)
sfx.freq(2, 0, -11, 0, 0)
sfx.tone(2, 0, 0, 0, 0)
sfx.repeat(2, 0, 0, 0)
sfx.filter(2, 255, 0, 0, 0, 0)
sfx.wave(2, 1)
sfx.render(2, 44)

'alien ship
sfx.env(3, 255, 255, 0, 255)
sfx.freq(3, 0, 19, 0, 0)
sfx.tone(3, 0, 0, 0, 0)
sfx.repeat(3, 50, -1, 0)
sfx.filter(3, 34, 127, 0, 0, 0)
sfx.wave(3, 1)
sfx.render(3, 69)

'alien shot
sfx.env(4, 0, 104, 86, 65)
sfx.freq(4, 0, 58, 0, 0)
sfx.tone(4, 0, 0, 0, 0)
sfx.repeat(4, 0, 0, 0)
sfx.filter(4, 48, -13, 208, 102, 109)
sfx.wave(4, 3)
sfx.render(4, 64)

'player explosion
sfx.env(5, 0, 79, 203, 255)
sfx.freq(5, -10, -63, 0, 0)
sfx.tone(5, 37, 121, 211, 88)
sfx.repeat(5, 12, -3, -15)
sfx.filter(5, 154, 53, 71, 0, 0)
sfx.wave(5, 3)
sfx.render(5, 39)

'Set graphics
setSprite(0,15,31,63,127,255,255,255,255)    'rock square
setSprite(1,255,255,255,255,240,224,192,128) 'rock square down
setSprite(2,255,255,255,255,255,255,255,255) 'rock full
setSprite(3,222,124,58,125,126,123,254,255)  'rock damange 3
setSprite(4,0,148,24,93,58,58,60,126)        'rock damange 2
setSprite(5,0,0,16,72,16,26,24,60)           'rock damange 1

setSprite(6,3,15,31,57,63,14,25,12)      'alien 1a
setSprite(7,3,15,31,57,63,15,10,20)      'alien 1b
setSprite(8,4,2,7,13,31,47,36,2)         'alien 2a
setSprite(9,4,2,7,13,31,47,66,4)         'alien 2b
setSprite(10,3,7,13,31,15,4,2,4)         'alien 3a
setSprite(11,3,7,15,29,15,4,8,4)         'alien 3b

setSprite(16,16,9,0,84,0,9,16,0)         'explosion

setSprite(17,128,128,64,64,128,128,64,64)     'alien bullet
setSprite(18,0,7,31,63,109,255,59,16)         'alien ship

renderSprites()

'Tools
'Collision
def checkCollisionAABB(x1, y1, w1, h1, x2, y2, w2, h2)
    xOverlap = (x1 + w1 >= x2) and (x2 + w2 >= x1)
    yOverlap = (y1 + h1 >= y2) and (y2 + h2 >= y1)
    return xOverlap and yOverlap
enddef
'Score
def addScore(s)
    score = score + s
    if hiScore < score then ' a realy dirty way to save data.
        hiScore = score
        t$ = intToText("hiScore = %i", hiScore) + Chr(13)
        savefile("assets/games/invaders_data.dat", t$)
    endif
enddef

'Classes
class bullet

    x = 0
    y = 0
    speed = 0
    explosionFrame = 0
    frames = list(5,4,4,5,16)
    maxFrames = 50

    def draw()
        if explosionFrame > 0 and explosionFrame < maxFrames then
            f = (explosionFrame/10.0)
            flag = 0
            if f > 4 then flag = 1 endif
            draw.sprite(frames(f),x-3,y,4,flag)
            'draw.text(intToText("%i",f), x + 4, y, 1,3) 'a debug print
            speed = 0
            return
        endif

        if speed < 0 then
            draw.rect(x,y,2,4,0,10) ' is a player bullet
        endif
        if speed > 0 then
            draw.sprite(17,x,y,6) ' is an alien bullet
        endif

    enddef

    def update()

        if explosionFrame > 0 then
            explosionFrame = explosionFrame + 1
            return
        endif

        y = y + speed
        if y > 186 then 
            explosionFrame = 1 
        endif

    enddef

    def isDead()
        return y < 14 or explosionFrame > maxFrames - 1
    enddef

endclass

class player

    x = 152
    y = 188
    speed = 2
    dead = 0

    def setup()
        x = 152
        y = 188
        speed = 2
        dead = 0
    enddef

    def draw()
        if lives < 1 then
            return
        endif
        col = 13
        if dead = 1 then
            col = 6
            if recoveryPlayerTick/200 mod 2 then
                col = 0
            endif
        endif

        draw.rect(x,y,16,5,0,col)
        draw.rect(x+1,y-1,14,1,0,col)
        draw.rect(x+6,y-3,4,2,0,col)
        draw.rect(x+7,y-4,2,1,0,col)

    enddef

    def update()

        if key.down(65) or pad.down(0, 4) then ' key A move left also pad left 
            x = x - speed
            if x < 0 then x = 0 endif
        endif

        if key.down(68) or pad.down(0, 2) then ' key D move right also pad right
            x = x + speed
            if x > 303 then x = 303 endif
        endif

        if key.pressed(32) or pad.pressed(0, 6) then ' key Space shot also pad button 6
            if len(bullets) < maxbullets then
                b = new (bullet)
                b.x = x + 7 
                b.y = y - 8
                b.speed = -2
                b.explosionFrame = 0
                push(bullets, b)
                sfx.play(0, 127)
            endif
        endif

        for b in aliensBullets
            if checkCollisionAABB(b.x,b.y,2,4,x,y,16,8) then
                dead = 1
                lives = lives - 1
                gameState = PLAYER_DEAD
                if lives < 1 then
                    gameState = GAME_OVER
                endif
                remove(aliensBullets, index_of(aliensBullets, b))
                sfx.play(5, 99)
            endif
        next

    enddef
endclass

class rock

    x = 0
    y = 0
    shape = 0
    state = 4
    breakType = 0

    def setup()
        x = 0
        y = 0
        shape = 0
        state = 4
        breakType = 0
    enddef

    def update()
        'Check for player bullets collision
        for b in bullets
            if checkCollisionAABB(b.x,b.y,2,4,x,y,8,8) then
                state = state - 1
                remove(bullets, index_of(bullets, b))
            endif
        next
        'Check for alien bullets collision
        for b in aliensBullets
            if checkCollisionAABB(b.x,b.y,2,4,x,y,8,8) then
                state = state - 1
                breakType = 16 'flip Horizontala
                remove(aliensBullets, index_of(aliensBullets, b))
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
            draw.sprite(state+2,x,y,0, breakType)
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
    st = 0
    dw = 0

    def setup()
        x = 0
        y = 0
        shape = 0
        dead = 0
        deadCounter = 0
        st = 0
        dw = 0
    enddef

    def shot()
        if len(aliensBullets) < maxAlienBullets + wave then
            b = new (bullet)
            b.x = x + 7 
            b.y = y + 12
            b.speed = 1
            b.explosionFrame = 0
            push(aliensBullets, b)
            sfx.play(4, 97)
        endif
    enddef

    def update()

        if dead then
            deadCounter = deadCounter + 1
            return
        endif

        if rnd(0,500) = 0 then shot() endif

        if st then
            st = 0
            x = x + alienDir
        endif

        if dw then
            dw = 0
            y = y + alienDown 
        endif

        'Check for player bullets collision
        for b in bullets
            if checkCollisionAABB(b.x,b.y,2,4,x+2,y,11,8) then
                remove(bullets, index_of(bullets, b))
                dead = 1
                sfx.play(1,127)
                addScore((shape + 1) * 10)
            endif
        next

    enddef
    def draw()
        if dead then
            draw.sprite(16,x,y,4)
            draw.sprite(16,x+8,y,4,8)
        else
            an = alienStep mod 2 = 0 
            draw.sprite(2 * shape + 6 + an,x,y,3)
            draw.sprite(2 * shape + 6 + an,x+8,y,3,8)
        endif
    enddef
    def isDead()
        return deadCounter > 10
    enddef
endclass

class ship
    x = 320
    y = 14
    speed = -1
    deadCounter = 0
    priceList = list(50,100,150,200,250,300)
    price = 0
    dead = 0
    def update()
        if dead then
            deadCounter = deadCounter + 1
            return
        endif
        x = x + speed
        for b in bullets
            if checkCollisionAABB(b.x,b.y,2,4,x,y,16,8) then
                remove(bullets, index_of(bullets, b))
                dead = 1
                sfx.play(1,127)
                price = priceList(rnd(0,len(priceList)))
                addScore(price)
            endif
        next
    enddef
    def draw()
        if dead then
            if deadCounter < 10 then
                draw.sprite(16,x,y,4)
                draw.sprite(16,x+8,y,4,8)
                sfx.stop(3)
            elseif
                draw.text(intToText("%03i",price),x-4,y,1,3)
            endif
        endif
        if dead = 0 then
            draw.sprite(18,x,y,6)
            draw.sprite(18,x+8,y,6,8)
        endif
    enddef
endclass

'draw game objects
def addRock(x, y, shape)
    r = new (rock)
    r.setup()
    r.x = x * 8
    r.y = y * 8
    r.shape = shape
    push(rocks, r)
enddef

def addMetaRock(x, y)
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

def addAlien(x, y, shape)
    a = new (alien)
    a.setup()
    a.x = x * 20
    a.y = y * 12
    a.shape = shape
    push(aliens, a)
enddef

def makeAlienWave()
    aliens = list()
    size = wave + 9
    if size > 14 then 
        size = 14 
    endif
    for n = 14 - size to size
        addAlien(n,2,2)
        addAlien(n,3,1)
        addAlien(n,4,1)
        addAlien(n,5,0)
        addAlien(n,6,0)
    next
enddef

playerShip = new (player)

def startGame()

    alienShip = nil
    alienStep = 0
    wave = 0
    lives = 3
    score = 0
    gameOverTime = 0
    menuTime = 0

    playerShip.setup()

    rocks = list()
    bullets = list()
    aliensBullets = list()

    addMetaRock(3,18)
    addMetaRock(13,18)
    addMetaRock(23,18)
    addMetaRock(33,18)

    makeAlienWave()
    gameState = WAVE_INFO

enddef

'UI
def drawUI()
    draw.rect(0,0,320,13,0,7)
    draw.line(0,12,320,12,1,4)
    draw.line(0,196,320,196,2,15)
    draw.text(intToText("SCORE %06i",score), 3,3,1,3)
    draw.text(intToText("LIVES %i",lives), 128,3,1,3)
    draw.text(intToText("HIGH %06i",hiScore), 224,3,1,3)
enddef

def drawGameOver()
        for i = 0 to 20
            y = i * 2 + 70
            draw.line(0, y, 320, y, 1, 1)
        next
        draw.text("GAME OVER", 55, 75, 3, 4)
        if gameOverTime > 1000 then
            col = 13
            if sin(gameOverTime * 0.02) < 0 then
                col = 9
            endif
            draw.text("Press -ENTER- to exit.", 75, 102, 1, col)
        endif
enddef

def drawWaveInfo()

    height = waveInfoTime * 0.03
    if height > 50 then height = 50 endif
    draw.rect(0,85-height * 0.5,320,height,0,8)
    draw.rect(-10,85-height*0.5,350,height,3,10)
    draw.text(intToText("WAVE > %i", wave), 53, 77, 3, 10)
    draw.text(intToText("WAVE > %i", wave), 57, 73, 3, 10)
    draw.text(intToText("WAVE > %i", wave), 57, 77, 3, 10)
    draw.text(intToText("WAVE > %i", wave), 53, 73, 3, 10)
    draw.text(intToText("WAVE > %i", wave), 55, 75, 3, 0)

enddef

def alienInfo(x,y,sp,txt,col)
        draw.sprite(sp,x,y,col)
        draw.sprite(sp,x+8,y,col,8)
        draw.text(txt, x+24,y,1,3)
enddef

def drawMenu()
        for i = 0 to 15
            y = i * 2
            draw.line(0, y, 320, y, 1, 7)
        next
        draw.text("INVADERS", 68, 6, 3, 5)
        draw.text("INVADERS", 66, 4, 3, 4)
        if menuTime > 1000 then
            col = 13
            if sin(menuTime * 0.02) < 0 then
                col = 9
            endif
            draw.text("Press -ENTER- to start.", 80, 182, 1, col)
        endif

        if menuTime > 3000 then
            alienInfo(120,100,10,"x 30 pts.",3)
        endif
        if menuTime > 4000 then
            alienInfo(120,120,8,"x 20 pts.",3)
        endif
        if menuTime > 5000 then
            alienInfo(120,140,6,"x 10 pts.",3)
        endif
        if menuTime > 6500 then
            alienInfo(120,70,18," mystery.",6)
        endif

enddef

'Updates
def alienMovement()
    alienTick = alienTick + 1
    stepSpeed = len(aliens)/(2 + (wave * 0.8))
    if stepSpeed < 2 then
        stepSpeed = 2
    endif
    if alienTick > stepSpeed then
        alienTick = 0
        alienStep = alienStep + 1
       
        sfx.stop(2)
        sfx.play(2,80)
        sfx.render(2, 44 - alienStep mod 4)

        maxX = 0
        minX = 320
        maxY = 0
        minY = 200
        for a in aliens
            a.st = 1
            if a.x > maxX then
                maxX = a.x
            endif 
            if a.x < minX then
                minX = a.x
            endif 
            if a.y > maxY then
                maxY = a.y
            endif
            if a.y < minY then
                minY = a.y
            endif
        next

        if maxX > 300 then
            alienDir = -4
            for a in aliens
                a.dw = 1
            next
        elseif minX < 2 then
            alienDir = 4
            for a in aliens
                a.dw = 1
            next
        endif
        if (maxY > 130 and len(rocks)) then
            rocks = list()
        endif
        if (maxY > 180) then
            gameState = GAME_OVER
        endif
    endif
enddef

def alienShipUpdate()
    alienShipTime = alienShipTime + delta()
    if alienShipTime > 6000 then
        alienShipTime = 0
        r = rnd(0,99)
        if r < 10 and alienShip = nil then
            alienShip = new (ship)
            alienShip.x = 320
            alienShip.dead = 0
            alienShip.deadCounter = 0
            sfx.play(3,90)
        endif
    endif

    if alienShip then
        alienShip.update()
        if alienShip.x < -16 or alienShip.deadCounter > 60 then 
            alienShip = nil
        endif
    endif
enddef

'Main update
def tick()

    if gameState = WAVE_INFO then
        waveInfoTime = waveInfoTime + delta()
        if waveInfoTime > 3000 then
            waveInfoTime = 0
            gameState = PLAYING
        endif
    endif

    if gameState = PLAYER_DEAD then
        recoveryPlayerTick = recoveryPlayerTick + delta()
        if recoveryPlayerTick > 4000 then
            recoveryPlayerTick = 0
            playerShip.dead = 0
            gameState = PLAYING
        endif
    endif

    if gameState = MENU then
        menuTime = menuTime + delta()
         if key.released(257) or pad.released(0, 15) and menuTime > 1000 then
            startGame()
         endif
    endif

    if gameState = GAME_OVER then
        gameOverTime = gameOverTime + delta()
        if key.released(257) or pad.released(0, 15) then
            gameState = MENU
        endif
    endif
    
    if gameState <> PLAYING then
        return
    endif

    alienShipUpdate()

    'update player
    playerShip.update()

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

    'update aliens bullets
    for ab in aliensBullets 
        ab.update()
    next
    
    if len(aliensBullets) then 
        ab = get(aliensBullets, 0) 
        if ab.isDead() then
            remove(aliensBullets, 0)
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

    'aliens movement
    alienMovement()

    if len(aliens) = 0 then
        gameState = WAVE_INFO
        wave = wave + 1
        makeAlienWave()
    endif 

enddef

'Main draw
def draw()

    cls(0)

    if gameState = MENU then
        drawMenu()
        return
    endif

    if gameState = WAVE_INFO then
        drawWaveInfo()
        return
    endif

    drawUI()

    playerShip.draw()

    if alienShip then
        alienShip.draw()
    endif

    for b in bullets
        b.draw() 
    next

    for ab in aliensBullets
        ab.draw() 
    next

    for r in rocks
        r.draw()
    next

    for a in aliens
        a.draw()
    next

    if gameState = GAME_OVER then
        drawGameOver()
    endif

enddef
