//geo_line.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_GEOMETRY_GEOMETRY_LINE_HPP__
#define __YGGR_GEOMETRY_GEOMETRY_LINE_HPP__

#include <complex>
#include <utility>
#include <vector>

#include <boost/array.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/mpl/equal_to.hpp>

#include <boost/serialization/access.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/math/sign.hpp>

#include <yggr/serialization/array.hpp>

namespace yggr
{

namespace geometry
{

template<typename Val,
			template <typename _Val, std::size_t> class Base>
class geo_plane;

template<typename Position,
			template<typename _Val, std::size_t> class Base>
class geo_line;

}//namesapce geometry

namespace geometry
{

template<typename Position,
			template<typename _TVal, std::size_t> class Base = boost::array>
class geo_line : public Base<Position, 2>
{
public:
	typedef Position pos_type;
	typedef typename pos_type::val_type val_type;

private:
	enum
	{
		E_LENGTH = 2,
		E_compile_u32 = 0xffffffff
	};

	typedef Base<pos_type, E_LENGTH> base_type;
	typedef geo_line this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:

	geo_line(void)
		: src_pos(base_type::operator[](0)), dst_pos(base_type::operator[](1))
	{
	}

	template<typename OPos>
	geo_line(const OPos& src, const OPos& dst)
		: src_pos(base_type::operator[](0)), dst_pos(base_type::operator[](1))
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		src_pos = src;
		dst_pos = dst;
	}

	geo_line(const pos_type& src, const pos_type& dst)
		: src_pos(base_type::operator[](0)), dst_pos(base_type::operator[](1))
	{
		src_pos = src;
		dst_pos = dst;
	}

	template<typename OPos>
	geo_line(const geo_line<OPos, Base>& right)
		: src_pos(base_type::operator[](0)), dst_pos(base_type::operator[](1))
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		(*this) = right;
	}

	geo_line(BOOST_RV_REF(this_type) right)
		: base_type(right), src_pos(base_type::operator[](0)), dst_pos(base_type::operator[](1))
	{
	}

	geo_line(const this_type& right)
		: base_type(right), src_pos(base_type::operator[](0)), dst_pos(base_type::operator[](1))
	{
	}

	~geo_line(void)
	{
	}

	inline const size_type size(void) const
	{
		return E_LENGTH;
	}

	inline static const size_type s_size(void)
	{
		return E_LENGTH;
	}

	void clear(void)
	{
		src_pos = dst_pos = pos_type();
	}

	template<typename OPos>
	this_type& operator=(const geo_line<OPos, Base>& right)
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		src_pos = right.src_pos;
		dst_pos = right.dst_pos;
		return *this;
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		if(this == &right)
		{
			return *this;
		}

