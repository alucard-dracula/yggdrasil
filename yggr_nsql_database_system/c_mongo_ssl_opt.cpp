// c_mongo_ssl_opt.cpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

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

#include <yggr/nsql_database_system/c_mongo_ssl_opt.hpp>
#include <yggr/nsql_database_system/bson_string_native_ex.hpp>
#include <yggr/utility/swap.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace nsql_database_system
{

c_mongo_ssl_opt::c_mongo_ssl_opt(void)
{
	this_type::pro_init();
}

c_mongo_ssl_opt::c_mongo_ssl_opt(const org_type* right)
{
	this_type::pro_init();
	if(right)
	{
		this_type::pro_copy(reinterpret_cast<const base_type&>(*right));
	}
}

c_mongo_ssl_opt::c_mongo_ssl_opt(const org_type& right)
{
	this_type::pro_init();
	this_type::pro_copy(reinterpret_cast<const base_type&>(right));
}

c_mongo_ssl_opt::c_mongo_ssl_opt(const this_type& right)
	: _pem_file(right._pem_file), 
		_pem_pwd(right._pem_pwd),
		_ca_file(right._ca_file),
		_ca_dir(right._ca_dir),
		_crl_file(right._crl_file),
		_internal_tls_opts(right._internal_tls_opts)
{
	this_type::pro_init();

	base_type::pem_file = _pem_file.empty()? 0 : _pem_file.data();
	base_type::pem_pwd = _pem_pwd.empty()? 0 : _pem_pwd.data();
	base_type::ca_file = _ca_file.empty()? 0 : _ca_file.data();
	base_type::ca_dir = _ca_dir.empty()? 0 : _ca_dir.data();
	base_type::crl_file = _crl_file.empty()? 0 : _crl_file.data();
	base_type::weak_cert_validation = right.weak_cert_validation;
	base_type::allow_invalid_hostname = right.allow_invalid_hostname;
	base_type::internal = reinterpret_cast<void*>(boost::addressof(_internal_tls_opts));
}

c_mongo_ssl_opt::~c_mongo_ssl_opt(void)
{
}

c_mongo_ssl_opt::this_type& c_mongo_ssl_opt::operator=(const org_type& right)
{
	return this_type::pro_copy(reinterpret_cast<const base_type&>(right));
}

c_mongo_ssl_opt::this_type& c_mongo_ssl_opt::operator=(const this_type& right)
{
	return this_type::pro_copy(right);
}

void c_mongo_ssl_opt::swap(this_type& right)
{
	this_type::pro_swap(right);
}

//protected:
void c_mongo_ssl_opt::pro_init(void)
{
	base_type& base = *this;
	memset(boost::addressof(base), 0, sizeof(base_type));
	base_type::internal = reinterpret_cast<void*>(boost::addressof(_internal_tls_opts));
}

c_mongo_ssl_opt::this_type& c_mongo_ssl_opt::pro_move(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return *this;
	}

	copy_or_move_or_swap(_pem_file, boost::move(right._pem_file));
	copy_or_move_or_swap(_pem_pwd, boost::move(right._pem_pwd));
	copy_or_move_or_swap(_ca_file, boost::move(right._ca_file));
	copy_or_move_or_swap(_ca_dir, boost::move(right._ca_dir));
	copy_or_move_or_swap(_crl_file, boost::move(right._crl_file));
	copy_or_move_or_swap(_internal_tls_opts, boost::move(right._internal_tls_opts));

	base_type::pem_file = _pem_file.empty()? 0 : _pem_file.data();
	base_type::pem_pwd = _pem_pwd.empty()? 0 : _pem_pwd.data();
	base_type::ca_file = _ca_file.empty()? 0 : _ca_file.data();
	base_type::ca_dir = _ca_dir.empty()? 0 : _ca_dir.data();
	base_type::crl_file = _crl_file.empty()? 0 : _crl_file.data();
	base_type::weak_cert_validation = right.weak_cert_validation;
	base_type::allow_invalid_hostname = right.allow_invalid_hostname;

	return *this;
}

