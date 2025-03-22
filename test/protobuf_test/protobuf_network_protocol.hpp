//protobuf_network_protocol.hpp

#ifndef __FLPX_NETWORK_PROTOBUF_NETWORK_PROTOCOL_HPP__
#define __FLPX_NETWORK_PROTOBUF_NETWORK_PROTOCOL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/network/hn_conv.hpp>
#include <yggr/network/network_protocol.hpp>
#include <yggr/serialization/string.hpp>

namespace flpx
{
namespace network
{

template<yggr::u32 ID, yggr::u32 Ver,
			yggr::u32 Cal_T = ::yggr::task_center::task_cal_type::E_CAL_FAST >
class protobuf_network_protocol
	: public yggr::network::network_protocol<yggr::string, ID, Ver, Cal_T>
{
public:
	typedef yggr::network::network_protocol<yggr::string, ID, Ver, Cal_T> base_type;
	typedef typename base_type::value_type buf_type;
	typedef int size_type;

private:
	typedef protobuf_network_protocol this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP("msg",
											boost::serialization::base_object< base_type >(*this));
	}

public:
	protobuf_network_protocol(void)
	{
	}

	template<typename T>
	protobuf_network_protocol(const T& data)
	{
		this_type::save(data);
	}

	protobuf_network_protocol(BOOST_RV_REF(buf_type) right)
		: base_type(boost::move(right))
	{
	}

	protobuf_network_protocol(const buf_type& right)
		: base_type(right)
	{
	}

	protobuf_network_protocol(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(right))
	{
	}

	protobuf_network_protocol(const base_type& right)
		: base_type(right)
	{
	}

	protobuf_network_protocol(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	protobuf_network_protocol(const this_type& right)
		: base_type(right)
	{
	}

	~protobuf_network_protocol(void)
	{
	}

	template<typename T> inline
	this_type& operator=(const T& data)
	{
		this_type::save(data);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(buf_type) right)
	{
		base_type::operator=(boost::move(right));
		return *this;
	}

	this_type& operator=(const buf_type& right)
	{
		base_type::operator=(right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type::operator=(boost::move(right));
	}

	this_type& operator=(const base_type& right)
	{
		base_type::operator=(right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = static_cast<this_type&>(right);
		if(this == &right_ref)
		{
			return *this;
		}

		base_type::operator=(boost::move(static_cast<base_type&>(right_ref)));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type::operator=(right);
		return *this;
	}

	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type::swap(boost::move(right));
	}

	inline void swap(base_type& right)
	{
		base_type::swap(right);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = static_cast<this_type&>(right);
		if(this == &right_ref)
		{
			return;
		}

		base_type::swap(boost::move(static_cast<base_type&>(right)));
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		base_type::swap(right);
	}

	// cpp used
	template<typename T>
	bool save(const T& t)
	{
		T nt(yggr::network::hton(t));
		size_type size = nt.ByteSizeLong();
		if(!size)
		{
			return false;
		}
		else
		{
			buf_type& buf = base_type::msg();
			buf.resize(size, 0);
			return t.SerializeToArray(&buf[0], buf.size());
		}
	}

	template<typename T>
	bool load(T& t) const
	{
		T nt;
		const buf_type& buf = base_type::msg();
		if(!nt.ParseFromArray(&buf[0], buf.size()))
		{
			return false;
		}
		else
		{
			t = yggr::network::ntoh(nt);
			return true;
		}
	}
};

} // namespace network
} // namespace flpx

#define FLPX_TMP_PP_PROTOBUF_NETOWRK_PROTOCOL_SWAP() \
	template<yggr::u32 ID1, yggr::u32 Ver1, yggr::u32 Cal_T1, \
				yggr::u32 ID2, yggr::u32 Ver2, yggr::u32 Cal_T2 > inline \
	void swap(typename flpx::network::protobuf_network_protocol<ID1, Ver1, Cal_T1>::msg_type& l, \
				flpx::network::protobuf_network_protocol<ID2, Ver2, Cal_T2>& r ) { \
		r.swap(l); } \
	\
	template<yggr::u32 ID1, yggr::u32 Ver1, yggr::u32 Cal_T1, \
				yggr::u32 ID2, yggr::u32 Ver2, yggr::u32 Cal_T2 > inline \
	void swap(flpx::network::protobuf_network_protocol<ID1, Ver1, Cal_T1>& l, \
				typename flpx::network::protobuf_network_protocol<ID2, Ver2, Cal_T2>::msg_type& r ) { \
		l.swap(r); } \
	\
	template<yggr::u32 ID1, yggr::u32 Ver1, yggr::u32 Cal_T1, \
				yggr::u32 ID2, yggr::u32 Ver2, yggr::u32 Cal_T2 > inline \
	void swap(typename flpx::network::protobuf_network_protocol<ID1, Ver1, Cal_T1>::base_type& l, \
				flpx::network::protobuf_network_protocol<ID2, Ver2, Cal_T2>& r ) { \
		r.swap(l); } \
	\
	template<yggr::u32 ID1, yggr::u32 Ver1, yggr::u32 Cal_T1, \
				yggr::u32 ID2, yggr::u32 Ver2, yggr::u32 Cal_T2 > inline \
	void swap(flpx::network::protobuf_network_protocol<ID1, Ver1, Cal_T1>& l, \
				typename flpx::network::protobuf_network_protocol<ID2, Ver2, Cal_T2>::base_type& r ) { \
		l.swap(r); } \
	\
	template<yggr::u32 ID1, yggr::u32 Ver1, yggr::u32 Cal_T1, \
				yggr::u32 ID2, yggr::u32 Ver2, yggr::u32 Cal_T2 > inline \
	void swap(flpx::network::protobuf_network_protocol<ID1, Ver1, Cal_T1>& l, \
				flpx::network::protobuf_network_protocol<ID2, Ver2, Cal_T2>& r ) { \
		l.swap(r); }

namespace std
{
	FLPX_TMP_PP_PROTOBUF_NETOWRK_PROTOCOL_SWAP()
} // namespace std

namespace boost
{
	FLPX_TMP_PP_PROTOBUF_NETOWRK_PROTOCOL_SWAP()
} // namespace boost

#endif // __FLPX_NETWORK_PROTOBUF_NETWORK_PROTOCOL_HPP__