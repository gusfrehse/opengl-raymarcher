#version 440 core
layout (location = 0) in vec2 pos;

void main()
{
    gl_Position = vec4(pos, 0.0, 1.0);
}
arandomstring
#version 440 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}