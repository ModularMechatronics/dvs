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


#include <ANN/ANN.h>			// ANN declarations
#include <LEDA/graph/graph.h>
#include <LEDA/core/list.h>
#include <LEDA/numbers/vector.h>

using namespace leda;

#include "nn.h"

ANN::ANN(const GRAPH<vector,vector> &g) {
  node n;
  int i,j;

  numPoints = g.number_of_nodes();
  n = g.first_node();
  if (n)
    dimension = g.inf(n).dim();
  else
    {
      std::cout << "ERROR: Empty graph in ANN constructor\n";
      exit(-1);
    }

  query_pt = annAllocPt(dimension);	// allocate query point
  data_pts = annAllocPts(numPoints, dimension); 	// allocate data points
  nn_idx = new ANNidx[1];   // allocate near neigh indices
  dists = new ANNdist[1];   // allocate near neighbor dists
  epsilon = 0.0;

  node_pointers = new node[numPoints];

  // Copy nodes to data_pts
  i = 0;
  forall_nodes(n,g) {
    node_pointers[i] = n;
    for (j = 0; j < dimension; j++) {
      data_pts[i][j] = g.inf(n)[j];
    }
    i++;
  }

  // Initialize ANN
  the_tree = new ANNkd_tree(data_pts,			// the data points
			    numPoints,			// number of points
			    dimension);			// dimension of space

}



ANN::ANN(const GRAPH<vector,vector> &g, const node &n1, const node &n2, int size) {
  node n;
  int i,j;

  numPoints = size;
  dimension = g.inf(n1).dim();

  query_pt = annAllocPt(dimension);	// allocate query point
  data_pts = annAllocPts(numPoints, dimension); 	// allocate data points
  nn_idx = new ANNidx[1];   // allocate near neigh indices
  dists = new ANNdist[1];   // allocate near neighbor dists
  epsilon = 0.0;

  node_pointers = new node[numPoints];

  // Copy nodes to data_pts
  i = 0;
  n = n1;
  while (n != g.succ_node(n2)) {
    node_pointers[i] = n;
    for (j = 0; j < dimension; j++) {
      data_pts[i][j] = g.inf(n)[j];
    }
    n = g.succ_node(n);
    i++;
  }

  // Initialize ANN
  the_tree = new ANNkd_tree(data_pts,			// the data points
			    numPoints,			// number of points
			    dimension);			// dimension of space

}





ANN::~ANN() {
  annDeallocPts(data_pts); 	// allocate data points
  delete(nn_idx);
  delete(dists);
  delete(the_tree);
}



node ANN::NearestNeighbor(const vector &x) {
  node n;
  int i;

  // Transfer x to query_pt
  for (i = 0; i < x.dim(); i++) {
    query_pt[i] = x[i];
  }

  the_tree->annkSearch(			
		       query_pt,			// query point
		       1,				// number of near neighbors
		       nn_idx,				// nearest neighbors (returned)
		       dists,				// distance (returned)
		       epsilon);	       		// error bound
  
  n = node_pointers[nn_idx[0]];

  //cout << "nn_idx: " << nn_idx[0] << "\n";
  //cout << "n: " << n << "\n";

  return n;
}



node ANN::LastNode() {
  return node_pointers[numPoints-1];
}






