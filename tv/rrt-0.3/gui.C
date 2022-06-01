//----------------------------------------------------------------------
//      File:           gui.C
//      Programmer:     Steve LaValle
//      Last modified:  9/18/00 (Release 0.3)
//      Description:    GUI for RRT planner
//----------------------------------------------------------------------
// Copyright (c) 1998-1999 Iowa State University and Steve LaValle.  
// All Rights Reserved.
// 
// This file and related documentation are part of the 
// Rapidly-Exploring Random Tree (RRT) software package.
// 
// Permission to use, copy, and distribute this software and its 
// documentation is hereby granted free of charge, provided that 
// (1) it is not a component of a commercial product, and 
// (2) this notice appears in all copies of the software and
//     related documentation. 
// 
// Iowa State University and the author make no representations
// about the suitability or fitness of this software for any purpose.  It is
// provided "as is" without express or implied warranty.
//----------------------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include <fstream>


#include <LEDA/system/file.h>
#include <LEDA/graph/graph.h>
#include <LEDA/core/list.h>
#include <LEDA/numbers/matrix.h>
// #include <Kernel/geo/plane_alg.h>
#include <LEDA/geo/polygon.h>
#include <LEDA/core/string.h>
#include <LEDA/numbers/vector.h>
#include <LEDA/graphics/window.h>
#include <LEDA/graphics/menu.h>
#include <LEDA/graphics/ps_file.h>

using namespace leda;

// Stuff that makes the LEDA plane algorithms happy
typedef point   POINT;
typedef segment SEGMENT;
typedef circle  CIRCLE;
typedef line    LINE;
typedef vector  VECTOR;
typedef double  COORD;



#include "model.h"
#include "rrt.h"
#include "gui.h"


Model2DPoint M("empty");
RRT R(&M);
RRTGui U(&R);


void RRTGui::NewPathName()
{
  window m(200,63);
  m.string_item("FilePath",rrt->m->FilePath);
  m.button("Done",51,ButtonHandle);
  m.display();
  m.read_mouse();
  m.close();
}


void RRTGui::DrawRRT()
{
  edge e;
  vector q1,q2;

  // Good for television
  w->set_line_width(LineWidth);

  // Show goal
  //q1 = rrt->m->StateToConfiguration(rrt->m->GoalState);
  //w->draw_disc(q1[0],q1[1],0.5,green);

  // Show first tree
  if (rrt->G.number_of_nodes() > 0) {
    q1 = rrt->m->StateToConfiguration(rrt->G.inf(rrt->G.first_node()));
    w->draw_disc(q1[0],q1[1],0.5,blue);
    forall_edges(e,rrt->G) {
      q1 = rrt->m->StateToConfiguration(rrt->G.inf(rrt->G.source(e)));
      q2 = rrt->m->StateToConfiguration(rrt->G.inf(rrt->G.target(e)));
      w->draw_segment(q1[0],q1[1],q2[0],q2[1],blue);
    }
  }
  // Show second tree (if it exists)
  if (rrt->GG.number_of_nodes() > 0) {
    q1 = rrt->m->StateToConfiguration(rrt->GG.inf(rrt->GG.first_node()));
    w->draw_disc(q1[0],q1[1],0.5,blue);
    forall_edges(e,rrt->GG) {
      q1 = rrt->m->StateToConfiguration(rrt->GG.inf(rrt->GG.source(e)));
      q2 = rrt->m->StateToConfiguration(rrt->GG.inf(rrt->GG.target(e)));
      w->draw_segment(q1[0],q1[1],q2[0],q2[1],red);
    }
  }

}


void RRTGui::DrawRRTNodes()
{
  node n;
  vector q;

  w->set_line_width(LineWidth);

  // Show root
  if (rrt->G.number_of_nodes() > 0) {
    forall_nodes(n,rrt->G) {
      q = rrt->m->StateToConfiguration(rrt->G.inf(n));
      w->draw_point(q[0],q[1],brown);
    }
  }
  // Show second tree (if it exists)
  if (rrt->GG.number_of_nodes() > 0) {
    forall_nodes(n,rrt->GG) {
      q = rrt->m->StateToConfiguration(rrt->G.inf(n));
      w->draw_point(q[0],q[1],brown);
    }
  }
}



