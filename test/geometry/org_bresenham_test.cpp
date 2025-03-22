// org_bresenham_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cstdio>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void draw_bresenham_line(int x0, int y0, int x1, int y1)
{ 
	int dx = x1 - x0; //xƫ����
	int dy = y1 - y0; //yƫ����
	int ux = dx >0 ?1:-1;//x��չ����
	int uy = dx >0 ?1:-1;//y��չ����
	int dx2 = dx <<1;//xƫ������2
	int dy2 = dy <<1;//yƫ������2
	if(abs(dx)>abs(dy))
	{//��xΪ�����������
		int e = -dx; //e = -0.5 * 2 * dx,��e ��2 * dx* e�滻
		int x = x0; //���x����
		int y = y0;//���y����
		for (x = x0; x < x1;x+=ux)
		{
			printf ("%d,%d\n",x, y);
			e=e + dy2;//���� 2*e*dx= 2*e*dx + 2dy  ��ԭ���� e = e + k��
			if (e > 0)//e�������Ҵ���0ʱ��ʾҪȡ���ϵĵ㣨���������µĵ㣩 
			{ 
			 	y += uy; 
				e= e - dx2;//2*e*dx = 2*e*dx - 2*dx  (ԭ���� e = e -1)
			}
		}
	}
	else
	{//��yΪ�����������
		int e = -dy; //e = -0.5 * 2 * dy,��e ��2 * dy* e�滻
		int x = x0; //���x����
		int y = y0;//���y����
		for (y = y0; y < y1;y += uy)
		{
			//printf ("%d,%d\n",x, y);
			std::cout << x << ", " << y << std::endl; 
			e=e + dx2;//���� 2*e*dy= 2*e*dy + 2dy  ��ԭ���� e = e + k��
			if (e > 0)//e�������Ҵ���0ʱ��ʾҪȡ���ϵĵ㣨���������µĵ㣩 
			{ 
			 	x += ux; 
				e= e - dy2;//2*e*dy = 2*e*dy - 2*dy  (ԭ���� e = e -1)
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
