varying vec4 vFragColor; 
uniform vec4 vColor;
uniform sampler3D tex_3d;
uniform sampler2D tex_2d;
uniform sampler2D tex_depth;
uniform sampler1D tex_opacity;
uniform sampler1D tex_color;

uniform float tex_color_width;
uniform float tex_color_height;

uniform float scale;
uniform float shift;
uniform float dataType;//0 unsigned short  1 short 2 usigned char 3 char  4 float
varying vec2 v_texCoord;
varying vec4 rayDir;
varying vec4 camara_pos;
void main(void) 
{ 
	
	// gl_FragColor = vec4(1.0,0.0,0.0,0.0);
	// return;


	vec2 tex_datapos =  vec2((v_texCoord.x/2.0+0.5), (v_texCoord.y/2.0+0.5));
	float depthvec =  texture2D(tex_depth,tex_datapos).x;


	vec3 boxMin = vec3(-1.0,-1.0,-1.0);
    vec3 boxMax = vec3(1.0,1.0,1.0);
	float steplong = 0.005;
	if(rayDir.x==0){rayDir.x = 0.0001;}//防止除零
	if(rayDir.y==0){rayDir.y = 0.0001;}
	if(rayDir.z==0){rayDir.z = 0.0001;}
    vec3 rayStep = normalize(rayDir.xyz)*steplong;
    vec3 tbot = (boxMin - camara_pos.xyz)/rayStep;//与X1 Y1 Z1 三个平面相交所花的步数 有可能是负数
    vec3 ttop = (boxMax - camara_pos.xyz)/rayStep;//X2 Y2 Z2 三个平面相交所花的步数 有可能是负数
    vec3 tmin = vec3(min(ttop.x,tbot.x), min(ttop.y,tbot.y), min(ttop.z,tbot.z));//进入的XYZ平面
    vec3 tmax = vec3(max(ttop.x,tbot.x), max(ttop.y,tbot.y), max(ttop.z,tbot.z));//离开的XYZ平面
    float largest_tmin  = max(max(tmin.x,tmin.y), max(tmin.x, tmin.z));//只有当光线进入了所有的平面才算是真正进入了盒子中
    float smallest_tmax = min(min(tmax.x,tmax.y), min(tmax.x, tmax.z));//只要当光线离开了任意平面就算是真正离开了盒子

     
    float tnear = largest_tmin;
    float tfar = smallest_tmax;

	vec4 destColor = vec4(0.0,0.0,0.0,0.0);
	 if(tfar<tnear)	//largest_tmin>tfar意味着在不经过包围盒里边
	 {
		// if(largest_tmin == tmin.x)
		// {
		// 	destColor = vec4(tmin.x/50.0,0.0,0.0,1.0);		
		// }
		// else if(largest_tmin == tmin.y)
		// {
		// 	destColor = vec4(0.0,tmin.y/50.0,0.0,1.0);		
		// }
		// else
		// {
		// 	destColor = vec4(0.0,0.0,tmin.z/50.0,1.0);		
		// }

		vec4 color =  texture2D(tex_2d,tex_datapos);

		destColor = color;
	 }
	 else
	 {
		 float fmax = -1.0;
		float fsum = 0.0;
		float step = 500;
		//float opacity_sum = 0.0;
		
		// for(int i = 0 ; i < step; i++)
		// {
		// 	if(1.0 / step * i > depthvec)
		// 	{
		// 		break;
		// 	}
		// 	vec3 datapos = camara_pos + rayDir / step * i;

		for(int i = tnear ; i < tfar; i++)
		{
			if(  (rayStep.x/rayDir.x) * i > depthvec)//rayDir和depthvec都是是从[-1.0,1.0]映射到[0,1]的，看看以后怎么同步这个深度
			{
				//vec4 color =  texture2D(tex_2d,tex_datapos);
			
				//destColor = destColor + color*(1.0-destColor.a);

				//destColor =  color;
				break;
			}

			vec3 datapos = camara_pos+rayStep*i;
			datapos.x = datapos.x/2.0 + 0.5;//把[-0.5,0.5]区间[0,1.0]区间
			datapos.y = datapos.y/2.0 + 0.5;
			datapos.z = datapos.z/2.0 + 0.5;

			if((datapos.x<0.0)||(datapos.x>1.0)||(datapos.y<0.0)||(datapos.y>1.0)||(datapos.z<0)||(datapos.z>1.0))
			{
				continue;
			}
			
			//vec3 datapos = vec3(v_texCoord,i/500.0);	
			float fsample = texture3D(tex_3d, datapos).r;
		
			if (dataType == 0.0)
			{
				fsample = fsample * 65536.0;
			}
			else if (dataType == 1.0)
			{
				
				float ctValue = fsample * 32768.0;
				if(ctValue>4000||ctValue<-1000)
				{
					ctValue = 0.0;//无视大于4000的值
				}
				else
				{
					//float scale = 32768.0/4000;
					//float shift = 1 /4000;
					
					//传进来的signed short 负值都会变成0，然后被归一化
					float scale = 32768.0/2500;

					float shift = 0;//200.0/500.0;
					fsample = fsample*scale + shift ;


					//fsample = 0.5;
					float opacity =  texture1D(tex_opacity,fsample).r;
					//opacity_sum+=opacity;
					vec4 color =  texture1D(tex_color,fsample);
					color.r = color.r * opacity;
					color.g = color.g * opacity;
					color.b = color.b * opacity;
					color.a = opacity.r;

					destColor = destColor + color*(1.0-destColor.a);
					if(destColor.a>0.95)
					{
						break;
					}
				}
				
			}
			else if (dataType == 2.0)
			{
				fsample = fsample * 256;
			}
			else if (dataType == 3.0)
			{
				fsample = fsample * 128;
			}
			if(fsample > fmax)
			{
				fmax = fsample;
			}
			fsum += fsample;
		}
	 }

	
	
	// //最大密度
	// //float gray = (fmax - shift) * scale;
	// //投影成像
	// //fsum = fsum/20;//随便设置的
	// //float gray = (fsum - shift) * scale;
	// //gray = gray / 256.0;

	//if(  (rayStep.x/rayDir.x) * tfar > depthvec)//rayDir和depthvec都是是从[-1.0,1.0]映射到[0,1]的，看看以后怎么同步这个深度
	if(  destColor.a<0.95)
	{
		vec4 color =  texture2D(tex_2d,tex_datapos);	
		destColor = destColor + color*(1.0-destColor.a);
	}

	destColor.a = 1.0;
	
	gl_FragColor = destColor;
}