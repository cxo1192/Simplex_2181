#version 330

in vec3 Color;

uniform vec3 compliment = vec3(-1,-1,-1);

out vec4 Fragment;

void main()
{
	Fragment = vec4(Color,1);
	
	if(compliment.r != -1.0 && compliment.g != -1.0 && compliment.b != -1.0)
	{
		compliment.r = (1 - compliment.r);
		compliment.g = (1 - compliment.g);
		compliment.b = (1-compliment.b);
		
		Fragment = vec4(compliment, 1);
	}
		

	return;
}