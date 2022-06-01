//----------------------------------------------------------------------
//      File:           model.C
//      Programmer:     Steve LaValle
//      Last modified:  9/18/00 (Release 0.3)
//      Description:    Models for RRT planner
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

#include <fstream>
#include <math.h>

#include <LEDA/system/file.h>
#include <LEDA/core/list.h>
#include <LEDA/numbers/matrix.h>
#include <LEDA/geo/point.h>
#include <LEDA/geo/polygon.h>
#include <LEDA/core/random.h>
#include <LEDA/system/stream.h>
#include <LEDA/core/string.h>
#include <LEDA/numbers/vector.h>

using namespace leda;

#include "model.h"

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

#define NUM_INPUTS_2DPOINT 50
#define NUM_INPUTS_2DRIGID 8
#define NORMAL_ITS 12   // Controls accuracy of Gaussian sampler


vector Model::RungeKuttaIntegrate(const vector &x, const vector &u, 
		  const double &h) 
{
  vector k1,k2,k3,k4;

  k1 = StateTransitionEquation(x,u);
  k2 = StateTransitionEquation(x + (0.5*h)*k1,u);
  k3 = StateTransitionEquation(x + (0.5*h)*k2,u);
  k4 = StateTransitionEquation(x + h*k3,u);

  return x + h/6.0*(k1 + 2.0*k2 + 2.0*k3 + k4);
}



vector Model::EulerIntegrate(const vector &x, const vector &u, 
		  const double &h) 
{
  return x + h*StateTransitionEquation(x,u);
}



vector Model::RandomState() {
  int i;
  //  random_source R;
  double r;
  vector rx;

  rx = LowerState;
  for (i = 0; i < StateDim; i++) {
      R >> r; 
      rx[i] += r * (UpperState[i] - LowerState[i]);
    }

  return rx;
}



vector Model::NormalState(double sd = 0.5) {
  int i,j;
  double r;
  vector rx;
  bool success = false;

  rx = GoalState;
  for (i = 0; i < StateDim; i++) {
    success = false;
    while (!success) {
      rx[i] = 0.0;
      for (j = 0; j < NORMAL_ITS; j++) {
	R >> r; rx[i] += r;
      }
      rx[i] = (rx[i] - NORMAL_ITS/2)*sd*(UpperState[i]-LowerState[i])+GoalState[i];
      if ((rx[i] <= UpperState[i])&&(rx[i] >= LowerState[i]))
	success = true;
    }
  }

  return rx;
}


list<vector> Model::GetInputs(const vector &x) {
  return Inputs;
}


point Model::StateToPoint(const vector &x) {
  return point(x[0],x[1]);
}



// By default, don't change anything
vector Model::StateToConfiguration(const vector &x) {
  return x;
}



// Overrides the default Inputs by reading from a file
void Model::ReadInputs() {
  Inputs.clear();
  file_istream fin(FilePath + "Inputs");
  fin >> Inputs;
}



void Model::ReadInitialState() {
  file_istream fin(FilePath + "InitialState");
  fin >> InitialState;
}



void Model::ReadGoalState() {
  file_istream fin(FilePath + "GoalState");
  fin >> GoalState;
}



void Model::ReadLowerState() {
  file_istream fin(FilePath + "LowerState");
  fin >> LowerState;
}



void Model::ReadUpperState() {
  file_istream fin(FilePath + "UpperState");
  fin >> UpperState;
}


bool Model::CollisionFree(const vector &x) {
  return true;
}


double Model::DistanceComp(const vector &x) {
  return 1.0;
}


double Model::Metric(const vector &x1, const vector &x2) {

  double rho;

  rho = (x1 - x2).length();

  return rho;
}


// Some models will interpolate differently because of 
// topology (e.g., S^1, P^3)
vector Model::LinearInterpolate(const vector &x1, const vector &x2, 
				const double &a) {
  return (1.0-a)*x1 + a*x2;
}


list<polygon> Model::TransformRobot(const vector &x) {
  list<polygon> trobot;

  point p1(0.5,0.5);
  point p2(-0.5,0.5);
  point p3(-0.5,-0.5);
  point p4(0.5,-0.5);
  list<point> pl;
  pl.push(p4); pl.push(p3); pl.push(p2); pl.push(p1);
  polygon p(pl);
  trobot.push(p.translate(StateToConfiguration(x)));
  
  return trobot;
}




// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model1D
// 
// *********************************************************************
// *********************************************************************

Model1D::Model1D(string path = ""):Model(path) {

  vector v;
  
  Force = 1.0;

  if ((path.length() > 0)&&(path[path.length()-1] != '/'))
    path += "/";

  FilePath = path;
  StateDim = 1;
  InputDim = 1;
  
  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  else {
    InitialState = vector(0.0,0.0);
  }

  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  else {
    GoalState = vector(50.0,0.0);
  }

  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  else {
    LowerState = vector(-50.0,-10.0);
  }

  if (is_file(FilePath+"UpperState"))
    ReadUpperState();
  else {
    UpperState = vector(50.0,10.0);
  }

  // Make the list of 1D Inputs
  if (is_file(FilePath+"Inputs"))
    ReadInputs();
  else {
    Inputs.clear();  // Just to be safe
    v = vector(1); v[0] = 0;  // Float
    Inputs.push_back(v);
    v = vector(1); v[0] = 1;  // Accel in +x
    Inputs.push_back(v);
    v = vector(1); v[0] = -1; // Accel in -x
    Inputs.push_back(v);
  }

}



vector Model1D::Integrate(const vector &x, const vector &u, const double &h)
{
  return RungeKuttaIntegrate(x,u,h);
}


vector Model1D::StateTransitionEquation(const vector &x, const vector &u)
{
  vector dx(2);

  dx[0] = x[1];
  dx[1] = u[0]*Force;

  return dx;
}


point Model1D::StateToPoint(const vector &x) {
  return point(x[0]+50.0,x[1]*(100.0/(UpperState[1]-LowerState[1]))+50.0);
}


vector Model1D::StateToConfiguration(const vector &x) {
  return vector(x[0]+50,x[1]*(100.0/(UpperState[1]-LowerState[1]))+50.0,0.0); // Yield a zero rotation every time
}

double Model1D::Metric(const vector &x1, const vector &x2) {
  double d = 0.0;
  int i;

  for (i = 0; i < 2; i++) {
    d += sqr((x1[i] - x2[i]) / (UpperState[i] - LowerState[i]));
  }
  
  //cout << "x1: " << x1 << "  x2: " << x2 << "   Metric: " << d << "\n";

  return d;
}



// *********************************************************************
// *********************************************************************
// CLASS:     Model::ModelLinear
// 
// *********************************************************************
// *********************************************************************

ModelLinear::ModelLinear(string path = ""):Model(path) {

  vector v;
  int i;

  if ((path.length() > 0)&&(path[path.length()-1] != '/'))
    path += "/";

  FilePath = path;

  file_istream fin(FilePath + "A");
  fin >> A;
  file_istream fin2(FilePath + "B");
  fin2 >> B;

  StateDim = A.dim1();
  InputDim = B.dim2();

  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  else {
    InitialState = vector(StateDim);
  }

  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  else {
    GoalState = vector(StateDim);
  }

  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  else {
    LowerState = vector(StateDim);
    for (i = 0; i < StateDim; i++) {
      LowerState[i] -= 10.0;
    }
  }

  if (is_file(FilePath+"UpperState"))
    ReadUpperState();
  else {
    UpperState = vector(StateDim);
    for (i = 0; i < StateDim; i++) {
      UpperState[i] += 10.0;
    }
  }

  if (is_file(FilePath+"Inputs"))
    ReadInputs();
  else {
    Inputs.clear();  // Just to be safe
    v = vector(InputDim);  // Initializes to the zero vector
    Inputs.push(v);
    for (i = 0; i < StateDim; i++) {
      v = vector(InputDim);
      v[i] = 1.0;
      Inputs.push(v);
      v[i] = -1.0;
      Inputs.push(v);
    }
  }

}


