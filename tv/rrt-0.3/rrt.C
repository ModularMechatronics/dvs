//----------------------------------------------------------------------
//      File:           rrt.C
//      Programmer:     Steve LaValle
//      Last modified:  9/18/00 (Release 0.3)
//      Description:    RRT planner code
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

// #include <Kernel/geo/plane_alg.h>
#include <LEDA/system/file.h>
#include <LEDA/numbers/matrix.h>
#include <LEDA/graph/graph.h>
#include <LEDA/core/list.h>
#include <LEDA/graphics/ps_file.h>
#include <LEDA/numbers/vector.h>

using namespace leda;

#ifndef PI
#define PI 3.1415926535897932385
#endif
#ifndef INFINITY
#define INFINITY 1.0e40
#endif
#ifndef sqr
#define sqr(x) ((x)*(x))
#endif
#ifndef min
#define min(x,y) ((x<y) ? x : y)
#endif

// Stuff that makes the LEDA plane algorithms happy
typedef point   POINT;
typedef segment SEGMENT;
typedef circle  CIRCLE;
typedef line    LINE;
typedef vector  VECTOR;
typedef double  COORD;

#include "model.h"
#include "rrt.h"

#ifdef USE_ANN
  #include <ANN/ANN.h>			// ANN declarations
  #include "nn.h"
  #define ANN_STARTING_INDEX 4 // Start a new ANN for each 2^n samples
  #define ANN_MAXIMUM_INDEX 30 // Absolute max on number of nodes (2^n)
  ANN *ann1[ANN_MAXIMUM_INDEX - ANN_STARTING_INDEX + 1];
  ANN *ann2[ANN_MAXIMUM_INDEX - ANN_STARTING_INDEX + 1];
#endif


RRT::RRT(Model *model) {
  m = model;
  NumNodes = 1000;
  MaxNodes = 15000;
  DeltaT = 1.0;  // This is what you get by default
  GoalDist = m->Metric(m->InitialState,m->GoalState);
  BestState = m->InitialState;
}



// Return the best new state in nx_best
// success will be false if no action is collision free
vector RRT::SelectBestInput(const vector &x1, const vector &x2, 
			    vector &nx_best, bool &success,
			    bool forward = true)
{
  vector u,u_best,nx;
  double d,d_min;
  success = false;
  d_min = m->Metric(x1,x2); //INFINITY;
  list<vector> il = m->GetInputs(x1);
  forall(u,il) {
    if (forward)
      nx = m->Integrate(x1,u,DeltaT);
    else
      nx = m->Integrate(x1,u,-DeltaT); 
    d  = m->Metric(nx,x2);
    if ((d < d_min)&&(m->CollisionFree(nx))&&(x1 != nx)) 
      {
	d_min = d; u_best = u; nx_best = nx; success = true;
      }
  }

  // Uncomment below for random inputs!
  //int n = il.length();
  //m->R >> d;
  //u_best = il.inf(il.get_item((int) (d*n)));
  //nx_best = m->Integrate(x1,u_best,DeltaT);
    
  //std::cout << "u_best: " << u_best << "\n";

  return u_best;
}



vector RRT::SelectRandomInput(const vector &x1, const vector &x2, 
			    vector &nx_best, bool &success,
			    bool forward = true)
{
  vector u,u_best;
  double d;
  success = false;
  int i,n;
  
  i = 0;
  list<vector> il = m->GetInputs(x1);
  n = il.length();
  while ((i < 5)&&(!success)) {
    m->R >> d;
    u_best = il.inf(il.get_item((int) (d*n)));
    nx_best = m->Integrate(x1,u_best,DeltaT);
    if (m->CollisionFree(nx_best))
      success = true;
    i++;
  }

  return u_best;
}



