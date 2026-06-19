/**/
#version 330
 
layout(location = 0) out vec4 OutColor;
 
in vec4 DrawColor;
in vec3 DrawNormal;
in vec3 DrawPos;
in vec3 DrawPosOrg;
 
uniform vec3 CamLoc;
uniform float Time, GlobalTime;
 
void main( void )
{                                     
  vec3 N = normalize(DrawNormal);
  vec3 L = normalize(1 + vec3(0 * sin(1 * Time), 1, 1));
     // Ks
  vec3 V = normalize(DrawPos - CamLoc);
  vec3 R = reflect(V, N);
  vec3 color = vec3(1 * sin(GlobalTime), 1 * sin(GlobalTime), 1 * cos(GlobalTime));
//  vec3 color = vec3(dot(R, L), 1 * sin(GlobalTime), 1 * cos(GlobalTime));
 

  OutColor = vec4(color, 1);
}