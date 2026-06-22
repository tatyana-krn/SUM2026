/**/
#version 460
 
layout(location = 0) out vec4 OutColor;
 
in vec4 DrawColor;
in vec3 DrawNormal;
in vec3 DrawPos;
in vec3 DrawPosOrg;
in vec2 OutTexCoord;


uniform vec3 Ka, Kd, Ks;
uniform float Ph;

uniform int IsTexture0;  
uniform sampler2D Texture0;                       
uniform vec3 CamLoc;
uniform float Time, GlobalTime;


void main( void )
{
/*
  if (DrawPos.y + DrawPos.x > 30 * abs(sin(Time)))
    ;//discard;
 
  if (DrawPosOrg.x * DrawPosOrg.x + DrawPosOrg.z *  DrawPosOrg.z < 1 + 2 * abs(sin(Time)))
    ;//discard;
*/ 
  vec3 N = normalize(DrawNormal);
  vec3 L = normalize(vec3(10, 5 * sin(1 * Time), 1));
  vec3 V = normalize(DrawPos - CamLoc);
  vec3 R = reflect(V, N);
 
  
  vec3 color = vec3(0);
  // Ka Kd Ks
  color += Ka + Kd * max(0, dot(N, L)) + Ks * max(0, pow(dot(R, L), Ph));
 
  vec4 texColor = vec4(1.0);
  if (IsTexture0 == 1)
    texColor = texture(Texture0, OutTexCoord);
  
  OutColor = texColor * vec4(color, 1.0) * DrawColor;                                                   
}