node RRT::NearestNeighbor(const vector &x, const GRAPH<vector,vector> &g) {
  double d,d_min;
  node n,n_best;
  
  n_best = n = g.first_node(); // Keeps the warnings away
  d_min = INFINITY; d = 0.0;

#ifndef USE_ANN
  forall_nodes(n,g) {
    d = m->Metric(g[n],x);
    if (d < d_min) {
      d_min = d; n_best = n; 
    }
  } 

#else
  ANN **ann;  
  // Need to select the appropriate graph/ANN structure
  ann = (&g == &G) ? ann1 : ann2;

  // Use the ANN code
  int num = g.number_of_nodes();
  int j,k; 
  bool done;
  node n_last = g.first_node();

  // Check if it is time to update the ANNs
  if ((num > 0)&&
      (num % ((int) pow(2,ANN_STARTING_INDEX)) == 0)) {

    // Make sure update has not been performed
    for (k = ANN_MAXIMUM_INDEX; k >= ANN_STARTING_INDEX; k--) {
      if (ann[k])
	n_last = ann[k]->LastNode();
    }
    if (n_last != g.last_node()) { // If equal, then update already performed

      k = ANN_STARTING_INDEX - 1;
      done = false;
      while ((k <= ANN_MAXIMUM_INDEX)&&(!done)) {
	k++;
	if (!ann[k]) { // If there is no tree, then make one
	  done = true;
	}
	else { 
	  delete(ann[k]);  // Blow this one away; these nodes are moved up
	  ann[k] = NULL;
	}
      }
      // k should be the location for making the new ANN tree
      // Need to find the starting node in G for the new ANN tree
      if (num == ((int) pow(2,ANN_STARTING_INDEX)))
	n = g.first_node();
      else {
	j = k; done = false;
	while ((j <= ANN_MAXIMUM_INDEX)&&(!done)) {
	  if (ann[j]) {
	    n = g.succ_node(ann[j]->LastNode());
	    done = true;
	  }
	  j++;
	}
      }
      // Finally, make the new ANN tree
      ann[k] = new ANN(g,n,g.last_node(),(int) pow(2,k));

      //std::cout << "New ANN Tree:  k: " << k << " num: " << num << "\n";
      //for (j = ANN_MAXIMUM_INDEX; j >= ANN_STARTING_INDEX; j--) {
      //	std::cout << (ann[j] ? "X" : ".");
      //}
      //std::cout << "\n";
    }
  }

  // Find the nearest neighbor

  // First check the ANN trees
  for (k = ANN_MAXIMUM_INDEX; k >= ANN_STARTING_INDEX; k--) {
    if (ann[k]) {
      n = ann[k]->NearestNeighbor(x);
      d = m->Metric(g[n],x);
      if (d < d_min) {
	d_min = d; n_best = n; 
      }
      n_last = ann[k]->LastNode();
    }
  }

  //std::cout << "nodes: " << num << "\n";

  // Check the new nodes, which are not yet in the ANN tree
  n = n_last;
  while (g.succ_node(n)) {
    n = g.succ_node(n);
    d = m->Metric(g[n],x);
    if (d < d_min) {
      d_min = d; n_best = n; 
    }
  }

#endif

  //std::cout << "n_best: " << n_best << "info: " << g.inf(n_best) << "\n";
  
  //n_best =  g.choose_node(); // Pick a node at random

  return n_best;
}



bool RRT::Extend(const vector &x, 
		 GRAPH<vector,vector> &g,
		 node &nn, bool forward = true) {
  node n_best;
  vector nx,u_best;
  bool success;
  
  n_best = NearestNeighbor(x,g);
  u_best = SelectBestInput(g.inf(n_best),x,nx,success,forward);
  // nx gets next state
  if (success) {   // If a collision-free input was found
    nn = g.new_node(nx); // Make a new node with state nx
    g.new_edge(n_best,nn,u_best);
    //std::cout << "New node: " << g.inf(nn) << "   " << "u:" << u_best << "\n";
  }

  return success;
}