point ModelLinear::StateToPoint(const vector &x) {
  return point(5.0*(x[0]+10.0),5.0*(x[1]+10.0));
}

vector ModelLinear::StateToConfiguration(const vector &x) {
  return vector(5.0*(x[0]+10.0),5.0*(x[1]+10.0)); 
}


vector ModelLinear::Integrate(const vector &x, const vector &u, const double &h)
{
  return RungeKuttaIntegrate(x,u,h);
}


vector ModelLinear::StateTransitionEquation(const vector &x, const vector &u)
{
  vector dx(StateDim);

  dx = A * x + B * u; // This looks too easy!

  return dx;
}


// *********************************************************************
// *********************************************************************
// CLASS:     Model::ModelNintegrator
// 
// *********************************************************************
// *********************************************************************


ModelNintegrator::ModelNintegrator(string path = ""):Model(path) {

  vector v;
  int i;
  double alpha,beta;

  UBound = 0.5;
  VBound = 0.5;

  if ((path.length() > 0)&&(path[path.length()-1] != '/'))
    path += "/";

  FilePath = path;

  StateDim = 3;
  InputDim = 2;

  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  else {
    InitialState = vector(StateDim);
  }

  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  else {
    GoalState = vector(StateDim);
  }

  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  else {
    LowerState = vector(StateDim);
    for (i = 0; i < StateDim; i++) {
      LowerState[i] -= 20.0;
    }
  }

  if (is_file(FilePath+"UpperState"))
    ReadUpperState();
  else {
    UpperState = vector(StateDim);
    for (i = 0; i < StateDim; i++) {
      UpperState[i] += 20.0;
    }
  }

  if (is_file(FilePath+"Inputs"))
    ReadInputs();
  else {
    Inputs.clear();  // Just to be safe
    for (alpha = -UBound; alpha <= UBound; 
	 alpha += 2.0*UBound/4.0) {
      for (beta = -VBound; beta <= VBound; 
	   beta += 2.0*VBound/4.0) {
	Inputs.push_back(vector(alpha,beta));
	Inputs.push_back(vector(alpha,beta)); 
      }
    }
    
  }
}



point ModelNintegrator::StateToPoint(const vector &x) {
  return point(2.5*(x[0]+20.0),2.5*(x[1]+20.0));
}

vector ModelNintegrator::StateToConfiguration(const vector &x) {
  return vector(2.5*(x[0]+20.0),2.5*(x[1]+20.0)); 
}


vector ModelNintegrator::Integrate(const vector &x, const vector &u, const double &h)
{
  return RungeKuttaIntegrate(x,u,h);
}


vector ModelNintegrator::StateTransitionEquation(const vector &x, const vector &u)
{
  vector dx(StateDim);

  dx[0] = u[0];
  dx[1] = u[1];
  dx[2] = x[0]*u[1] - x[1]*u[0];

  return dx;
}








// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model2D
// 
// *********************************************************************
// *********************************************************************



Model2D::Model2D(string path = ""):Model(path) {

  Speed = 1.0;

  if ((path.length() > 0)&&(path[path.length()-1] != '/'))
    path += "/";

  FilePath = path;
  
  if (is_file(FilePath + "Obst")) {
    file_istream fin(FilePath + "Obst");
    fin >> Obst;
  }
}



// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model2DPoint
// 
// *********************************************************************
// *********************************************************************


// Constructor
Model2DPoint::Model2DPoint(string path = ""):Model2D(path) {
  double theta;

  StateDim = 2;
  InputDim = 2;

  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  else {
    InitialState = vector(50.0,50.0);
  }

  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  else {
    GoalState = vector(90.0,10.0);
  }

  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  else {
    LowerState = vector(0.0,0.0);
  }

  if (is_file(FilePath+"UpperState"))
    ReadUpperState();
  else {
    UpperState = vector(100.0,100.0);
  }

  // Make the list of Inputs
  if (is_file(FilePath+"Inputs"))
    ReadInputs();
  else {
    for (theta = 0.0; theta < 2.0*PI; theta += 2.0*PI/NUM_INPUTS_2DPOINT) {
      Inputs.push_back(vector(cos(theta),sin(theta)));
    }
  }
}




vector Model2DPoint::StateTransitionEquation(const vector &x, const vector &u) {

  vector dx(2);

  dx = Speed*u;
  return dx;
}



double Model2DPoint::Metric(const vector &x1, const vector &x2) {

  double rho;

  rho = (x1 - x2).length();

  return rho;
}


// This simple collision checker uses LEDA's p.inside method.
// Certainly more efficient methods exist (incremental).
bool Model2DPoint::CollisionFree(const vector &x) {
  polygon p;

  forall(p,Obst) {
    if (p.inside(StateToPoint(x))) {
      return false;
    }
  }

  return true;
}


double Model2DPoint::DistanceComp(const vector &x) {
  polygon p;
  double d_min,d;

  d_min = INFINITY;

  if (CollisionFree(x)) {
    forall(p,Obst) {
      d = p.distance(StateToPoint(x));
      if (d < d_min)
	d_min = d;
    }
  }
  else
    d_min = -1.0;
  
  return d_min;
}



vector Model2DPoint::Integrate(const vector &x, const vector &u, const double &h)
{
  return EulerIntegrate(x,u,h);
}




// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model2DPointCar
// 
// *********************************************************************
// *********************************************************************


// Constructor
Model2DPointCar::Model2DPointCar(string path = ""):Model2DPoint(path) {
  double alpha;

  StateDim = 3;
  InputDim = 2;

  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  else {
    InitialState = vector(45.0,60.0,PI/2.0);
  }

  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  else {
    GoalState = vector(90.0,10.0,1.5*PI);
  }

  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  else {
    LowerState = vector(0.0,0.0,0.0);
  }

  if (is_file(FilePath+"UpperState"))
    ReadUpperState();
  else {
    UpperState = vector(100.0,100.0,2.0*PI);
  }

  MaxSteeringAngle = PI/6.0;
  CarLength = 4.0;

  // Make the list of Inputs
  Inputs.clear();  // Otherwise its parent constructor will make some inputs
  if (is_file(FilePath+"Inputs"))
    ReadInputs();
  else {
    for (alpha = -MaxSteeringAngle; alpha <= MaxSteeringAngle; 
	 alpha += 2.0*MaxSteeringAngle/6.0) {
      Inputs.push_back(vector(1.0,alpha));
      Inputs.push_back(vector(-1.0,alpha)); 
    }
  }
}



vector Model2DPointCar::StateTransitionEquation(const vector &x, const vector &u) {

  vector dx(3);

  dx[0] = Speed*u[0]*cos(x[2]);
  dx[1] = Speed*u[0]*sin(x[2]);
  dx[2] = Speed*u[0]*tan(u[1])/CarLength;
  return dx;
}



