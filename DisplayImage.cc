#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
// #include "Quadtree.h"

using namespace std;
using namespace cv;

/// Global Variables
const int DELAY = 5;

const int window_width = 1024;
const int window_height = 1024;

int x_1 = 0;
int x_2 = window_width;
int y_1 = 0;
int y_2 = window_height;
int x = window_width / 2;
int y = window_height / 2;
int size = 50;
int delta = 0;
int step = 25;
double wordsize = 3;
int lineType = 2;
int dx[4] = {0,1,0,-1};
int dy[4] = {1,0,-1,0};
int number;
string word[100];
Mat textImg;
int testcount;

class Quadtree{
public:
  Quadtree(float _x, float _y, float _width, float _height, int _level, int _maxLevel);
  // ~Quadtree();
  
  float x;
  float y;
  float width;
  float height;
  int level;
  int maxLevel;
  int colored;
  // vector<Object*>       objects;

  Quadtree * parent;
  Quadtree * NW;
  Quadtree * NE;
  Quadtree * SW;
  Quadtree * SE;

  void Add(Quadtree qtree);
  // sf::RectangleShape shape;
  // sf::Text text;
  // bool contains(Quadtree *child, Object *object);
};

Quadtree::Quadtree(float _x, float _y, float _width, float _height, int _level, int _maxLevel){
  x = _x;
  y = _y;
  width = _width;
  height = _height;
  level = _level;
  maxLevel = _maxLevel;
  colored = 1;
  
  if (level == maxLevel) {
    return;
  }
  
  NW = new Quadtree(x, y, width / 2, height / 2, level+1, maxLevel);
  NE = new Quadtree(x + width / 2, y, width / 2, height / 2, level+1, maxLevel);
  SW = new Quadtree(x, y + height / 2, width / 2, height / 2, level+1, maxLevel);
  SE = new Quadtree(x + width / 2, y + height / 2, width / 2, height / 2, level+1, maxLevel);

}

void Quadtree::Add(Quadtree qtree){
  if(qtree.level==qtree.maxLevel){
    if(textImg.at<int>(qtree.x,qtree.y)!=0)
      qtree.colored = 2;
    return;
  }
  Add(qtree->NW);
  // Add(qtree.NE);
}

static Scalar randomColor( RNG& rng ) {
  int icolor = (unsigned) rng;
  return Scalar( icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255 );
}
static bool randomBool (){
  return rand()%2 ? true : false;
}

void rotate(Mat& src, double angle, Mat& dst) {
    int len = max(src.cols, src.rows);
    Point2f pt(len/2., len/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);

    warpAffine(src, dst, r, Size(len, len));
}

int Displaying_Random_Text( Mat image, char* window_name, RNG rng ) {

  bool judge;
  freopen("t.in","r",stdin);
  cin>>number;
  
  Point org;
  org.x = x;
  org.y = y;

  Quadtree qtree(0,0,16,16,1,1);
//???????????????????????????????????????????????????????????????????????????
  qtree.Add(qtree);
//???????????????????????????????????????????????????????????????????????????
  
  for(int i=1;i<=number;++i){
    cin>>word[i];
  }

  for ( int i = 1; i <= number; ++i ) {
    
    if(i%2) delta = delta + step;

    org.x+=delta*dx[i%4];
    org.y+=delta*dy[i%4];
    
    judge = randomBool();
    textImg = Mat::zeros(image.rows,image.cols,image.type());
    putText( textImg, word[i], org, 2,
             wordsize, randomColor(rng), CV_FONT_HERSHEY_SCRIPT_SIMPLEX, lineType);
    
    if(judge) rotate(textImg,90,textImg);

    // if(qtree.Judge(*qtree,textImg))
      image = image + textImg;

    if(randomBool()){
      wordsize-=0.1;
      // lineType--;
    }

    imshow( window_name, image );
    if ( waitKey(DELAY) >= 0 )
    { return -1; }
  }

  return 0;
}

int main() {
  int c;
  srand((unsigned)time(NULL));
  /// Start creating a window

  char window_name[] = "TagCloud";

  /// Also create a random object (RNG)
  RNG rng( 0xFFFFFFFF) ;

  /// Initialize a matrix filled with zeros
  Mat image = Mat::zeros( window_height, window_width, CV_8UC3 );
  /// Show it in a window during DELAY ms
  imshow( window_name, image );
  waitKey( DELAY );

/// Display text in random positions
  c = Displaying_Random_Text( image, window_name, rng );
  // if( c != 0 ) return 0;

  waitKey(0);
  return 0;
}