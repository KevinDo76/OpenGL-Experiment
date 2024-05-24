#version 460 core
        
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 uNormal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_fragPos;

uniform mat4 u_MVP;
uniform mat4 u_Model;

void main()
{
    //gl_Position = vec4(position.x + offset, position.yzw);
    gl_Position = u_MVP * vec4(position, 1.0f);

    v_fragPos = vec3(u_Model * vec4(position, 1));
    v_TexCoord = texCoord;
    v_Normal = uNormal;
}