// This function essentially iterates Extend until it has to stop
// The same action is used for every iteration
bool RRT::Connect(const vector &x, 
		  GRAPH<vector,vector> &g,
		  node &nn, bool forward = true) {
  node nn_prev,n_best;
  vector nx,nx_prev,u_best;
  bool success;
  double d,d_prev;
  
  n_best = NearestNeighbor(x,g);
  u_best = SelectBestInput(g.inf(n_best),x,nx,success,forward); 
           // nx gets next state
  if (success) {   // If a collision-free input was found
    d = m->Metric(nx,x); d_prev = d;
    nx_prev = nx; // Initialize
    nn = n_best;
    while((m->CollisionFree(nx))&&(d <= d_prev))
      {
	nx_prev = nx;
	d_prev = d; nn_prev = nn;
	// Uncomment line below to select best action each time
	//u_best = SelectBestInput(g.inf(nn),x,nx,success,forward); 
	if (forward)
	  nx = m->Integrate(nx_prev,u_best,DeltaT);
	else
	  nx = m->Integrate(nx_prev,u_best,-DeltaT);
	d = m->Metric(nx,x);
	// Uncomment the subsequent two lines to
	//   make each intermediate node added
	//nn = g.new_node(nx_prev); // Make a new node
	//g.new_edge(nn_prev,nn,u_best);
      }
    nn = g.new_node(nx_prev); // Make a new node
    g.new_edge(n_best,nn,u_best);
  }

  return success;
}


// Like RRT::Extend, except try ALL inputs
bool RRT::Project(const vector &x, 
		 GRAPH<vector,vector> &g,
		 node &nn, bool forward = true) {
  node n_best;
  vector nx,x1,u;
  bool success;
  
  n_best = NearestNeighbor(x,g);
  x1 = g.inf(n_best);
  list<vector> il = m->GetInputs(x1);
  success = false;
  forall(u,il) {
    if (forward)
      nx = m->Integrate(x1,u,DeltaT);
    else
      nx = m->Integrate(x1,u,-DeltaT); 
    if (m->CollisionFree(nx)) {   // If a collision-free input was found
      nn = g.new_node(nx); // Make a new node with state nx
      g.new_edge(n_best,nn,u);
      success = true;
      //std::cout << "New node: " << g.inf(nn) << "   " << "u:" << u << "\n";
    }
  } 

  return success;
}



// Edges of G are input vectors, and vertices are states
// Note that this method also adds to an existing RRT
void RRT::Explore()
{
  int i;
  vector u_best,nx;
  node nn;
  double d;

  // Make the root node of G
  if (G.number_of_nodes() == 0)
    G.new_node(m->InitialState);

  for (i=0; i < NumNodes; i++) {
    if (Extend(ChooseState(),G,nn)) { 
      d = m->Metric(G.inf(nn),m->GoalState);
      if (d < GoalDist)  {  // Decrease if goal closer
	GoalDist = d;
	BestState = G.inf(nn);
      }
      if (G.number_of_nodes() % 1000 == 0)
	std::cout << G.number_of_nodes() << " nodes in the RRT.\n";
    }
  }

}



bool RRT::Plan(double gdist, list<node> &path)
{
  int i;
  double d;
  node n,nn,n_goal;
  vector nx,u_best;

  // Make the root node of G
  if (G.number_of_nodes() == 0) 
    n_goal = G.new_node(m->InitialState);

  i = 0;
  n = NearestNeighbor(m->GoalState,G);

  GoalDist = m->Metric(G.inf(n),m->GoalState);
  while ((i < NumNodes)&&(GoalDist > gdist)) {
    if (Extend(ChooseState(),G,nn)) { 
      d = m->Metric(G.inf(nn),m->GoalState);
      if (d < GoalDist) {  // Decrease if goal closer
	GoalDist = d;
	BestState = G.inf(nn);
	n_goal = nn;
	std::cout << "GoalDist " << GoalDist << "\n";
      }
    }
    i++;
  }

  // Get the solution path
  if (GoalDist < gdist) {
    std::cout << "Successful Path Found\n";
    path = PathToLeaf(n_goal,G);
    return true;
  }
  else
    return false;

}




