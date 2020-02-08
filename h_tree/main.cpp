#include <cmath>

#include "image.hpp"

void Htree(int cx, int cy, int size, int width, int height, Image& img, int depth, int orientation)
{
	if(orientation == 0)
		img.drawLine(Point(cx - size/2, cy), Point(cx + size/2, cy), Color::Black);
	else
		img.drawLine(Point(cx, cy - size/2), Point(cx, cy + size/2), Color::Black);
	if(depth > 0 && size > 1)
	{
		int hs = size/2;
		int ns = size*std::sqrt(0.5);
		if(orientation == 0)
		{
			Htree(cx - hs, cy, ns, width, height, img, depth-1, 1);
			Htree(cx + hs, cy, ns, width, height, img, depth-1, 1);
		}
		else
		{
			Htree(cx, cy - hs, ns, width, height, img, depth-1, 0);
			Htree(cx, cy + hs, ns, width, height, img, depth-1, 0);
		}
	}
}

int main()
{
	unsigned int height = 1024;
	unsigned int width = height*sqrt(2) + 0.5;
	
	Image result{width, height};
	result.clear(Color::White);

	Htree(width/2, height/2, width/2, width, height, result, 11, 0);
	result.save("output/h_tree.ppm");

	return 0;
}
