#version 330

in vec3 Color;

uniform vec3 SolidColor = vec3(-1,-1,-1);
uniform bool Compliment = false;

out vec4 Fragment;

void main()
{
	Fragment = vec4(Color,1);
	
	if(SolidColor.r != -1.0 && SolidColor.g != -1.0 && SolidColor.b != -1.0)
	{	
		Fragment = vec4(SolidColor, 1);
		
	}
	
	if(Compliment == true)
		Fragment = vec4(vec3(1.0) - Fragment.xyz, 1.0);
	
	return;
}