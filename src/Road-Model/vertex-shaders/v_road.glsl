#version 410

in vec4 vPosition;
uniform mat4 uModelViewMatrix;
out vec2 tex;
in vec2 texCoord;
void main ()
{
  gl_Position = uModelViewMatrix * vPosition;
    tex = texCoord;
}