		src_pos = right.src_pos;
		dst_pos = right.dst_pos;
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		src_pos = right.src_pos;
		dst_pos = right.dst_pos;
		return *this;
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		src_pos.swap(right.src_pos);
		dst_pos.swap(right.dst_pos);
	}

	template<typename OPos>
	bool operator==(const geo_line<OPos, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		if(!(is_line() && right.is_line()))
		{
			return false;
		}
		return is_in_it(right);
	}

	template<typename OPos>
	bool operator!=(const geo_line<OPos, Base>& right) const
	{
		return !(this->operator==(right));
	}

	template<typename OPos>
	this_type& set_line(const OPos& src, const OPos& dst)
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		src_pos = src;
		dst_pos = dst;
		return *this;
	}

	bool is_line(void) const
	{
		return src_pos != dst_pos;
	}

	pos_type src_to_dst(void) const
	{
		return dst_pos - src_pos;
	}

	pos_type dst_to_src(void) const
	{
		return src_pos - dst_pos;
	}

	template<typename OPos>
	std::pair<pos_type, bool> cross_point(const geo_line<OPos, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		typedef std::pair<pos_type, bool> rst_type;
		typedef typename upper_float<val_type>::value_type float_type;
		typedef math::value_miss_comparer<float_type> float_cmper_type;
		typedef math::value_miss_comparer<val_type> val_cmper_type;

		struct t_caler
		{
		public:
			t_caler(const pos_type& mnp1, const pos_type mnp2, const pos_type& src_vec)
				: t1_mnp1_sum(), t1_mnp2_sum(), t1_src_vec_sum(),
					t2_mnp1_sum(), t2_mnp2_sum(), t2_src_vec_sum(),
					_is_use_enable_t1(false),
					_is_use_enable_t2(false)
			{
				if(!parse_entire_equation(mnp1, mnp2, src_vec))
				{
					if(!parse_eins_equation(mnp1, mnp2, src_vec))
					{
						parse_zwei_equation(mnp1, mnp2, src_vec);
					}
				}
			}

			bool enable(void) const
			{
				return _is_use_enable_t1 && _is_use_enable_t2;
			}

			bool t1_enalbe(void) const
			{
				return _is_use_enable_t1;
			}

			bool t2_enable(void) const
			{
				return _is_use_enable_t2;
			}

			float_type get_t1(const float_type& t2) const
			{
				if(!_is_use_enable_t1)
				{
					return float_type();
				}
				return (t1_mnp2_sum * t2 + t1_src_vec_sum) / t1_mnp1_sum;
			}

			float_type get_t2(const float_type& t1) const
			{
				if(!_is_use_enable_t2)
				{
					return float_type();
				}
				return (t2_mnp1_sum * t1 - t2_src_vec_sum) / t2_mnp2_sum;
			}

		private:
			bool parse_eins_equation(const pos_type& mnp1, const pos_type mnp2, const pos_type& src_vec)
			{
				for(size_type i = 0, isize = pos_type::s_size(); i != isize; ++i)
				{
					if(val_cmper_type(mnp1[i]) != val_type() && val_cmper_type(mnp2[i]) != val_type())
					{
						t1_mnp1_sum = t2_mnp1_sum = static_cast<float_type>(mnp1[i]);
						t1_mnp2_sum = t2_mnp2_sum = static_cast<float_type>(mnp2[i]);
						t1_src_vec_sum = t2_src_vec_sum = static_cast<float_type>(src_vec[i]);
						_is_use_enable_t1 = _is_use_enable_t2 = true;
						break;
					}

					if(!_is_use_enable_t1)
					{
						if(val_cmper_type(mnp1[i]) != val_type())
						{
							t1_mnp1_sum = static_cast<float_type>(mnp1[i]);
							t1_mnp2_sum = static_cast<float_type>(mnp2[i]);
							t1_src_vec_sum = static_cast<float_type>(src_vec[i]);
							_is_use_enable_t1 = true;
						}
						continue;
					}

					if(!_is_use_enable_t2)
					{
						if(val_cmper_type(mnp2[i]) != val_type())
						{
							t2_mnp1_sum = static_cast<float_type>(mnp1[i]);
							t2_mnp2_sum = static_cast<float_type>(mnp2[i]);
							t2_src_vec_sum = static_cast<float_type>(src_vec[i]);
							_is_use_enable_t2 = true;
						}
						continue;
					}
				}

				return enable();
			}

			bool parse_zwei_equation(const pos_type& mnp1, const pos_type mnp2, const pos_type& src_vec)
			{
				float_type tmp_mnp1_sum = float_type();
				float_type tmp_mnp2_sum = float_type();
				float_type tmp_src_vec_sum = float_type();
				for(size_type i = 0, isize = pos_type::s_size() - 1; i != isize; ++i)
				{
					for(size_type j = i + 1, jsize = pos_type::s_size(); j != jsize; ++j)
					{
						tmp_mnp1_sum = static_cast<float_type>(mnp1[i] + mnp1[j]);
						tmp_mnp2_sum = static_cast<float_type>(mnp2[i] + mnp2[j]);
						tmp_src_vec_sum = static_cast<float_type>(src_vec[i] + src_vec[j]);

						if(float_cmper_type(tmp_mnp1_sum) != float_type() && float_cmper_type(tmp_mnp2_sum) != float_type())
						{
							t1_mnp1_sum = t2_mnp1_sum = tmp_mnp1_sum;
							t1_mnp2_sum = t2_mnp2_sum = tmp_mnp2_sum;
							t1_src_vec_sum = t2_src_vec_sum = tmp_src_vec_sum;
							_is_use_enable_t1 = _is_use_enable_t2 = true;
							break;
						}

						if(!_is_use_enable_t1)
						{
							if(float_cmper_type(tmp_mnp1_sum) != float_type())
							{
								t1_mnp1_sum = tmp_mnp1_sum;
								t1_mnp2_sum = tmp_mnp2_sum;
								t1_src_vec_sum = tmp_src_vec_sum;
								_is_use_enable_t1 = true;
							}
							continue;
						}

						if(!_is_use_enable_t2)
						{
							if(float_cmper_type(tmp_mnp2_sum) != float_type())
							{
								t2_mnp1_sum = tmp_mnp1_sum;
								t2_mnp2_sum = tmp_mnp2_sum;
								t2_src_vec_sum = tmp_src_vec_sum;
								_is_use_enable_t2 = true;
							}
							continue;
						}
					}
				}

				return enable();
			}

			bool parse_entire_equation(const pos_type& mnp1, const pos_type mnp2, const pos_type& src_vec)
			{
				float_type tmp_mnp1_sum = static_cast<float_type>(mnp1.sum());
				float_type tmp_mnp2_sum = static_cast<float_type>(mnp2.sum());
				float_type tmp_src_vec_sum = static_cast<float_type>(src_vec.sum());

				if(float_cmper_type(tmp_mnp1_sum) != float_type() && float_cmper_type(tmp_mnp2_sum) != float_type())
				{
					t1_mnp1_sum = t2_mnp1_sum = tmp_mnp1_sum;
					t1_mnp2_sum = t2_mnp2_sum = tmp_mnp2_sum;
					t1_src_vec_sum = t2_src_vec_sum = tmp_src_vec_sum;
					_is_use_enable_t1 = _is_use_enable_t2 = true;
				}
				else
				{
					if(!_is_use_enable_t1 && float_cmper_type(tmp_mnp1_sum) != float_type())
					{
						t1_mnp1_sum = tmp_mnp1_sum;
						t1_mnp2_sum = tmp_mnp2_sum;
						t1_src_vec_sum = tmp_src_vec_sum;
						_is_use_enable_t1 = true;
					}

					if(!_is_use_enable_t2 && float_cmper_type(tmp_mnp2_sum) != float_type())
					{
						t2_mnp1_sum = tmp_mnp1_sum;
						t2_mnp2_sum = tmp_mnp2_sum;
						t2_src_vec_sum = tmp_src_vec_sum;
						_is_use_enable_t2 = true;
					}
				}

				return enable();
			}
		public:
			float_type t1_mnp1_sum;
			float_type t1_mnp2_sum;
			float_type t1_src_vec_sum;

			float_type t2_mnp1_sum;
			float_type t2_mnp2_sum;
			float_type t2_src_vec_sum;

		private:
			bool _is_use_enable_t1;
			bool _is_use_enable_t2;
		};

		if(!(is_line() && right.is_line()))
		{
			return rst_type(pos_type(), false);
		}

		pos_type rst_pos;
		boost::array<yggr::u32, pos_type::E_LENGTH> rst_ends = {0};
		pos_type mnp1 = src_to_dst();
		pos_type mnp2 = right.src_to_dst();
		pos_type src_vec = right.src_pos - src_pos;

		t_caler caler(mnp1, mnp2, src_vec);
		if(!caler.enable())
		{
			return rst_type(pos_type(), false);
		}

		float_type t1 = float_type(), t2 = float_type();
		bool t_end = false;

		float_type tmp_rst = float_type();

		// first check;
		{
			float_type ms1 = mnp1.sum();
			float_type ms2 = mnp2.sum();
			float_type svs = src_vec.sum();

			if(float_cmper_type(ms1) != float_type() && float_cmper_type(ms2) == float_type())
			{
				t1 = svs / ms1;
				t2 = caler.get_t2(t1);
				t_end = true;
			}
			else
			{
				if(float_cmper_type(ms1) == float_type() && float_cmper_type(ms2) != float_type())
				{
					t2 = svs / ms2;
					t1 = caler.get_t1(t2);
					t_end = true;
				}
			}
		}

		//secone check;
		if(!t_end)
		{
			for(size_type i = 0, isize = pos_type::s_size(); i != isize; ++i)
			{
				if(val_cmper_type(mnp1[i]) == val_type() && val_cmper_type(mnp2[i]) == val_type())
				{
					if(val_cmper_type(src_pos[i]) == right.src_pos[i])
					{
						rst_pos[i] = src_pos[i];
						rst_ends[i] = 1;
						continue;
					}

					return rst_type(pos_type(), false);
				}

				if(val_cmper_type(mnp1[i]) != val_type() && val_cmper_type(mnp2[i]) == val_type())
				{
					tmp_rst = right.src_pos[i];
					t1 = (tmp_rst - static_cast<float_type>(src_pos[i])) / static_cast<float_type>(mnp1[i]);
					t2 = caler.get_t2(t1);
					t_end = true;
					rst_pos[i] = tmp_rst;
					rst_ends[i] = 1;
					break;
				}

				if(val_cmper_type(mnp1[i]) == val_type() && val_cmper_type(mnp2[i]) != val_type())
				{
					tmp_rst = right.src_pos[i];
					t2 = (tmp_rst - static_cast<float_type>(right.src_pos[i])) / static_cast<float_type>(mnp2[i]);
					t1 = caler.get_t1(t2);
					t_end = true;
					rst_pos[i] = tmp_rst;
					rst_ends[i] = 1;
					break;
				}

				if(val_cmper_type(mnp1[i] - mnp2[i]) != val_type())
				{
					tmp_rst = static_cast<float_type>(right.src_pos[i] * mnp1[i] - src_pos[i] * mnp2[i])
								/ static_cast<float_type>(mnp1[i] - mnp2[i]);
					t1 = (tmp_rst - static_cast<float_type>(src_pos[i]))
							/ static_cast<float_type>(dst_pos[i] - src_pos[i]);
					t2 = caler.get_t2(t1);
					t_end = true;

					rst_pos[i] = tmp_rst;
					rst_ends[i] = 1;
					break;
				}
			}
		}

		{
			float_type tmp_rst2 = float_type();

			for(size_type i = 0, isize = pos_type::s_size(); i != isize; ++i)
			{
				if(rst_ends[i])
				{
					continue;
				}

				tmp_rst = static_cast<float_type>(src_pos[i]) + static_cast<float_type>(mnp1[i]) * t1;
				tmp_rst2 = static_cast<float_type>(right.src_pos[i]) + static_cast<float_type>(mnp2[i]) * t2;
				if(float_cmper_type(tmp_rst) != tmp_rst2)
				{
					return rst_type(pos_type(), false);
				}

				rst_pos[i] = tmp_rst;
				rst_ends[i] = true;
			}
		}

		return rst_type(rst_pos, true);
	}

	template<typename OPos>
	bool is_in_it(const OPos& point) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		typedef typename pos_type::cross_result_type cross_result_type;
		typedef math::value_miss_comparer<cross_result_type> cmper_type;

		if(!is_line())
		{
			return false;
		}

		pos_type vec1 = point - src_pos;
		pos_type vec2 = dst_pos - src_pos;

		return cmper_type(vec1.cross(vec2)) == cross_result_type();
	}


	template<typename OPos>
	bool is_in_it(const geo_line<OPos, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		return right.is_line() && is_in_it(right.src_pos) && is_in_it(right.dst_pos);
	}

	template<typename OPos>
	bool is_point_at_same_area(const OPos& p1, const OPos& p2) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		return is_point_at_same_area_check(p1, p2);
	}

	template<typename OPos>
	bool is_parallel_line(const geo_line<OPos, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		typedef typename pos_type::cross_result_type cross_result_type;
		typedef math::value_miss_comparer<cross_result_type> cmper_type;

		if(!(is_line() && right.is_line()))
		{
			return false;
		}

		pos_type vec1 = dst_pos - src_pos;
		pos_type vec2 = right.dst_pos - right.src_pos;

		return cmper_type(vec1.cross(vec2)) == cross_result_type();
	}

	template<typename OPos>
	bool is_perpendicular_line(const geo_line<OPos, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));

		if(!(is_line() && right.is_line()))
		{
			return false;
		}

		typedef typename pos_type::val_type val_type;
		typedef typename upper_float<val_type>::value_type rst_type;
		typedef math::value_miss_comparer<rst_type> cmper_type;

		pos_type vec1 = dst_pos - src_pos;
		pos_type vec2 = right.dst_pos - right.src_pos;

		return cmper_type(vec1.dot(vec2)) == rst_type();
	}

	template<typename OPos>
	bool is_cross_line(const geo_line<OPos, Base>& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		return is_cross_line_check(right);
	}

	this_type& swap_src_dst(void)
	{
		src_pos.swap(dst_pos);
		return *this;
	}

	template<typename OPos>
	static bool is_line(const OPos& pos1, const OPos& pos2, const OPos& pos3)
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_LENGTH>,
												boost::mpl::size_t<OPos::E_LENGTH> >));
		typedef geo_line<OPos> line_type;
		line_type line(pos1, pos2);
		return line.is_in_it(pos3);
	}

	template<typename Pos_Container>
	static bool is_line(const Pos_Container& pos_list)
	{
		typedef Pos_Container pos_container_type;
		typedef typename pos_container_type::value_type now_pos_type;
		typedef geo_line<now_pos_type> now_line_type;
		typedef typename pos_container_type::const_iterator pos_conatiner_citer_type;
		size_type isize = pos_list.size();

		if(isize < 2)
		{
			return false;
		}

		pos_conatiner_citer_type iter0 = pos_list.begin();
		pos_conatiner_citer_type iter1 = iter0;
		++iter1;

		now_line_type line(*iter0, *iter1);

		if(isize == 2)
		{
			return line.is_line();
		}

		pos_conatiner_citer_type i = iter1;
		++i;
		for(pos_conatiner_citer_type isize = pos_list.end(); i != isize; ++i)
		{
			if(!line.is_in_it(*i))
			{
				return false;
			}
		}

		return true;
	}

	template<typename Matrix>
	const this_type operator*(const Matrix& right) const
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
												boost::mpl::size_t<Matrix::E_MAX_RANK> >));
		return this_type(src_pos * right, dst_pos * right);
	}

	template<typename Matrix>
	this_type& operator*=(const Matrix& right)
	{
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size_t<pos_type::E_ARRAY_LENGTH>,
												boost::mpl::size_t<Matrix::E_MAX_RANK> >));
		src_pos *= right;
		dst_pos *= right;
		return *this;
	}

