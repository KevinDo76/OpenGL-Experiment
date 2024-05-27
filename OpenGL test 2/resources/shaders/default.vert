#version 460 core
        
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec3 vNormal;

//out vec2 v_TexCoord;
//out vec3 v_Normal;
//out vec3 v_fragPos;

out DATA
{
    vec3 glPositionOriginal;
    vec3 Normal;
    vec3 fragPos;
    vec2 texCoord;
    mat4 projection;
    mat4 model;
    mat4 inverseModel;
} data_out;

uniform mat4 u_MV;
uniform mat4 u_Projection;
uniform mat4 u_Model;
uniform mat4 u_InverseModel;

void main()
{
    gl_Position = u_MV * vec4(vPosition, 1.0f);

    //v_fragPos = vec3(u_Model * vec4(position, 1));
    //v_TexCoord = texCoord;
    //v_Normal = uNormal;

    data_out.Normal = vec3(u_InverseModel * vec4(vNormal, 1.f));
    data_out.fragPos = vec3(u_Model * vec4(vPosition, 1));
    data_out.texCoord = vTexCoord;
    data_out.projection = u_Projection;
    data_out.model = u_Model;
    data_out.inverseModel = u_InverseModel;
    data_out.glPositionOriginal = vPosition;
}
