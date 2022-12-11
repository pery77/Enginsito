/*
[gd_resource type="ShaderMaterial" load_steps=3 format=2]

[ext_resource path="res://art/Grille_2.png" type="Texture" id=1]

[sub_resource type="Shader" id=1]
code = "/*
	This shader is based in:
	Libretro's crt-geom.glsl
	Source: https://github.com/libretro/glsl-shaders/blob/master/crt/shaders/crt-geom.glsl
	and
	libretro's crt-lottes.glsl
	Source: https://github.com/libretro/glsl-shaders/blob/master/crt/shaders/crt-lottes.glsl
	
	
	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2 of the License, or (at your option)
	any later version.
*/
/*
shader_type canvas_item;
render_mode blend_mix;
*/

#version 330

uniform bool enabled = true;

// Curvature Toggle
uniform bool useCurvature = true;
// Curvature Distance
uniform float curvatureDistance = 1.6;
uniform float barrelPower = 1.6;
// Curvature Radius
uniform float curvatureRadius = 2.0;
// Corner Size
uniform float cornersize = 0.03;
// Corner Smoothness
uniform float cornersmooth = 100.0;


// Horizontal Tilt
uniform float x_tilt = 0.0;
// Vertical Tilt

uniform float y_tilt= 0.0;
// Horiz. Overscan %
uniform float overscan_x = 100.0;
// Vert. Overscan %
uniform float overscan_y = 100.0;

uniform vec4 border_color;

uniform float vignetteIntensity = 0.25;
uniform float vignetteOpacity = 1.0;

// hardScan
uniform float hardScan = -8.0;
// hardPix
uniform float hardPix = -3.0;

// brightness boost
uniform float brightBoost = 1.0;
// bloom-x soft
uniform float hardBloomPix = -1.5;
// bloom-y soft
uniform float hardBloomScan = -2.0;
// bloom ammount
uniform float bloomAmount = 0.15;
// filter kernel shape
uniform float shape = 2.0;


uniform float hue = 0.0;
uniform float saturation = 1.0;
uniform float brightness = 1.0;
uniform float contrast = 1.0;
uniform float distortion = 0.001;
uniform float flicker = 0.001;
uniform float noiseAmount = 0.001;
uniform float noiseSize = 0.001;


uniform vec2 TextureSize = vec2(320,200);
uniform float GrilleSize = 3.0;
uniform sampler2D grille;
// maskDark
uniform float maskDark = 0.5;
// maskLight
uniform float maskLight = 1.5;
// shadowMask
uniform float shadowMask = 3.0;

//Blur
uniform float blur_directions = 5.0;
uniform float blur_quality = 9.0;
uniform float blur_size = 10.0;
uniform float blur_amount = 0.5;
uniform vec2 chromatic;

  vec2 overscan;
  vec2 aspect;
  vec3 stretch;
  vec2 sinangle;
  vec2 cosangle;

float TIME = 0.0;
in vec2 fragTexCoord;
vec2 UV = fragTexCoord;
vec2 FRAGCOORD = UV;
uniform sampler2D texture0;
float SCREEN_PIXEL_SIZE = 0.001;

out vec4 COLOR;

//Cuvature functions
float intersect(vec2 xy) {
	float d = curvatureDistance;
	float r = curvatureRadius;
	float A = dot(xy,xy)+d*d;
	float B = 2.0*(r*(dot(xy,sinangle)-d*cosangle.x*cosangle.y)-d*d);
	float C = d*d + 2.0*r*d*cosangle.x*cosangle.y;
	return (-B-sqrt(B*B-4.0*A*C))/(2.0*A);
}

vec2 bkwtrans(vec2 xy) {
	float R = curvatureRadius;
	float c = intersect(xy);
	vec2 point = vec2(c)*xy;
	point -= vec2(-R)*sinangle;
	point /= vec2(R);
	vec2 tang = sinangle/cosangle;
	vec2 poc = point/cosangle;
	float A = dot(tang,tang)+1.0;
	float B = -2.0*dot(poc,tang);
	float C = dot(poc,poc)-1.0;
	float a = (-B+sqrt(B*B-4.0*A*C))/(2.0*A);
	vec2 uv = (point-a*sinangle)/cosangle;
	float r = max(abs(R*acos(a) ), 1e-5);
	return uv*r/sin(r/R);
}

