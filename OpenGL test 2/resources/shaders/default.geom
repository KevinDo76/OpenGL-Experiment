#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 v_Normal;
out vec3 v_fragPos;
out vec2 v_TexCoord;

in DATA
{
    vec3 Normal;
    vec3 fragPos;
    vec2 texCoord;
    mat4 projection;
} data_in[];

void main()
{

    vec3 vector0 = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
    vec3 vector1 = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
    vec4 surfaceNormal = vec4(normalize(cross(vector0, vector1)),0);
    for (int i=0;i<3;i++)
    {
        gl_Position = data_in[i].projection * (gl_in[i].gl_Position);
        v_Normal = data_in[i].Normal;
        v_fragPos = data_in[i].fragPos;
        v_TexCoord = data_in[i].texCoord;
        EmitVertex();
    }

    EndPrimitive();
}