attribute highp vec3 qt_Vertex;
attribute highp vec3 qt_Normal;
//attribute highp vec2 texCoord;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectMatrix;
varying vec4 vFragColor;
varying vec2 v_texCoord;
void main(void)
{
    //mat4 mvMatrix = u_viewMatrix*u_modelMatrix ;
    mat4 mvMatrix = u_viewMatrix *  inverse(u_modelMatrix) ;
    vec4 vColor = vec4(0.5, 0.5, 0.5, 1.0);
    mat3 mNormalMatrix;
    mNormalMatrix[0] = mvMatrix[0].xyz;
    mNormalMatrix[1] = mvMatrix[1].xyz;
    mNormalMatrix[2] = mvMatrix[2].xyz;
    vec3 vNorm = normalize(mNormalMatrix * qt_Normal);
    vec3 vLightDir = vec3(0.0,0.0, 1.0 ); 
    float fDot = max(0.0, 1.0*dot(vNorm, vLightDir)); 
    if(fDot > 1.0)
    {
        vFragColor.rgba = vec4(1.0,0.0,0.0,1.0);
    }
    else
    {
        vFragColor.rgb = vColor.rgb* fDot;
        vFragColor.a = 1.0;
    }
    // vFragColor.rgb = vColor.rgb ;
    // vFragColor.a = 1.0;
    //gl_Position = u_projectMatrix * u_viewMatrix * u_modelMatrix * vec4(qt_Vertex, 1.0f);

    gl_Position = u_projectMatrix * u_viewMatrix *  inverse(u_modelMatrix) * vec4(qt_Vertex, 1.0f);
    //gl_Position.z = gl_Position.z+0.5;//深度缓存实验

    //gl_Position = u_projectMatrix * u_viewMatrix * inverse(u_modelMatrix) * vec4(qt_Vertex, 1.0f);
    // v_texCoord = texCoord;
}



//uniform mat4 mvMatrix;
//uniform mat4 pMatrix;
//in vec3 vVertex;
//in vec3 vNormal;
//uniform vec4 vColor;
//varying vec4 vFragColor; 
//uniform vec4 vScale;
//uniform vec4 vOffset;
//void main(void) 
//{ 
// mat3 mNormalMatrix;
// mNormalMatrix[0] = mvMatrix[0].xyz;
// mNormalMatrix[1] = mvMatrix[1].xyz;
// mNormalMatrix[2] = mvMatrix[2].xyz;
// vec3 vNorm = normalize(mNormalMatrix * vNormal);
// vec3 vLightDir = vec3(0.0,0.0, 1.0 ); 
// float fDot = max(0.0, 1.0*dot(vNorm, vLightDir)); 
// if(fDot > 1.0)
//{
// vFragColor.rgba = vec4(1.0,0.0,0.0,1.0);
//}
//else
//{
// vFragColor.rgb = vColor.rgb* fDot;
// vFragColor.a = 1.0;
//}
// mat4 mvpMatrix;
//vec4 vvVerttex; 
//vvVerttex.x = (vVertex.x+vOffset.x)*vScale.x; 
//vvVerttex.y = (vVertex.y+vOffset.y)*vScale.y; 
//vvVerttex.z = (vVertex.z+vOffset.z)*vScale.z; 
//vvVerttex.w = 1.0; 
//mvpMatrix = pMatrix * mvMatrix;
//gl_Position = mvpMatrix * vvVerttex; 
//}"
