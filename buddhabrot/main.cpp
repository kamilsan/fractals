#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <thread>

#include "complex.hpp"
#include "image.hpp"

typedef unsigned short HeatmapType;

std::vector<Complex> getOrbit(const Complex& c, int maxIterations)
{
	std::vector<Complex> result;
	result.reserve(maxIterations);
	Complex z;
	int iter = 0;
	while(iter < maxIterations && z.magnitudeSq() <= 4)
	{
		z = z*z + c;
		++iter;
		result.push_back(z);
	}
	if(iter == maxIterations) return std::vector<Complex>();

	return result;
}

void generateHeatmap(HeatmapType* heatmap, int width, int height, const Complex& min, const Complex& max,
	int samples, int maxIterations, int& maxValue)
{
	std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<float> reDist(min.re, max.re);
	std::uniform_real_distribution<float> imDist(min.im, max.im);
	Complex c;
	for(int i = 0; i < samples; ++i)
	{
		c = Complex(reDist(generator), imDist(generator));
		std::vector<Complex> points = getOrbit(c, maxIterations);
		for(Complex& point : points)
		{
			if(point.re > min.re && point.re < max.re && point.im > min.im && point.im < max.im)
			{
				int x = static_cast<int>((point.im - min.im)*(width/(max.im-min.im)));
				int y = static_cast<int>((point.re - min.re)*(height/(max.re-min.re)));
				int idx = y*width + x;
				++heatmap[idx];
				if(heatmap[idx] > maxValue) maxValue = heatmap[idx];
			}
		}
	}
}

int main()
{
	int width = 1024, height = 1024;

	Complex min(-2.0f, -2.0f);
	Complex max(1.2f, 2.0f);

	float rangeRe = max.re - min.re;
	float rangeIm = rangeRe*((float)width/height);

	min.im = -0.5f*rangeIm;
	max.im = 0.5f*rangeIm;

	int samples = width*height*30;
	int maxIterationsR = 500;
	int maxIterationsG = 500;
	int maxIterationsB = 500;
	int maxValueR = 0;
	int maxValueG = 0;
	int maxValueB = 0;

	HeatmapType* heatmapR = new HeatmapType[width*height];
	HeatmapType* heatmapG = new HeatmapType[width*height];
	HeatmapType* heatmapB = new HeatmapType[width*height];

	auto start = std::chrono::high_resolution_clock::now();

	std::thread threadR = std::thread(generateHeatmap, heatmapR, width, height, min, max, samples, maxIterationsR, std::ref(maxValueR));
	std::thread threadG = std::thread(generateHeatmap, heatmapG, width, height, min, max, samples, maxIterationsG, std::ref(maxValueG));
	std::thread threadB = std::thread(generateHeatmap, heatmapB, width, height, min, max, samples, maxIterationsB, std::ref(maxValueB));
	
	threadR.join();
	threadG.join();
	threadB.join();
	
	auto end = std::chrono::high_resolution_clock::now();

	auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
	std::cout << "Elapsed: " << time_span.count() << "s\n";

	Image result{(unsigned)width, (unsigned)height};

	float fr = 1.0f/maxValueR;
	float fg = 1.0f/maxValueG;
	float fb = 1.0f/maxValueB;

	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width; ++x)
		{
			int idx = y*width + x;
			unsigned char r = (unsigned char)std::max(0.0, std::min(255.0, 255.0*heatmapR[idx]*fr + 0.5));
			unsigned char g = (unsigned char)std::max(0.0, std::min(255.0, 255.0*heatmapG[idx]*fg + 0.5));
			unsigned char b = (unsigned char)std::max(0.0, std::min(255.0, 255.0*heatmapB[idx]*fb + 0.5));
			result.setPixel(3*idx, Color(r, g, b));
		}
	}

	result.save("output/buddhabrot.ppm");

	delete[] heatmapR;
	delete[] heatmapG;
	delete[] heatmapB;

	return 0;
}
