' Written by Fer (Spanish Archer) for Pery with love. 
' Remebering the sweet Amiga scene times.

' cube
let cube = list()
'AC/DC Thunderstruck from Felore (https://archeagemmllibrary.com/acdc-thunderstruck/)
let TS1 = "@0t134l16c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-bc-ac-g+c-ac-g+c-f+c-g+c-ec-f+c-d+c-ec-d+c-ec-d+c-ec-d+c-bc-ac-g+c-ac-g+c-f+c-g+c-ec-f+c-d+c-ec-d+c-ec-d+c-ec-d+c-bc-ac-g+c-ac-g+c-f+c-g+c-ec-f+c-d+c-ec-d+c-ec-d+c-ec-d+"
let TS2 = "@1t134l16O3c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-d+c-f+c-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-ec-gc-bc-ac-g+c-ac-g+c-f+c-g+c-ec-f+c-d+c-ec-d+c-ec-d+c-ec-d+c-bc-ac-g+c-ac-g+c-f+c-g+c-ec-f+c-d+c-ec-d+c-ec-d+c-ec-d+c-bc-ac-g+c-ac-g+c-f+c-g+c-ec-f+c-d+c-ec-d+c-ec-d+c-ec-d+"
'point 2d
class point2d
	var x = 0.0
	var y = 0.0
endclass

'aux point2d to use in routines without creating it (to avoid NEWS in draw or tick)
let auxp2d  = new (point2d)
let auxp2d1 = new (point2d)
let auxp2d2 = new (point2d)

' point3d
class point3d
	var x = 0.0
 	var y = 0.0
 	var z = 0.0
 	
  	var px = x
 	var py = y
 	var pz = z
 		
 	var npx = 0
 	var npy = 0
 	var npz = 0
 		
 	var fov = 200
 	
 	def transform(rot, pos, scale)
 		px = x
 		py = y
 		pz = z
 	
 		'rot z axis
 		npx = (px*cos(rot.z))-(py*sin(rot.z))
 		npy = (px*sin(rot.z))+(py*cos(rot.z))
 		px = npx
 		py = npy
 	
 		'rot y axix
 		npx = (px*cos(rot.y))-(pz*sin(rot.y))
 		npz = (px*sin(rot.y))+(pz*cos(rot.y))
 		px = npx
 		pz = npz
 	
 		'rot x axix
 		npy = (py*cos(rot.x))-(pz*sin(rot.x))
 		npz = (py*sin(rot.x))+(pz*cos(rot.x))
 		py = npy
 		pz = npz
 	
 		' pos + scale
 		px = (px*scale) + pos.x - camera.x
 		py = (py*scale) + pos.y - camera.y
 		pz = (pz*scale) + pos.z - camera.z
 	
 		fov = 200.0
 	
 		auxp2d.x = 160.0 + ((fov*px)/pz);
 		auxp2d.y = 100.0 - ((fov*py)/pz);
	enddef
endclass

' main camera
let camera = new (point3d)

'aux point2d to use in routines without creating it (to avoid NEWS in draw or tick)
let auxp3d1 = new (point3d)
let auxp3d2 = new (point3d)
let auxp3dpos = new (point3d)
let auxp3drot = new (point3d)

' draw Line (from 2 point3d)
def drawLine (p1, p2, pos, rot, scale, color)
	p1.transform(rot, pos, scale)
	auxp2d1.x = auxp2d.x
	auxp2d1.y = auxp2d.y
	p2.transform(rot, pos, scale)
	auxp2d2.x = auxp2d.x
	auxp2d2.y = auxp2d.y
	line (auxp2d1.x, auxp2d1.y, auxp2d2.x, auxp2d2.y, 1, color)
enddef

' cube geometry
def cubeAddPoint (x, y, z)
	let p = new (point3d)
	p.x = x;
	p.y = y;
	p.z = z;
	push(cube, p)
enddef

