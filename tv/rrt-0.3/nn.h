//----------------------------------------------------------------------
//      File:           nn.C
//      Programmer:     Steve LaValle
//      Last modified:  10/19/99 (Release 0.1)
//      Description:    Interface class for ANN package (S. Arya, D. Mount)
//                      The ANN package is available at
//                         http://www.cs.umd.edu/~mount/ANN/
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


class ANN {
 private:
  int numPoints;  // Total number currently stored in the ANN tree
  int dimension;
  ANNpointArray	data_pts;		// data points
  ANNpoint query_pt;		// query point
  ANNidxArray nn_idx;			// near neighbor indices
  ANNdistArray dists;			// near neighbor distances
  ANNkd_tree *the_tree;		// search structure
  double epsilon;
  node *node_pointers;
 public:
  ANN(const GRAPH<vector,vector> &g);
  ANN(const GRAPH<vector,vector> &g, const node &n1, const node &n2, int size);
  ~ANN();
  void UpdateTree(const GRAPH<vector,vector> &g); // Actually rebuilds from scratch
  node NearestNeighbor(const vector &x); // Nearest neighbor query
  node LastNode(); // Last node in G that is stored in the ANN tree
};