double Model2DPointCar::Metric(const vector &x1, const vector &x2) {

  double rho,dtheta;
  
  dtheta = min(fabs(x1[2]-x2[2]),2.0*PI - fabs(x1[2]-x2[2]));

  rho = sqrt(sqr(x1[0] - x2[0]) + sqr(x1[1] - x2[1]) + 50.0/PI*sqr(dtheta));

  return rho;
}



vector Model2DPointCar::Integrate(const vector &x, const vector &u, double const &h) {
  vector nx(3);

  nx = RungeKuttaIntegrate(x,u,h);

  // Make sure the S^1 topology is preserved for 2D rotation
  if (nx[2] > 2.0*PI)
    nx[2] -= 2.0*PI;
  if (nx[2] < 0.0)
    nx[2] += 2.0*PI;

  return nx;
}


// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model2DRigid
// 
// *********************************************************************
// *********************************************************************



// Constructor
Model2DRigid::Model2DRigid(string path = ""):Model2D(path) {
  double theta;

  StateDim = 3;
  InputDim = 3;

  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  else {
    InitialState = vector(50.0,50.0,0.0);
  }

  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  else {
    GoalState = vector(75.0,15.0,PI);
  }

  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  else {
    LowerState = vector(0.0,0.0,0.0);
  }

  if (is_file(FilePath+"UpperState"))
    ReadUpperState();
  else {
    UpperState = vector(100.0,100.0,2.0*PI);
  }

  // Make the list of Inputs
  if (is_file(FilePath+"Inputs"))
    ReadInputs();
  else {
    for (theta = 0.0; theta < 2.0*PI; theta += 2.0*PI/NUM_INPUTS_2DRIGID) {
      Inputs.push_back(vector(cos(theta),sin(theta),0.0));
      //Inputs.push_back(vector(cos(theta),sin(theta),-0.1));
      //Inputs.push_back(vector(cos(theta),sin(theta),0.1));
    }
    Inputs.push_back(vector(0.0,0.0,-0.1));
    Inputs.push_back(vector(0.0,0.0,0.1));
  }

  // Read the robot model
  file_istream fin(FilePath + "Robot");
  fin >> Robot;

}

#define IDIM 40
#define JDIM 40
#define KDIM 32

#define B(i,j,k)  Bitmap[((k) + KDIM * ((j) + (i) * JDIM))]

void Model2DRigid::MakeBitmap()
{
  Bitmap = new bool[IDIM*JDIM*KDIM];

  //cout << "Initializing Bitmap ";

  int i,j,k;
  double x,y,theta;
  bool collide;
  polygon op,rp,trp,rrp;
  point pt;
  double ominx,omaxx,ominy,omaxy;
  double rminx,rmaxx,rminy,rmaxy;

  for (i = 0; i < IDIM; i++) {
    for (j = 0; j < JDIM; j++) {
      for (k = 0; k < KDIM; k++) {
	B(i,j,k) = false;
      }
    }
  }

  forall(op,Obst) {
    std::cout << "." << std::flush;

    ominx = ominy = INFINITY;
    omaxx = omaxy = -INFINITY;
    forall_vertices(pt,op) {
      if (pt.xcoord() < ominx)
	ominx = pt.xcoord();
      if (pt.xcoord() > omaxx)
	omaxx = pt.xcoord();
      if (pt.ycoord() < ominy)
	ominy = pt.ycoord();
      if (pt.ycoord() > omaxy)
	omaxy = pt.ycoord();
    }

    forall(rp,Robot) {
      theta = 0.0;
      for (k = 0; k < KDIM; k++) {
	rrp = rp.rotate(theta);

	rminx = rminy = INFINITY;
	rmaxx = rmaxy = -INFINITY;
	forall_vertices(pt,rrp) {
	  if (pt.xcoord() < rminx)
	    rminx = pt.xcoord();
	  if (pt.xcoord() > rmaxx)
	    rmaxx = pt.xcoord();
	  if (pt.ycoord() < rminy)
	    rminy = pt.ycoord();
	  if (pt.ycoord() > rmaxy)
	    rmaxy = pt.ycoord();
	}

	x = 0.0; 
	for (i = 0; i < IDIM; i++) {
	  y = 0.0;
	  for (j = 0; j < JDIM; j++) {
	    // Configuration (x,y,theta)
	    // Only do the test if the translated bbox overlaps
	    if ((rmaxx + x <= ominx)&&
		(rmaxy + y >= ominy)&&
		(rminx + x <= omaxx)&&
		(rminy + y >= omaxy)) {}
	    else {
	      collide = false;
	      trp = rrp.translate(x,y);
	      //if (trp.intersection(op).length() > 0)
	      //collide = true;
	      B(i,j,k) |= collide;
	    }
	    y += 100.0/JDIM;
	  }
	  x += 100.0/IDIM;
	}
	theta += 6.0*PI/KDIM;
      }
    }
  }

  MadeBitmap = true;

  std::cout << "\n";
}



vector Model2DRigid::StateTransitionEquation(const vector &x, const vector &u) {

  vector dx(3);

  dx = Speed*u;
  return dx;
}



double Model2DRigid::Metric(const vector &x1, const vector &x2) {

  double fd = fabs(x1[2]-x2[2]);
  double dtheta = min(fd,2.0*PI - fd);

  return sqrt(sqr(x1[0] - x2[0]) + sqr(x1[1] - x2[1]) + sqr(50.0/PI*dtheta));

}



// This is a very naive collision checker
// This should certainly be replaced some day with a 
// a more efficient algorithm.  It would be best to add
// a 2D incremental distance algorithm, based on the
// ideas of Lin and Canny, and Mirtich.  One could also
// use bitmaps since this is only 2D.

bool Model2DRigid::CollisionFree(const vector &x) {
  polygon op,rp;
  segment seg,seg2;

  list<polygon> trobot = TransformRobot(x);

  forall(rp,trobot) {
    forall(op,Obst) {
      forall_segments(seg,rp) {
	forall_segments(seg2,op) {
	  if (seg.intersection(seg2)) {
	    return false;
	  }
	}
      }
    }
  }

  // Check for containment
  forall(rp,trobot) {
    forall(op,Obst) {
      if (op.contains(rp.vertices().head()))
	return false;
    }
  }

  return true;
}


// This should certainly be replaced some day with a 
// a more efficient algorithm.  It would be best to add
// a 2D incremental distance algorithm, based on the
// ideas of Lin and Canny, and Mirtich.

double Model2DRigid::DistanceComp(const vector &x) {
  polygon rp,op;
  double d_min,d;
  point rpt;

  d_min = INFINITY;

  list<polygon> trobot = TransformRobot(x);

  // This is obviously inefficient
  if (CollisionFree(x)) {
    forall(rp,trobot) {
      forall_vertices(rpt,rp) {
	forall(op,Obst) {
	  d = op.distance(rpt);
	  if (d < d_min)
	    d_min = d;
	}
      }
    }
  }
  else
    d_min = -1.0;
  
  return d_min;
}


// Handle S^1 topology properly (for rotation)
vector Model2DRigid::LinearInterpolate(const vector &x1, const vector &x2, 
				const double &a) {

  vector v;

  v = (1.0-a)*x1 + a*x2;

  if (x2[2] - x1[2] > PI) {
    if (x1[2] > x2[2])
      v[2] = (1.0-a)*x1[2] + a*(x2[2]+2.0*PI);
    else
      v[2] = (1.0-a)*(x1[2]+2.0*PI) + a*x2[2];
  }

  if (v[2] > 2.0*PI)
    v[2] -= 2.0*PI;

  return v;
}



