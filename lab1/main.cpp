#include <QImage>
#include <QColor>
#include <iostream>
#include <vector>
#include <numeric>
//Clifford Kallem
//Adam jacoby
using namespace std;

int main(int argc, char** argv) {
	cout<< "Enter 3 points (enter a point as x,y:r,g,b)\n";
	int points[3][2];
	float color[3][3];
	char stuff;
	int i = 0;
	for(i = 0; i<3;i++)
	{
		cin >> points[i][0];
		cin >> stuff;
		cin >> points[i][1];
		cin >> stuff;
		cin >> color[i][0];
		cin >> stuff;
		cin >> color[i][1];
		cin >> stuff;
		cin >> color[i][2];
	}
	cout<<"\n";
    for(i = 0; i < 3; i++)
    {
        cout << points[i][0] <<", " << points[i][1] << " : ";
        cout << color[i][0] << ", " <<color[i][1] << ", " <<color[i][2] << "\n";
    }

    QImage image(640, 480, QImage::Format_RGB32); 
    int boxPoints[4][2] = {{640, 480},
						   {640,0},
						   {0,480},
						   {0,0,}};
    for(i = 0; i< 3; i++)
    {
		if(points[i][0]<boxPoints[0][0])
		{
			boxPoints[0][0] = points[i][0];
			boxPoints[1][0] = points[i][0];
		}
		if(points[i][1]<boxPoints[0][1])
		{
			boxPoints[0][1] = points[i][1];
			boxPoints[2][1] = points[i][1];
		}
		if(points[i][0] > boxPoints[3][0])
		{
			boxPoints[3][0]=points[i][0];
			boxPoints[2][0]=points[i][0];			
		}
		if(points[i][1] > boxPoints[3][1])
		{
			boxPoints[3][1]=points[i][1];
			boxPoints[1][1]=points[i][1];			
		}

    } 

    /*
      Part 1:

          Calculate the bounding box of the 3 provided points and loop
          over each pixel in that box and set it to white using:

          image.setPixel(x,y, qRgb(255,255,255));

      Part 2:
          Modify your loop from part 1. Using barycentric coordinates,
          determine if the pixel lies within the triangle defined by
          the 3 provided points. If it is color it white, otherwise
          move on to the next pixel.

          For more on barycentric coordinates:
          http://en.wikipedia.org/wiki/Barycentric_coordinate_system

      Part 3:
          For each pixel in the triangle, calculate the color based on
          the calculated barycentric coordinates and the 3 provided
          colors. Your colors should have been entered as floating point
          numbers from 0 to 1. Since we have a 32 bit image, the red,
          green and blue components range from 0 to 255. Be sure to make
          the conversion.
    */

    int j = 0;
	i = 0;

	for(j = boxPoints[0][0]; j<boxPoints[2][0];j++)
	{
		for(i = boxPoints[0][1];i<boxPoints[1][1];i++)
		{

			double y1Top = ((points[1][1]-points[2][1])*(j-points[2][0]))+(points[2][0]-points[1][0])*(i-points[2][1]);
			double y1Bottom = (points[1][1] - points[2][1])*(points[0][0] - points[2][0])+(points[2][0] - points[1][0])*(points[0][1] - points[2][1]);
			double y2Top = (points[2][1]-points[0][1])*(j-points[2][0])+(points[0][0]-points[2][0])*(i-points[2][1]);
			double y2Bottom = (points[1][1] - points[2][1])*(points[0][0] - points[2][0])+(points[2][0] - points[1][0])*(points[0][1] - points[2][1]);
			double v = y1Top/y1Bottom;
			double w = y2Top/y2Bottom;
			double u = 1-v-w;
			if(v>=0&&w>=0&&u>=0)
			{
				double r = 255* (v*color[0][0]+w*color[1][0]+u*color[2][0]);
				double b = 255* (v*color[0][1]+w*color[1][1]+u*color[2][1]);
				double g = 255* (v*color[0][2]+w*color[1][2]+u*color[2][2]);
                image.setPixel(j,i, qRgb(r,b,g));
            }
            else{
               // image.setPixel(j,i, qRgb(255,255,255));
            }


		}
	}	





    if(image.save("triangle.jpg",0,100)) {
        cout << "Output triangle.jpg" << endl;
    } else {
        cout << "Unable to save triangle.jpg" << endl;
    }
}
