/**/
#version 330 
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;
 
uniform mat4 MatrWVP;
uniform float Time;

out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPos;
 
void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition + vec3(0, 0.3 * sin(10), 0), 1);
  DrawColor = InColor;
  DrawNormal = InNormal;
  DrawPos = InPosition;
}