vec2 fwtrans(vec2 uv) {
	float r = max(abs( sqrt(dot(uv,uv)) ), 1e-5);
	float R = curvatureRadius;
	uv *= sin(r/R)/r;
	float x = 1.0-cos(r/R);
	float D = curvatureDistance/R + x*cosangle.x*cosangle.y+dot(uv,sinangle);
	return curvatureDistance*(uv*cosangle-x*sinangle)/D;
}

vec3 maxscale() {
	vec2 c = bkwtrans(-curvatureRadius * sinangle / (1.0 + curvatureRadius/curvatureDistance*cosangle.x*cosangle.y));
	vec2 a = vec2(0.5,0.5)*aspect;
	vec2 lo = vec2(fwtrans(vec2(-a.x,c.y)).x, fwtrans(vec2(c.x,-a.y)).y)/aspect;
	vec2 hi = vec2(fwtrans(vec2(+a.x,c.y)).x, fwtrans(vec2(c.x,+a.y)).y)/aspect;
	return vec3((hi+lo)*aspect*0.5,max(hi.x-lo.x,hi.y-lo.y));
}


const float PI = 3.141592653589;

vec2 transform(vec2 coord) {
	coord = (coord-vec2(0.5))*aspect*stretch.z+stretch.xy;
	return (bkwtrans(coord)/vec2(overscan_x / 100.0, overscan_y / 100.0)/aspect+vec2(0.5));
}
float corner(vec2 coord) {
	coord = (coord - vec2(0.5)) * vec2(overscan_x / 100.0, overscan_y / 100.0) + vec2(0.5);
	coord = min(coord, vec2(1.0)-coord) * aspect;
	vec2 cdist = vec2(cornersize);
	coord = (cdist - min(coord,cdist));
	float dist = sqrt(dot(coord,coord));
	return clamp((cdist.x-dist)*cornersmooth,0.0, 1.0);
}

float ToLinear1(float c) {
    return(c<=0.04045) ? c/12.92 : pow((c + 0.055)/1.055, 2.4);
}

vec3 ToLinear(vec3 c) {
    return vec3(ToLinear1(c.r), ToLinear1(c.g), ToLinear1(c.b));
}
float ToSrgb1(float c) { 
    return(c<0.0031308 ? c*12.92 : 1.055*pow(c, 0.41666) - 0.055);
}
vec3 ToSrgb(vec3 c) {
    return vec3(ToSrgb1(c.r), ToSrgb1(c.g), ToSrgb1(c.b));
}

// Nearest emulated sample given floating point position and texel offset.
// Also zero's off screen.
vec3 Fetch(sampler2D _TEXTURE, vec2 pos,vec2 off) {

	pos=(floor(pos*vec4(TextureSize, 1.0 / TextureSize).xy+off)+vec2(0.5,0.5))/vec4(TextureSize, 1.0 / TextureSize).xy;
	
	vec2 chrm = chromatic / TextureSize;
	float r = texture(_TEXTURE,vec2(pos.x - chrm.x, pos.y - chrm.y)).r;
	float g = texture(_TEXTURE,pos.xy).g;
	float b = texture(_TEXTURE,vec2(pos.x + chrm.x, pos.y + chrm.y)).b;
	
	return ToLinear(brightBoost * vec3(r,g,b));
}

// Distance in emulated pixels to nearest texel.
vec2 Dist(vec2 pos) {
	//pos = pos*SourceSize.xy;
	pos = pos*vec4(TextureSize, 1.0 / TextureSize).xy;
	
	return -((pos - floor(pos)) - vec2(0.5));
}

// 1D Gaussian.
float Gaus(float pos, float scale) {
	return exp2(scale*pow(abs(pos), shape));
}

// 3-tap Gaussian filter along horz line.
vec3 Horz3(sampler2D _TEXTURE, vec2 pos, float off) {
	vec3 b    = Fetch(_TEXTURE, pos, vec2(-1.0, off));
	vec3 c    = Fetch(_TEXTURE, pos, vec2( 0.0, off));
	vec3 d    = Fetch(_TEXTURE, pos, vec2( 1.0, off));
	float dst = Dist(pos).x;
	
	// Convert distance to weight.
	float scale = hardPix;
	float wb = Gaus(dst-1.0,scale);
	float wc = Gaus(dst+0.0,scale);
	float wd = Gaus(dst+1.0,scale);
	
	// Return filtered sample.
	return (b*wb+c*wc+d*wd)/(wb+wc+wd);
}

