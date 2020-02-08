#include "image.hpp"


void Tsquere(int cx, int cy, int size, int width, int height, Image& img, int depth)
{
	int hs = size / 2;
	img.fillRect(Point(cx - hs, cy - hs), size, size, Color::Black);
	if(depth > 0 && hs >= 1)
	{
		Tsquere(cx - hs, cy - hs, hs, width, height, img, depth-1);
		Tsquere(cx + hs, cy - hs, hs, width, height, img, depth-1);
		Tsquere(cx - hs, cy + hs, hs, width, height, img, depth-1);
		Tsquere(cx + hs, cy + hs, hs, width, height, img, depth-1);
	}
}

int main()
{
	unsigned int width = 1024, height = 1024;
	Image result{width, height};
	result.clear(Color::White);

	Tsquere(512, 512, 512, width, height, result, 7);
	result.save("output/t_squere.ppm");

	return 0;
}