void RRT::WriteRRT(const string &fname)
{
  G.write(m->FilePath + fname);
}



void RRT::ReadRRT(const string &fname)
{
  G.read(m->FilePath + fname);
}




vector RRT::ChooseState() {
  return m->RandomState();
}


list<node> RRT::PathToLeaf(const node &n, const GRAPH<vector,vector> &g) 
{
   list<node> path;
   node ni;
   int i;

   path.clear();
   path.push(n); 

   i = 0;
   ni = n;
   while ((ni != g.first_node())&&(i < g.number_of_nodes())) {
     path.push(g.source(g.first_in_edge(ni)));
     ni = g.source(g.first_in_edge(ni));
     i++;
   }

   return path;
}




RRTGoalBias::RRTGoalBias(Model *model):RRT(model) {
  GoalProb = 0.05;
}



vector RRTGoalBias::ChooseState()
{
  double rv;

  R >> rv;
  if (rv > GoalProb)
    return m->RandomState();
  else
    return m->GoalState;

}




RRTGoalPull::RRTGoalPull(Model *model):RRT(model) {
}



void RRTGoalPull::Explore()
{
  int i;
  vector u_best,nx;
  node nn,nn2;
  bool success;

  // Make the root node of G
  if (G.number_of_nodes() == 0)
    G.new_node(m->InitialState);
  
  for (i=0; i < NumNodes; i++) {
    if (Extend(ChooseState(),G,nn)) { 
      u_best = SelectBestInput(G.inf(nn),m->GoalState,nx,success); // nx gets next state
      if (success) {   // If a collision-free input was found
	nn2 = G.new_node(nx); // Make a new node with state nx
	G.new_edge(nn,nn2,u_best);
      }
    }
  }
}


// Build an RRT that tries to go as far as it can for each edge
RRTCon::RRTCon(Model *model):RRT(model) {
}


void RRTCon::Explore()
{
  int i;
  vector u_best,nx,nx_prev;
  node nn;


  // Make the root node of G
  if (G.number_of_nodes() == 0)
    G.new_node(m->InitialState);

  for (i=0; i < NumNodes; i++) {
    Connect(ChooseState(),G,nn);
  }
}



RRTDual::RRTDual(Model *model):RRT(model) {
}




bool RRTDual::Plan(double gdist, list<node> &path)
{
  int i;
  vector rx,u_best,nx;
  node nn,nn2;
  bool connected;
  list<node> path1;

  connected = false;

  //int total = 0;
  //for (int j=0; j < 100; j++) {

  //G.clear();
  if (G.number_of_nodes() == 0)
    G.new_node(m->InitialState);
  //GG.clear();
  if (GG.number_of_nodes() == 0)
    GG.new_node(m->GoalState);

  nn = G.first_node();
  nn2 = GG.first_node();

  i = 0;
  connected = false;
  while ((i < NumNodes) && (!connected)) {
    rx = ChooseState();
    Extend(rx,G,nn);
    Extend(rx,GG,nn2,false);  // false means reverse-time integrate

    if (m->Metric(G.inf(nn),GG.inf(nn2)) < gdist) {
      std::cout << "CONNECTED!!  Nodes: " << G.number_of_nodes()+GG.number_of_nodes() << "\n";
      connected = true;
      path1 = PathToLeaf(nn2,GG); path1.reverse();
      path = PathToLeaf(nn,G); path.conc(path1);
    }

    i++;
  }

  if (!connected)
    std::cout << "Failure to connect after " << G.number_of_nodes()+GG.number_of_nodes() 
	 << " nodes\n";

  //total += i;
  // }
  //std::cout << "Avg: " << total/100 << "\n";

  return connected;
}



