# version 120 

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Mine is an old machine.  For version 130 or higher, do 
// in vec4 color ;  
// in vec4 mynormal ; 
// in vec4 myvertex ;
// That is certainly more modern

varying vec4 color ;
varying vec3 mynormal ; 
varying vec4 myvertex ; 

const int numLights = 10 ; 
uniform bool enablelighting ; // are we lighting at all (global).
uniform vec4 lightposn[numLights] ; // positions of lights 
uniform vec4 lightcolor[numLights] ; // colors of lights
uniform int numused ;               // number of lights used

// Now, set the material parameters.  These could be varying and/or bound to 
// a buffer.  But for now, I'll just make them uniform.  
// I use ambient, diffuse, specular, shininess as in OpenGL.  
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient ; 
uniform vec4 diffuse ; 
uniform vec4 specular ; 
uniform vec4 emission ; 
uniform float shininess ; 

void main (void) 
{       
    if (enablelighting) {       
        
        vec4 finalcolor ; 

        // Implementation of Fragment Shader
        
        finalcolor = ambient + emission;
        
        const vec3 eyepos = vec3(0,0,0) ; 
		vec4 _mypos = gl_ModelViewMatrix * myvertex ; 
		vec3 mypos = _mypos.xyz / _mypos.w ;
		vec3 eyedir = normalize(eyepos - mypos) ;

		vec3 normal = normalize((gl_ModelViewMatrixInverseTranspose*vec4(mynormal,0.0)).xyz) ; 
        
        //Iterate through all lights
        for(int i = 0; i < numused; i++){
        
            //Decalare variables that will be used for computation
            vec3 direction;
            vec3 halfVec;
            vec4 lightColor = lightcolor[i];
        
            //Directional lights
            if(lightposn[i][3] == 0){
                vec3 currentPos = lightposn[i].xyz;
                direction = normalize(currentPos);
                halfVec = normalize(direction + eyedir);
            }
            
            //Point lights
            else{
                vec4 currentPos = lightposn[i];
                vec3 position = currentPos.xyz / currentPos.w;
                direction = normalize(position - mypos);
                halfVec = normalize(direction + eyedir);
            }
            
            //Lambert
            float nDotL = dot(normal, direction);
            vec4 lambert = diffuse * lightColor * max(nDotL, 0.0);
            
            //Phong
            float nDotH = dot(normal, halfVec);
            vec4 phong = specular * lightColor * pow(max(nDotH, 0.0), shininess);
            
            vec4 lightContribution = lambert + phong;
            
            finalcolor += lightContribution;
        }
        
        gl_FragColor = finalcolor ; 
        }
    else gl_FragColor = color ; 
}
