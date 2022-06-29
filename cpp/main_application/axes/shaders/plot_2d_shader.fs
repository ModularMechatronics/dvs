#version 330

out vec3 color;
in vec3 fragment_color;
in vec4 coord_out;
flat in vec3 start_pos;
in vec3 vert_pos;

uniform vec4 clip_plane0;
uniform vec4 clip_plane1;
uniform vec4 clip_plane2;
uniform vec4 clip_plane3;
uniform vec4 clip_plane4;
uniform vec4 clip_plane5;

uniform float axes_width;
uniform float axes_height;
uniform float dash_size;
uniform float gap_size;

uniform int use_dash;

void main()
{
   if(dot(clip_plane0, coord_out) < 0.0)
   {
      discard;
   }
   else if(dot(clip_plane1, coord_out) < 0.0)
   {
      discard;
   }
   else if(dot(clip_plane2, coord_out) < 0.0)
   {
      discard;
   }
   else if(dot(clip_plane3, coord_out) < 0.0)
   {
      discard;
   }
   else if(dot(clip_plane4, coord_out) < 0.0)
   {
      discard;
   }
   else if(dot(clip_plane5, coord_out) < 0.0)
   {
      discard;
   }

   vec2 dir = (vert_pos.xy - start_pos.xy) * vec2(axes_width, axes_height) / 2.0;
   float dist = length(dir);

   if ((use_dash == 1) && (fract(dist / (dash_size + gap_size)) > dash_size / (dash_size + gap_size)) )
   {
      discard;
   }
   color = fragment_color;

}
