#version 460 core
        
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 uNormal;

//out vec2 v_TexCoord;
//out vec3 v_Normal;
//out vec3 v_fragPos;


out DATA
{
    vec3 Normal;
    vec3 fragPos;
    vec2 texCoord;
    mat4 projection;
} data_out;

uniform mat4 u_MV;
uniform mat4 u_Projection;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_MV * vec4(position, 1.0f);

    //v_fragPos = vec3(u_Model * vec4(position, 1));
    //v_TexCoord = texCoord;
    //v_Normal = uNormal;

    data_out.Normal = uNormal;
    data_out.fragPos = vec3(u_Model * vec4(position, 1));
    data_out.texCoord = texCoord;
    data_out.projection = u_Projection;
}
