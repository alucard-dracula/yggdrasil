//bresenham.cpp

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER
//#pragma comment(lib, "vld.lib")

#include <iostream>
#include <complex>

#ifdef __GNUC__
#   include <stdlib.h>
#endif //__GNUC__


void bresenham(int x0, int y0, int x1, int y1)
{
#ifdef _MSC_VER
	int dx = std::abs(x1-x0);
	int dy = std::abs(y1-y0);
#else
    int dx = abs(x1-x0);
    int dy = abs(y1-y0);
#endif //__MSC_VER

	int sx = x0 < x1? 1 : -1;
	int sy = y0 < y1? 1 : -1;

	int err = dx - dy;

	for(;;)
	{
		std::cout << x0 << "," << y0 << std::endl;
		if(x0 == x1 && y0 == y1)
		{
			break;
		}

		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}

		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

int main()
{
	bresenham(0, 0, 3, 2);

	char cc = 0;
	std::cin >> cc;
}