// 5-tap Gaussian filter along horz line.
vec3 Horz5(sampler2D _TEXTURE, vec2 pos,float off) {
	vec3 a = Fetch(_TEXTURE, pos,vec2(-2.0, off));
	vec3 b = Fetch(_TEXTURE, pos,vec2(-1.0, off));
	vec3 c = Fetch(_TEXTURE, pos,vec2( 0.0, off));
	vec3 d = Fetch(_TEXTURE, pos,vec2( 1.0, off));
	vec3 e = Fetch(_TEXTURE, pos,vec2( 2.0, off));
	
	float dst = Dist(pos).x;
	// Convert distance to weight.
	float scale = hardPix;
	float wa = Gaus(dst - 2.0, scale);
	float wb = Gaus(dst - 1.0, scale);
	float wc = Gaus(dst + 0.0, scale);
	float wd = Gaus(dst + 1.0, scale);
	float we = Gaus(dst + 2.0, scale);
	
	// Return filtered sample.
	return (a*wa+b*wb+c*wc+d*wd+e*we)/(wa+wb+wc+wd+we);
}

// 7-tap Gaussian filter along horz line.
vec3 Horz7(sampler2D _TEXTURE, vec2 pos,float off) {
	vec3 a = Fetch(_TEXTURE, pos, vec2(-3.0, off));
	vec3 b = Fetch(_TEXTURE, pos, vec2(-2.0, off));
	vec3 c = Fetch(_TEXTURE, pos, vec2(-1.0, off));
	vec3 d = Fetch(_TEXTURE, pos, vec2( 0.0, off));
	vec3 e = Fetch(_TEXTURE, pos, vec2( 1.0, off));
	vec3 f = Fetch(_TEXTURE, pos, vec2( 2.0, off));
	vec3 g = Fetch(_TEXTURE, pos, vec2( 3.0, off));
	
	float dst = Dist(pos).x;
	// Convert distance to weight.
	float scale = hardBloomPix;
	float wa = Gaus(dst - 3.0, scale);
	float wb = Gaus(dst - 2.0, scale);
	float wc = Gaus(dst - 1.0, scale);
	float wd = Gaus(dst + 0.0, scale);
	float we = Gaus(dst + 1.0, scale);
	float wf = Gaus(dst + 2.0, scale);
	float wg = Gaus(dst + 3.0, scale);
	
	// Return filtered sample.
return (a*wa+b*wb+c*wc+d*wd+e*we+f*wf+g*wg)/(wa+wb+wc+wd+we+wf+wg);
}

// Return scanline weight.
float Scan(vec2 pos, float off) {
	float dst = Dist(pos).y;
	
	return Gaus(dst + off, hardScan);
}
  
// Return scanline weight for bloom.
float BloomScan(vec2 pos, float off) {
	float dst = Dist(pos).y;
	
	return Gaus(dst + off, hardBloomScan);
}

// Allow nearest three lines to effect pixel.
vec3 Tri(sampler2D _TEXTURE, vec2 pos) {
	vec3 a = Horz3(_TEXTURE, pos,-1.0);
	vec3 b = Horz5(_TEXTURE, pos, 0.0);
	vec3 c = Horz3(_TEXTURE, pos, 1.0);
	
	float wa = Scan(pos,-1.0); 
	float wb = Scan(pos, 0.0);
	float wc = Scan(pos, 1.0);
	
	return a*wa + b*wb + c*wc;
}
  
// Small bloom.
vec3 Bloom(sampler2D _TEXTURE, vec2 pos) {
	vec3 a = Horz5(_TEXTURE, pos,-2.0);
	vec3 b = Horz7(_TEXTURE, pos,-1.0);
	vec3 c = Horz7(_TEXTURE, pos, 0.0);
	vec3 d = Horz7(_TEXTURE, pos, 1.0);
	vec3 e = Horz5(_TEXTURE, pos, 2.0);
	
	float wa = BloomScan(pos,-2.0);
	float wb = BloomScan(pos,-1.0); 
	float wc = BloomScan(pos, 0.0);
	float wd = BloomScan(pos, 1.0);
	float we = BloomScan(pos, 2.0);
	
	return a*wa+b*wb+c*wc+d*wd+e*we;
}

