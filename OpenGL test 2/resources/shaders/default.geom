#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 v_Normal;
out vec3 v_fragPos;
out vec2 v_TexCoord;
out mat3 v_TBN;

in DATA
{
    vec3 glPositionOriginal;
    vec3 Normal;
    vec3 fragPos;
    vec2 texCoord;
    mat4 projection;
    mat4 model;
    mat4 inverseModel;
} data_in[];

void main()
{

    vec3 vector0 = vec3(data_in[1].glPositionOriginal - data_in[0].glPositionOriginal);
    vec3 vector1 = vec3(data_in[2].glPositionOriginal - data_in[0].glPositionOriginal);
    vec4 surfaceNormal = vec4(normalize(cross(vector0, vector1)),0);


    vec2 deltaUV0 = data_in[1].texCoord - data_in[0].texCoord;
    vec2 deltaUV1 = data_in[2].texCoord - data_in[0].texCoord;
    float invDet = 1.0f /(deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

    vec3 tangent = vec3(invDet * (deltaUV1.y * vector0 - deltaUV0.y * vector1));
    vec3 bitangent = vec3(invDet * (-deltaUV1.x * vector0 + deltaUV0.x * vector1));

    vec3 T = normalize(vec3(data_in[0].model * vec4(tangent,0.f)));
    vec3 B = normalize(vec3(data_in[0].model * vec4(bitangent,0.f)));
    vec3 N = normalize(vec3(data_in[0].model * surfaceNormal));
    

    mat3 TBN = mat3(T,B,N);

    //TBN = transpose(TBN);

    for (int i=0;i<3;i++)
    {
        gl_Position = data_in[i].projection * (gl_in[i].gl_Position);
        v_Normal = data_in[i].Normal;
        v_fragPos = data_in[i].fragPos;
        v_TexCoord = data_in[i].texCoord;
        v_TBN = TBN;
        EmitVertex();
    }

    EndPrimitive();
}