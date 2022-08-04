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


// Highest-level class.  All others are derived from Model.
class Model {
 protected:
  list<vector> Inputs;
 public:
  list<polygon> Obst;  // Ultimately, these should be a class called Region
  list<polygon> Robot; //    with Region2D, Region3D, etc...
  double Speed;
  vector InitialState;
  vector GoalState;
  vector LowerState;   // Vector of minimum values for each state variable
  vector UpperState;   // Vector of maximum values for each state variable
  int StateDim;
  int InputDim;
  random_source R;
  string FilePath;
  vector RungeKuttaIntegrate(const vector &x, const vector &u, const double &h);
  vector EulerIntegrate(const vector &x, const vector &u, const double &h);
  vector RandomState();
  vector NormalState(double sd);
  void ReadInputs();
  void ReadInitialState();
  void ReadGoalState();
  void ReadLowerState();
  void ReadUpperState();
  Model(string path) {};
  virtual ~Model() {};
  virtual list<vector> GetInputs(const vector &x);
  virtual point StateToPoint(const vector &x); // Get a 2D point from state
  virtual vector StateToConfiguration(const vector &x);
  virtual vector Integrate(const vector &x, const vector &u, 
			   const double &h) = 0; 
  virtual vector StateTransitionEquation(const vector &x, const vector &u) = 0;
  virtual double Metric(const vector &x1, const vector &x2);
  virtual bool CollisionFree(const vector &x);
  virtual double DistanceComp(const vector &x);  // Distance in world
  virtual vector LinearInterpolate(const vector &x1, const vector &x2, 
				   const double &a);  // Depends on topology
  virtual list<polygon> TransformRobot(const vector &x);
};


// A simple one-dimensional model for dynamics studies
class Model1D: public Model {
 public:
  double Force;
  Model1D(string path);
  virtual ~Model1D() {};
  virtual point StateToPoint(const vector &x);
  virtual vector StateToConfiguration(const vector &x);
  virtual vector Integrate(const vector &x, const vector &u, const double &h);
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
  virtual double Metric(const vector &x1, const vector &x2);
};


class ModelLinear: public Model {
 public:
  matrix A;
  matrix B;
  ModelLinear(string path);
  virtual ~ModelLinear() {};
  void ReadA();  // Read state transition matrix
  void ReadB();  // Read input matrix
  virtual point StateToPoint(const vector &x);
  virtual vector StateToConfiguration(const vector &x);
  virtual vector Integrate(const vector &x, const vector &u, const double &h);
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
};

// The "nonholonomic integrator".   This model has been considered
// by R. Brockett and many others.
class ModelNintegrator: public Model {
 public:
  double UBound;
  double VBound;
  ModelNintegrator(string path);
  virtual ~ModelNintegrator() {};
  virtual point StateToPoint(const vector &x);
  virtual vector StateToConfiguration(const vector &x);
  virtual vector Integrate(const vector &x, const vector &u, const double &h);
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
};


// A planning problem in a 2D world
// There are two derived classes: Model2DPoint and Model2DRigid
class Model2D: public Model {
 public:
  Model2D(string path);
  virtual ~Model2D() {};
};


// A point robot in a 2D world
class Model2DPoint: public Model2D {
 public:
  Model2DPoint(string path);
  virtual ~Model2DPoint() {};
  virtual vector Integrate(const vector &x, const vector &u, const double &h);
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
  virtual double Metric(const vector &x1, const vector &x2);
  virtual bool CollisionFree(const vector &x);
  virtual double DistanceComp(const vector &x);
};


// A point car-like robot in a 2D world
class Model2DPointCar: public Model2DPoint {
 public:
  double MaxSteeringAngle;
  double CarLength;
  Model2DPointCar(string path);
  virtual ~Model2DPointCar() {};
  virtual vector Integrate(const vector &x, const vector &u, const double &h);
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
  virtual double Metric(const vector &x1, const vector &x2);
};


// A rigid robot in a 2D world.  This class can be used for holonomic
// planning in Re^2 X S^1.  Nonholonomic problems are derived.
class Model2DRigid: public Model2D {
 public:
  Model2DRigid(string path);
  virtual ~Model2DRigid() {};
  void MakeBitmap();
  bool *Bitmap;
  bool MadeBitmap; // true if the bitmap has been made
  virtual vector Integrate(const vector &x, const vector &u, const double &h);
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
  virtual double Metric(const vector &x1, const vector &x2);
  virtual bool CollisionFree(const vector &x);
  virtual double DistanceComp(const vector &x);
  virtual vector LinearInterpolate(const vector &x1, const vector &x2, 
				   const double &a);
  virtual list<polygon> TransformRobot(const vector &x);
};

// A rigid car-like robot in a 2D world
class Model2DRigidCar: public Model2DRigid {
 public:
  double MaxSteeringAngle;
  double CarLength;
  Model2DRigidCar(string path);
  virtual ~Model2DRigidCar() {};
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
};


// A rigid car-like robot that can only go forward in a 2D world
class Model2DRigidCarForward: public Model2DRigidCar {
 public:
  Model2DRigidCarForward(string path);
  virtual ~Model2DRigidCarForward() {};
};