void RRTGui::PSDrawRRT()
{
  edge e;
  polygon p;
  vector q1,q2;

  string fname;
  fname = rrt->m->FilePath + "edges.ps";
  window m(200,63);
  m.string_item("Filename",fname);
  m.button("Done",51,ButtonHandle);
  m.display();
  m.read_mouse();
  m.close();

  ps_file f(15.0,15.0,fname);
  //f.set_draw_bb(false);
  f.set_line_width(PSLineWidth);

  // Display the obstacles
  forall(p,rrt->m->Obst) {
    f.draw_filled_polygon(p,black);
  }

  // Show goal
  //q1 = rrt->m->StateToConfiguration(rrt->m->GoalState);
  //f.draw_disc(q1[0],q1[1],0.5);

  // Show root
  if (rrt->G.number_of_nodes() > 0) {
    q1 = rrt->m->StateToConfiguration(rrt->G.inf(rrt->G.first_node()));
    f.draw_disc(q1[0],q1[1],0.5);
  }
  forall_edges(e,rrt->G) {
    q1 = rrt->m->StateToConfiguration(rrt->G.inf(rrt->G.source(e)));
    q2 = rrt->m->StateToConfiguration(rrt->G.inf(rrt->G.target(e)));
    f.draw_segment(q1[0],q1[1],q2[0],q2[1]);
  }

  f.close();

}





void RRTGui::DrawVoronoi()
{
  node n;
  vector q;
  point *p;
  
  list<point> rlist;

  w->set_line_width(LineWidth);

 // Make a list of points
  forall_nodes(n,rrt->G) {
    q = rrt->m->StateToConfiguration(rrt->G.inf(n));
    p = new point(q[0],q[1]);
    rlist.push(*p);
  }

  GRAPH<CIRCLE,POINT> VD;
  // VORONOI(rlist,VD);
  color col = red;

  edge_array<bool> drawn(VD,false);

  edge e;
  forall_edges(e,VD) 
  { 
    if (drawn[e]) continue;

    drawn[VD.reversal(e)] = drawn[e] = true;

    node v = source(e);
    node n = target(e);

    if (VD.outdeg(v) == 1 && VD.outdeg(n) == 1)
      { LINE l = p_bisector(VD[v].point1(),VD[v].point3());
        w->draw_line(l.to_line(),col);
       }
    else
      if (VD.outdeg(v) == 1) 
         { POINT  cw  = VD[n].center();
           VECTOR vec = VD[v].point3() - VD[v].point1();
           POINT  cv  = cw + vec.rotate90();
           w->draw_ray(cw.to_point(),cv.to_point(),col);
          }
       else
         if (VD.outdeg(n) == 1) 
          { POINT  cv  = VD[v].center();
            VECTOR vec = VD[n].point3() - VD[n].point1();
            POINT  cw  = cv + vec.rotate90();
            w->draw_ray(cv.to_point(),cw.to_point(),col);
           }
         else 
          { POINT  cv  = VD[v].center();
            POINT  cw  = VD[n].center();
            w->draw_segment(cv.to_point(),cw.to_point(),col);
         }
            
  }  

}



void RRTGui::DrawHull()
{
  list<point> vlist,hull;
  node n;
  
  w->set_line_width(LineWidth);

  vlist.clear();
  forall_nodes(n,rrt->G) {
    vlist.push(rrt->G.inf(n));
  }

  // hull = CONVEX_HULL(vlist);
  w->draw_polygon(hull,green);

  std::cout << "Number of vertices in hull: " << hull.length() << "\n";
}


