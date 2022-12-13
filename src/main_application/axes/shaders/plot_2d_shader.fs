#version 330

out vec4 color;
in vec3 fragment_color;
in vec4 coord_out;
flat in vec3 p0_out;
flat in vec3 p1_out;
flat in float length_along_fs;
flat in float theta;
in vec3 fragment_pos;
flat in int triangle_id;

uniform int use_dash;

uniform float half_line_width;

uniform int use_clip_plane;
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

void main()
{
   if(use_clip_plane == 1)
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
   }

   color = vec4(fragment_color, 1.0);
   // color = fragment_color;

   if(triangle_id == 3)
   {
      float dist = length(fragment_pos - p1_out);
      if(dist > half_line_width)
      {
         discard;
      }
   }

   float gap_size_ = 0.05;
   float dash_size_ = 0.05;

   if(triangle_id < 2)
   {
      vec3 vec_to_fragment;
      vec3 vec_along_line;

      vec_to_fragment = fragment_pos - p0_out;
      vec_along_line = p1_out - p0_out;

      vec3 projected_vec = (dot(vec_to_fragment, vec_along_line) / dot(vec_along_line, vec_along_line)) * vec_along_line;

      vec2 dir = (projected_vec.xy);

      float dist = length(dir) + length_along_fs;

      if ((use_dash == 1) && (fract(dist / (dash_size_ + gap_size_)) > dash_size_ / (dash_size_ + gap_size_)) )
      {
         discard;
      }
   }
   else
   {
      vec3 vec_q = fragment_pos - p1_out;
      float phi = atan(vec_q.y, vec_q.x);
      float delta_angle = phi - theta;

      float radius = length(vec_q);

      float ratio = radius / half_line_width;

      float dist = delta_angle * half_line_width + length_along_fs;

      if (((use_dash == 1) && fract(dist / (dash_size_ + gap_size_)) > dash_size_ / (dash_size_ + gap_size_))) 
      {
         discard;
      }
   }

}
