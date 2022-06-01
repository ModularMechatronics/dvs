//----------------------------------------------------------------------
//      File:           gui.h
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


void ButtonHandle(int b);

class RRTGui {
  // friend void ButtonHandle(int b);
 public:
  window *w,*m1,*m2,*m3,*m4,*m5,*mm1,*mm2;
  list<node> Path;
  double Gdist;  // Gap or goal distance for planners
  double GifSize;  // Size of animated gif
  bool DumpTrees;  // Add the RRTs to the PS file output
  double LineWidth;
  double PSLineWidth;
  RRT *rrt;
  RRTGui(RRT *r);  
  void NewPathName();
  void DrawRRT();
  void DrawRRTNodes();
  void PSDrawRRT();
  void DrawVoronoi();
  void DrawHull();
  void PSDrawVoronoi();
  void DrawPath(color c);
  void PSDrawPath();
  void AnimatePath();
  void MakeAnimatedGif();
  void ResetWindow();
  void ResetRRT();
  void WriteRRT();
  void ReadRRT();
};

