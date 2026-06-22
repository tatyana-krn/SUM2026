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
  if (DrawPos.y + DrawPos.x > 30 * abs(sin(Time)))
    ;//discard;
 
  if (DrawPosOrg.x * DrawPosOrg.x + DrawPosOrg.z *  DrawPosOrg.z < 1 + 2 * abs(sin(Time)))
    ;//discard;
  /*       
 
  OutColor = DrawColor;
 
  OutColor = vec4(vec3(1, 0.8, 0.1) * dot(N, L) + vec3(1, 0.8, 1)  * dot(N, L1), 1);
  vec3 L = normalize(vec3(10 * sin(0.8 * Time), 1, 1));
  vec3 L1 = normalize(vec3(1, 1, 10 * sin(0.5 * Time + 1)));
 
  vec3 N = normalize(DrawNormal);
  vec3 color =
    vec3(1, 1, 0) * max(0.1, dot(N, L)) +
    vec3(0, 1, 1) * max(0.1, dot(N, L1));
  */
  /*
  color += vec3(0, 1, 0) * pow(abs(sin(3 * GlobalTime + 18 * DrawPos.x)), 300);
  color += vec3(0, 0, 1) * pow(abs(sin(3 * GlobalTime + -18 * DrawPos.z)), 300);
  color += vec3(1, 0, 0) * pow(abs(sin(3 * GlobalTime + -18 * DrawPos.y)), 300);
  */
 
  vec3 N = normalize(DrawNormal);
  vec3 L = normalize(1 + vec3(0 * sin(1 * Time), 1, 1));
 
  vec3 LPos = vec3(cos(Time) * 8, 10, sin(Time) * 8);
  L = normalize(LPos - DrawPos);
  vec3 D = normalize(vec3(0, 0, 0) - LPos);//vec3(0, -1, 0);
  float F = 1, a = radians(36.30), b = radians(27.0);
 
 
  vec3 color = vec3(1, 0, 0);
  vec3
    Ka = vec3(0.1),
    Kd = vec3(0.8),
    Ks = vec3(0.5);
  float Ph = 90;
 
  Ka = vec3(0.2295, 0.08825, 0.0275);
  Kd = vec3(0.5508, 0.2118, 0.066);
  Ks = vec3(0.580594, 0.223257, 0.0695701);
  Ph = 51.2;
 
  float ld = dot(-L, D), start = cos(a), end = cos(b);
  if (ld < start)
    F = 0;
  else if (ld < end)
    F = 1 - (ld - end) / (start - end);
  else
    F = 1;
 
  color = vec3(0);
  // Ka
  color += Ka;// * DrawColor.rgb;
 
  // Kd
  color += F * Kd * max(0, dot(N, L));// * DrawColor.rgb;
 
  // Ks
  vec3 V = normalize(DrawPos - CamLoc);
  vec3 R = reflect(V, N);
  color += F * Ks * max(0, pow(dot(R, L), Ph));
 
  OutColor = vec4(color, 1);
  OutColor = vec3(1, 0, 0, 1);
}