def createCube ()
	cubeAddPoint (-1.0, -1.0, -1)
	cubeAddPoint (1.0, -1.0, -1)
	cubeAddPoint (-1.0,  1.0, -1)
	cubeAddPoint (1.0,  1.0, -1)
	cubeAddPoint (-1.0, -1.0, 1)
	cubeAddPoint (1.0, -1.0, 1)
	cubeAddPoint (-1.0,  1.0, 1)
	cubeAddPoint (1.0,  1.0, 1)
enddef

' draw Cube
def drawCube (pos, rot, scale)
	drawLine (cube(0),cube(1),pos, rot, scale,3)
	drawLine (cube(1),cube(3),pos, rot, scale,3)
	drawLine (cube(3),cube(2),pos, rot, scale,3)
	drawLine (cube(2),cube(0),pos, rot, scale,3)
	drawLine (cube(4+0),cube(4+1),pos, rot, scale,3)
	drawLine (cube(4+1),cube(4+3),pos, rot, scale,3)
	drawLine ( cube(4+3),cube(4+2),pos, rot, scale,3)
	drawLine (cube(4+2),cube(4+0),pos, rot, scale,3)
	drawLine (cube(0),cube(4),pos, rot, scale,3)
	drawLine (cube(1),cube(5),pos, rot, scale,3)
	drawLine (cube(2),cube(6),pos, rot, scale,3)
	drawLine (cube(3),cube(7),pos, rot, scale,3)
enddef

' Init
def init()
	camera.x = 0;
	camera.y = 5;
	camera.z = 0;

	createCube()
	chSet(0,TS1)
	chSet(1,TS2)
	chPlay(0)
	chPlay(1)
enddef

' Tick
def tick()
	camera.x = 10 * sin (frame/100)
	camera.y = 5 + 2 * sin (frame/200)
	camera.z = camera.z + 0.1
enddef

'draw LandScape
def drawLandScape()
	let bw = 2.5
	let pz = floor (camera.z / bw)
	let px = floor (camera.x / bw)
	auxp3dpos.x = (px*bw)
	auxp3dpos.y = 0
	auxp3dpos.z = (pz*bw) + 30

	auxp3drot.x = 0
	auxp3drot.y = 0
	auxp3drot.z = 0

	let scale = 1
	
	let nb = 15
	let offset = ((nb-1) * bw)/2
	for z = 0 to nb-1
		auxp3d1.x = -offset
		auxp3d1.y = 0
		auxp3d1.z = (z * bw) - offset
		auxp3d2.x = offset
		auxp3d2.y = 0
		auxp3d2.z = (z * bw) - offset
		drawLine (auxp3d1, auxp3d2, auxp3dpos, auxp3drot, scale,1)
	next z
	for x = 0 to nb-1
		auxp3d1.x = (x * bw) - offset
		auxp3d1.y = 0
		auxp3d1.z = offset
		auxp3d2.x = (x * bw) - offset
		auxp3d2.y = 0
		auxp3d2.z = -offset
		drawLine (auxp3d1, auxp3d2, auxp3dpos, auxp3drot, scale,1)
	next z
enddef

' Draw cubes
def drawCubes()
	for n = 1 to 5
		auxp3dpos.x = camera.x + (3 * sin (frame/80))
		auxp3dpos.y = 10 + (4 * sin (frame/60))
		auxp3dpos.z = camera.z + 20 + (5 * sin (frame/20))

		auxp3drot.x = frame / 80.0
		auxp3drot.y = frame / 100.0
		auxp3drot.z = frame / 100.0 * n
	
		auxp3dpos.y =  auxp3dpos.y - n * 5
		auxp3dpos.z =  auxp3dpos.z + n * 10
	
		drawCube (auxp3dpos, auxp3drot, 5)
	next
enddef

' Draw
def draw()
	cls (0)	
	drawLandScape()
	drawCubes()
enddef