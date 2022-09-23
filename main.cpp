#include<iostream>
#include"BMP.h"
#include<cmath>

//
// classes
//
struct Coordinate {
  float x, y;
  Coordinate(float x0 = 0, float y0 = 0) {
    x = x0;
    y = y0;
  }
};

struct ellipse_info { // ellipse defined as x^2*b^2 + y^2*a^2 = a^2*b^2
  int a, b;
  Coordinate center;

  ellipse_info(int a, int b, Coordinate center) {
    this->a = a;
    this->b = b;
    this->center = center;
  }

  long a2() {
    return pow(a, 2);
  }

  long b2() {
    return pow(b,2);
  }

  long a2b2() {
    return pow(a,2) * pow(b,2);
  }
};

//
// functions
//

void light_pixel_within_boundaries(BMP* shape, int x, int y, int B, int G, int R, int A, const int START_X, const int END_X) {
  Coordinate pixel_to_be_lit((shape->bmp_info_header.width) / 2 + x, (shape->bmp_info_header.height) / 2 + y);
  if ( pixel_to_be_lit.x > START_X && pixel_to_be_lit.x < END_X) { 
    // within desired x-boundaries
    if (pixel_to_be_lit.y < shape->bmp_info_header.height && pixel_to_be_lit.y > 0) {
      // if coordinate is within image
      shape->set_pixel(pixel_to_be_lit.x, pixel_to_be_lit.y, B, G, R, A);
    }
  }
}

void light_ellipse_quadrants(BMP* ellipse, Coordinate center, int x, int y,
                             int B, int G, int R, int A,
                             const int START_X, const int END_X)
{
  light_pixel_within_boundaries(ellipse, x + center.x, y + center.y,
                                255, 255, 255, 0, START_X, END_X);
  light_pixel_within_boundaries(ellipse, x + center.x,-y + center.y,
                                255, 255, 255, 0, START_X, END_X);
  light_pixel_within_boundaries(ellipse,-x + center.x, y + center.y,
                                255, 255, 255, 0, START_X, END_X);
  light_pixel_within_boundaries(ellipse,-x + center.x,-y + center.y,
                                255, 255, 255, 0, START_X, END_X);
}


void draw_ellipse(BMP* ellipse, ellipse_info my_ellipse, const int START_X, const int END_X) 
{
  Coordinate pixel_current(0, my_ellipse.b);
  Coordinate pixel_east(1, my_ellipse.b);
  Coordinate pixel_south_east(1, my_ellipse.b - 1);
  Coordinate pixel_south;
  
  const long double DISTANCE_START = my_ellipse.b2() + my_ellipse.a2()*pow(my_ellipse.b - 0.5f, 2) - my_ellipse.a2b2();
  
  long double distanceR1 = DISTANCE_START;
  long double distanceR2;
  
  light_ellipse_quadrants(ellipse, my_ellipse.center, pixel_current.x, pixel_current.y, 255, 255, 255, 0, START_X, END_X);
  bool region1 = true;
  bool region2 = false;
  
  for(int i = 1; i < END_X; i++) {
    if(region1) {
      if (distanceR1 < 0) {
        // pixel_east is lit
        distanceR1 = distanceR1 + my_ellipse.b2()*(2*pixel_current.x + 3);
        light_ellipse_quadrants(ellipse, my_ellipse.center, 
                                pixel_east.x, pixel_east.y, 
                                255, 255, 255, 0, START_X, END_X);

        pixel_east.x++;
        pixel_south_east.x++;
        pixel_current = pixel_east;
      }
      else {
        // pixel_south_east is lit
        distanceR1 = distanceR1 + my_ellipse.b2()*(2*pixel_current.x + 3) + my_ellipse.a2()*(-2 * pixel_current.y + 2);
        light_ellipse_quadrants(ellipse, my_ellipse.center, 
                                pixel_south_east.x, pixel_south_east.y, 
                                255, 255, 255, 0, START_X, END_X);

        pixel_east.x++;
        pixel_east.y--;

        pixel_south_east.x++;
        pixel_south_east.y--;

        pixel_current = pixel_east;
      }
    }
    
    if(region2) {
      if (distanceR2 < 0) {
        // pixel_south_east is lit
        distanceR2 = distanceR2 + my_ellipse.b2()*(2*pixel_current.x + 2) + my_ellipse.a2()*(-2 * pixel_current.y + 3);
        light_ellipse_quadrants(ellipse, my_ellipse.center, 
                                pixel_south_east.x, pixel_south_east.y, 
                                255, 255, 255, 0, START_X, END_X);

        pixel_south.x++;
        pixel_south.y--;

        pixel_south_east.x++;
        pixel_south_east.y--;

        pixel_current = pixel_south_east;
      }
      else {
        //pixel_south is lit
        distanceR2 = distanceR2 + my_ellipse.a2()*(-2*pixel_current.y + 3);
        light_ellipse_quadrants(ellipse, my_ellipse.center, 
                                pixel_south.x, pixel_south.y, 
                                255, 255, 255, 0, START_X, END_X);

        pixel_south.y--;
        pixel_south_east.y--;
        pixel_current = pixel_south;
      }
    }

    if (region1 && my_ellipse.b2()*(pixel_current.x + 1) >= my_ellipse.a2()*(pixel_current.y - 0.5f)) {
      //entering region 2
      region1 = false;
      region2 = true;
      
      distanceR2 = 
        (my_ellipse.b2()*pow(pixel_current.x + 0.5, 2) + my_ellipse.a2()*pow(pixel_current.y - 1, 2) - my_ellipse.a2b2());

      pixel_south.x = pixel_current.x;
      pixel_south.y = pixel_current.y - 1;

      pixel_south_east.x = pixel_current.x+1;
      pixel_south_east.y = pixel_current.y -1;
    }

    if (pixel_current.x >= my_ellipse.a) {
      region2 = false;
      if (pixel_current.y != 0) {
        for(int i=0; i<pixel_current.y; i++) {
          light_ellipse_quadrants(ellipse, my_ellipse.center, my_ellipse.a, i, 255, 255, 255, 0, START_X, END_X);
        }
      }
      return;
    }
    
  }
}

//
// main
//
int main() 
{
  // Draw an ellipse
  BMP* ellipse = new BMP(1600,1600,false);
  ellipse->fill_region(0, 0, 1600, 1600, 0, 0, 0, 0);

  Coordinate ellipse_center(0,0);
  ellipse_info my_ellipse(768, 384, ellipse_center);
 
  for(int i=0; i<1600; i+=20) {
    for(int j=i; j<i+10; j++) {
      ellipse->set_pixel(800, j, 255,255,255,0);
      ellipse->set_pixel(j, 800, 255,255,255,0);
    }
  }
  draw_ellipse(ellipse, my_ellipse, 800, 1600);
  
  ellipse->write("output.bmp");
}
