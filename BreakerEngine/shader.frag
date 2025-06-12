#version 120
uniform sampler2D texture;

struct Particle
{
    vec2 rect_pos;     // position du rectangle dans la fenêtre (en pixels)
    vec2 rect_size;    // taille du rectangle (en pixels)
};

uniform Particle particles[255];
uniform int count;

bool PointInRectangle(vec2 _pos, vec2 pos, vec2 size)
{
    return (_pos.x >= pos.x &&
            _pos.x <= pos.x + size.x &&
            _pos.y >= pos.y &&
            _pos.y <= pos.y + size.y);
}

void main() {
    // Par défaut, pixel transparent
    gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    
    for (int i = 0; i < count; i++)
    {
        if(PointInRectangle(gl_FragCoord.xy, particles[i].rect_pos, particles[i].rect_size))
        {            
            vec2 local = (gl_FragCoord.xy - particles[i].rect_pos) / particles[i].rect_size;
            local = clamp(local, 0.0, 1.0);
            local.y = 1.0 - local.y;
            
            // Récupérer toute la couleur de la texture (avec alpha)
            vec4 texColor = texture2D(texture, local);
            
            // Si le pixel de la texture est transparent, on l'ignore
            if (texColor.a < 0.01) {
                continue; // Passer au rectangle suivant
            }
            
            gl_FragColor = texColor;
            return;
        }
    }
    
    // Si aucun rectangle ne contient ce pixel, il reste transparent
}