// Converter from idraw to ASCII polygons
// Finally converted to LEDA/C++ by Steve LaValle  12/19/98
// This makes it easy to generate new 2D environments
// Be careful: this problem is sometimes sensitive to the
// exact version of idraw.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <LEDA/geo/polygon.h>
#include <LEDA/core/list.h>
#include <LEDA/geo/point.h>

using namespace leda;

#define BUFSIZE 100
#define NUMREGIONS 1

#define SCALED_X_MAX 100.0
#define SCALED_X_MIN 0.0
#define SCALED_Y_MIN 0.0
#define INFINITY 1.0e40

char   buf[BUFSIZE];

list<polygon> rv[NUMREGIONS];
char *rnames[] = {"OBST"};
char *rcolors[] = {"DkGray"};
int  rcenter[] = {0}; /* Should the region be centered at the origin? */



point idraw_transform_point(double x, double y,
			    double m1, double m2, double m3,
			    double m4, double m5, double m6)
{
  point *p = new point(x*m1-y*m2+m5,y*m4-x*m3+m6);
  return *p;
}


void convert_rectangle(FILE *ifp)
{
  char color[20];
  float m1,m2,m3,m4,m5,m6,x1,y1,x2,y2;
  int i;
  list<point> plist;
  polygon *p;

  //cout << "RECTANGLE DETECTED" << endl;

  fgets(buf,BUFSIZE,ifp);
  //fgets(buf,BUFSIZE,ifp);  
  fgets(buf,BUFSIZE,ifp);
  strcpy(color,&buf[7]);

  while (strncmp("concat",&buf[strlen(buf) - 7],6) != 0) {
    fgets(buf,BUFSIZE,ifp);
  }
  sscanf(buf,"[ %f %f %f %f %f %f ]",&m1,&m2,&m3,&m4,&m5,&m6);


  fgets(buf,BUFSIZE,ifp);
  fgets(buf,BUFSIZE,ifp);

  sscanf(buf,"%f %f %f %f",&x1,&y1,&x2,&y2);

  // cout << "color: " << color << "\n";

  for (i=0; i < NUMREGIONS; i++) {
    if (strncmp(color,rcolors[i],strlen(color) - 1) == 0)
      {
	plist.push(idraw_transform_point(x1,y2,m1,m2,m3,m4,m5,m6) );
	plist.push(idraw_transform_point(x2,y2,m1,m2,m3,m4,m5,m6) );
	plist.push(idraw_transform_point(x2,y1,m1,m2,m3,m4,m5,m6) );
	plist.push(idraw_transform_point(x1,y1,m1,m2,m3,m4,m5,m6) );
	p = new polygon(plist);
	// cout << "poly " << *p << "\n";
	rv[i].push(*p);
      }
  }
}


void convert_polygon(FILE *ifp)
{
  char color[20];
  float m1,m2,m3,m4,m5,m6,x,y;
  int i;
  list<point> plist;

  //cout << "POLYGON DETECTED" << endl;

  fgets(buf,BUFSIZE,ifp);
  fgets(buf,BUFSIZE,ifp);
  fgets(buf,BUFSIZE,ifp);
  strcpy(color,&buf[7]);

  while (strncmp("concat",&buf[strlen(buf) - 7],6) != 0) {
    fgets(buf,BUFSIZE,ifp);
  }
  sscanf(buf,"[ %f %f %f %f %f %f ]",&m1,&m2,&m3,&m4,&m5,&m6);

  int numpts;
  fgets(buf,BUFSIZE,ifp);
  sscanf(buf,"%%I %d",&numpts);

  for (i=0; i < numpts; i++) {
    fgets(buf,BUFSIZE,ifp);
    sscanf(buf,"%f %f",&x,&y);
    plist.push(idraw_transform_point(x,y,m1,m2,m3,m4,m5,m6) );
  }

  for (i=0; i < NUMREGIONS; i++) {
    if (strncmp(color,rcolors[i],strlen(color) - 1) == 0)
      {
	polygon *p = new polygon(plist);
	rv[i].push(*p);
      }
  }
}


