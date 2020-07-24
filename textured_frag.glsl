#version 330 core

out vec4 FragColor;

in vec3 TexCoords;
in vec3 FragPos;

uniform vec4 Color;
uniform sampler2D Texture;


struct light
{
	vec3 Pos;
	vec3 Dir;
	
	float Radius;
};

uniform int StageEffect;
uniform int FlashLightActive;
uniform light Light;

void main()
{
	switch(StageEffect)
	{
		case 1:
		{
			vec3 Distance = FragPos - Light.Pos;
			if(abs(length(Distance)) < Light.Radius)
			{
				float Intensity = abs((length(Distance) - Light.Radius) / Light.Radius);
				
				vec4 TexColor = texture(Texture, TexCoords.xy) * Color * Intensity;
				if(TexColor.a < 0.1)
					discard;
				FragColor = TexColor;
			}
			else
			{
				FragColor = vec4(0, 0, 0, 1.0);			
			}
		} break;
		case 2:
		{
			vec4 TexColor = texture(Texture, TexCoords.xy) * Color;
			if(FragPos.y < 500)
			{
				float Alpha = (500 - FragPos.y) / 255.0f;
				TexColor.a -= Alpha + 0.3f;
				if(TexColor.a < 0.1)
					discard;
			}
				FragColor = TexColor;
		} break;
		default:
		{
			vec4 TexColor = texture(Texture, TexCoords.xy) * Color;
			if(TexColor.a < 0.1)
				discard;
			FragColor = TexColor;
		}
	}
}