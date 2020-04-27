#version 410

in vec4 vPosition;
in vec4 vColor;
out vec4 vColor1;
uniform mat4 uModelViewMatrix;
void main ()
{
  gl_Position = uModelViewMatrix * vPosition;
    vColor1 = vColor;
}

