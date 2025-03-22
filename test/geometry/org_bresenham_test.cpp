// org_bresenham_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cstdio>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void draw_bresenham_line(int x0, int y0, int x1, int y1)
{ 
	int dx = x1 - x0; //x偏移量
	int dy = y1 - y0; //y偏移量
	int ux = dx >0 ?1:-1;//x伸展方向
	int uy = dx >0 ?1:-1;//y伸展方向
	int dx2 = dx <<1;//x偏移量乘2
	int dy2 = dy <<1;//y偏移量乘2
	if(abs(dx)>abs(dy))
	{//以x为增量方向计算
		int e = -dx; //e = -0.5 * 2 * dx,把e 用2 * dx* e替换
		int x = x0; //起点x坐标
		int y = y0;//起点y坐标
		for (x = x0; x < x1;x+=ux)
		{
			printf ("%d,%d\n",x, y);
			e=e + dy2;//来自 2*e*dx= 2*e*dx + 2dy  （原来是 e = e + k）
			if (e > 0)//e是整数且大于0时表示要取右上的点（否则是右下的点） 
			{ 
			 	y += uy; 
				e= e - dx2;//2*e*dx = 2*e*dx - 2*dx  (原来是 e = e -1)
			}
		}
	}
	else
	{//以y为增量方向计算
		int e = -dy; //e = -0.5 * 2 * dy,把e 用2 * dy* e替换
		int x = x0; //起点x坐标
		int y = y0;//起点y坐标
		for (y = y0; y < y1;y += uy)
		{
			//printf ("%d,%d\n",x, y);
			std::cout << x << ", " << y << std::endl; 
			e=e + dx2;//来自 2*e*dy= 2*e*dy + 2dy  （原来是 e = e + k）
			if (e > 0)//e是整数且大于0时表示要取右上的点（否则是右下的点） 
			{ 
			 	x += ux; 
				e= e - dy2;//2*e*dy = 2*e*dy - 2*dy  (原来是 e = e -1)
			}
		}
	}
}

int main(int argc, char* argv[])
{
	draw_bresenham_line(0, 0, 5, 5);

	wait_any_key(argc, argv);
	return 0;
}
