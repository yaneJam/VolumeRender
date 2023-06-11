varying vec4 vFragColor; 
uniform vec4 vColor;
uniform sampler3D tex_3d;
uniform float scale;
uniform float shift;
uniform float dataType;//0 unsigned short  1 short 2 usigned char 3 char  4 float
varying vec3 v_texCoord;
void main(void) 
{ 
	//float ww = 18765;
	//float wc = (1632 + 20379) / 2.0;
	//scale = 256.0 / wc;
	//shift = wc-ww/2.0;
	//dataType = 0.0;

	//���Դ���3������texCoord�Ƿ�׼ȷ

	vec3 datapos = v_texCoord;// vec3(v_texCoord.x, v_texCoord.y, 0.9);
	float fsample = texture3D(tex_3d, datapos).r;
	if (dataType == 0.0)
	{
		fsample = fsample * 65536.0;
	}
	else if (dataType == 1.0)
	{
		fsample = fsample * 32768.0;
	}
	else if (dataType == 2.0)
	{
		fsample = fsample * 256;
	}
	else if (dataType == 3.0)
	{
		fsample = fsample * 128;
	}

	
	float gray = (fsample - shift) * scale;
	gray = gray / 256.0;
	vec4 color = vec4(gray, gray, gray, 1.0);
	//vec4 color2 = vec4(0.0, 0.0, 0.5, 1.0);
	//vec4 color =  texture3D(tex_3d, datapos);
	gl_FragColor = color;
	
	//���Դ���2���������������Ƿ�׼ȷ
	// 	vec3 datapos = vec3(0.5,0.5,0.5);
	//float fsample = texture3D(tex_3d, datapos).r; 
	//float target = 99.0;
	//if (fsample >= 1.0)
	//{
	//	gl_FragColor = vec4(0.5, 0.0, 0.0, 1.0);//��;
	//}
	//else if (fsample <= 0.0)
	//{
	//	gl_FragColor = vec4(0.5, 0.0, 0.0, 1.0);//��;
	//}
	// else  if (fsample > (target -0.5)/256.0 && fsample < (target + 0.5) /256.0)
	// {
	//	 gl_FragColor = vec4(0.0, 0.5, 0.0, 1.0);//��;
	// }
	//else
	//{
	//	gl_FragColor = vec4(0.0, 0.0, 0.5, 1.0);//��;
	//}
	

	//���Դ���1��������ɫ���Ƿ���Ч
	//gl_FragColor = vec4(0.0, 0.0, 0.5, 1.0);//��;

	
}


