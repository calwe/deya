/**
  * Simple shader for loading textures onto objects
  */

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Colour;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec4 v_Colour;
out vec2 v_TexCoord;
flat out float v_TexIndex;
out float v_TilingFactor;
flat out int v_EntityID;

void main()
{
	v_Colour = a_Colour;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
    v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 colour;
layout(location = 1) out int colour2;

in vec4 v_Colour;
in vec2 v_TexCoord;
flat in float v_TexIndex;
in float v_TilingFactor;
flat in int v_EntityID;

uniform float u_TilingFactor;
uniform sampler2D u_Textures[32];

void main()
{
	colour = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Colour;
	// TODO: Branching for TextureSlot
    // vec4 texColour = v_Colour
    // switch(int(v_TexIndex))
	// case (0-31): texColour *= texture(u_Textures[0], v_TexCoord * v_TilingFactor); break; 

    colour2 = v_EntityID;
}