RRTExtExt::RRTExtExt(Model *model):RRT(model) {
}


bool RRTExtExt::Plan(double gdist, list<node> &path)
{
  int i;
  vector u_best,nx,nx2;
  node nn,nn2;
  bool connected;
  list<node> path1;

  connected = false;

  //int total = 0;
  //for (int j=0; j < 100; j++) {
  
  //G.clear();
  if (G.number_of_nodes() == 0)
    G.new_node(m->InitialState);
  //GG.clear();
  if (GG.number_of_nodes() == 0)
    GG.new_node(m->GoalState);

  nn = G.first_node();
  nn2 = GG.first_node();

  i = 0;
  connected = false;
  while ((i < NumNodes) && (!connected)) {
    if (Extend(ChooseState(),G,nn)) {
      if (Extend(G.inf(nn),GG,nn2,false)) {
	i++;
	if (m->Metric(G.inf(nn),G.inf(nn2)) < gdist) {
	  std::cout << "CONNECTED!!  Nodes: " << G.number_of_nodes()+GG.number_of_nodes() 
	       << "\n";
	  connected = true;
	  path1 = PathToLeaf(nn2,GG); path1.reverse();
	  path = PathToLeaf(nn,G); path.conc(path1);
	}
      }
    }
    i++;

    if ((!connected) && (Extend(ChooseState(),GG,nn,false))) {
      if (Extend(G.inf(nn),G,nn2)) {
	i++;
	if (m->Metric(GG.inf(nn),G.inf(nn2)) < gdist) {
	  std::cout << "CONNECTED!!  Nodes: " << G.number_of_nodes()+GG.number_of_nodes() << "\n";
	  connected = true;
	  path1 = PathToLeaf(nn,GG); path1.reverse();
	  path = PathToLeaf(nn2,G); path.conc(path1);
	}
      }
    }
  }

  i++;

  if (!connected)
    std::cout << "Failure to connect after " << G.number_of_nodes()+GG.number_of_nodes() 
	 << " nodes\n";

  //total += i;
  // }
  //std::cout << "Avg: " << total/100 << "\n";

  return connected;
}



RRTGoalZoom::RRTGoalZoom(Model *model):RRT(model) {
  GoalProb = 0.05;
  ZoomProb = 0.5;
  ZoomFactor = 2.0;
}


// Zoom using a square box
vector RRTGoalZoom::ChooseState()
{
  double rv,r,diff;
  vector zx;
  int i;

  R >> rv;
  diff = 0.0;
  zx = m->LowerState;  // Initialize to anything

  if (rv < GoalProb)
    return m->GoalState;
  else {
    if (rv < (ZoomProb+GoalProb)) {
      for (i = 0; i < m->GoalState.dim(); i++) {
	if (fabs(m->GoalState[i] - BestState[i]) > diff)
	  diff = fabs(m->GoalState[i] - BestState[i]);
      }
      for (i = 0; i < m->GoalState.dim(); i++) {
	R >> r;
	zx[i] += m->GoalState[i] - diff + 2.0*r*ZoomFactor*diff;
      }
      return zx;
    }
    else
      return m->RandomState();
  }
  
}



RRTPolar::RRTPolar(Model *model):RRT(model) {
  // RadiusExp = 1.0/(m->InitialState.dim() - 1);
  RadiusExp = 1.0;
}






