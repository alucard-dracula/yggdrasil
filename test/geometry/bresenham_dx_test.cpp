// bresenham_dx_test.cpp

#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#if defined(YGGR_AT_WINDOWS)

#include <yggr/base/static_constant.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/geometry/bresenham_disperse.hpp>
#include <yggr/geometry/geo_rectangle.hpp>

#include <boost/mpl/integral_c.hpp>

#include <windows.h>
#include <d3dx9.h>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "Winmm.lib")


typedef yggr::math::vector3d<yggr::u64> pos_type;
typedef yggr::geometry::geo_line_segment<pos_type> line_seg_type;
typedef std::vector<pos_type> pos_vt_type;
typedef std::vector<pos_vt_type> pos_vt_vt_type;

typedef yggr::geometry::geo_rectangle<pos_type> aabb_box_type;

inline yggr::u32 f2dw(yggr::f32 n) //flaot to dword
{
	return *reinterpret_cast<yggr::u32*>(&n);
}

struct dx_vertex
{
public:
	typedef boost::mpl::integral_c<yggr::u32, D3DFVF_XYZ | D3DFVF_DIFFUSE> fvf_type;
	YGGR_STATIC_CONSTANT(yggr::u32, FVF = fvf_type::value);

private:
	typedef dx_vertex this_type;


public:
	dx_vertex(void)
		: x(), y(), z(), color()
	{

	};

	dx_vertex(yggr::f32 nx, yggr::f32 ny, yggr::f32 nz, yggr::u32 nc)
		: x(nx), y(ny), z(nz), color(nc)
	{
	};

	template<typename T,
				template<typename _T, std::size_t> class Array>
	dx_vertex(const yggr::math::vector3d<T, Array>& pos, yggr::u32 nc)
		: x(static_cast<yggr::f32>(pos.x) / static_cast<yggr::f32>(pos.w)),
			y(static_cast<yggr::f32>(pos.y) / static_cast<yggr::f32>(pos.w)),
			z(static_cast<yggr::f32>(pos.z) / static_cast<yggr::f32>(pos.w)),
			color(nc)
	{
	}

	dx_vertex(BOOST_RV_REF(this_type) right)
		: x(boost::move(right.x)),
			y(boost::move(right.y)),
			z(boost::move(right.z)),
			color(boost::move(right.color))
	{
	}

	dx_vertex(const this_type& right)
		: x(right.x), y(right.y), z(right.z), color(right.color)
	{
	};

	~dx_vertex(void)
	{
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		x = boost::move(right_ref.x);
		y = boost::move(right_ref.y);
		z = boost::move(right_ref.z);
		color = boost::move(right_ref.color);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		x = right.x;
		y = right.y;
		z = right.z;
		color = right.color;
		return *this;
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		yggr::swap(x, right.x);
		yggr::swap(y, right.y);
		yggr::swap(z, right.z);
		yggr::swap(color, right.color);
	}

	this_type& set_data(yggr::f32 nx, yggr::f32 ny, yggr::f32 nz, yggr::u32 nc)
	{
		x = nx;
		y = ny;
		z = nz;
		color = nc;
		return *this;
	}

	template<typename T,
				template<typename _T, std::size_t> class Array>
	this_type& set_data(const yggr::math::vector3d<T, Array>& pos, yggr::u32 nc)
	{
		x = (static_cast<yggr::f32>(pos.x) / static_cast<yggr::f32>(pos.w));
		y = (static_cast<yggr::f32>(pos.y) / static_cast<yggr::f32>(pos.w));
		z = (static_cast<yggr::f32>(pos.z) / static_cast<yggr::f32>(pos.w));
		color = nc;
		return *this;
	}

public:

	//D3DXVECTOR3
	yggr::f32 x, y, z; // XYZ
	D3DCOLOR color; //Diffuse ARGB
};

const int g_box_buf_size = 3;