//
//#version 330
//uniform mat4 mvMatrix;
//uniform sampler1D colorTexture;
//uniform sampler1D opacityTexture;
//uniform sampler3D dataSetTexture;//������
//uniform sampler3D masktesture;//������
//uniform sampler2D depthTexture;//֡����
//uniform sampler2D depthcolorTexture;//֡����
//uniform vec3 boundingbox1;
//uniform vec3 boundingbox2;
//uniform float hasMask;
//uniform float X;//XYZ���ֵΪ1��������������Ϊ(-1,+1)
//uniform float Y;
//uniform float Z;
//uniform float nearest_plane;
//vec4 pos_origin;//
//vec4 pos_end;//
//vec3 pos_3origin;//��ǰ���ص�
//vec4 rayDir;
//vec3 rayDir3;
//uniform float rectx;
//uniform float recty;
//uniform float render_speed;
//varying vec2 vTex;
//void main()
//{
//	//֡�����Ӧ��ƬԪλ��
//	vec2 index = vec2(gl_FragCoord.x/rectx,gl_FragCoord.y/recty);
//	//��ʼ��ɫ
//	float remainOpacity = 1.0;
//
//
//	//����ͶӰ
//	//���߷���
//	rayDir = vec4(0.0,0.0,1.0,0.0);
//	rayDir = mvMatrix*rayDir;
//	rayDir3 = vec3(rayDir.x,rayDir.y,rayDir.z);
//	//��ֹ����
//	rayDir3.x = rayDir3.x==0.0?0.001:rayDir3.x;
//	rayDir3.y = rayDir3.y==0.0?0.001:rayDir3.y;
//	rayDir3.z = rayDir3.z==0.0?0.001:rayDir3.z;
//	//���߳�����
//	 pos_origin.x = vTex.x*2.0 - 100.0;
//	 pos_origin.y = vTex.y*2.0 - 100.0;
//	 pos_origin.z = -500.0;
//	 pos_origin.w = 1;
//	 pos_origin = mvMatrix * pos_origin;
//	 pos_3origin = vec3(pos_origin.x,pos_origin.y,pos_origin.z);
//
//
//	////��Χ��
//	//vec3 boxMin = vec3(-1.0, -1.0, -1.0);
//	//vec3 boxMax = vec3(1.0, 1.0, 1.0);
//	vec3 boxMin = boundingbox1;
//	vec3 boxMax = boundingbox2;
//	vec3 tbot = (boxMin - pos_3origin)/rayDir3;
//	vec3 ttop = (boxMax - pos_3origin)/rayDir3;
//	vec3 tmin = vec3(min(ttop.x,tbot.x), min(ttop.y,tbot.y), min(ttop.z,tbot.z));
//	vec3 tmax = vec3(max(ttop.x,tbot.x), max(ttop.y,tbot.y), max(ttop.z,tbot.z));
//	float largest_tmin  = max(max(tmin.x,tmin.y), max(tmin.x, tmin.z));
//	float smallest_tmax = min(min(tmax.x,tmax.y), min(tmax.x, tmax.z));
//
//	#ifdef CompositePerspective
//	#else
//	largest_tmin = max(largest_tmin,nearest_plane);
//	#endif
//
//	float tnear = largest_tmin;
//	float tfar = smallest_tmax;
//	if(tnear>=tfar)
//	{
//	 gl_FragColor =  texture(depthcolorTexture,index);
//	 return;
//	}
//
//
//	float fstep = 0.0010;
//	//float fstep = 0.254;
//	vec3 normal;
//	vec4 opacity;
//	vec4 color;
//	float depth;
//	vec4 destColor=vec4(0.0,0.0,0.0,0.0);
//
//	//depth=texture2D(depthTexture,index).r*10.0;//0.0��1.0
//	depth=texture2D(depthTexture,index).r*1000.0;//0.0��1000
//
//	//float tstep = 0.002;//��������4����2ǧ��
//
//	//if(true)//ֻ����ֲ��
//	//{
//	//color = texture(depthcolorTexture,index);
//	//gl_FragColor = color;
//	//return;
//	//}
//
//
//	//float tstep = 0.250*render_speed;
//	float tstep = 0.250;
//	if(render_speed == 0){tstep = 0.250;}
//
//	vec3 step = vec3(tstep*rayDir3.x,tstep*rayDir3.y,tstep*rayDir3.z);
//	pos_3origin += tnear*rayDir3;
//	float t = tnear;
//	 vec3 datapos;
//	for(int i = 0;i<2000;i++)
//	{
////֡���� ͼ���ں� �ж�
//	if(t>=depth)
//	{
//		color = texture(depthcolorTexture,index);
//	}
//	else
//	{
//			//������[(-1,-1,-1),(1,1,1)]����Ͷ�䵽[��0,0,0��,(1,1,1)]
//			// datapos.x = pos_3origin.x/X/2 + 0.5;//����������(-1,+1)ת��Ϊ�����е�(0,1)
//			// datapos.y = pos_3origin.y/Y/2 + 0.5;
//			// datapos.z = pos_3origin.z/Z/2 + 0.5;
//
//			 datapos.x = -pos_3origin.x/X + 0.5;//���ؿռ�ת��Ϊ���ݿռ�,CT���ݵ�x���Ƿ��ģ��÷���
//			 datapos.y = pos_3origin.y/Y + 0.5;
//			 datapos.z = pos_3origin.z/Z + 0.5;
//
//
//			//�߽��ж�
//			float fffsample;
//			if((datapos.x<0)||(datapos.x>1)||(datapos.y<0)||(datapos.y>1)||(datapos.z<0)||(datapos.z>1))
//			{
//				//fffsample = 0;
//				pos_3origin += step;
//				t+=tstep;
//				continue;
//			}
//			else
//			{
//			}
//
//				bool isInMask = true;
//				if (hasMask==1.0)
//				{
//					float fffmasksample = texture(masktesture,datapos).r;
//					if(fffmasksample >= (7.0-0.5)/256.0&&fffmasksample <= (7.0+0.5)/256.0)//0.5�ǲ�����Χ
//					{
//
//					}
//					else
//					{
//						isInMask = false;
//					}
//				}
//
//				if(isInMask == true)//0.5�ǲ�����Χ
//				{
//
//					fffsample = texture(dataSetTexture,datapos).r;
//
//
//					float value = (fffsample*32768)/3000.0;//20230207��DW��Ŀ ��Χ�ĳ�[-1000,3000]
//
//					opacity=texture(opacityTexture,value);
//					color=texture(colorTexture,value);
//					//opacity.a = 0.001;
//					color.r=color.r*opacity.r;
//					color.g=color.g*opacity.r;
//					color.b=color.b*opacity.r;
//					color.a=opacity.r;
//
//
//					if(color.a>0.10)//��Ӱ����
//					{
//						float fleft,fright,ftop,fbottom,ffront,fback;
//						vec3 temp = vec3(datapos.x-fstep,datapos.y,datapos.z);
//						fleft += texture(dataSetTexture,temp).r;
//						temp = vec3(datapos.x+fstep,datapos.y,datapos.z);
//						fright += texture(dataSetTexture,temp).r;
//						temp = vec3(datapos.x,datapos.y-fstep,datapos.z);
//						fbottom += texture(dataSetTexture,temp).r;
//						temp = vec3(datapos.x-fstep,datapos.y+fstep,datapos.z);
//						ftop += texture(dataSetTexture,temp).r;
//						temp = vec3(datapos.x,datapos.y,datapos.z-fstep);
//						fback += texture(dataSetTexture,temp).r;
//						temp = vec3(datapos.x,datapos.y,datapos.z+fstep);
//						ffront += texture(dataSetTexture,temp).r;
//						normal = vec3(fleft-fright,fbottom-ftop,ffront-fback);
//						if(length(normal)> 200.0 / 32768.0)//CTֵ�仯С��������
//						{
//
//							vec3 vNorm = normalize(normal);
//							vec3 vLightDir = normalize(rayDir3); 
//							float fDot = abs(dot(vNorm, vLightDir)); 
//							color.rgb = color.rgb *0.6+color.rgb * fDot*0.6 ;
//						}
//				}
//				else
//				{
//					color.rgb = color.rgb * 0.9;
//				}
//				color.a=opacity.r;
//			}
//
//
//		}
//		destColor=destColor+color*(1.0-destColor.a);
//		if(destColor.a>0.95)
//		{break;};
//		pos_3origin += step;
//		t+=tstep;
//		if(t>tfar)
//		{
//		color = texture(depthcolorTexture,index);//��Χ�к�ߵ��߲���
//		destColor=destColor+color*(1.0-destColor.a);
//		break;
//		}
//	}
//
//	gl_FragColor = destColor;
//	gl_FragColor.a = 1.0-remainOpacity;
//}
