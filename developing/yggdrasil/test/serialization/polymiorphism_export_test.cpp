//polymiorphism_export_test.cpp

#include <iostream>

#include <boost/serialization/access.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/serialization/nvp.hpp>

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
		ar & YGGR_SERIALIZE_NAME_NVP("base",
                                        boost::serialization::base_object<base_type>(*this));
		ar & YGGR_SERIALIZE_NVP(_n);
	}

private:
	T _n;
};

#include <yggr/serialization/export.hpp>
YGGR_CLASS_EXPORT_KEY(child<int>)
YGGR_CLASS_EXPORT_IMPLEMENT(child<int>)

#include <yggr/base/interface_ptr.hpp>
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
	opak.save(YGGR_SERIALIZE_NVP(ptr));

	std::cout << opak.org_buf() << std::endl;

	char cc = 0;
	std::cin >> cc;

	return 0;
}