pos_vt_vt_type& make_aabb_box_pos_list(pos_vt_vt_type& vtvt, const aabb_box_type& aabb_box)
{
	vtvt.clear();
	vtvt.resize(g_box_buf_size, pos_vt_type());

	//vtvt[0].clear();
	//vtvt[1].clear();
	//vtvt[2].clear();
	//vtvt[3].clear();
	//vtvt[4].clear();
	//vtvt[5].clear();

	vtvt[0].resize(5, pos_type());
	vtvt[1].resize(5, pos_type());
	vtvt[2].resize(8, pos_type());
	//vtvt[2].resize(2, pos_type());
	//vtvt[3].resize(2, pos_type());
	//vtvt[4].resize(2, pos_type());
	//vtvt[5].resize(2, pos_type());

	// front
	vtvt[0][0] = aabb_box.min_pos;
	vtvt[0][1] = pos_type(aabb_box.max_pos.x, aabb_box.min_pos.y, aabb_box.min_pos.z, 1);
	vtvt[0][2] = pos_type(aabb_box.max_pos.x, aabb_box.max_pos.y, aabb_box.min_pos.z, 1);
	vtvt[0][3] = pos_type(aabb_box.min_pos.x, aabb_box.max_pos.y, aabb_box.min_pos.z, 1);
	vtvt[0][4] = vtvt[0][0];

	// back
	vtvt[1][0] = pos_type(aabb_box.min_pos.x, aabb_box.min_pos.y, aabb_box.max_pos.z, 1);
	vtvt[1][1] = pos_type(aabb_box.max_pos.x, aabb_box.min_pos.y, aabb_box.max_pos.z, 1);
	vtvt[1][2] = aabb_box.max_pos;
	vtvt[1][3] = pos_type(aabb_box.min_pos.x, aabb_box.max_pos.y, aabb_box.max_pos.z, 1);
	vtvt[1][4] = vtvt[1][0];

	//left
	vtvt[2][0] = aabb_box.min_pos;
	vtvt[2][1] = pos_type(aabb_box.min_pos.x, aabb_box.min_pos.y, aabb_box.max_pos.z, 1);

	//vtvt[3][0] = pos_type(aabb_box.min_pos.x, aabb_box.max_pos.y, aabb_box.min_pos.z, 1);
	//vtvt[3][1] = pos_type(aabb_box.min_pos.x, aabb_box.max_pos.y, aabb_box.max_pos.z, 1);

	vtvt[2][2] = pos_type(aabb_box.min_pos.x, aabb_box.max_pos.y, aabb_box.min_pos.z, 1);
	vtvt[2][3] = pos_type(aabb_box.min_pos.x, aabb_box.max_pos.y, aabb_box.max_pos.z, 1);

	// right

	//vtvt[4][0] = pos_type(aabb_box.max_pos.x, aabb_box.min_pos.y, aabb_box.min_pos.z, 1);
	//vtvt[4][1] = pos_type(aabb_box.max_pos.x, aabb_box.min_pos.y, aabb_box.max_pos.z, 1);

	vtvt[2][4] = pos_type(aabb_box.max_pos.x, aabb_box.min_pos.y, aabb_box.min_pos.z, 1);
	vtvt[2][5] = pos_type(aabb_box.max_pos.x, aabb_box.min_pos.y, aabb_box.max_pos.z, 1);

	//vtvt[5][0] = pos_type(aabb_box.max_pos.x, aabb_box.max_pos.y, aabb_box.min_pos.z, 1);
	//vtvt[5][1] = aabb_box.max_pos;

	vtvt[2][6] = pos_type(aabb_box.max_pos.x, aabb_box.max_pos.y, aabb_box.min_pos.z, 1);
	vtvt[2][7] = aabb_box.max_pos;

	return vtvt;
}