c_mongo_ssl_opt::this_type& c_mongo_ssl_opt::pro_copy(const base_type& right)
{
	if(static_cast<base_type*>(this) == boost::addressof(right))
	{
		return *this;
	}

#ifdef YGGR_AT_WINDOWS
	_pem_file.assign(right.pem_file? right.pem_file : "");
	_pem_pwd.assign(right.pem_pwd? right.pem_pwd : "");
	_ca_file.assign(right.ca_file? right.ca_file : "");
	_ca_dir.assign(right.ca_dir? right.ca_dir : "");
	_crl_file.assign(right.crl_file? right.crl_file : "");
#else
	_pem_file.assign(right.pem_file? right.pem_file : "", 
						YGGR_STR_UTF8_STRING_CHARSET_NAME());
	_pem_pwd.assign(right.pem_pwd? right.pem_pwd : "");
	_ca_file.assign(right.ca_file? right.ca_file : "", 
						YGGR_STR_UTF8_STRING_CHARSET_NAME());
	_ca_dir.assign(right.ca_dir? right.ca_dir : "", 
						YGGR_STR_UTF8_STRING_CHARSET_NAME());
	_crl_file.assign(right.crl_file? right.crl_file : "", 
						YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_AT_WINDOWS

	if(right.internal)
	{
		_internal_tls_opts = *(reinterpret_cast<internal_tls_opts_type*>(right.internal));
	}

	base_type::pem_file = _pem_file.empty()? 0 : _pem_file.data();
	base_type::pem_pwd = _pem_pwd.empty()? 0 : _pem_pwd.data();
	base_type::ca_file = _ca_file.empty()? 0 : _ca_file.data();
	base_type::ca_dir = _ca_dir.empty()? 0 : _ca_dir.data();
	base_type::crl_file = _crl_file.empty()? 0 : _crl_file.data();
	base_type::weak_cert_validation = right.weak_cert_validation;
	base_type::allow_invalid_hostname = right.allow_invalid_hostname;

	return *this;

}

c_mongo_ssl_opt::this_type& c_mongo_ssl_opt::pro_copy(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	_pem_file = right._pem_file;
	_pem_pwd = right._pem_pwd;
	_ca_file = right._ca_file;
	_ca_dir = right._ca_dir;
	_crl_file = right._crl_file;
	_internal_tls_opts = right._internal_tls_opts;

	base_type::pem_file = _pem_file.empty()? 0 : _pem_file.data();
	base_type::pem_pwd = _pem_pwd.empty()? 0 : _pem_pwd.data();
	base_type::ca_file = _ca_file.empty()? 0 : _ca_file.data();
	base_type::ca_dir = _ca_dir.empty()? 0 : _ca_dir.data();
	base_type::crl_file = _crl_file.empty()? 0 : _crl_file.data();
	base_type::weak_cert_validation = right.weak_cert_validation;
	base_type::allow_invalid_hostname = right.allow_invalid_hostname;

	return *this;
}

void c_mongo_ssl_opt::pro_swap(this_type& right)
{
	if(this == boost::addressof(right))
	{
		return;
	}

	_pem_file.swap(right._pem_file);
	_pem_pwd.swap(right._pem_pwd);
	_ca_file.swap(right._ca_file);
	_ca_dir.swap(right._ca_dir);
	_crl_file.swap(right._crl_file);
	yggr::swap(base_type::weak_cert_validation, right.weak_cert_validation);
	yggr::swap(base_type::allow_invalid_hostname, right.allow_invalid_hostname);
	yggr::swap(_internal_tls_opts, right._internal_tls_opts);

	base_type::pem_file = _pem_file.empty()? 0 : _pem_file.data();
	base_type::pem_pwd = _pem_pwd.empty()? 0 : _pem_pwd.data();
	base_type::ca_file = _ca_file.empty()? 0 : _ca_file.data();
	base_type::ca_dir = _ca_dir.empty()? 0 : _ca_dir.data();
	base_type::crl_file = _crl_file.empty()? 0 : _crl_file.data();

	right.base_type::pem_file = right._pem_file.empty()? 0 : right._pem_file.data();
	right.base_type::pem_pwd = right._pem_pwd.empty()? 0 : right._pem_pwd.data();
	right.base_type::ca_file = right._ca_file.empty()? 0 : right._ca_file.data();
	right.base_type::ca_dir = right._ca_dir.empty()? 0 : right._ca_dir.data();
	right.base_type::crl_file = right._crl_file.empty()? 0 : right._crl_file.data();
}

} // namespace nsql_database_system
} // namespace yggr

