#version 330

in vec3 fragment_color;
uniform int scatter_mode;

out vec3 color;


void main()
{
   // Default scatter_mode == 0 is square mode, which requires no modifications

   if (scatter_mode != -1)
   {
      if(scatter_mode == 1) // Circle
      {
         vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
         float squared_dist = dot(circCoord, circCoord);
         if ((squared_dist > 1.0) || (squared_dist < 0.4))
         {
               discard;
         }
      }
      else if(scatter_mode == 2) // Disc
      {
         vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
         float squared_dist = dot(circCoord, circCoord);
         if (squared_dist > 1.0)
         {
               discard;
         }
      }
      else if(scatter_mode == 3) // Plus
      {
         vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
         float f = 0.2;

         if (((circCoord.x < -f) && (circCoord.y < -f)) ||
               ((circCoord.x > f) && (circCoord.y < -f)) ||
               ((circCoord.x > f) && (circCoord.y > f)) ||
               ((circCoord.x < -f) && (circCoord.y > f)))
         {
               discard;
         }
      }
      else if(scatter_mode == 4) // Cross
      {
         vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
         float f = 0.2;

         if ( ((circCoord.x + circCoord.y) < -f) && ((circCoord.x - circCoord.y) < -f) ||
               ((circCoord.x + circCoord.y) > f) && ((circCoord.x - circCoord.y) < -f) ||
               ((circCoord.x + circCoord.y) > f) && ((circCoord.x - circCoord.y) > f) ||
               ((circCoord.x + circCoord.y) < -f) && ((circCoord.x - circCoord.y) > f) )
         {
               discard;
         }
      }
   }

   color = fragment_color;
}