// A rigid car-like robot with continuous steering angles
// This model is used by Th. Fraichard, Scheuer, Laugier
class Model2DRigidCarSmooth: public Model2DRigidCar {
 public:
  double SteeringSpeed;
  Model2DRigidCarSmooth(string path);
  virtual ~Model2DRigidCarSmooth() {};
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
  vector Integrate(const vector &x, const vector &u, const double &h);
  virtual double Metric(const vector &x1, const vector &x2);
  virtual bool CollisionFree(const vector &x);
};

// A rigid car-like robot with continuous steering angles and 
// a trailer.
// The trailer models are taken from Murray and Sastry, Trans. 
//  Automatic Control, Vol 38, No 5, 1993, pp. 700-716
class Model2DRigidCarSmoothTrailer: public Model2DRigidCarSmooth {
 public:
  double HitchLength;
  double HitchMaxAngle;
  Model2DRigidCarSmoothTrailer(string path);
  virtual ~Model2DRigidCarSmoothTrailer() {};
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
  virtual list<polygon> TransformRobot(const vector &x);
  virtual double Metric(const vector &x1, const vector &x2);
  virtual bool CollisionFree(const vector &x);
};


// A rigid car-like robot with continuous steering angles and 
// two trailers.
class Model2DRigidCarSmooth2Trailers: public Model2DRigidCarSmoothTrailer {
 public:
  double Hitch2Length;
  double Hitch2MaxAngle;
  Model2DRigidCarSmooth2Trailers(string path);
  virtual ~Model2DRigidCarSmooth2Trailers() {};
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
  virtual list<polygon> TransformRobot(const vector &x);
  virtual double Metric(const vector &x1, const vector &x2);
  virtual bool CollisionFree(const vector &x);
};


// A rigid car-like robot with continuous steering angles and 
// three trailers.
class Model2DRigidCarSmooth3Trailers: public Model2DRigidCarSmooth2Trailers {
 public:
  double Hitch3Length;
  double Hitch3MaxAngle;
  Model2DRigidCarSmooth3Trailers(string path);
  virtual ~Model2DRigidCarSmooth3Trailers() {};
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
  virtual list<polygon> TransformRobot(const vector &x);
  virtual double Metric(const vector &x1, const vector &x2);
  virtual bool CollisionFree(const vector &x);
};



// A 5DOF dynamical model of a rigid car.  This model uses a linear
// tire model, which is far from reality.  The model was donated by
// Jim Bernard (who has made no claims on its accuracy).
class Model2DRigidDyncar: public Model2DRigid {
 public:
  double Mass;       // slugs
  double CAF;        // Front cornering stiffness
  double CAR;        // Rear cornering stiffness
  double Adist;      // Mass center to front tires - feet
  double Bdist;      // Mass center to rear tires - feet
  double Izz;        // yaw moment of interia - ft slugs^2
  double WorldScale; // Feet per world unit (100x100 world)
  double MaxSteeringAngle;
  Model2DRigidDyncar(string path);
  virtual ~Model2DRigidDyncar() {};
  vector Integrate(const vector &x, const vector &u, const double &h);
  virtual point StateToPoint(const vector &x);
  virtual vector StateToConfiguration(const vector &x);
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
  virtual double Metric(const vector &x1, const vector &x2);
};


// A 5DOF dynamical model of a rigid car.  This model uses a nonlinear
// tire model.  The model was donated by Jim Bernard (who has made no 
// claims on its accuracy).
class Model2DRigidDyncarNtire: public Model2DRigidDyncar {
 public:
  // The following constants represent the nonlinear tire model
  double Mu;
  double Nf;
  double Nr;
  Model2DRigidDyncarNtire(string path);
  virtual ~Model2DRigidDyncarNtire() {};
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
};


// A rigid body with two small side thrusters, and a larger lower
// thruster.  The goal is to navigate and softly "land" the craft by
// firing thrusters, in spite of gravity.
class Model2DRigidLander: public Model2DRigid {
 public:
  double Mass;       // kg
  double G;          // Accel of gravity (m/s)
  double Fs;         // Side thruster force
  double Fu;         // Upward thruster force
  Model2DRigidLander(string path);
  virtual ~Model2DRigidLander() {};
  vector Integrate(const vector &x, const vector &u, const double &h);
  virtual vector StateToConfiguration(const vector &x);
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
  virtual double Metric(const vector &x1, const vector &x2);
};


// A 2D kinematic chain of bodies.
class Model2DRigidChain: public Model2DRigid {
 public:
  int NumLinks;
  vector A;
  double StopAngle;  // From 0 to PI
  Model2DRigidChain(string path);
  virtual ~Model2DRigidChain() {};
  virtual vector LinearInterpolate(const vector &x1, const vector &x2, 
				   const double &a);
  virtual list<polygon> TransformRobot(const vector &x);
  virtual bool CollisionFree(const vector &x);
  virtual vector StateTransitionEquation(const vector &x, const vector &u);
  virtual double Metric(const vector &x1, const vector &x2);
};