void RRTGui::PSDrawVoronoi()
{
  edge e;
  vector q1,q2;

  string fname;
  fname = rrt->m->FilePath + "voronoi.ps";
  window m(200,63);
  m.string_item("Filename",fname);
  m.button("Done",51,ButtonHandle);
  m.display();
  m.read_mouse();
  m.close();


  ps_file f(15.0,15.0,fname);
  //f.set_draw_bb(false);
  f.set_line_width(PSLineWidth);


  // Show root
  q1 = rrt->m->StateToConfiguration(rrt->G.inf(rrt->G.first_node()));
  f.draw_disc(q1[0],q1[1],0.5);
  forall_edges(e,rrt->G) {
    q1 = rrt->m->StateToConfiguration(rrt->G.inf(rrt->G.source(e)));
    q2 = rrt->m->StateToConfiguration(rrt->G.inf(rrt->G.target(e)));
    f.draw_segment(q1[0],q1[1],q2[0],q2[1],blue);
  }

  node n;
  vector q;
  point *p;
  ray r;

  list<point> rlist;

  // Make a list of points
  forall_nodes(n,rrt->G) {
    q = rrt->m->StateToConfiguration(rrt->G.inf(n));
    p = new point(q[0],q[1]);
    rlist.push(*p);
  }

  GRAPH<CIRCLE,POINT> VD;
  // VORONOI(rlist,VD);
  color col = red;

  edge_array<bool> drawn(VD,false);

  forall_edges(e,VD) 
  { 
    if (drawn[e]) continue;

    drawn[VD.reversal(e)] = drawn[e] = true;

    node v = source(e);
    node n = target(e);

    if (VD.outdeg(v) == 1 && VD.outdeg(n) == 1)
      { LINE l = p_bisector(VD[v].point1(),VD[v].point3());
        f.draw_line(l.to_line(),col);
       }
    else
      if (VD.outdeg(v) == 1) 
         { POINT  cw  = VD[n].center();
           VECTOR vec = VD[v].point3() - VD[v].point1();
           POINT  cv  = cw + vec.rotate90();
	   // Should be ray
	   r = ray(cw.to_point(),cv.to_point());
           f.draw_segment(cw.to_point().xcoord(),
			  cw.to_point().ycoord(),
			  cv.to_point().xcoord()+10000.0*cos(r.direction()),
			  cv.to_point().ycoord()+10000.0*sin(r.direction()),
			  col);
          }
       else
         if (VD.outdeg(n) == 1) 
          { POINT  cv  = VD[v].center();
            VECTOR vec = VD[n].point3() - VD[n].point1();
            POINT  cw  = cv + vec.rotate90();
	    // Should be ray
	    r = ray(cv.to_point(),cw.to_point());
	    f.draw_segment(cv.to_point().xcoord(),
			   cv.to_point().ycoord(),
			   cw.to_point().xcoord()+10000.0*cos(r.direction()),
			   cw.to_point().ycoord()+10000.0*sin(r.direction()),
			   col);
           }
         else 
          { POINT  cv  = VD[v].center();
            POINT  cw  = VD[n].center();
            f.draw_segment(cv.to_point(),cw.to_point(),col);
         }
            
  }  

  f.close();

}




void RRTGui::DrawPath(color c = yellow)
{
  node n;
  polygon p;

  w->set_line_width(LineWidth);

  forall(n,Path) {
   list<polygon> trobot = rrt->m->TransformRobot(rrt->G.inf(n));
   forall(p,trobot) {
	w->draw_filled_polygon(p,c);
	w->draw_polygon(p,black);
    }
  }
}



void RRTGui::PSDrawPath()
{
  list<node> nl;
  node tn;
  polygon p;
  vector q;

  string fname;
  fname = rrt->m->FilePath + "path.ps";
  window m(200,63);
  m.string_item("Filename",fname);
  m.button("Done",51,ButtonHandle);
  m.display();
  m.read_mouse();
  m.close();

  ps_file f(15.0,15.0,fname);
  f.set_line_width(PSLineWidth);

  // Draw the obstacles
  forall(p,rrt->m->Obst) {
    f.draw_filled_polygon(p,black);
  }

  // Draw the whole tree
  if (DumpTrees) {
    vector q1,q2;
    edge e;
    forall_edges(e,rrt->G) {
      q1 = rrt->m->StateToConfiguration(rrt->G.inf(rrt->G.source(e)));
      q2 = rrt->m->StateToConfiguration(rrt->G.inf(rrt->G.target(e)));
      f.draw_segment(q1[0],q1[1],q2[0],q2[1]);
    }
    if (rrt->GG.number_of_nodes() > 0) {
      forall_edges(e,rrt->GG) {
	q1 = rrt->m->StateToConfiguration(rrt->GG.inf(rrt->GG.source(e)));
	q2 = rrt->m->StateToConfiguration(rrt->GG.inf(rrt->GG.target(e)));
	f.draw_segment(q1[0],q1[1],q2[0],q2[1]);
      }
    }
  }
  
  //f.set_line_width(2.0);
  if (Path.length() > 0) {
    forall(tn,Path) {
      list<polygon> trobot = rrt->m->TransformRobot(rrt->G.inf(tn));
      forall(p,trobot) {
	f.draw_filled_polygon(p,yellow);
	f.draw_polygon(p,black);
      }
    }
  }
  else {  // Just show the initial and final
    list<polygon> trobot = rrt->m->TransformRobot(rrt->m->InitialState);
    forall(p,trobot) {
      f.draw_filled_polygon(p,grey2);
      f.draw_polygon(p,black);
    }
    trobot = rrt->m->TransformRobot(rrt->m->GoalState);
    forall(p,trobot) {
      f.draw_polygon(p,black);
    }
  }

  f.close();

}



