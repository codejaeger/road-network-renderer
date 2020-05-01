#version 410

in vec2 vPosition;
uniform mat4 uModelViewMatrix;
void main ()
{
  gl_Position = vec4(vPosition, 0.0f, 1.0f);
}