vector Model2DRigid::Integrate(const vector &x, const vector &u, const double &h)
{
  vector nx(3);

  nx = RungeKuttaIntegrate(x,u,h);

  // Make sure the S^1 topology is preserved for 2D rotation
  if (nx[2] > 2.0*PI)
    nx[2] -= 2.0*PI;
  if (nx[2] < 0.0)
    nx[2] += 2.0*PI;

  return nx;
}


list<polygon> Model2DRigid::TransformRobot(const vector &x) {
  list<polygon> trobot;
  polygon p,tp;
  vector q(3);
  
  q = StateToConfiguration(x);
  forall(p,Robot) {
    tp = p.rotate(q[2]).translate(q[0],q[1]);
    trobot.push_front(tp);
  }

  return trobot;
}




// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model2DRigidCar
// 
// *********************************************************************
// *********************************************************************


// Constructor
Model2DRigidCar::Model2DRigidCar(string path = ""):Model2DRigid(path) {
  double alpha;

  StateDim = 3;
  InputDim = 2;

  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  else {
    InitialState = vector(45.0,60.0,PI/2.0);
  }

  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  else {
    GoalState = vector(85.0,5.0,PI);
  }

  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  else {
    LowerState = vector(0.0,0.0,0.0);
  }

  if (is_file(FilePath+"UpperState"))
    ReadUpperState();
  else {
    UpperState = vector(100.0,100.0,2.0*PI);
  }

  MaxSteeringAngle = PI/12.0;
  CarLength = 2.0;

  // Make the list of Inputs
  Inputs.clear();  // Otherwise its parent constructor will make some inputs
  if (is_file(FilePath+"Inputs"))
    ReadInputs();
  else {
    for (alpha = -MaxSteeringAngle; alpha <= MaxSteeringAngle; 
	 alpha += 2.0*MaxSteeringAngle/6.0) {
      Inputs.push_back(vector(1.0,alpha)); 
      Inputs.push_back(vector(-1.0,alpha)); 
    }
  }
}


vector Model2DRigidCar::StateTransitionEquation(const vector &x, const vector &u) {

  vector dx(3);
  dx[0] = Speed*u[0]*cos(x[2]);
  dx[1] = Speed*u[0]*sin(x[2]);
  dx[2] = Speed*u[0]*tan(u[1])/CarLength;
  return dx;
}


// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model2DRigidCarForward
// 
// *********************************************************************
// *********************************************************************

Model2DRigidCarForward::Model2DRigidCarForward(string path = ""):Model2DRigidCar(path) {
  double alpha;

  StateDim = 3;
  InputDim = 2;

 // Defaults are inherited
  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  if (is_file(FilePath+"UpperState"))
    ReadUpperState();

  // Make the list of Inputs
  Inputs.clear();  // Otherwise its parent constructor will make some inputs
  if (is_file(FilePath+"Inputs"))
    ReadInputs();
  else {
    for (alpha = -MaxSteeringAngle; alpha <= MaxSteeringAngle; 
	 alpha += 2.0*MaxSteeringAngle/6.0) {
      Inputs.push_back(vector(1.0,alpha)); 
    }
  }
}


// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model2DRigidSmooth
//  Smooth steering 
// *********************************************************************
// *********************************************************************


Model2DRigidCarSmooth::Model2DRigidCarSmooth(string path = ""):Model2DRigidCar(path) {

  StateDim = 4;
  InputDim = 2;
  SteeringSpeed = 0.05;  // Default 0.05
  
  InitialState = vector(4);
  GoalState = vector(4);
  LowerState = vector(4);
  UpperState = vector(4);

  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  else {
    InitialState[0] = 50.0; InitialState[1] = 40.0; InitialState[2] = PI/2.0;
    InitialState[3] = 0.0;
  }

  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  else {
    GoalState[0] = 85.0; GoalState[1] = 5.0; GoalState[2] = PI; 
    GoalState[3] = 0.0;
  }

  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  else {
    LowerState[0] = 0.0; LowerState[1] = 0.0; LowerState[2] = 0.0; 
    LowerState[3] = -MaxSteeringAngle;
  }

  if (is_file(FilePath+"UpperState"))
    ReadUpperState();
  else {
    UpperState[0] = 100.0; UpperState[1] = 100.0; UpperState[2] = 2.0*PI; 
    UpperState[3] = MaxSteeringAngle;
  }

  // Make the list of Inputs
  Inputs.clear();  // Otherwise its parent constructor will make some inputs
  if (is_file(FilePath+"Inputs"))
    ReadInputs();
  else {
    Inputs.push_back(vector(1.0,0.0)); // Keep the steering angle fixed
    Inputs.push_back(vector(-1.0,0.0)); // Keep the steering angle fixed
    Inputs.push_back(vector(1.0,SteeringSpeed)); 
    Inputs.push_back(vector(-1.0,SteeringSpeed)); 
    Inputs.push_back(vector(1.0,-SteeringSpeed)); 
    Inputs.push_back(vector(-1.0,-SteeringSpeed)); 
   }
}



vector Model2DRigidCarSmooth::StateTransitionEquation(const vector &x, const vector &u) {

  vector dx(4);

  dx[0] = Speed*u[0]*cos(x[2]);
  dx[1] = Speed*u[0]*sin(x[2]);
  dx[2] = Speed*u[0]*tan(x[3])/CarLength;
  dx[3] = u[1];

  //cout << "DX: " << dx << "\n";

  return dx;
}




bool Model2DRigidCarSmooth::CollisionFree(const vector &x) {
  // Need to check steering limits (bounds on x[3])
  return ((Model2DRigid::CollisionFree(x))&&
	  (x[3] <= UpperState[3])&&
	  (x[3] >= LowerState[3]));
}


vector Model2DRigidCarSmooth::Integrate(const vector &x, const vector &u, const double &h)
{
  return RungeKuttaIntegrate(x,u,h);
}


double Model2DRigidCarSmooth::Metric(const vector &x1, const vector &x2) {

  double rho,dphi,dtheta;

  dphi = min(fabs(x1[3]-x2[3]),2.0*PI - fabs(x1[3]-x2[3]));
  dtheta = min(fabs(x1[2]-x2[2]),2.0*PI - fabs(x1[2]-x2[2]));

  rho = sqrt(sqr(x1[0] - x2[0]) + sqr(x1[1] - x2[1]) +
    sqr(2.0/PI*dphi) +
    sqr(50.0/PI*dtheta));

  return rho;
}


// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model2DRigidCarSmoothTrailer
//  Smooth steering 
// *********************************************************************
// *********************************************************************


