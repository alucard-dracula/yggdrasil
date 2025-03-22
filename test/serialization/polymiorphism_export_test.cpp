//polymiorphism_export_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/serialization/access.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/serialization/nvp.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(base)

class basic
{
public:
	basic(void)
	{
	}

	virtual ~basic(void)
	{
	}

	virtual void out(void) const = 0;

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version){}
};

template<typename T>
class child
	: public basic
{
public:
	typedef basic base_type;
public:
	child(void)
	{
	}

	child(const T& n)
		: _n(n)
	{
	}

	virtual ~child(void)
	{
	}

	virtual void out(void) const
	{
		std::cout << _n << std::endl;
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP("base",
                                        boost::serialization::base_object<base_type>(*this));
		ar & YGGR_SERIALIZATION_NVP(_n);
	}

private:
	T _n;
};

#include <yggr/serialization/export.hpp>
YGGR_CLASS_EXPORT_KEY(child<int>)
YGGR_CLASS_EXPORT_IMPLEMENT(child<int>)

#include <yggr/smart_ptr_ex/interface_ptr.hpp>
#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/packet/packet.hpp>

typedef yggr::interface_ptr<basic> basic_ptr_type;

typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak_type;

int main(int argc, char* argv[])
{
	typedef child<int> child_int_type;
	basic_ptr_type ptr(new child_int_type(100));

	opak_type opak;
	opak.save(YGGR_SERIALIZATION_NVP(ptr));

	std::cout << opak.org_buf() << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