void RRTGui::AnimatePath()
{
  node n;
  polygon p;

  // Show initial and goal states, and wait
  list<polygon> trobot = rrt->m->TransformRobot(rrt->m->InitialState);
  forall(p,trobot) {
    w->draw_filled_polygon(p,red);
    w->draw_polygon(p,black);
  }
  trobot = rrt->m->TransformRobot(rrt->m->GoalState);
  forall(p,trobot) {
    w->draw_filled_polygon(p,green);
    w->draw_polygon(p,black);
  }
  
  if (Path.length() > 0) {
    w->set_mode(xor_mode);
    std::cout << "Click any mouse button to continue\n";
    w->read_mouse();

    forall(n,Path) {
      list<polygon> trobot = rrt->m->TransformRobot(rrt->G.inf(n));
      forall(p,trobot) {
	w->draw_filled_polygon(p,yellow);
	w->draw_polygon(p,black);
      }
      leda_wait(0.05);
      forall(p,trobot) {
	w->draw_filled_polygon(p,yellow);
	w->draw_polygon(p,black);
      }
    }
    
    // Show the last frame again
    list<polygon> trobot = rrt->m->TransformRobot(rrt->G.inf(Path.tail()));
    forall(p,trobot) {
      w->draw_filled_polygon(p,yellow);
      w->draw_polygon(p,black);
    }
 
    // Unshow the last frame
    forall(p,trobot) {
      w->draw_filled_polygon(p,yellow);
      w->draw_polygon(p,black);
    }

   w->set_mode(src_mode);

   // Put the goal configuration back
    trobot = rrt->m->TransformRobot(rrt->m->GoalState);
    forall(p,trobot) {
      w->draw_filled_polygon(p,yellow);
      w->draw_polygon(p,black);
    }
  }
  else {
    std::cout << "No path has been generated\n";
  }

}



void RRTGui::MakeAnimatedGif()
{
  node n;
  polygon p;
  int i;  // Frame index
  string fname,ofname;
  list<polygon> trobot;

  ofname = rrt->m->FilePath + "movie.gif";
  window m(200,63);
  m.string_item("Filename",ofname);
  m.button("Done",51,ButtonHandle);
  m.display();
  m.read_mouse();
  m.close();

  i = 1;

  if (Path.length() > 0) {
    // Dump the frames

    forall(n,Path) {
      fname = string("frame%04d.ps",i);
      ps_file f(GifSize,GifSize,fname);

      // Draw obstacles
      forall(p,rrt->m->Obst) {
	f.draw_filled_polygon(p,black);
      }

      // Show initial and goal states
      trobot = rrt->m->TransformRobot(rrt->m->InitialState);
      forall(p,trobot) {
	f.draw_filled_polygon(p,red);
	f.draw_polygon(p,black);
      }
      trobot = rrt->m->TransformRobot(rrt->m->GoalState);
      forall(p,trobot) {
	f.draw_filled_polygon(p,green);
	f.draw_polygon(p,black);
      }

      // Draw robot
      trobot = rrt->m->TransformRobot(rrt->G.inf(n));
      forall(p,trobot) {
	f.draw_filled_polygon(p,yellow);
	f.draw_polygon(p,black);
      }

      f.close();
      i++;
    }

    // Do the conversion and make the file
    cmd_ostream O("pstogif frame*.ps; "
                   "gifmerge -10 -l100 frame*.gif > " + ofname + " ;"
                   "rm frame*.ps; rm frame*.gif");
  }
  else {
    std::cout << "No path has been generated\n";
  }

}






void RRTGui::ResetWindow()
{
  polygon p;

  w->clear();
  forall(p,rrt->m->Obst) {
    w->draw_filled_polygon(p,black);
  }
}


// THIS should be changed to make a new RRT constuction
void RRTGui::ResetRRT()
{
  Path.clear();
  rrt->G.clear();
  rrt->GG.clear();
  if (rrt->m->InitialState.dim() == rrt->m->GoalState.dim())
    rrt->GoalDist = rrt->m->Metric(rrt->m->InitialState,rrt->m->GoalState);
  rrt->BestState = rrt->m->InitialState;
  ResetWindow();
}



