#version 330

out vec3 color;
in vec3 fragment_color;
in vec4 coord_out;
flat in vec3 start_pos;
flat in vec3 p1_out;
flat in vec2 v0_out;
flat in vec2 v1_out;
in vec3 vert_pos;
flat in float length_along_fs;
flat in float line_width_in;
flat in float should_remove_pixels;

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

struct Line2D
{
    float a;
    float b;
    float c;
};

Line2D lineFromPointAndNormalVec(vec2 p, vec2 n)
{
   Line2D line;
   line.a = n.x;
   line.b = n.y; 
   line.c = -(line.a * p.x + line.b * p.y);

   return line;
}

float evalLine(Line2D line, vec2 point)
{
   return line.a * point.x + line.b * point.y + line.c;
}

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

   Line2D line0 = lineFromPointAndNormalVec(p1_out.xy, v0_out);
   Line2D line1 = lineFromPointAndNormalVec(p1_out.xy, v1_out);

   float val0 = evalLine(line0, vert_pos.xy);
   float val1 = evalLine(line1, vert_pos.xy);

   color = fragment_color;
   // TODO: Can these calculations be done in the vertex shader, and val0 and val1
   // be interpolated?

   //if((val0 > 0.0) && (val1 > 0.0))
   // {
   if(should_remove_pixels > 0.5)
   {
      float dist = length(vert_pos - p1_out);
      if(dist > line_width_in)
      {
         discard;
         // color = vec3(0.0, 1.0, 0.0);
      }
   }
   // }
   

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
   // dist = length(dir) + length_along_fs;

   float gap_size_ = 0.05;
   float dash_size_ = 0.05;

   // float gap_size_ = 5.0;
   // float dash_size_ = 5.0;

   // if ((use_dash == 1) && (fract(dist) * 10.0 > 0.9) )
   /*if ((use_dash == 1) && (fract(dist / (dash_size_ + gap_size_)) > dash_size_ / (dash_size_ + gap_size_)) )
   {
      discard;
   }*/
   // color = fragment_color;

}