// Shadow mask.
vec3 Mask(vec2 pos) {
	vec3 mask = vec3(maskDark, maskDark, maskDark);
	
	// Very compressed TV style shadow mask.
	if (shadowMask == 1.0) 
	{
	    float line = maskLight;
	    float odd = 0.0;
	    
	    if (fract(pos.x*0.166666666) < 0.5) odd = 1.0;
	    if (fract((pos.y + odd) * 0.5) < 0.5) line = maskDark;  
	    
	    pos.x = fract(pos.x*0.333333333);
	
	    if      (pos.x < 0.333) mask.r = maskLight;
	    else if (pos.x < 0.666) mask.g = maskLight;
	    else                    mask.b = maskLight;
	    mask*=line;  
	} 
	
	// Aperture-grille.
	else if (shadowMask == 2.0) 
	{
	    pos.x = fract(pos.x*0.333333333);
	
	    if      (pos.x < 0.333) mask.r = maskLight;
	    else if (pos.x < 0.666) mask.g = maskLight;
	    else                    mask.b = maskLight;
	} 
	
	// Stretched VGA style shadow mask (same as prior shaders).
	else if (shadowMask == 3.0) 
	{
	    pos.x += pos.y*3.0;
	    pos.x  = fract(pos.x*0.166666666);
	
	    if      (pos.x < 0.333) mask.r = maskLight;
	    else if (pos.x < 0.666) mask.g = maskLight;
	    else                    mask.b = maskLight;
	}
	
	// VGA style shadow mask.
	else if (shadowMask == 4.0) 
	{
	    pos.xy  = floor(pos.xy*vec2(1.0, 0.5));
	    pos.x  += pos.y*3.0;
	    pos.x   = fract(pos.x*0.166666666);
	
	    if      (pos.x < 0.333) mask.r = maskLight;
	    else if (pos.x < 0.666) mask.g = maskLight;
	    else                    mask.b = maskLight;
	}
	
	return mask;
}

vec3 Luminance(vec3 color){
	return vec3(dot(color, vec3(0.2126729,  0.7151522, 0.0721750)));
}

vec3 applyHue(vec3 aColor, float aHue){
	float angle = radians(aHue);
	vec3 k = vec3(0.57735, 0.57735, 0.57735);
	float cosAngle = cos(angle);
	return aColor * cosAngle + cross(k, aColor) * sin(angle) + k * dot(k, aColor) * (1.0 - cosAngle);
}

vec3 applyHSBEffect(vec3 startColor){
	vec3 outputColor = startColor;
	outputColor.rgb = applyHue(outputColor.rgb, hue * 360.0);
	outputColor.rgb = (outputColor.rgb - 0.5f) * contrast + 0.5f + (brightness - 1.0);
	outputColor.rgb = mix(Luminance(outputColor.rgb), outputColor.rgb, saturation);
	return outputColor;
}

//Noise
float rand(vec2 co){
	float speed = 0.001 * TIME;
	return fract((sin( dot(co.xy , vec2(100.0 * speed, 1.0 * speed) )) * 10000.0+ speed));
}
            
float noise (in vec2 st){
	vec2 i = floor(st);
	vec2 f = fract(st);

	// Four corners in 2D of a tile
	float a = rand(i);
	float b = rand(i + vec2(1.0, 0.0));
	float c = rand(i + vec2(0.0, 1.0));
	float d = rand(i + vec2(1.0, 1.0));

	// Smooth Interpolation
	// Cubic Hermine Curve.  Same as SmoothStep()
	vec2 u = f*f*(3.0-2.0*f);
	u = smoothstep(0.,1.,f);
	// Mix 4 coorners percentages
	return mix(a, b, u.x) +
       (c - a)* u.y * (1.0 - u.x) +
       (d - b) * u.x * u.y;
}

float vignette(vec2 uv){
	uv *= 1.0 - uv.xy;
	float vignette = uv.x * uv.y * 15.0;
	return clamp(pow(vignette, vignetteIntensity * vignetteOpacity),0.0, 1.0);
}

vec2 barrel(vec2 p, float BarrelPower){
    float theta  = atan(p.y , p.x);
    float radius = length(p);
    radius = pow(radius, BarrelPower);
    p.x = radius * cos(theta);
    p.y = radius * sin(theta);
    return 0.5 * (p + 1.0);
}

vec4 blur( sampler2D _Texture, vec2 uv ){
    float Pi2 = 6.28318530718; // Pi*2
   
    vec2 Radius = vec2(blur_size/TextureSize);

    // Pixel colour
    vec4 Color = texture(_Texture, uv);
    
    // Blur calculations
    for( float d=0.0; d<Pi2; d+=Pi2/blur_directions)
    {
		for(float i=1.0/blur_quality; i<=1.0; i+=1.0/blur_quality)
        {
			Color += texture( _Texture, uv+vec2(cos(d),sin(d))*Radius*i);
        }
    }
    
    Color /= blur_quality * blur_directions - 15.0;
    return Color;
}