Model2DRigidCarSmoothTrailer::Model2DRigidCarSmoothTrailer(string path = ""):Model2DRigidCarSmooth(path) {
  
  StateDim = 5;
  InputDim = 2;
  HitchLength = 10.0;
  HitchMaxAngle = PI/2.0;  // From 0 to PI (PI allows 360 spin of trailer)

  InitialState = vector(5);
  GoalState = vector(5);
  LowerState = vector(5);
  UpperState = vector(5);

  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  else {
    InitialState[0] = 30.0; InitialState[1] = 40.0; InitialState[2] = PI/2.0;
    InitialState[3] = 0.0; InitialState[4] = PI/2.0;
  }

  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  else {
    GoalState[0] = 75.0; GoalState[1] = 5.0; GoalState[2] = PI; 
    GoalState[3] = 0.0; GoalState[4] = PI;
  }

  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  else {
    LowerState[0] = 0.0; LowerState[1] = 0.0; LowerState[2] = 0.0; 
    LowerState[3] = -MaxSteeringAngle; LowerState[4] = 0.0;
  }

  if (is_file(FilePath+"UpperState"))
    ReadUpperState();
  else {
    UpperState[0] = 100.0; UpperState[1] = 100.0; UpperState[2] = 2.0*PI; 
    UpperState[3] = MaxSteeringAngle; UpperState[4] = 2.0*PI;
  }

  if (is_file(FilePath+"Inputs"))
    ReadInputs();

}



vector Model2DRigidCarSmoothTrailer::StateTransitionEquation(const vector &x, const vector &u) {

  vector dx(5);

  dx[0] = Speed*u[0]*cos(x[2]);
  dx[1] = Speed*u[0]*sin(x[2]);
  dx[2] = Speed*u[0]*tan(x[3])/CarLength;
  dx[3] = u[1];
  dx[4] = Speed*u[0]*sin(x[2] - x[4])/HitchLength;
  return dx;
}


// Assume the robot is a single polygon
list<polygon> Model2DRigidCarSmoothTrailer::TransformRobot(const vector &x) {
  list<polygon> trobot;
  polygon p,tp,tp2;

  trobot = Model2DRigid::TransformRobot(x);
  p = Robot.front();
  tp2 = p.rotate(x[4]).translate(x[0]-HitchLength*cos(x[4]),
				 x[1]-HitchLength*sin(x[4]));
  trobot.push_front(tp2);

  // Show the connection
  point p1(x[0],x[1]);
  point p2(x[0]-HitchLength*cos(x[4]),
	   x[1]-HitchLength*sin(x[4]));
  list<point> pl;
  pl.push(p1); pl.push(p2); pl.push(p1);
  polygon bar(pl);
  trobot.push_back(bar);

  return trobot;
}



bool Model2DRigidCarSmoothTrailer::CollisionFree(const vector &x) {
  polygon op,rp;
  segment seg,seg2;

  if ((x[3] >= UpperState[3])|| // Steering is too sharp!
      (x[3] <= LowerState[3])||
      (cos(x[2]-x[4]) < cos(HitchMaxAngle)))   // Hitch angle too great
    return false;  
  else  // Check if the chain of bodies collide
    return Model2DRigid::CollisionFree(x); // Relies on TransformRobot

}


double Model2DRigidCarSmoothTrailer::Metric(const vector &x1, const vector &x2) {

  double rho,dphi,dtheta,dtheta1;

  dphi = min(fabs(x1[3]-x2[3]),2.0*PI - fabs(x1[3]-x2[3]));
  dtheta = min(fabs(x1[2]-x2[2]),2.0*PI - fabs(x1[2]-x2[2]));
  dtheta1 = min(fabs(x1[4]-x2[4]),2.0*PI - fabs(x1[4]-x2[4]));

  rho = sqrt(sqr(x1[0] - x2[0]) + 
	     sqr(x1[1] - x2[1]) +
	     sqr(2.0/PI*dphi) +
	     sqr(5.0/PI*dtheta) +
	     sqr(5.0/PI*dtheta1));

  return rho;
}



// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model2DRigidCarSmooth2Trailers
//  Smooth steering 
// *********************************************************************
// *********************************************************************


Model2DRigidCarSmooth2Trailers::Model2DRigidCarSmooth2Trailers(string path = ""):Model2DRigidCarSmoothTrailer(path) {
  
  StateDim = 6;
  InputDim = 2;
  Hitch2Length = 10.0;
  Hitch2MaxAngle = PI/2.0;  // From 0 to PI (PI allows 360 spin of trailer)

  InitialState = vector(6);
  GoalState = vector(6);
  LowerState = vector(6);
  UpperState = vector(6);

  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  else {
    InitialState[0] = 30.0; InitialState[1] = 40.0; InitialState[2] = PI/2.0;
    InitialState[3] = 0.0; InitialState[4] = PI/2.0; InitialState[5] = PI/2.0;
  }

  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  else {
    GoalState[0] = 75.0; GoalState[1] = 5.0; GoalState[2] = PI; 
    GoalState[3] = 0.0; GoalState[4] = PI; GoalState[5] = PI;
  }

  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  else {
    LowerState[0] = 0.0; LowerState[1] = 0.0; LowerState[2] = 0.0; 
    LowerState[3] = -MaxSteeringAngle; LowerState[4] = 0.0;
    LowerState[5] = 0.0;
  }

  if (is_file(FilePath+"UpperState"))
    ReadUpperState();
  else {
    UpperState[0] = 100.0; UpperState[1] = 100.0; UpperState[2] = 2.0*PI; 
    UpperState[3] = MaxSteeringAngle; UpperState[4] = 2.0*PI;
    UpperState[5] = 2.0*PI;
  }

  if (is_file(FilePath+"Inputs"))
    ReadInputs();

}



vector Model2DRigidCarSmooth2Trailers::StateTransitionEquation(const vector &x, const vector &u) {

  vector dx(6);

  dx[0] = Speed*u[0]*cos(x[2]);
  dx[1] = Speed*u[0]*sin(x[2]);
  dx[2] = Speed*u[0]*tan(x[3])/CarLength;
  dx[3] = u[1];
  dx[4] = Speed*u[0]*sin(x[2] - x[4])/HitchLength;
  dx[5] = Speed*u[0]*cos(x[2] - x[4])*sin(x[4] - x[5])/Hitch2Length;
  return dx;
}


// Assume the robot is a single polygon
list<polygon> Model2DRigidCarSmooth2Trailers::TransformRobot(const vector &x) {
  list<polygon> trobot;
  polygon p,tp;

  p = Robot.front();

  // The car and first trailer
  trobot = Model2DRigidCarSmoothTrailer::TransformRobot(x);

  // Second trailer
  tp = p.rotate(x[5]).translate(x[0]-HitchLength*cos(x[4])
				-Hitch2Length*cos(x[5]),
				x[1]-HitchLength*sin(x[4])
				-Hitch2Length*sin(x[5]));
  trobot.push_front(tp);

  // Show the connection
  point p1(x[0]-HitchLength*cos(x[4]),x[1]-HitchLength*sin(x[4]));
  point p2(x[0]-HitchLength*cos(x[4])-Hitch2Length*cos(x[5]),
	   x[1]-HitchLength*sin(x[4])-Hitch2Length*sin(x[5]));
  list<point> pl;
  pl.push(p1); pl.push(p2); pl.push(p1);
  polygon bar2(pl);
  trobot.push_back(bar2);
  

  return trobot;
}



bool Model2DRigidCarSmooth2Trailers::CollisionFree(const vector &x) {
  polygon op,rp;
  segment seg,seg2;

  if (cos(x[4]-x[5]) < cos(Hitch2MaxAngle))  // Hitch angle too great
    return false;  
  else  // Check if the chain of bodies collide
    return Model2DRigidCarSmoothTrailer::CollisionFree(x); // Relies on TransformRobot

}