void RRTGui::WriteRRT()
{
  string fname;
  fname = rrt->m->FilePath + "rrt";
  window m(200,63);
  m.string_item("Filename",fname);
  m.button("Done",51,ButtonHandle);
  m.display();
  m.read_mouse();
  m.close();

  std::ofstream fout(fname);
  rrt->G.write(fout);
  rrt->GG.write(fout);
}



void RRTGui::ReadRRT()
{
  string fname;
  fname = rrt->m->FilePath + "rrt";
  window m(200,63);
  m.string_item("Filename",fname);
  m.button("Done",51,ButtonHandle);
  m.display();
  m.read_mouse();
  m.close();

  std::ifstream fin(fname);
  rrt->G.read(fin);
  rrt->GG.read(fin);
  std::cout << "RRT from initial state has " << rrt->G.number_of_nodes() 
       << " nodes\n";
  std::cout << "RRT from goal state has " << rrt->GG.number_of_nodes() 
       << " nodes\n";
}




// This function is a "friend" of RRTGui
void ButtonHandle(int b)
{
  //std::cout << "Button " << b << "\n";
  window m(200,250);
  
  switch (b) {
  case 12: std::cout << "Read Inputs\n";
    U.rrt->m->ReadInputs();
    break;
  case 13: std::cout << "Read Initial State\n";
    U.rrt->m->ReadInitialState();
    std::cout << "  New Initial State: " << U.rrt->m->InitialState << "\n";
    break;
  case 14: std::cout << "Read Goal State\n";
    U.rrt->m->ReadGoalState();
    std::cout << "  New Goal State: " << U.rrt->m->GoalState << "\n";
    break;
  case 15: std::cout << "Change File Path\n";
    U.NewPathName();
    U.ResetRRT();
    break;
  case 22: std::cout << "Explore\n";
      U.rrt->Explore();
      U.DrawRRT();
    break;
  case 23: std::cout << "Plan\n";
    if (U.rrt->Plan(U.Gdist,U.Path))
       U.DrawPath(white);
    U.DrawRRT();
    break;
  case 24: std::cout << "Clear\n";
    U.rrt->G.clear();
    U.rrt->GG.clear();
    U.Path.clear();
    U.ResetWindow();
    break;
  case 31: std::cout << "Draw RRT Edges\n";
    U.DrawRRT();
    break;
  case 32: std::cout << "Draw RRT Nodes\n";
    U.DrawRRTNodes();
    break;
  case 33: std::cout << "Draw Voronoi Diagram\n";
    U.DrawVoronoi();
    break;
  case 34: std::cout << "Draw Path\n";
    U.DrawPath();
    break;
  case 35: std::cout << "Animate Path\n";
    U.AnimatePath();
    break;
  case 36: std::cout << "Reset Window\n";
    U.ResetWindow();
    break;
  case 37: std::cout << "Convex Hull\n";
    U.DrawHull();
    break;
  case 41: std::cout << "Write RRT Edges to PS\n";
    U.PSDrawRRT();
    break;
  case 42: std::cout << "Write Voronoi Edges to PS\n";
    U.PSDrawVoronoi();
    break;
  case 43: std::cout << "Write Path to PS\n";
    U.PSDrawPath();
    break;
  case 44: std::cout << "Write RRT\n";
    U.WriteRRT();
    break;
  case 45: std::cout << "Read RRT\n";
    U.ReadRRT();
    break;
  case 46: std::cout << "Make Animated GIF\n";
    U.MakeAnimatedGif();
    break;
  case 50: std::cout << "Open Settings Menu\n";
    m.double_item("DeltaT",U.rrt->DeltaT);
    m.double_item("Speed",U.rrt->m->Speed);
    m.int_item("NumNodes",U.rrt->NumNodes);
    m.double_item("Gdist",U.Gdist);
    m.double_item("GifSize",U.GifSize);
    m.double_item("LineWidth",U.LineWidth);
    m.double_item("PSLineWidth",U.PSLineWidth);
    m.bool_item("Include RRTs in PS Path",U.DumpTrees);
    m.button("Done",51,ButtonHandle);
    m.display();
    m.read_mouse();
    m.close();
    break;
  case 51: std::cout << "Done\n";
    break;
  case 101: std::cout << "Switch to 2DPoint Model\n";
    U.rrt->m = new Model2DPoint(U.rrt->m->FilePath);
    U.ResetRRT();
    break;
  case 102: std::cout << "Switch to 2DPointCar Model\n";
    U.rrt->m = new Model2DPointCar(U.rrt->m->FilePath);
    U.ResetRRT();
    break;
  case 103: std::cout << "Switch to 2DRigid Model\n";
    U.rrt->m = new Model2DRigid(U.rrt->m->FilePath);
    U.ResetRRT();
    break;
  case 104: std::cout << "Switch to 2DRigidCar Model\n";
    U.rrt->m = new Model2DRigidCar(U.rrt->m->FilePath);
    U.ResetRRT();
    break;
  case 105: std::cout << "Switch to 2DRigidCarForward Model\n";
    U.rrt->m = new Model2DRigidCarForward(U.rrt->m->FilePath);
    U.ResetRRT();
   break;
  case 106: std::cout << "Switch to 2DRigidCarSmooth Model\n";
    U.rrt->m = new Model2DRigidCarSmooth(U.rrt->m->FilePath);
    U.ResetRRT();
    break;
  case 107: std::cout << "Switch to 2DRigidCarSmoothTrailer Model\n";
    U.rrt->m = new Model2DRigidCarSmoothTrailer(U.rrt->m->FilePath);
    U.ResetRRT();
    break;
  case 108: std::cout << "Switch to 2DRigidCarSmooth2Trailers Model\n";
    U.rrt->m = new Model2DRigidCarSmooth2Trailers(U.rrt->m->FilePath);
    U.ResetRRT();
    break;
  case 109: std::cout << "Switch to 2DRigidCarSmooth3Trailers Model\n";
    U.rrt->m = new Model2DRigidCarSmooth3Trailers(U.rrt->m->FilePath);
    U.ResetRRT();
    break;
  case 110: std::cout << "Switch to 2DRigidDyncar Model\n";
    U.rrt->m = new Model2DRigidDyncar(U.rrt->m->FilePath);
    U.ResetRRT();
    break;
  case 112: std::cout << "Switch to 2DRigidDyncarNtire Model\n";
    U.rrt->m = new Model2DRigidDyncarNtire(U.rrt->m->FilePath);
    U.ResetRRT();
    break;
  case 111: std::cout << "Switch to 2DRigidLander Model\n";
    U.rrt->m = new Model2DRigidLander(U.rrt->m->FilePath);
    U.ResetRRT();
    break;
  case 113: std::cout << "Switch to 2DRigidChain Model\n";
    U.rrt->m = new Model2DRigidChain(U.rrt->m->FilePath);
    U.ResetRRT();
    break;
  case 116: std::cout << "Switch to Linear Model\n";
    U.rrt->m = new ModelLinear(U.rrt->m->FilePath);
    U.ResetRRT();
    break;
  case 117: std::cout << "Switch to Nonholonomic Integrator Model\n";
    U.rrt->m = new ModelNintegrator(U.rrt->m->FilePath);
    U.ResetRRT();
    break;
  case 201: std::cout << "Switch to RRT Planner\n";
    U.ResetRRT();
    U.rrt = new RRT(U.rrt->m); // Keep the old model, change the planner
    break;
  case 202: std::cout << "Switch to RRTGoalBias Planner\n";
    U.ResetRRT();
    U.rrt = new RRTGoalBias(U.rrt->m); // Keep the old model, change the planner
    break;
  case 203: std::cout << "Switch to RRTGoalPull Explorer\n";
    U.ResetRRT();
    U.rrt = new RRTGoalPull(U.rrt->m); // Keep the old model, change the planner
    break;
  case 204: std::cout << "Switch to RRTCon Explorer\n";
    U.ResetRRT();
    U.rrt = new RRTCon(U.rrt->m); // Keep the old model, change the planner
    break;
  case 205: std::cout << "Switch to RRTDual Planner\n";
    U.ResetRRT();
    U.rrt = new RRTDual(U.rrt->m); // Keep the old model, change the planner
    break;
  case 206: std::cout << "Switch to RRTExtExt Planner\n";
    U.ResetRRT();
    U.rrt = new RRTExtExt(U.rrt->m); // Keep the old model, change the planner
    break;
  case 207: std::cout << "Switch to RRTGoalZoom Planner\n";
    U.ResetRRT();
    U.rrt = new RRTGoalZoom(U.rrt->m); // Keep the old model, change the planner
    break;
  case 208: std::cout << "Switch to RRTPolar Planner\n";
    U.ResetRRT();
    U.rrt = new RRTPolar(U.rrt->m); // Keep the old model, change the planner
    break;
  case 209: std::cout << "Switch to RRTHull Planner\n";
    U.ResetRRT();
    U.rrt = new RRTHull(U.rrt->m); // Keep the old model, change the planner
    break;
  case 210: std::cout << "Switch to RRTExtCon (RRT-Connect) Planner\n";
    U.ResetRRT();
    U.rrt = new RRTExtCon(U.rrt->m); // Keep the old model, change the planner
    break;
  case 211: std::cout << "Switch to RRTConCon Planner\n";
    U.ResetRRT();
    U.rrt = new RRTConCon(U.rrt->m); // Keep the old model, change the planner
    break;
  case 212: std::cout << "Switch to RRTStar Planner\n";
    U.ResetRRT();
    U.rrt = new RRTStar(U.rrt->m); // Keep the old model, change the planner
    break;
  default: std::cout << "Function not implemented\n";
	     break;
  }
}



