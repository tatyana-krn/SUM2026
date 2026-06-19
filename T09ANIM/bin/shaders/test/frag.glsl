/**/
#version 330

layout(location = 0) out vec4 OutColor;
 
in vec4 DrawColor;
in vec3 DrawNormal;
in vec3 DrawPos;

uniform float Time;
 
void main( void )
{                                                         
  OutColor = DrawColor;
  vec3 L = normalize(vec3(10 * sin(8 *Time), 1, 1));
  vec3 L1 = normalize(vec3(1, 1, 10 * sin(5 * Time)));

  vec3 N = normalize(DrawNormal);
                                                 
  OutColor = vec4(vec3(1, 0.8, 0.1) * 0.50 * dot(N, L) + vec3(1, 0, 1) * dot(N, L1), 1);
}