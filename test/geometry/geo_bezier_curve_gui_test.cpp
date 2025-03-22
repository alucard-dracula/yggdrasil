//geo_bezier_curve_gui_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#if defined(YGGR_AT_WINDOWS)

#include <yggr/math/vector2d.hpp>
#include <yggr/geometry/geo_bezier_curve.hpp>
#include <windows.h>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#pragma comment(lib, "Winmm.lib")

HPEN hpen_curve;
HPEN hpen_tan;

typedef yggr::math::vector2d<yggr::f32> vec2_type;
typedef yggr::geometry::geo_bezier_curve<vec2_type, 3> geo_bezier_type;

geo_bezier_type g_geo_bezier;
geo_bezier_type::point_array_type g_arr = {vec2_type(0, 240), vec2_type(210, 5), vec2_type(420, 475), vec2_type(640, 240)};

LRESULT CALLBACK MyWndProc(     
						   HWND hwnd,
							UINT uMsg,
							WPARAM wParam,
							LPARAM lParam
)
{
	switch(uMsg)
	{
	case WM_CREATE:
		{
			hpen_curve = CreatePen(PS_SOLID, 5, 0x000000ff);
			hpen_tan = CreatePen(PS_SOLID, 1, 0x00ff0088);
#if 0
			g_geo_bezier = geo_bezier_type(g_arr);
#else
			g_geo_bezier = geo_bezier_type(g_arr[0], g_arr[1], g_arr[2], g_arr[3]);
#endif // 0, 1

		}
		break;
	case WM_PAINT:
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			{
				Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom); 

				MoveToEx(hdc, g_arr[0].x, g_arr[0].y, 0); 
				LineTo(hdc, g_arr[g_arr.size() - 1].x, g_arr[g_arr.size() - 1].y);

				{
					for(int i = 0, isize = g_arr.size() - 1; i != isize; ++i)
					{
						MoveToEx(hdc, g_arr[i].x, g_arr[i].y, 0); 
						LineTo(hdc, g_arr[i + 1].x, g_arr[i + 1].y);
					}
				}
				
				vec2_type point_src = g_geo_bezier[0];
				vec2_type point_dst;
				vec2_type tan_src = g_geo_bezier(0);
				vec2_type tan_dst;
				vec2_type tan;
				int count = 16;
				HPEN oldpen;

				{
					for(int i = 1, isize = count; i != isize; ++i)
					{
						point_dst = g_geo_bezier[std::make_pair(i, isize)];
						tan_dst = g_geo_bezier(std::make_pair(i, isize));

						oldpen = static_cast<HPEN>(SelectObject(hdc, hpen_curve));

						MoveToEx(hdc, point_src.x, point_src.y, 0); 
						LineTo(hdc, point_dst.x, point_dst.y);

						SelectObject(hdc, hpen_tan);
				
						tan = point_src + 100 * tan_src.normalize();

						MoveToEx(hdc, point_src.x, point_src.y, 0); 
						LineTo(hdc, tan.x, tan.y);

						point_src = point_dst;
						tan_src = tan_dst;

						SelectObject(hdc, oldpen);
					}
				}

				point_dst = yggr::geometry::geo_bezier_curve_value(g_geo_bezier, 1);
				tan_dst = yggr::geometry::geo_bezier_curve_derivation(g_geo_bezier, 1);

				oldpen = static_cast<HPEN>(SelectObject(hdc, hpen_curve));

				MoveToEx(hdc, point_src.x, point_src.y, 0); 
				LineTo(hdc, point_dst.x, point_dst.y);

				SelectObject(hdc, hpen_tan);

				tan = point_src + 100 * tan_src.normalize();

				MoveToEx(hdc, point_src.x, point_src.y, 0); 
				LineTo(hdc, tan.x, tan.y);

				tan = point_dst + 100 * tan_dst.normalize();

				MoveToEx(hdc, point_dst.x, point_dst.y, 0); 
				LineTo(hdc, tan.x, tan.y);

				SelectObject(hdc, oldpen);

			}
			EndPaint(hwnd, &ps);
		}
		break;
	case WM_DESTROY:
		DeleteObject(hpen_tan);
		DeleteObject(hpen_curve);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

int main(int argc, char* argv[])
{
	HINSTANCE hInstance = GetModuleHandle(0);
	WNDCLASS wc;
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(0, IDC_CROSS);
	wc.hIcon = LoadIcon(0, IDI_HAND);
	wc.hInstance = hInstance; 
	wc.lpszClassName = "geo_bezier_curve_gui_test"; 
	wc.lpszMenuName = 0; 
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MyWndProc; 

	RegisterClass(&wc); 

	HWND hwnd = CreateWindow(wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW,
									0, 0, 1024, 768, GetDesktopWindow(), 0, hInstance, 0);

	ShowWindow(hwnd, /*nCmdShow*/SW_SHOWNORMAL);
	UpdateWindow(hwnd); 

	MSG msg; 
	yggr::u32 start_tm = ::timeGetTime();

	for(;;) 
	{ 
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(::GetKeyState(VK_ESCAPE) < 0)
		{
			break;
		}

		if((argc > 1) && (yggr::string("--batch-test") == argv[argc - 1]))
		{
			if(5000 <= (::timeGetTime() - start_tm))
			{
				break;
			}
		}
	}

	return msg.wParam;
}

#else

int main(int argc, char* argv[])
{
     std::cout << "need add this example linux version" << std::endl;

    wait_any_key(argc, argv);
    return 0;
}

#endif // YGGR_AT_WINDOWS