int main(int argc, char* argv[])
{
	HINSTANCE hins = GetModuleHandle(0);

	WNDCLASS wc;
	memset(&wc, 0, sizeof(WNDCLASS));
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(hins, 0);
	wc.lpfnWndProc = DefWindowProc;
	wc.lpszClassName = "DrawTriangle";
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if(!RegisterClass(&wc))
	{
		MessageBox(0, "Reg Error", "Error", MB_OK);
		return 1;
	}

	yggr::s32 width = 1280;
	yggr::s32 height = 720;

	HWND hwnd = CreateWindow(wc.lpszClassName, wc.lpszClassName, WS_EX_TOPMOST,
								0, 0, width, height, 0, 0, hins, 0);

	if(!hwnd)
	{
		MessageBox(0, "Create Window Err", "Err", MB_OK);
		return 1;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	IDirect3D9 *pd3d = ::Direct3DCreate9(D3D_SDK_VERSION);
	D3DDISPLAYMODE ddp;

	if(pd3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &ddp) < 0)
	{
		MessageBox(0, "get adapter Error", "Error", MB_OK);
		return 1;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.Windowed = true;
	d3dpp.BackBufferFormat = ddp.Format;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	//d3dpp.Windowed = true;
	//d3dpp.BackBufferWidth = width;
	//d3dpp.BackBufferHeight = height;
	//d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//d3dpp.BackBufferFormat = ddp.Format;
	////d3dpp.EnableAutoDepthStencil = true;


	IDirect3DDevice9 *pdevice = 0;

	if(pd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
							D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp,
							&pdevice) < 0)
	{
		MessageBox(0, "Create Device Error", "Error", MB_OK);
		return 1;
	}

	line_seg_type line_seg(pos_type(-50, -50, -50), pos_type(50, 50, 50));
	pos_vt_type pos_vt;
	yggr::geometry::bresenham_disperse(pos_vt, line_seg);

	{
		for(int i = 0, isize = pos_vt.size(); i != isize; ++i)
		{
			std::cout << pos_vt[i] << std::endl;
		}
	}

	aabb_box_type aabb_box(line_seg.src_pos, line_seg.dst_pos);
	pos_vt_vt_type pos_vt_vt;
	make_aabb_box_pos_list(pos_vt_vt, aabb_box);

	yggr_test_assert(pos_vt.size());

	IDirect3DVertexBuffer9* pvtxbuf = 0;
	IDirect3DVertexBuffer9* pvtxbuf_box[g_box_buf_size] = {0};

	pdevice->CreateVertexBuffer(pos_vt.size() * sizeof(dx_vertex),
									D3DUSAGE_WRITEONLY,
									dx_vertex::FVF,
									D3DPOOL_MANAGED,
									&pvtxbuf,
									0 //保留，必须填0
									);

	dx_vertex *pvtx = 0;
	yggr::u32 clr_arr[] = {0xffff0000, 0xff0000ff, 0xff00ff00};
	pvtxbuf->Lock(0, 0, (void**)&pvtx, 0);
	{
		for(std::size_t i = 0, isize = pos_vt.size(); i != isize; ++i)
		{
			pvtx[i].set_data(pos_vt[i], clr_arr[i % 3]);
		}
	}
	pvtxbuf->Unlock();

	{
		yggr::u32 clr = 0;
		for(int i = 0, isize = g_box_buf_size; i != isize; ++i)
		{
			pdevice->CreateVertexBuffer(pos_vt_vt[i].size() * sizeof(dx_vertex),
										D3DUSAGE_WRITEONLY,
										dx_vertex::FVF,
										D3DPOOL_MANAGED,
										&pvtxbuf_box[i],
										0 //保留，必须填0
										);
			if(!pvtxbuf_box[i])
			{
				{
					{
						for(int i = 0, isize = g_box_buf_size; i != isize; ++i)
						{
							if(pvtxbuf_box[i]) pvtxbuf_box[i]->Release();
						}
					}
					if(pvtxbuf) pvtxbuf->Release();
					if(pdevice) pdevice->Release();
					if(pd3d) pd3d->Release();
				}

				return 0;
			}

			switch(i)
			{
			case 0:
				clr = ~clr_arr[0] | 0xff000000;
				break;
			case 1:
				clr = ~clr_arr[1] | 0xff000000;
				break;
			default:
				clr = ~clr_arr[2] | 0xff000000;
				break;
			}

			for(int j = 0, jsize = pos_vt_vt[i].size(); j != jsize; ++j)
			{
				dx_vertex *pvtx_box = 0;
				pvtxbuf_box[i]->Lock(0, 0, (void**)&pvtx_box, 0);
				{
					pvtx_box[j].set_data(pos_vt_vt[i][j], clr);
				}
				pvtxbuf_box[i]->Unlock();
			}
		}
	}

	D3DXMATRIX proj;
	//生成投影矩阵
	D3DXMatrixPerspectiveFovLH(&proj,
								D3DX_PI  / 4, //张角（弧度）
								static_cast<yggr::f32>(width) / static_cast<yggr::f32>(height), //纵横比
								1.0f, //近平面
								1000.0f //远平面
								);

	//设置投影变换
	pdevice->SetTransform(D3DTS_PROJECTION, &proj);

	D3DXMATRIX matView; //视图矩阵
	D3DXVECTOR3 pos(0.0f, 0.0f, -200.0f); //摄象机所在位置
	D3DXVECTOR3 pex(0.0f, 0.0f, -100.0f);//摄象机看的位置
	D3DXVECTOR3 pasp(0.0f, 1.0f, 0.0f); //摄象机摆放的方向

	D3DXMatrixLookAtLH(&matView, &pos, &pex, &pasp);//生成视图矩阵

	pdevice->SetTransform(D3DTS_VIEW, &matView); //设置视图变换

	// Setup E

	D3DVIEWPORT9 dview; //视口
	dview.X = 0; //开始X点
	dview.Y = 0; //开始Y 点
	dview.Width = width; //宽度
	dview.Height = height; //高度
	dview.MinZ = 0.0f; //最小Z（百分比）
	dview.MaxZ = 1.0f; //最大Z
	pdevice->SetViewport(&dview); //设置视口变换

	//pdevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	//pdevice->SetRenderState(D3DRS_POINTSIZE, f2dw(2.0f));
	pdevice->SetRenderState(D3DRS_LIGHTING, false);
	pdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	pdevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);


	pdevice->SetFVF(dx_vertex::FVF); //告诉设备顶点格式

	int ax = 0;
	int ay = 0;
	int az = 0;
	int len = 0;

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

		if(GetKeyState('W') < 0)
		{
			ax += 5;
		}

		if(GetKeyState('S') < 0)
		{
			ax -= 5;
		}

		if(GetKeyState('A') < 0)
		{
			ay += 5;
		}

		if(GetKeyState('D') < 0)
		{
			ay -= 5;
		}

		if(GetKeyState('Q') < 0)
		{
			az += 5;
		}

		if(GetKeyState('E') < 0)
		{
			az -= 5;
		}

		if(ax > 360 || ax < -360)
		{
			ax = 0;
		}

		if(ay > 360 || ay < -360)
		{
			ay = 0;
		}

		if(az > 360 || az < -360)
		{
			az = 0;
		}

		if(GetKeyState(VK_UP) < 0)
		{
			len += 5;
		}

		if(GetKeyState(VK_DOWN) < 0)
		{
			len -= 5;
		}

		D3DXMATRIX matworldx, matworldy, matworldz, matworld, matmove;
		D3DXMatrixIdentity(&matworld);
		D3DXMatrixTranslation(&matmove, 0, 0, len);
		D3DXMatrixRotationX(&matworldx, (float)ax / 180.0f * D3DX_PI);
		D3DXMatrixRotationY(&matworldy, (float)ay / 180.0f * D3DX_PI);
		D3DXMatrixRotationZ(&matworldz, (float)az / 180.0f * D3DX_PI);

		D3DXMatrixMultiply(&matworldx, &matworldx, &matmove);
		D3DXMatrixMultiply(&matworld, &matworldy, &matworldx);
		D3DXMatrixMultiply(&matworld, &matworldz, &matworld);

		pdevice->SetTransform(D3DTS_WORLD, &matworld);


		//pdevice->Clear(0, 0, D3DCLEAR_TARGET /*| D3DCLEAR_ZBUFFER*/,
		//					0xffffffff, /*0x00000000,*/ 1.0f, 0);
		pdevice->Clear(D3DADAPTER_DEFAULT, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		pdevice->BeginScene();
		{
			{
				//D3DXMATRIX matWorld; //世界矩阵
				//D3DXMatrixIdentity(&matWorld); //初始化世界矩阵为单位矩阵
				//pdevice->SetTransform(D3DTS_WORLD, &matWorld); //设置世界变换

				for(int i = 0, isize = g_box_buf_size; i != isize; ++i)
				{
					yggr_test_assert(pvtxbuf_box[i]);
					pdevice->SetStreamSource(0, //数据流编号
								pvtxbuf_box[i], //顶点缓冲
								0, //开始点
								sizeof(dx_vertex) //每个顶点的大小
								);
					switch(pos_vt_vt[i].size())
					{
					case 8:
						pdevice->DrawPrimitive(D3DPT_LINELIST, //图元类型
												0,//开始顶点的索引
												4 //画图元的总数
												);
						break;
					case 5:
						pdevice->DrawPrimitive(D3DPT_LINESTRIP, //图元类型
												0,//开始顶点的索引
												4 //画图元的总数
												);
						break;
					default:
						yggr_test_assert(false);
					}

				}

				pdevice->SetStreamSource(0, //数据流编号
								pvtxbuf, //顶点缓冲
								0, //开始点
								sizeof(dx_vertex) //每个顶点的大小
								);

				pdevice->DrawPrimitive(D3DPT_POINTLIST, //图元类型
										0,//开始顶点的索引
										pos_vt.size() //画图元的总数
										);
			}

			//{
			//	D3DXMATRIX matWorld;
			//	D3DXMatrixIdentity(&matWorld);
			//	D3DXMatrixTranslation(&matWorld, 50.0f, 0.0f, 0.0f);
			//	pdevice->SetTransform(D3DTS_WORLD, &matWorld);
			//											//多个顶点的颜色插值作为面颜色
			//	pdevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
			//	pdevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
			//}
		}
		pdevice->EndScene();
		pdevice->Present(0, 0, 0, 0);
	}

//exe_exit:
	{
		{
			for(int i = 0, isize = g_box_buf_size; i != isize; ++i)
			{
				if(pvtxbuf_box[i]) pvtxbuf_box[i]->Release();
			}
		}
		if(pvtxbuf) pvtxbuf->Release();
		if(pdevice) pdevice->Release();
		if(pd3d) pd3d->Release();
	}

	return 0;
}

#else

int main(int argc, char* argv[])
{

    std::cout << "driectx only support windows" << std::endl;

    wait_any_key(argc, argv);
    return 0;
}
#endif // #endif // YGGR_AT_WINDOWS