// This implementation ignores C-space topology, VERY BAD!
vector RRTPolar::ChooseState()
{
  double r,w;
  vector zx;
  int i,j;
  bool success;
  
  w = 0.0;
  zx = m->GoalState;  // Initialize to anything

  success = false;

  while (!success) {
    for (i = 0; i < m->GoalState.dim(); i++) {
      // Generate sample from N(0,1)
      zx[i] = 0.0;
      for (j = 0; j < 12; j++) {
	R >> r; zx[i] += r;
      }
      zx[i] -= 6.0;
      w += zx[i]*zx[i];
    }
    
    w = sqrt(w);
    
    //std::cout << "RadiusExp: " << RadiusExp;
    
    R >> r;  // Radius
    r = pow(r,RadiusExp);
    for (i = 0; i < m->GoalState.dim(); i++) {
      zx[i] = (m->UpperState[i] - m->LowerState[i])*
	sqrt(m->GoalState.dim())*r*zx[i]/w + 
	m->GoalState[i];
    }
    
    // Check if sample is within bounds
    success = true;
    for (i = 0; i < m->GoalState.dim(); i++) {
      if ((zx[i] >= m->UpperState[i])||(zx[i] <= m->LowerState[i]))
	success = false;
    }
  }

  return zx;
}



vector RRTPolar::SelectBestInput(const vector &x1, const vector &x2, 
				 vector &nx_best, bool &success)
{
  vector u,u_best,nx;
  double d,dg,dmax,d_min;
  success = false;
  d_min = INFINITY;
  dg  = m->Metric(x1,x2);
  dmax  = m->Metric(m->LowerState,m->UpperState);
  list<vector> il = m->GetInputs(x1);
  forall(u,il) {
    //nx = m->Integrate(x1,u,DeltaT*sqrt(dg/dmax));  // Slow down near goal
    nx = m->Integrate(x1,u,DeltaT);
    d  = m->Metric(nx,x2);
    if ((d < d_min)&&(m->CollisionFree(nx))&&(x1 != nx)) 
      {
	d_min = d; u_best = u; nx_best = nx; success = true;
      }
  }
  
  //std::cout << "u_best: " << u_best << "\n");

  return u_best;
}



// Only works for 2DPoint model!!!!!
RRTHull::RRTHull(Model *model):RRT(model) {
  Radius = 100000000.0;
}



vector RRTHull::ChooseState() {
  double theta;
  vector v(2);

  R >> theta;  theta *= 2.0*PI;

  v[0] = Radius*cos(theta);
  v[1] = Radius*sin(theta);

  return v;
}



RRTExtCon::RRTExtCon(Model *model):RRT(model) {
}



bool RRTExtCon::Plan(double gdist, list<node> &path)
{
  int i;
  vector nx,nx_prev;
  node nn,nn2;
  bool connected;
  list<node> path1;

  connected = false;

  //G.clear();
  if (G.number_of_nodes() == 0)
    G.new_node(m->InitialState);
  //GG.clear();
  if (GG.number_of_nodes() == 0)
    GG.new_node(m->GoalState);

  nn = G.first_node();
  nn2 = GG.first_node();

  i = 0;
  connected = false;
  while ((i < NumNodes) && (!connected)) {
    if (Extend(ChooseState(),G,nn)) { 
      // Update the goal RRT
      if (Connect(G.inf(nn),GG,nn2,false)) {
	if (m->Metric(G.inf(nn),GG.inf(nn2)) < gdist) {
	  std::cout << "CONNECTED!!  Nodes: " << G.number_of_nodes()+GG.number_of_nodes()
	       << "\n";
	  connected = true;
	  path1 = PathToLeaf(nn2,GG); path1.reverse();
	  path = PathToLeaf(nn,G); path.conc(path1);
	}
      }
    }
    i++;

    if ((!connected)&&(Extend(ChooseState(),GG,nn,false))) { 
      // Update the initial RRT
      if (Connect(GG.inf(nn),G,nn2)) { 
	if (m->Metric(GG.inf(nn),G.inf(nn2)) < gdist) {
	  std::cout << "CONNECTED!!  Nodes: " << G.number_of_nodes()+GG.number_of_nodes()
	       << "\n";
	  connected = true;
	  path1 = PathToLeaf(nn,GG); path1.reverse();
	  path = PathToLeaf(nn2,G); path.conc(path1);
	}
      }
    }
    i++;
  }

  if (!connected)
    std::cout << "Failure to connect after " << G.number_of_nodes()+GG.number_of_nodes() 
	 << " nodes\n";

  return connected;
}



