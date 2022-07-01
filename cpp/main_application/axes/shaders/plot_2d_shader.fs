#version 330

out vec3 color;
in vec3 fragment_color;
in vec4 coord_out;
flat in vec3 start_pos;
flat in vec3 p1_out;
in vec3 vert_pos;
flat in float length_along_fs;
flat in float idx_out;

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

   // vec2 dir = (vert_pos.xy - start_pos.xy) * vec2(axes_width, axes_height) / 2.0;
   // float dist = length(dir);

   vec3 a = vert_pos - start_pos;
   vec3 b = p1_out - start_pos;

   vec3 a1 = (dot(a, b) / dot(b, b)) * b;

   // vec2 dir = (vert_pos.xy - start_pos.xy) * vec2(axes_width, axes_height) / 2.0;
   // vec2 dir = (a1.xy) * vec2(axes_width, axes_height) / 2.0;
   vec2 dir = (a1.xy);

   float dist;
   /*if(idx_out < 2.1)
   {
      float segment_len = length(start_pos - p1_out);
      dist = length(dir);
      // dist = length(dir) + length_along_fs - segment_len;
   }
   else
   {
      // dist = length(dir) + length_along_fs;
      dist = length(dir);
   }*/
   dist = length(dir) + length_along_fs;

   float gap_size_ = 0.05;
   float dash_size_ = 0.05;

   // float gap_size_ = 5.0;
   // float dash_size_ = 5.0;

   // if ((use_dash == 1) && (fract(dist) * 10.0 > 0.9) )
   if ((use_dash == 1) && (fract(dist / (dash_size_ + gap_size_)) > dash_size_ / (dash_size_ + gap_size_)) )
   {
      discard;
   }
   color = fragment_color;

}