void vertex() {

	overscan = vec2(1.00,1.00);

	aspect = vec2(1.0, 0.5625);
	
	const vec2 angle = vec2(0.0, 0.0);
	
	sinangle = sin(vec2(x_tilt, y_tilt)) + vec2(0.001);//sin(vec2(max(abs(x_tilt), 1e-3), max(abs(y_tilt), 1e-3)));
	cosangle = cos(vec2(x_tilt, y_tilt)) + vec2(0.001);//cos(vec2(max(abs(x_tilt), 1e-3), max(abs(y_tilt), 1e-3)));
	stretch = maxscale();
}

void main() {

vertex();

	vec2 xy = useCurvature ? transform(fragTexCoord.xy) : fragTexCoord.xy;
	float cval = useCurvature ? corner(xy) : 1.0;
	
	float x = sin(0.1*TIME+(xy.y*10.0))
    	* sin(0.2*TIME+(xy.y*80.01))
    	* cos(0.3*TIME+(xy.y*40.01));

    x *= distortion * distortion * 0.05;
	
	vec3 outColor = Tri(texture0, xy + x);
	vec3 bloom = Bloom(texture0, xy + x);
	vec4 blur = blur_amount > 0.0 ? blur(texture0, xy + x) : vec4(0.0);
	
	outColor += bloom * bloomAmount;
	
	outColor = applyHSBEffect(outColor.rgb);
	
	if (shadowMask > 0.0)
	    outColor *= Mask( FRAGCOORD.xy * 1.00001);
	else
	{
		if (GrilleSize > 0.0)
		{
			vec2 pos = fract(FRAGCOORD.xy * 0.04166 * GrilleSize);
			outColor *= texture(grille, pos).rgb + maskDark;
		}
		else
		{
			outColor *= texture(grille, UV * (1.0/SCREEN_PIXEL_SIZE) * 0.2).rgb + maskDark;
		}

		
	}
		
	/* Flicker */
	outColor *= (1.0 - flicker * 0.2 * (sin(50.0*TIME+xy.y*2.0)*0.5+0.5));
	
	/* Noise */
	float nA = noiseAmount*noiseAmount;
	outColor.rgb += mix( -nA, nA, noise(xy* 1.0/noiseSize*100.0));
	
	vec4 border = border_color * blur;
	outColor *= vignette(xy);
	
	/* Blur */
	outColor += mix (vec3(0.0) , blur.rgb * blur.rgb, blur_amount);
	
	if (enabled) 
	{
		COLOR = mix (border , vec4(ToSrgb(outColor.rgb), 1.0), cval); 
		COLOR = vec4(Tri(texture0, xy + x), 1); 
	}
	else
		COLOR = texture(texture0, fragTexCoord.xy);
		
} /*"

[resource]
shader = SubResource( 1 )
shader_param/enabled = true
shader_param/useCurvature = true
shader_param/curvatureDistance = 1.6
shader_param/barrelPower = 1.6
shader_param/curvatureRadius = 2.0
shader_param/cornersize = 0.03
shader_param/cornersmooth = 100.0
shader_param/x_tilt = 0.0
shader_param/y_tilt = 0.0
shader_param/overscan_x = 100.0
shader_param/overscan_y = 100.0
shader_param/border_color = Color( 0.152941, 0.14902, 0.14902, 1 )
shader_param/vignetteIntensity = 0.25
shader_param/vignetteOpacity = 1.0
shader_param/hardScan = -9.0
shader_param/hardPix = -3.0
shader_param/brightBoost = 1.2
shader_param/hardBloomPix = -1.5
shader_param/hardBloomScan = -2.0
shader_param/bloomAmount = 0.15
shader_param/shape = 2.0
shader_param/hue = 0.0
shader_param/saturation = 1.0
shader_param/brightness = 1.0
shader_param/contrast = 1.0
shader_param/distortion = 0.12
shader_param/flicker = 0.27
shader_param/noiseAmount = 0.14
shader_param/noiseSize = 0.21
shader_param/TextureSize = Vector2( 384, 216 )
shader_param/GrilleSize = 0.0
shader_param/maskDark = 0.5
shader_param/maskLight = 1.1
shader_param/shadowMask = 0.0
shader_param/blur_directions = 10.0
shader_param/blur_quality = 10.0
shader_param/blur_size = 3.0
shader_param/blur_amount = 0.2
shader_param/chromatic = Vector2( -0.947, -0.48 )
shader_param/grille = ExtResource( 1 )
*/