RRTConCon::RRTConCon(Model *model):RRT(model) {
}



bool RRTConCon::Plan(double gdist, list<node> &path)
{
  int i;
  vector nx,nx_prev;
  node nn,nn2;
  bool connected;
  list<node> path1;

  connected = false;

  // Code for time trials
  //float t = used_time();
  //int total = 0;
  //for (int j=0; j < 100; j++) {

  //G.clear();
  if (G.number_of_nodes() == 0)
    G.new_node(m->InitialState);
  //GG.clear();
  if (GG.number_of_nodes() == 0)
    GG.new_node(m->GoalState);

  nn = G.first_node();
  nn2 = GG.first_node();

  i = 0;
  connected = false;
  while ((i < NumNodes) && (!connected)) {
    if (Connect(ChooseState(),G,nn)) { 
      // Update the goal RRT
      if (Connect(G.inf(nn),GG,nn2,false)) {
	if (m->Metric(G.inf(nn),GG.inf(nn2)) < gdist) {
	  std::cout << "CONNECTED!!  Nodes: " << G.number_of_nodes()+GG.number_of_nodes() 
	       << "\n";
	  connected = true;
	  path1 = PathToLeaf(nn2,GG); path1.reverse();
	  path = PathToLeaf(nn,G); path.conc(path1);
	}
      }
    }
    i++;

    if ((!connected)&&(Connect(ChooseState(),GG,nn,false))) { 
      // Update the initial RRT
      if (Connect(GG.inf(nn),G,nn2)) { 
	if (m->Metric(GG.inf(nn),G.inf(nn2)) < gdist) {
	  std::cout << "CONNECTED!!  Nodes: " << G.number_of_nodes()+GG.number_of_nodes()  
	       << "\n";
	  connected = true;
	  path1 = PathToLeaf(nn,GG); path1.reverse();
	  path = PathToLeaf(nn2,G); path.conc(path1);
	}
      }
    }
    i++;
  }

  if (!connected)
    std::cout << "Failure to connect after " << G.number_of_nodes()+GG.number_of_nodes() 
	 << " nodes\n";

  // Code for time trials
  //total += i;
  //  }
  //std::cout << "Avg: " << total/100.0 << "\n";
  //std::cout << "Avg time: " << ((double)used_time(t)/100.0) << "\n"; 

  return connected;
}





RRTStar::RRTStar(Model *model):RRT(model) {
  NodeDepth.init(G);
  OptimalityFactor = -0.9;
}



node RRTStar::NearestNeighbor(const vector &x, const GRAPH<vector,vector> &g) {
  double d,d_min;
  node n,n_best;
  
  n_best = g.choose_node(); // Keeps the warnings away
  d_min = INFINITY;
  forall_nodes(n,g) {
    d = m->Metric(g[n],x) + 
      OptimalityFactor*m->Speed*DeltaT*NodeDepth[n]; // Here is the new part
    if (d < d_min) {
      d_min = d; n_best = n; 
    }
  } 

  return n_best;
}



bool RRTStar::Extend(const vector &x, 
		 GRAPH<vector,vector> &g,
		 node &nn, bool forward = true) {
  node n_best;
  vector nx,u_best;
  bool success;
  
  n_best = NearestNeighbor(x,g);
  u_best = SelectBestInput(g.inf(n_best),x,nx,success); // nx gets next state
  if (success) {   // If a collision-free input was found
    nn = g.new_node(nx); // Make a new node with state nx
    // This NodeDepth relies on root initialization to zero!!!!
    NodeDepth[nn] = NodeDepth[n_best] + 1;
    //std::cout << "Node Depth: " << NodeDepth[nn] << "\n";
    g.new_edge(n_best,nn,u_best);
    //std::cout << "New node: " << g.inf(nn) << "   " << "u:" << u_best << "\n";
  }

  return success;
}