void idraw_normalize_results()
{
  int i;
  double minx = INFINITY;
  double miny = INFINITY;
  double maxx = -INFINITY;
  polygon poly,*npoly;
  point pt,*npt;
  list_item it;
  list<point> plist;

  // Find the min and max x values for the non-shiftable regions
  for (i=0; i < NUMREGIONS; i++) {
    if (!rcenter[i]) {
      forall(poly,rv[i]) {
	forall_vertices(pt,poly) {
	  if (pt.xcoord() < minx)
	    minx = pt.xcoord();
	  if (pt.ycoord() < miny)
	    miny = pt.ycoord();
	  if (pt.xcoord() > maxx)
	    maxx = pt.xcoord();
	}
      }
    }
  }

  double scale = (SCALED_X_MAX - SCALED_X_MIN) / (maxx - minx);

  //cout << "scale: " << scale << "  minx: " << minx << "  maxx" << maxx << "\n";

  // Scale all of the regions
  for (i=0; i < NUMREGIONS; i++) {
    forall_items(it,rv[i]) {
      plist.clear();
      forall_vertices(pt,rv[i].inf(it)) {
	npt = new point(SCALED_X_MIN + scale * (pt.xcoord() - minx),
			SCALED_Y_MIN + scale * (pt.ycoord() - miny));
	plist.push(*npt);
      }
      npoly = new polygon(plist);
      rv[i].assign(it,*npoly);
    }
  }

  // Need to move the shiftable regions to the origin
  double sumx,sumy;
  int numpts;
  for (i=0; i < NUMREGIONS; i++) {

    sumx = 0.0; sumy = 0.0; numpts = 0;

    if (rcenter[i]) {
      forall(poly,rv[i]) {
	forall_vertices(pt,poly) {
	  numpts++;
	  sumx += pt.xcoord();
	  sumy += pt.ycoord();
	}
      }
      forall_items(it,rv[i]) { 
	rv[i].assign(it,rv[i].inf(it).translate(-sumx/numpts,-sumy/numpts));
      }
    }
  }
}




int main(int argc,char **argv)
{
  FILE *ifp;
  polygon poly;
  point pt;
  int i;

  ifp = fopen(argv[1], "r");

  if (!ifp) {
    cerr << "File Open Failed\n";
    exit(-1);
  }

  fgets(buf,BUFSIZE,ifp);
  while (strncmp("%%Page: 1 1",buf,strlen("%%Page: 1 1")) != 0) {
    fgets(buf,BUFSIZE,ifp);
  }
  fgets(buf,BUFSIZE,ifp);

  while (fgets(buf,BUFSIZE,ifp) != NULL) {
    if (strncmp("Begin %I Rect",buf,strlen("Begin %I Rect")) == 0)
      convert_rectangle(ifp);
    if (strncmp("Begin %I MLine",buf,strlen("Begin %I MLine")) == 0)
      convert_polygon(ifp);
    if (strncmp("Begin %I Poly",buf,strlen("Begin %I Poly")) == 0)
      convert_polygon(ifp);
  }

  fclose(ifp);

  idraw_normalize_results();

  //for (i=0; i < NUMREGIONS; i++) {
  //  printf("\n%s REGION (%s)\n",rnames[i],rcolors[i]);
  //  cout << rv[i] << "\n\n";
  //}

  // Write the results to standard out
  for (i=0; i < NUMREGIONS; i++) {
    //cout << rnames[i] << "\n";
    forall(poly,rv[i]) {
      forall_vertices(pt,poly) {
	std::cout << "(" << pt.xcoord() << "," << pt.ycoord() << ") "; 
      }
      std::cout << "\n ";
    }
  }
  std::cout << "\n\n";
}