double Model2DRigidCarSmooth2Trailers::Metric(const vector &x1, const vector &x2) {

  double rho,dphi,dtheta,dtheta1,dtheta2;

  dphi = min(fabs(x1[3]-x2[3]),2.0*PI - fabs(x1[3]-x2[3]));
  dtheta = min(fabs(x1[2]-x2[2]),2.0*PI - fabs(x1[2]-x2[2]));
  dtheta1 = min(fabs(x1[4]-x2[4]),2.0*PI - fabs(x1[4]-x2[4]));
  dtheta2 = min(fabs(x1[5]-x2[5]),2.0*PI - fabs(x1[5]-x2[5]));

  rho = sqrt(sqr(x1[0] - x2[0]) + sqr(x1[1] - x2[1]) +
    sqr(2.0/PI*dphi) +
    sqr(5.0/PI*dtheta) +
    sqr(5.0/PI*dtheta1) +
    sqr(5.0/PI*dtheta2));

  return rho;
}


// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model2DRigidCarSmooth3Trailers
//  Smooth steering 
// *********************************************************************
// *********************************************************************


Model2DRigidCarSmooth3Trailers::Model2DRigidCarSmooth3Trailers(string path = ""):Model2DRigidCarSmooth2Trailers(path) {
  
  StateDim = 7;
  InputDim = 2;
  Hitch3Length = 10.0;
  Hitch3MaxAngle = PI/2.0;  // From 0 to PI (PI allows 360 spin of trailer)

  InitialState = vector(7);
  GoalState = vector(7);
  LowerState = vector(7);
  UpperState = vector(7);

  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  else {
    InitialState[0] = 30.0; InitialState[1] = 40.0; InitialState[2] = PI/2.0;
    InitialState[3] = 0.0; InitialState[4] = PI/2.0; InitialState[5] = PI/2.0; 
    InitialState[6] = PI/2.0;
  }

  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  else {
    GoalState[0] = 65.0; GoalState[1] = 5.0; GoalState[2] = PI; 
    GoalState[3] = 0.0; GoalState[4] = PI; GoalState[5] = PI; 
    GoalState[6] = PI;
  }

  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  else {
    LowerState[0] = 0.0; LowerState[1] = 0.0; LowerState[2] = 0.0; 
    LowerState[3] = -MaxSteeringAngle; LowerState[4] = 0.0;
    LowerState[5] = 0.0; LowerState[6] = 0.0;
  }

  if (is_file(FilePath+"UpperState"))
    ReadUpperState();
  else {
    UpperState[0] = 100.0; UpperState[1] = 100.0; UpperState[2] = 2.0*PI; 
    UpperState[3] = MaxSteeringAngle; UpperState[4] = 2.0*PI;
    UpperState[5] = 2.0*PI; UpperState[6] = 2.0*PI;
  }

  if (is_file(FilePath+"Inputs"))
    ReadInputs();

}



vector Model2DRigidCarSmooth3Trailers::StateTransitionEquation(const vector &x, const vector &u) {

  vector dx(7);

  dx[0] = Speed*u[0]*cos(x[2]);
  dx[1] = Speed*u[0]*sin(x[2]);
  dx[2] = Speed*u[0]*tan(x[3])/CarLength;
  dx[3] = u[1];
  dx[4] = Speed*u[0]*sin(x[2] - x[4])/HitchLength;
  dx[5] = Speed*u[0]*cos(x[2] - x[4])*sin(x[4] - x[5])/Hitch2Length;
  dx[6] = Speed*u[0]*cos(x[2] - x[4])*cos(x[4] - x[5])*sin(x[5]-x[6])/Hitch3Length;
  return dx;
}


// Assume the robot is a single polygon
list<polygon> Model2DRigidCarSmooth3Trailers::TransformRobot(const vector &x) {
  list<polygon> trobot;
  polygon p,tp;

  p = Robot.front();

  // The car and first two trailers
  trobot = Model2DRigidCarSmooth2Trailers::TransformRobot(x);

  // Third trailer
  tp = p.rotate(x[6]).translate(x[0]-HitchLength*cos(x[4])
				-Hitch2Length*cos(x[5])-Hitch3Length*cos(x[6]),
				x[1]-HitchLength*sin(x[4])
				-Hitch2Length*sin(x[5])-Hitch3Length*sin(x[6]));
  trobot.push_front(tp);

  // Show the connection
  point p1(x[0]-HitchLength*cos(x[4])-Hitch2Length*cos(x[5]),
	   x[1]-HitchLength*sin(x[4])-Hitch2Length*sin(x[5]));
  point p2(x[0]-HitchLength*cos(x[4])-Hitch2Length*cos(x[5])
	   -Hitch2Length*cos(x[6]),
	   x[1]-HitchLength*sin(x[4])-Hitch2Length*sin(x[5])
	   -Hitch2Length*sin(x[6]));
  list<point> pl;
  pl.push(p1); pl.push(p2); pl.push(p1);
  polygon bar2(pl);
  trobot.push_back(bar2);
  

  return trobot;
}


bool Model2DRigidCarSmooth3Trailers::CollisionFree(const vector &x) {
  polygon op,rp;
  segment seg,seg2;

  if (cos(x[5]-x[6]) < cos(Hitch3MaxAngle))  // Hitch angle too great
    return false;  
  else  // Check if the chain of bodies collide
    return Model2DRigidCarSmooth2Trailers::CollisionFree(x); // Relies on TransformRobot

}



double Model2DRigidCarSmooth3Trailers::Metric(const vector &x1, const vector &x2) {

  double rho,dphi,dtheta,dtheta1,dtheta2,dtheta3;

  dphi = min(fabs(x1[3]-x2[3]),2.0*PI - fabs(x1[3]-x2[3]));
  dtheta = min(fabs(x1[2]-x2[2]),2.0*PI - fabs(x1[2]-x2[2]));
  dtheta1 = min(fabs(x1[4]-x2[4]),2.0*PI - fabs(x1[4]-x2[4]));
  dtheta2 = min(fabs(x1[5]-x2[5]),2.0*PI - fabs(x1[5]-x2[5]));
  dtheta3 = min(fabs(x1[6]-x2[6]),2.0*PI - fabs(x1[6]-x2[6]));

  rho = sqrt(sqr(x1[0] - x2[0]) + sqr(x1[1] - x2[1]) +
    sqr(2.0/PI*dphi) +
    sqr(5.0/PI*dtheta) +
    sqr(5.0/PI*dtheta1) +
    sqr(5.0/PI*dtheta2) +
    sqr(5.0/PI*dtheta3));

  return rho;
}




// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model2DRigidDyncar
//  
// *********************************************************************
// *********************************************************************

