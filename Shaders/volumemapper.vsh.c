attribute highp vec3 qt_Vertex;
attribute highp vec3 qt_Normal;
attribute highp vec2 texCoord;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectMatrix;
uniform mat4 u_dataMatrix;
varying vec4 vFragColor;
varying vec2 v_texCoord;
varying vec4 rayDir;
varying vec4 camara_pos;

void main(void)
{
    gl_Position = u_projectMatrix * vec4(qt_Vertex,1.0f);
    
    v_texCoord = vec2(gl_Position.x, gl_Position.y);
    float near = -1.0;//要和u_projectMatrix的nearfar一致
    float far = 1.0;
    
    //正交投影
    //vec4 ray_start = vec4(-texCoord.x, -texCoord.y,near,1.0);//用负号roate和poly保持一致，但translate不一致了
    vec4 ray_start = vec4(texCoord.x, texCoord.y,near,1.0);//不用负号translate和poly保持一致，但rotate相反
    
    //透视投影
    //vec4 ray_start = vec4(0.0,0.0,near,1.0/*是否可以平移*/);
    //vec4 ray_end = vec4(-texCoord.x, -texCoord.y,far,1.0);
    vec4 ray_end = vec4(texCoord.x, texCoord.y,far,1.0);

    ray_start =  u_modelMatrix * u_dataMatrix * ray_start;
    ray_end =  u_modelMatrix *  u_dataMatrix * ray_end;

    camara_pos = ray_start;
    rayDir = ray_end - ray_start;
}



