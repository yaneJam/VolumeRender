attribute highp vec3 qt_Vertex;
attribute highp vec3 qt_Normal;
attribute highp vec2 texCoord;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectMatrix;
varying vec4 vFragColor;
varying vec3 v_texCoord;
uniform float tmpValue;
void main(void)
{
    gl_Position = u_projectMatrix * u_viewMatrix * u_modelMatrix * vec4(qt_Vertex, 0.6f);
    v_texCoord = vec3(texCoord.x, texCoord.y, tmpValue);
}

//
//#version 330
//uniform mat4 pMatrix;
//uniform mat4 mvpMatrix;
//in vec4 vVertex;
//in vec2 vTexCoord0;
//varying vec2 vTex;
//void main(void) 
//{ 
//vTex = vTexCoord0;
// gl_Position = pMatrix * vVertex; 
//}