protected:
	template<typename Val>
	bool is_cross_line_check(const geo_line< math::vector2d<Val, Base>, Base>& right) const
	{
		return !is_parallel_line(right);
	}

	template<typename Val>
	bool is_cross_line_check(const geo_line< math::vector3d<Val, Base>, Base>& right) const
	{
		typedef geo_plane<Val, Base> plane_type;

		plane_type plane(src_pos, dst_pos, right.src_pos);
		return plane.is_in_it(right.dst_pos) && !is_parallel_line(right);
	}

	template<typename Val,
			template<typename _Val, std::size_t> class TBase>
	bool is_point_at_same_area_check(const math::vector2d<Val, TBase>& p1, const math::vector2d<Val, TBase>& p2) const
	{
		pos_type vec1(p1 - src_pos);
		pos_type vec2(dst_pos - p1);

		pos_type vec3(p2 - src_pos);
		pos_type vec4(dst_pos - p2);

		//float ta = vec1.cross(vec2);
		//float tb = vec3.cross(vec4);

		return math::sign(vec1.cross(vec2)) == math::sign(vec3.cross(vec4));
	}

	template<typename Val>
	bool is_point_at_same_area_check(const math::vector3d<Val, Base>& p1, const math::vector3d<Val, Base>& p2) const
	{
		typedef geo_plane<Val, Base> plane_type;
		plane_type plane(src_pos, dst_pos, p1);

		if(!plane.is_in_it(p2))
		{
			return false;
		}

		pos_type vec1(p1 - src_pos);
		pos_type vec2(dst_pos - p1);

		pos_type vec3(p2 - src_pos);
		pos_type vec4(dst_pos - p2);

		pos_type tvec1 = vec1.cross(vec2);
		pos_type tvec2 = vec3.cross(vec4);
		return math::sign(vec1.cross(vec2)) == math::sign(vec3.cross(vec4));
	}


protected:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(src_pos);
		ar & YGGR_SERIALIZE_NVP(dst_pos);
	}

public:
	pos_type& src_pos;
	pos_type& dst_pos;
};

} // namespace geometry
} // namespace yggr

namespace std
{

template<typename Val,
			template <typename _Val, std::size_t> class Base>
void swap(yggr::geometry::geo_line<Val, Base>& l, yggr::geometry::geo_line<Val, Base>& r)
{
	l.swap(r);
}

} // namespace std

#include <yggr/geometry/geo_plane.hpp>

#endif //__YGGR_GEOMETRY_GEOMETRY_LINE_HPP__
