#version 330

out vec4 color;
in vec3 fragment_color;
in vec4 coord_out;
in vec3 frag_pos;
in vec3 frag_normal;
flat in vec3 light_pos_in;
uniform int use_color;

uniform vec4 clip_plane0;
uniform vec4 clip_plane1;
uniform vec4 clip_plane2;
uniform vec4 clip_plane3;
uniform vec4 clip_plane4;
uniform vec4 clip_plane5;

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

   vec3 object_color = vec3(233.0 / 255.0, 116.0 / 255.0, 74.0 / 255.0);
   vec3 light_color = vec3(1.0, 1.0, 1.0);
   float ambient_strength = 1.0;
   vec3 ambient = ambient_strength * light_color;

   vec3 light_pos = vec3(6.0, 6.0, 6.0);
   vec3 light_dir = normalize(light_pos - frag_pos);
   float diff = max(dot(frag_normal, light_dir), 0.0);

   vec3 diffuse = diff * light_color;

   color = vec4((diffuse + ambient) * object_color, 1.0);
   color.r = min(color.r, 1.0);
   color.g = min(color.g, 1.0);
   color.b = min(color.b, 1.0);
   color.a = 1.0;

}
