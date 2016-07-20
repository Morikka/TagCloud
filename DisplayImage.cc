#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
const int DELAY = 5;

const int window_width = 900;
const int window_height = 900;
int x_1 = -window_width / 2;
int x_2 = window_width * 3 / 2;
int y_1 = -window_width / 2;
int y_2 = window_width * 3 / 2;

static Scalar randomColor( RNG& rng ) {
  int icolor = (unsigned) rng;
  return Scalar( icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255 );
}
static bool randomBool (){
  return rand()%2 ? true : false;
}

void rotate(Mat& src, double angle, Mat& dst) {
    int len = std::max(src.cols, src.rows);
    Point2f pt(len/2., len/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);

    warpAffine(src, dst, r, Size(len, len));
}

int Displaying_Random_Text( Mat image, char* window_name, RNG rng ) {
  int lineType = 8,NUMBER = 10;
  bool judge;
  freopen("t.in","r",stdin);
  cin>>NUMBER;
  for ( int i = 1; i <= NUMBER; ++i ) {
    Point org;
    org.x = rng.uniform(x_1, x_2);
    org.y = rng.uniform(y_1, y_2);

    judge = randomBool();

    string word;
    cin>>word;
    Mat textImg = Mat::zeros(image.rows,image.cols,image.type());
    putText( textImg, word, org, rng.uniform(0, 8),
             rng.uniform(0, 100) * 0.05 + 0.1, randomColor(rng), rng.uniform(1, 10), lineType);
    
    if(judge) rotate(textImg,90,textImg);
    image = image + textImg;


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

  waitKey( 0 );
  return 0;
}