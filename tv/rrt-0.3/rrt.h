//----------------------------------------------------------------------
//      File:           rrt.h
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

class RRT {
 public:
  double GoalDist;  // How far is the tree from the goal?
  vector BestState; // The closest state to the goal
  window *w;
  GRAPH<vector,vector> G;
  GRAPH<vector,vector> GG;  // RRT from Goal (G is from init)
  Model *m;
  RRT(Model *model);
  virtual ~RRT() {};
  int NumNodes;
  int MaxNodes;
  double DeltaT;
  virtual vector SelectBestInput(const vector &x1, const vector &x2, 
			 vector &nx_best, bool &success, bool forward);
  virtual vector SelectRandomInput(const vector &x1, const vector &x2, 
			 vector &nx_best, bool &success, bool forward);
  virtual node NearestNeighbor(const vector &x, const GRAPH<vector,vector> &g);
  virtual bool Extend(const vector &x, GRAPH<vector,vector> &g, node &nn,
		      bool forward);
  virtual bool Connect(const vector &x, GRAPH<vector,vector> &g, node &nn,
		       bool forward);
  virtual bool Project(const vector &x, GRAPH<vector,vector> &g, node &nn,
		      bool forward);
  virtual void Explore();
  virtual bool Plan(double gdist,list<node> &path);
  void WriteRRT(const string &fname);
  void ReadRRT(const string &fname);
  virtual vector ChooseState();
  list<node> PathToLeaf(const node &n, const GRAPH<vector,vector> &g);
};



class RRTGoalBias: public RRT {
 public:
  double GoalProb;
  random_source R;
  RRTGoalBias(Model *model);
  virtual ~RRTGoalBias() {};
  virtual vector ChooseState();
};


class RRTGoalPull: public RRT {
 public:
  RRTGoalPull(Model *model);
  virtual ~RRTGoalPull() {};
  virtual void Explore();
};


class RRTCon: public RRT {
 public:
  RRTCon(Model *model);
  virtual ~RRTCon() {};
  virtual void Explore();
};



class RRTDual: public RRT {
 public:
  RRTDual(Model *model);
  virtual ~RRTDual() {};
  virtual bool Plan(double gdist,list<node> &path);
};



class RRTExtExt: public RRT {
 public:
  RRTExtExt(Model *model);
  virtual ~RRTExtExt() {};
  virtual bool Plan(double gdist,list<node> &path);
};



class RRTGoalZoom: public RRT {
 public:
  double GoalProb,ZoomProb,ZoomFactor;
  random_source R;
  RRTGoalZoom(Model *model);
  virtual ~RRTGoalZoom() {};
  virtual vector ChooseState();
};


class RRTPolar: public RRT {
 public:
  double RadiusExp;
  random_source R;
  RRTPolar(Model *model);
  virtual ~RRTPolar() {};
  virtual vector ChooseState();
  virtual vector SelectBestInput(const vector &x1, const vector &x2, 
			 vector &nx_best, bool &success);
};


class RRTHull: public RRT {
 public:
  double Radius;
  random_source R;
  RRTHull(Model *model);
  virtual ~RRTHull() {};
  virtual vector ChooseState();
};


// The RRT-Connect planner from ICRA 2000 paper
class RRTExtCon: public RRT {
 public:
  RRTExtCon(Model *model);
  virtual ~RRTExtCon() {};
  virtual bool Plan(double gdist,list<node> &path);
};


class RRTConCon: public RRT {
 public:
  RRTConCon(Model *model);
  virtual ~RRTConCon() {};
  virtual bool Plan(double gdist,list<node> &path);
};


class RRTStar: public RRT {
 public:
  node_map<int> NodeDepth;
  double OptimalityFactor;  // Should in (0,1); negative numbers are interesting too 
  RRTStar(Model *model);
  virtual ~RRTStar() {};
  virtual node NearestNeighbor(const vector &x, const GRAPH<vector,vector> &g);
  virtual bool Extend(const vector &x, GRAPH<vector,vector> &g, node &nn,
		      bool forward);
};