RRTGui::RRTGui(RRT *r)
{
  rrt = r;

  // Defaults
  Gdist = 1.0; 
  GifSize = 7.0;
  DumpTrees = false;
  LineWidth = 1.0;
  PSLineWidth = 1.0;

  // This is my preferred window size
  w = new window(600,637,"Rapidly-Exploring Random Trees          Steve LaValle, Iowa State, 1999");
  // This is one makes the video conversion easier
  //w = new window(600,450+37+50,"Rapidly-Exploring Random Trees          Steve LaValle, Iowa State, 1999");
  //w->init(-16.0,117.0,-5.0);

  m1 = new window();
  m2 = new window();
  m3 = new window();
  m4 = new window();
  mm1 = new window();
  mm2 = new window();
  polygon p;

  mm1->button("2DPoint",101,ButtonHandle);
  mm1->button("2DPointCar",102,ButtonHandle);
  mm1->button("2DRigid",103,ButtonHandle);
  mm1->button("2DRigidCar",104,ButtonHandle);
  mm1->button("2DRigidCarForward",105,ButtonHandle);
  mm1->button("2DRigidCarSmooth",106,ButtonHandle);
  mm1->button("2DRigidCarSmoothTrailer",107,ButtonHandle);
  mm1->button("2DRigidCarSmooth2Trailers",108,ButtonHandle);
  mm1->button("2DRigidCarSmooth3Trailers",109,ButtonHandle);
  mm1->button("2DRigidDyncar",110,ButtonHandle);
  mm1->button("2DRigidDyncarNtire",112,ButtonHandle);
  mm1->button("2DRigidLander",111,ButtonHandle);
  mm1->button("2DRigidChain",113,ButtonHandle);
  mm1->button("Linear",116,ButtonHandle);
  mm1->button("Nintegrator",117,ButtonHandle);
  m1->button("  Select Model Type",*mm1);

  //m1->button("Read Model Type",11,ButtonHandle);
  m1->button("Read Inputs",12,ButtonHandle);
  m1->button("Read Initial State",13,ButtonHandle);
  m1->button("Read Goal State",14,ButtonHandle);
  m1->button("Change File Path",15,ButtonHandle);
  //m1->button("Read Obst",15,ButtonHandle);
  w->button("Model",*m1);

  mm2->button("RRT",201,ButtonHandle);
  mm2->button("RRTGoalBias",202,ButtonHandle);
  mm2->button("RRTGoalPull",203,ButtonHandle);
  mm2->button("RRTCon",204,ButtonHandle);
  mm2->button("RRTPolar",208,ButtonHandle);
  mm2->button("RRTHull",209,ButtonHandle);
  mm2->button("RRTStar",212,ButtonHandle);
  mm2->button("RRTGoalZoom",207,ButtonHandle);
  mm2->button("RRTDual",205,ButtonHandle);
  mm2->button("RRTExtExt",206,ButtonHandle);
  mm2->button("RRTExtCon",210,ButtonHandle);
  mm2->button("RRTConCon",211,ButtonHandle);
  m2->button("  Select RRT Type",*mm2);
  m2->button("Explore",22,ButtonHandle);
  m2->button("Plan",23,ButtonHandle);
  m2->button("Clear RRT",24,ButtonHandle);
  w->button("Run",*m2);

  m3->button("RRT Edges",31,ButtonHandle);
  m3->button("RRT Nodes",32,ButtonHandle);
  m3->button("Voronoi Diagram",33,ButtonHandle);
  m3->button("Draw Path",34,ButtonHandle);
  m3->button("Animate Path",35,ButtonHandle);
  m3->button("Convex Hull",37,ButtonHandle);
  m3->button("Reset Window",36,ButtonHandle);
  w->button("Display",*m3);

  m4->button("RRT Edges to PS",41,ButtonHandle);
  m4->button("Voronoi Diagram to PS",42,ButtonHandle);
  m4->button("Write Path to PS",43,ButtonHandle);
  m4->button("Make Animated GIF",46,ButtonHandle);
  m4->button("Write RRT",44,ButtonHandle);
  m4->button("Read RRT",45,ButtonHandle);
  w->button("File",*m4);

  w->button("Settings",50,ButtonHandle);

  w->button("Exit",-1,exit);

  w->display();

  // Display the obstacles
  forall(p,rrt->m->Obst) {
    w->draw_filled_polygon(p,black);
  }
}


