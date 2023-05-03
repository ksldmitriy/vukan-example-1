#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inZalupaPos;
layout(location = 3) in float inZalupaRot;

layout(location = 0) out vec3 fragColor;

mat2 rotate(float angle) {
  return mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
}

void main() {
  vec2 pos = inPosition * rotate(inZalupaRot) + inZalupaPos; 
  pos *= vec2(9, 16);
  pos /= 16.f;
  gl_Position = vec4(pos, 0.0, 1.0);
  fragColor = inColor;
}
