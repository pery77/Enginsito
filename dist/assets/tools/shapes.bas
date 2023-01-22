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
	rIn = slider(rIn,20,2,100,100)
	rOut = slider(rOut,20,14,100,100)
	startAngle = slider(startAngle,20,30,100,980)
	endAngle = slider(endAngle,20,42,100,980)
	segment = slider(segment,20,54,100,100)
	style = slider(style,160,2,10,3)
	col = 11

    draw.ring(x,y,rIn,rOut,startAngle,endAngle,segment,style,col)

ENDDEF

DEF drawPoly()
    x = 160
    y = 100
    segment = slider(segment,20,54,100,100)
    rOut = slider(rOut,20,2,100,100)
    rotation = slider(rotation,20,30,100,360)
    lineThick = slider(lineThick,160,2,10,10)
    col = 10

    draw.poly(x,y,segment,rOut,rotation,lineThick,col)

ENDDEF

DEF draw()

    cls(1)

    draw.text(textformat("%i",mouse.x()),4,180,8,11)
    draw.text(textformat("%i",mouse.y()),4,190,8,11)

    'drawRing()
    drawpoly()

    drawmouse()

ENDDEF