// Constructor
Model2DRigidDyncar::Model2DRigidDyncar(string path = ""):Model2DRigid(path) {
  double alpha;
  vector v;

  StateDim = 5;
  InputDim = 1;
  Mass = 100.0;
  CAF = 17000.0;
  CAR = 20000.0;
  Adist = 4.0;
  Bdist = 5.0;
  Izz = 1600.0;

  WorldScale = 0.1;
  Speed = 88.0;   // Feet per sec
  
  InitialState = vector(5);
  GoalState = vector(5);
  LowerState = vector(5);
  UpperState = vector(5);

  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  else {
    InitialState[0] = 0.0; InitialState[1] = 0.0; InitialState[2] = 5.0; 
    InitialState[3] = -540.0; InitialState[4] = 0.0;
  }

  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  else {
    GoalState[0] = 0.0; GoalState[1] = 0.0; GoalState[2] = 995.0; 
    GoalState[3] = -540.0; GoalState[4] = 0.0;
  }

  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  else {
    LowerState[0] = -50.0; LowerState[1] = -5.0; LowerState[2] = 0.0; 
    LowerState[3] = -1000.0; LowerState[4] = 0.0;
  }

  if (is_file(FilePath+"UpperState"))
    ReadUpperState();
  else {
    UpperState[0] = 50.0; UpperState[1] = 5.0; UpperState[2] = 1000.0; 
    UpperState[3] = 0.0; UpperState[4] = 2.0*PI;
  }

  MaxSteeringAngle = 0.6;

  // Make the list of 1D Inputs
  Inputs.clear();  // Otherwise its parent constructor will make some inputs
  if (is_file(FilePath+"Inputs"))
    ReadInputs();
  else {
    for (alpha = -MaxSteeringAngle; alpha <= MaxSteeringAngle; 
	 alpha += 2.0*MaxSteeringAngle/24.0) {
      v = vector(1); v[0] = alpha;
      Inputs.push_back(v);
    }
  }
}


// This is xdot = f(x,u) for a 5dof state-space, and 1dof input
//  Model taken from Jim Bernard...
// alphaf = (v + a*r)/u - deltaf
// alphar = (v-b*r)/u
// FYF = -CAF*alphaf
// FYR = -CAR*alphar
// vdot = -u*r  + (FYF + FYR)/m
// rdot = (FYF*a - FYR*b)/Izz
// Xdot = u*cos(psi) - v*sin(psi)
// Ydot = u*sin(spi) + v*cos(psi)
// psidot = r
// 
// m = mass of car, say about 100 slugs
// CAF = front cornering stiffness in pounds per radiaof the tires, say about
// 17000
// CAR = rear cornering stiffness, say about 20000
// a is dist from mass center to front tires, say 4 feet
// b is dist from mass center to rear tires, say 5 feet
// Izz is yaw moment of intertia, say about 1600 slug ft**2
// u is forward speed which is assumed constant, input is in feet/sec
// delta is your input, it is the steer angle of the tires in radians.

vector Model2DRigidDyncar::StateTransitionEquation(const vector &x, const vector &u)
{
  double alphaf,alphar,fyf,fyr,v,r,psi;
  vector dx(5);

  v = x[0]; r = x[1]; psi = x[4];

  alphaf = (v + Adist * r) / Speed - u[0];
  alphar = (v - Bdist * r) / Speed;
  fyf = -CAF * alphaf;
  fyr = -CAR * alphar;

  /* Transfer the velocity */
  dx[0] = -Speed * r  + (fyf + fyr) / Mass;
  dx[1] = (fyf * Adist - fyr * Bdist) / Izz;
  dx[2] = Speed * cos(psi) - v * sin(psi);
  dx[3] = Speed * sin(psi) + v * cos(psi);
  dx[4] = r;

  //cout << "x: " << x << "  Dx: " << dx << "  u: " << u[0] << "\n";

  return dx;
}



double Model2DRigidDyncar::Metric(const vector &x1, const vector &x2) {
  double d = 0.0;
  int i;

  for (i = 0; i < 5; i++) {
    d += sqr((x1[i] - x2[i]) / (UpperState[i] - LowerState[i]));
  }

  return sqrt(d);
}



vector Model2DRigidDyncar::Integrate(const vector &x, const vector &u, const double &h)
{
  return RungeKuttaIntegrate(x,u,h);
}



point Model2DRigidDyncar::StateToPoint(const vector &x) {
  return point(x[2]*WorldScale,-x[3]*WorldScale);
}



// NOTE: This neglects the S^1 effect of orientation!!
vector Model2DRigidDyncar::StateToConfiguration(const vector &x) {
  return vector(x[2]*WorldScale,-x[3]*WorldScale,-x[4]); 
}



// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model2DRigidDyncarNtire
//  
// *********************************************************************
// *********************************************************************

// Constructor
Model2DRigidDyncarNtire::Model2DRigidDyncarNtire(string path = ""):Model2DRigidDyncar(path) {
  double alpha;
  vector v;

  StateDim = 5;
  InputDim = 2;

  // These are the exact parameters from Ric's model
  Mass = 3518.0/32.2;
  Adist = 0.45*100.5/12.0;
  Bdist = 0.55*100.5/12.0;
  Izz = 0.25*Mass*(Adist+Bdist)*(Adist+Bdist);

  // Constants for the nonlinear tire model
  Mu = 0.85;
  Nf = Mass*32.2*0.55;
  Nr = Mass*32.2*0.45;

  // The defaults are inherited from parent class
  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  if (is_file(FilePath+"UpperState"))
    ReadUpperState();


  // Make the list of 2D Inputs
  // Apparently, this models allows rear-wheel steering
  //    This option is set to zero for now...hence v[1]=0.0
  Inputs.clear();  // Otherwise its parent constructor will make some inputs
  if (is_file(FilePath+"Inputs"))
    ReadInputs();
  else {
    for (alpha = -MaxSteeringAngle; alpha <= MaxSteeringAngle; 
	 alpha += 2.0*MaxSteeringAngle/24.0) {
      v = vector(2); v[0] = alpha; v[1] = 0.0;
      Inputs.push_back(v);
    }
  }
}


// The is the model from Jim Bernard and Ric Menendez
vector Model2DRigidDyncarNtire::StateTransitionEquation(const vector &x, const vector &u)
{
  double alphaf,alphar,fyf,fyr,v,r,psi;
  double talff,talfr,xiblf,xiblr;
  vector dx(5);
  

  v = x[0]; r = x[1]; psi = x[4];

  alphaf = (v + Adist * r) / Speed - u[0];
  alphar = (v - Bdist * r) / Speed - u[1];

  // Here is where the nonlinear tire model is used
  //  The result is new values for fyf and fyr
  talff = tan(fabs(alphaf));
  talfr = tan(fabs(alphar));
  xiblf = (CAF*talff == 0) ? 
    INFINITY :
    Mu*Nf/(2.0*CAF*talff);
  xiblr = (CAR*talfr == 0) ? 
    INFINITY :
    Mu*Nr/(2.0*CAR*talfr);
  fyf = (xiblf >= 1.0) ?
    CAF*talff :
    Mu*Nf*(1.0-0.5*xiblf);
  fyr = (xiblr >= 1.0) ?
    CAR*talfr :
    Mu*Nr*(1.0-0.5*xiblr);
  fyf = (alphaf > 0) ? -1.0*fabs(fyf) : fabs(fyf);
  fyr = (alphar > 0) ? -1.0*fabs(fyr) : fabs(fyr);

  //cout << "talff: " << talff << "  xiblf: " << xiblf << " fyf: " << fyf << "\n";

  /* Transfer the velocity */
  dx[0] = -Speed * r  + (fyf + fyr) / Mass;
  dx[1] = (fyf * Adist - fyr * Bdist) / Izz;
  dx[2] = Speed * cos(psi) - v * sin(psi);
  dx[3] = Speed * sin(psi) + v * cos(psi);
  dx[4] = r;

  //cout << "x: " << x << "  Dx: " << dx << "  u: " << u[0] << "\n";

  return dx;
}







// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model2DRigidLander
//  
// *********************************************************************
// *********************************************************************

