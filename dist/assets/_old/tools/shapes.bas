import "assets/lib/ui.bas"

x = 160;
y = 100;
rIn = 20;
rOut = 30;
startAngle = -90;
endAngle = 90;
segment = 10;
style = 0;
col = 11;

DEF drawRing()
	x = 160;
	y = 100;
	rIn = ui.slider(rIn,20,2,100,100)
	rOut = ui.slider(rOut,20,14,100,100)
	startAngle = ui.slider(startAngle,20,30,100,980)
	endAngle = ui.slider(endAngle,20,42,100,980)
	segment = ui.slider(segment,20,54,100,100)
	style = ui.slider(style,160,2,10,3)
	col = 11

    draw.ring(x,y,rIn,rOut,startAngle,endAngle,segment,style,col)

ENDDEF

DEF drawPoly()
    x = 160
    y = 100
    segment = ui.slider(segment,20,54,100,100)
    rOut = ui.slider(rOut,20,2,100,100)
    rotation = ui.slider(rotation,20,30,100,360)
    lineThick = ui.slider(lineThick,160,2,10,10)
    col = 10

    draw.poly(x,y,segment,rOut,rotation,lineThick,col)

ENDDEF

DEF drawrectr()
    x = 160
    y = 100
    segment = ui.slider(segment,20,2,100,100)

    rou = ui.slider(rou,20,30,100,10)
    lineThick = ui.slider(lineThick,160,2,10,10)
    col = 10

    draw.rectround(x,y,100,30,rou,segment,lineThick,col)

ENDDEF

DEF draw()

    cls(1)

    draw.text(intToText("%i,%i",mouse.x(),mouse.y()),4,190,1,11)

    'drawRing()
    'drawpoly()
    drawrectR()

    ui.drawmouse()

ENDDEF