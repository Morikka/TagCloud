#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
const int DELAY = 5;

const int window_width = 512;
const int window_height = 512;
const int Try = 68;

int x_1 = 0;
int x_2 = window_width;
int y_1 = 0;
int y_2 = window_height;
int x = window_width / 2;
int y = window_height / 2;
int size = 50;
int dta = 0;
int step = 10;
double wordsize = 1.6;
int lineType = 1;
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};

int number;
string word[100];
Mat textImg;
int testcount;
int maxHeight;

struct Node {
  int x, y;
  int len;
  int height;
  int color;
};

Node tree[10000005];
//  Node treetext[100005];

void Build(int r, int height, int x, int y, int len) {
  tree[r].x = x;
  tree[r].y = y;
  tree[r].len = len;
  tree[r].color = 1;
  tree[r].height = height;
  if (tree[r].height == maxHeight)
    return;
  int delta = len / 2;
  Build(r * 4 + 1, height + 1, x, y, delta);
  Build(r * 4 + 2, height + 1, x + delta, y, delta);
  Build(r * 4 + 3, height + 1, x, y + delta, delta);
  Build(r * 4 + 4, height + 1, x + delta, y + delta, delta);
  return;
}

void PushUp(int r) {
  int root = (r - 1) / 4;
  int temp;
  bool jud = false;
  for (int i = 0; i <= 3; ++i) {
    temp = tree[root * 4 + i + 1].color;
    if (temp == 1 || temp == 3)jud = true;
  }
  if (jud)tree[root].color = 3;
  else tree[root].color = 2;
  if (tree[root].height == 1) return;
  PushUp(root);
}

bool Judge(int r) {
  if (tree[r].color == 1) return true;
  if (tree[r].height == maxHeight) {
    int a1 = textImg.at<Vec3b>(tree[r].x, tree[r].y)[0];
    int a2 = textImg.at<Vec3b>(tree[r].x, tree[r].y)[1];
    int a3 = textImg.at<Vec3b>(tree[r].x, tree[r].y)[2];
    if (tree[r].color == 2 && (a1 != 0 || a2 != 0 || a3 != 0)) {
      // cout<<tree[r].x<<" "<<tree[r].y<<" "<<tree[r].color<<" "<<a1<<" "<<a2<<" "<<a3<<endl;
      return false;
    }
    // if (tree[r].x == 0 || tree[r].x == 511 || tree[r].y == 0 || tree[r].y == 511)return false;
    return true;
  }
  bool temp = true;
  if (Judge(r * 4 + 1) == false)temp = false;
  if (Judge(r * 4 + 2) == false)temp = false;
  if (Judge(r * 4 + 3) == false)temp = false;
  if (Judge(r * 4 + 4) == false)temp = false;

  return temp;
}

void Add(int r) {
  if (tree[r].height == maxHeight) {
    int a1 = textImg.at<Vec3b>(tree[r].x, tree[r].y)[0];
    int a2 = textImg.at<Vec3b>(tree[r].x, tree[r].y)[1];
    int a3 = textImg.at<Vec3b>(tree[r].x, tree[r].y)[2];
    if (a1 != 0 || a2 != 0 || a3 != 0) {
      tree[r].color = 2;
      PushUp(r);
    }
    return;
  }
  Add(r * 4 + 1);
  Add(r * 4 + 2);
  Add(r * 4 + 3);
  Add(r * 4 + 4);
}

static Scalar randomColor( RNG& rng ) {
  int icolor = (unsigned) rng;
  return Scalar( icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255 );
}
static bool randomBool (int X) {
  return rand() % X ? true : false;
}

void rotate(Mat& src, double angle, Mat& dst) {
  int len = max(src.cols, src.rows);
  Point2f pt(len / 2., len / 2.);
  Mat r = getRotationMatrix2D(pt, angle, 1.0);

  warpAffine(src, dst, r, Size(len, len));
}

int Displaying_Random_Text( Mat image, char* window_name, RNG rng ) {

  bool judge;

  maxHeight = 10;

  Point org;
  org.x = x;
  org.y = y;

  Build(0, 1, 0, 0, window_width);

  for (int j = 1; j <= number; ++j) {
    cout << "_____________" << j << "________________" << endl;
    dta = 0;
    org.x = x;
    org.y = y;
    int count = 0;
    bool jump = false;//used to jump the outside cycle
    for ( int i = 1; i <= Try ; ++i ) {

      if (jump) break;
      if (i % 2) {
        // dta = dta + step;
        count++;
      }
      for (int k = 1; k <= count; k++) {
        judge = randomBool(2);
        textImg = Mat::zeros(image.rows, image.cols, image.type());

        putText( textImg, word[j], org, 2,
                 wordsize, randomColor(rng), CV_FONT_HERSHEY_PLAIN, lineType);

        if (judge) rotate(textImg, 90, textImg);

        // cout << j << " " << i << " " << org.x << " " << org.y << " " << Judge(0) << endl;

        if (Judge(0)) {
          image = image + textImg;
          cout << org.x << " " << org.y << endl;
          Add(0);
          jump = true;
          break;
        }
        org.x = org.x + step * dx[i % 4];
        org.y = org.y + step * dy[i % 4];
      }


      imshow( window_name, image );
      if ( waitKey(DELAY) >= 0 ) { return -1; }
    }
    if (randomBool(5)) {
      if (wordsize > 0.5)
        wordsize -= 0.05;
      // if (step > 3)
      //   step--;
      // lineType--;
    }
  }
  return 0;
}

int main() {

  freopen("t.in", "r", stdin);
  freopen("t.out", "w", stdout);

  int c;
  cin >> number;

  for (int i = 1; i <= number; ++i) {
    cin >> word[i];
  }

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