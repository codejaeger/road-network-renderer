#version 410


out vec4 frag_color;
uniform sampler2D texture1;
in vec2 tex;
void main ()
{
      frag_color = texture(texture1, tex);
    
}