// Constructor
Model2DRigidLander::Model2DRigidLander(string path = ""):Model2DRigid(path) {
  vector v;

  StateDim = 4;
  InputDim = 1;
  Mass = 1.0;
  G = 1.568;  // Accel of gravity on moon (use 9.8 for earth)
  Fs = 10.0;
  Fu = 20.0;

  InitialState = vector(4);
  GoalState = vector(4);
  LowerState = vector(4);
  UpperState = vector(4);

  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  else {
    InitialState[0] = 90.0; InitialState[1] = 65.0; InitialState[2] = 0.0; 
    InitialState[3] = 5.0; 
  }

  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  else {
    GoalState[0] = 45.0; GoalState[1] = 15.0; GoalState[2] = 0.0; 
    GoalState[3] = 0.0; 
  }

  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  else {
    LowerState[0] = 0.0; LowerState[1] = 0.0; LowerState[2] = -10.0; 
    LowerState[3] = -10.0; 
  }

  if (is_file(FilePath+"UpperState"))
    ReadUpperState();
  else {
    UpperState[0] = 100.0; UpperState[1] = 100.0; UpperState[2] = 10.0; 
    UpperState[3] = 10.0; 
  }

  // Make the list of 1D Inputs
  Inputs.clear();  // Otherwise its parent constructor will make some inputs
  if (is_file(FilePath+"Inputs"))
    ReadInputs();
  else {
    v = vector(1); v[0] = 0;
    Inputs.push_back(v);
    v = vector(1); v[0] = 1;
    Inputs.push_back(v);
    v = vector(1); v[0] = 2;
    Inputs.push_back(v);
    v = vector(1); v[0] = 3;
    Inputs.push_back(v);
  }
}



vector Model2DRigidLander::Integrate(const vector &x, const vector &u, const double &h)
{
  return RungeKuttaIntegrate(x,u,h);
}



vector Model2DRigidLander::StateToConfiguration(const vector &x) {
  return vector(x[0],x[1],0.0); // Yield a zero rotation every time
}


vector Model2DRigidLander::StateTransitionEquation(const vector &x, const vector &u)
{
  vector dx(4);

  /* Transfer the velocity */
  dx[0] = x[2];
  dx[1] = x[3];
  dx[2] = 0.0;
  if (u[0] == 1) 
    dx[2] = Fs;
  if (u[0] == 3) 
    dx[2] = -Fs;
  dx[3] = -Mass*G;
  if (u[0] == 2) 
    dx[3] += Fu;

  //cout << "x: " << x << "  Dx: " << dx << "  u: " << u[0] << "\n";

  return dx;
}





double Model2DRigidLander::Metric(const vector &x1, const vector &x2) {
  double d = 0.0;
  int i;

  for (i = 0; i < 4; i++) {
    d += sqr((x1[i] - x2[i]) / (UpperState[i] - LowerState[i]));
  }
  
  //cout << "x1: " << x1 << "  x2: " << x2 << "   Metric: " << d << "\n";

  return sqrt(d);
}



// *********************************************************************
// *********************************************************************
// CLASS:     Model::Model2DRigidChain
//  
// *********************************************************************
// *********************************************************************

Model2DRigidChain::Model2DRigidChain(string path = ""):Model2DRigid(path) {

  int i;

  NumLinks = Robot.length();
  StopAngle = PI/1.5;

  StateDim = NumLinks+2;
  InputDim = NumLinks+2;

  file_istream fin(FilePath + "A");
  fin >> A;
  
  InitialState = vector(NumLinks+2);
  GoalState = vector(NumLinks+2);
  LowerState = vector(NumLinks+2);
  UpperState = vector(NumLinks+2);

  LowerState[0] = 0.0; UpperState[0] = 100.0;
  LowerState[1] = 0.0; UpperState[1] = 100.0;

  if (is_file(FilePath+"InitialState"))
    ReadInitialState();
  else {
    InitialState[0] = 15.0; 
    InitialState[1] = 10.0; 
    for (i = 0; i < NumLinks; i++) {
      InitialState[i+2] = 0.0;
    }
  }

  if (is_file(FilePath+"GoalState"))
    ReadGoalState();
  else {
    GoalState[0] = 70.0; 
    GoalState[1] = 75.0; 
    for (i = 0; i < NumLinks; i++) {
      GoalState[i+2] = 0.0;
    }
  }

  if (is_file(FilePath+"LowerState"))
    ReadLowerState();
  else {
    LowerState[0] = 0.0; 
    LowerState[1] = 0.0; 
    for (i = 0; i < NumLinks; i++) {
      LowerState[i+2] = -StopAngle;
    }
  }

  if (is_file(FilePath+"UpperState"))
    ReadUpperState();
  else {
    UpperState[0] = 100.0; 
    UpperState[1] = 100.0; 
    for (i = 0; i < NumLinks; i++) {
      UpperState[i+2] = StopAngle;
    }
  }

  Inputs.clear();  // Otherwise its parent constructor will make some inputs
  if (is_file(FilePath+"Inputs"))
    ReadInputs();
  else {
    // NEED A DEFAULT HERE!!!!
  }

}



vector Model2DRigidChain::LinearInterpolate(const vector &x1, 
					    const vector &x2, 
					    const double &a) {
  return (1.0 - a)*x1 + a*x2;
}



list<polygon> Model2DRigidChain::TransformRobot(const vector &x) {
  list<polygon> trobot,tnrobot,utrobot;
  polygon p,tp;
  int i;

  utrobot = Robot;
  trobot.clear();
  tnrobot.clear();

  for (i = NumLinks - 1; i >= 0; i--) {
   trobot = tnrobot;
    tnrobot.clear();
    trobot.push(utrobot.pop_back());
    forall(p,trobot) {
      tp = p.rotate(x[i+2]);
      if (i > 0)
	tp = tp.translate(A[i-1],0.0);
      else
	tp = tp.translate(x[0],x[1]);
      tnrobot.append(tp);
    }
  }

  //cout << "tnrobot: " << tnrobot << "\n";

  return tnrobot;
}


vector Model2DRigidChain::StateTransitionEquation(const vector &x, const vector &u) {

  vector dx(StateDim);

  dx = Speed*u;
  return dx;
}



double Model2DRigidChain::Metric(const vector &x1, const vector &x2) {

  double rho;
  vector dtheta(StateDim);
  int i;
  
  rho = sqr(x1[0] - x2[0]) + sqr(x1[1] - x2[1]);

  for (i = 2; i < StateDim; i++) {
    dtheta[i] = min(fabs(x1[i]-x2[i]),2.0*PI - fabs(x1[i]-x2[i]));
    rho += sqr(50.0/PI*dtheta[i]);
  }

  rho = sqrt(rho);

  return rho;
}


// Use the inherited checker for robot-obst collisions
// Check also for robot-robot collisions
bool Model2DRigidChain::CollisionFree(const vector &x) {
  segment seg1,seg2;
  int i,j;

  if (!Model2DRigid::CollisionFree(x))
    return false;
  else {
    list<polygon> trobot = TransformRobot(x);
    for (i = 0; i < NumLinks; i++) {
      for (j = i+2; j < NumLinks; j++) {
	forall_segments(seg1,trobot[trobot[i]]) {
	  forall_segments(seg2,trobot[trobot[j]]) {
	    if (seg1.intersection(seg2)) {
	      return false;
	    }
	  }
	}
      }
    }
  }

  return true;
}




