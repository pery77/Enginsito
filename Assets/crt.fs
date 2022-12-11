#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;

//Properties
float distortion = 0.05; // 0.0 none - 0.05 good - 0.2 too much
float scanlineForce = 0.8; // 0.0 none - 0.6 good - 1.0 too dark
vec2 TextureSize = textureSize(texture0, 0); //Get game texture size ej. (320,240)

 //Glow
float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

float threshold = 0.9;
float brightnessPower = 1.0;
vec2 spread = vec2(0.5,0.25);
float glowPower = 0.25;

//One pixel
vec2 one = vec2(1.0/TextureSize.x, 1.0/TextureSize.y);

// Simulate a CRT gamma of 2.4.
#define inputGamma  2.2
// Compensate for the standard sRGB gamma of 2.2.
#define outputGamma 3.0
 
// Macros.
#define TEX2D(c) pow(texture2D(texture0, (c)), vec4(inputGamma)).xyz
#define PI 3.14159265359

vec2 calcDistortion(vec2 coord)
{
        float zoom = 1.0 - distortion;

        vec2 p1 = vec2(2.0 * coord - 1.0);
        vec2 p2 = p1 / (1.0 - distortion * length(p1));

        p2 = (p2 + 1.0) * 0.5;
        p2 = vec2(zoom * p2 + (distortion * 0.5));
        
        return p2;
}

 
// Calculate the influence of a scanline on the current pixel.
vec3 scanlineWeights(float distance, vec3 color)
{
        vec3 wid = 0.3 + 0.1 * pow(abs(color), vec3(3.0));
	vec3 weights = vec3(distance / wid) * scanlineForce;
	return 0.35 * exp(-weights * weights) / wid;
}


vec4 extractBrightness(vec2 coords)
{       
        vec4 source = texture(texture0, vec2(coords.x, coords.y));
	vec4 brightColor;

	float brightness = dot(source.rgb, vec3(0.2126, 0.7152, 0.0722));
    
	if(brightness > threshold)
                brightColor = vec4(source.rgb, 1.0);
        else
                brightColor = vec4(0.0, 0.0, 0.0, 0.0);

	return brightColor * brightnessPower;
}

float normpdf(in float x, in float sigma)
{
	return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}

vec3 blur(vec2 coords)
{
        //declare stuff
        const int mSize = 11;
	const int kSize = (mSize-1)/2;
	float kernel[mSize];
	vec3 final_colour = vec3(0.0);

        //create the 1-D kernel
        float sigma = 7.0;
	float Z = 0.0;
	for (int j = 0; j <= kSize; ++j)
	{
		kernel[kSize+j] = kernel[kSize-j] = normpdf(float(j), sigma);
	}
		
	//get the normalization factor (as the gaussian has been clamped)
	for (int j = 0; j < mSize; ++j)
	{
		Z += kernel[j];
	}
		
	//read out the texels
	for (int i=-kSize; i <= kSize; ++i)
	{
	        for (int j=-kSize; j <= kSize; ++j)
		{
			final_colour += kernel[kSize+j] * kernel[kSize+i] *
                        extractBrightness((coords + vec2(float(i) * one.x * spread.x, float(j) * one.y * spread.y))).rgb;
		}
	}

        return vec3(final_colour/(Z*Z)) * glowPower;
}

vec3 vignette(vec2 coord )
{
	vec2 uv = coord.xy;
        uv = uv * (1.0 - uv);
        float vig = pow(uv.x*uv.y*100.0, 0.15)-0.6;
        vig *= 1.5;
        vig = clamp(vig, 0.0, 1.0);  
        return vec3(vig); 
}

void main()
{
        // Texture coordinates of the texel containing the active pixel.
        vec2 xy =  (distortion > 0.0) ? calcDistortion(fragTexCoord) : fragTexCoord;
                
        vec2 ratio_scale = xy * TextureSize;
        vec2 uv_ratio = fract(ratio_scale);

        // Snap to the center of the underlying texel.
        xy.y = (floor(ratio_scale.y) + 0.5) / TextureSize.y;
        vec3 col  = TEX2D(xy);
        vec3 col2 = TEX2D(xy + vec2(0, one.y));

        // Calculate the influence of the current and next scanlines on
        // the current pixel.             
        vec3 weights  = scanlineWeights(uv_ratio.y, col);
        vec3 weights2 = scanlineWeights(1.0 - uv_ratio.y, col2);
        vec3 mul_res  = (col * weights + col2 * weights2).rgb;

        // dot-mask emulation:
        // Output pixels are alternately tinted green and magenta.
        vec3 dotMaskWeights = mix( vec3(1.0, 0.7, 1.0), vec3(0.7, 1.0, 0.7), floor(mod(gl_FragCoord.x, 2.0)));
        mul_res *= dotMaskWeights;

        //Add glow
        //mul_res += blur(xy).rgb; //BLUR IS VERY EXPENSIVE

        //Add Vignette
        mul_res *= vignette(fragTexCoord.xy);

        gl_FragColor = 	vec4(pow(mul_res, vec3(1.0 / outputGamma)), 1.0);
 }