main(int argc, char **argv)
{ 
  if (argc >= 2) {
    U.rrt->m = new Model2DPoint(string(argv[1]));
    U.ResetRRT();
  }

  // Read the model type, if exists
  if (is_file(U.rrt->m->FilePath + "Model2DPoint"))
    ButtonHandle(101);
  if (is_file(U.rrt->m->FilePath + "Model2DPointCar"))
    ButtonHandle(102);
  if (is_file(U.rrt->m->FilePath + "Model2DRigid"))
    ButtonHandle(103);
  if (is_file(U.rrt->m->FilePath + "Model2DRigidCar"))
    ButtonHandle(104);
  if (is_file(U.rrt->m->FilePath + "Model2DRigidCarForward"))
    ButtonHandle(105);
  if (is_file(U.rrt->m->FilePath + "Model2DRigidCarSmooth"))
    ButtonHandle(106);
  if (is_file(U.rrt->m->FilePath + "Model2DRigidCarSmoothTrailer"))
    ButtonHandle(107);
  if (is_file(U.rrt->m->FilePath + "Model2DRigidCarSmooth2Trailers")) 
    ButtonHandle(108);
  if (is_file(U.rrt->m->FilePath + "Model2DRigidCarSmooth3Trailers")) 
    ButtonHandle(109);
  if (is_file(U.rrt->m->FilePath + "Model2DRigidDyncar")) 
    ButtonHandle(110);
  if (is_file(U.rrt->m->FilePath + "Model2DRigidDyncarNtire"))
    ButtonHandle(112);
  if (is_file(U.rrt->m->FilePath + "Model2DRigidLander"))
    ButtonHandle(111);
  if (is_file(U.rrt->m->FilePath + "Model2DRigidChain"))
    ButtonHandle(113);
  if (is_file(U.rrt->m->FilePath + "ModelLinear"))
    ButtonHandle(116);
  if (is_file(U.rrt->m->FilePath + "ModelNintegrator"))
    ButtonHandle(117);

  // Read the planner type, if exists
  if (is_file(U.rrt->m->FilePath + "RRT"))
    ButtonHandle(201);
  if (is_file(U.rrt->m->FilePath + "RRTGoalBias"))
    ButtonHandle(202);
  if (is_file(U.rrt->m->FilePath + "RRTGoalPull"))
    ButtonHandle(203);
  if (is_file(U.rrt->m->FilePath + "RRTCon"))
    ButtonHandle(204);
  if (is_file(U.rrt->m->FilePath + "RRTPolar"))
    ButtonHandle(208);
  if (is_file(U.rrt->m->FilePath + "RRTHull"))
    ButtonHandle(209);
  if (is_file(U.rrt->m->FilePath + "RRTStar"))
    ButtonHandle(212);
  if (is_file(U.rrt->m->FilePath + "RRTGoalZoom"))
    ButtonHandle(207);
  if (is_file(U.rrt->m->FilePath + "RRTDual")) 
    ButtonHandle(205);
  if (is_file(U.rrt->m->FilePath + "RRTExtExt"))
    ButtonHandle(206);
  if (is_file(U.rrt->m->FilePath + "RRTExtCon"))
    ButtonHandle(210);
  if (is_file(U.rrt->m->FilePath + "RRTConCon"))
    ButtonHandle(211);

  //float t = used_time();
  //U.rrt->NumNodes = 80;
  //U.rrt->Explore();
  //std::cout << "Time to build RRT: " << (double)used_time(t) << "\n"; 

  while(1) {  
    U.w->read_mouse();
  }
}






