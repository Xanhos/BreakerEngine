#version 120

uniform sampler2D texture;

struct Particle
{
 vec2 rect_pos;     // position du rectangle dans la fenêtre (en pixels)
 vec2 rect_size;    // taille du rectangle (en pixels)

};

uniform Particle particles[256];
uniform int count;

bool PointInRectangle(vec2 _pos, vec2 pos, vec2 size)
{

	return (_pos.x >= pos.x &&
		_pos.x <= pos.x + size.x &&
		_pos.y >= pos.y &&
		_pos.y <= pos.y + size.y);
}

void main() {

for	(int i = 0; i< count;i++)
{

	if(PointInRectangle(gl_FragCoord.xy, particles[i].rect_pos, particles[i].rect_size))
	{			
		vec2 local = (gl_FragCoord.xy - particles[i].rect_pos) / particles[i].rect_size;
		local = clamp(local, 0.0, 1.0);  // ← ça évite les débordements
		local.y = 1.0 - local.y;
		gl_FragColor = texture2D(texture, local);
	}
}

}
