// c_mongo_ssl_opt_test.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/nsql_database_system/c_mongo_ssl_opt.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test_construct(void)
{
	typedef yggr::nsql_database_system::c_mongo_ssl_opt ssl_opt_type;

	{
		ssl_opt_type ssl_opt;
		yggr_test_assert(ssl_opt.empty());
	}

	{
		ssl_opt_type ssl_opt("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
								true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		const ssl_opt_type& ssl_opt_cref = ssl_opt;
		yggr_test_assert(!ssl_opt_cref.empty());
		yggr_test_assert(ssl_opt_cref.var_pem_file() == "pem_file");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd() == "pem_passwd");
		yggr_test_assert(ssl_opt_cref.var_ca_file() == "ca_file");
		yggr_test_assert(ssl_opt_cref.var_ca_dir() == "ca_dir");
		yggr_test_assert(ssl_opt_cref.var_crl_file() == "crl_file");
		yggr_test_assert(ssl_opt_cref.var_weak_cert_validation());
		yggr_test_assert(ssl_opt_cref.var_allow_invalid_hostname());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_certificate_revocation_check());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_ocsp_endpoint_check());

		const ssl_opt_type::org_type& org_ssl_opt_cref = *(ssl_opt.org_pointer());
		yggr_test_assert((org_ssl_opt_cref.pem_file && 0 == strcmp(org_ssl_opt_cref.pem_file, "pem_file")));
		yggr_test_assert((org_ssl_opt_cref.pem_pwd && 0 == strcmp(org_ssl_opt_cref.pem_pwd, "pem_passwd")));
		yggr_test_assert((org_ssl_opt_cref.ca_file && 0 == strcmp(org_ssl_opt_cref.ca_file, "ca_file")));
		yggr_test_assert((org_ssl_opt_cref.ca_dir && 0 == strcmp(org_ssl_opt_cref.ca_dir, "ca_dir")));
		yggr_test_assert((org_ssl_opt_cref.crl_file && 0 == strcmp(org_ssl_opt_cref.crl_file, "crl_file")));

		yggr_test_assert(org_ssl_opt_cref.weak_cert_validation);
		yggr_test_assert(org_ssl_opt_cref.allow_invalid_hostname);

		yggr_test_assert(org_ssl_opt_cref.internal 
				&& org_ssl_opt_cref.internal 
					== reinterpret_cast<const void*>(boost::addressof(ssl_opt_cref.var_internal_tls_opts())));
	}

	{
		ssl_opt_type ssl_opt_sample("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type ssl_opt(ssl_opt_sample.org_pointer());

		const ssl_opt_type& ssl_opt_cref = ssl_opt;
		yggr_test_assert(!ssl_opt_cref.empty());
		yggr_test_assert(ssl_opt_cref.var_pem_file() == "pem_file");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd() == "pem_passwd");
		yggr_test_assert(ssl_opt_cref.var_ca_file() == "ca_file");
		yggr_test_assert(ssl_opt_cref.var_ca_dir() == "ca_dir");
		yggr_test_assert(ssl_opt_cref.var_crl_file() == "crl_file");
		yggr_test_assert(ssl_opt_cref.var_weak_cert_validation());
		yggr_test_assert(ssl_opt_cref.var_allow_invalid_hostname());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_certificate_revocation_check());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_ocsp_endpoint_check());

		const ssl_opt_type::org_type& org_ssl_opt_cref = *(ssl_opt.org_pointer());
		yggr_test_assert((org_ssl_opt_cref.pem_file && 0 == strcmp(org_ssl_opt_cref.pem_file, "pem_file")));
		yggr_test_assert((org_ssl_opt_cref.pem_pwd && 0 == strcmp(org_ssl_opt_cref.pem_pwd, "pem_passwd")));
		yggr_test_assert((org_ssl_opt_cref.ca_file && 0 == strcmp(org_ssl_opt_cref.ca_file, "ca_file")));
		yggr_test_assert((org_ssl_opt_cref.ca_dir && 0 == strcmp(org_ssl_opt_cref.ca_dir, "ca_dir")));
		yggr_test_assert((org_ssl_opt_cref.crl_file && 0 == strcmp(org_ssl_opt_cref.crl_file, "crl_file")));

		yggr_test_assert(org_ssl_opt_cref.weak_cert_validation);
		yggr_test_assert(org_ssl_opt_cref.allow_invalid_hostname);

		yggr_test_assert(org_ssl_opt_cref.internal 
				&& org_ssl_opt_cref.internal 
					== reinterpret_cast<const void*>(boost::addressof(ssl_opt_cref.var_internal_tls_opts())));
	}

	{
		ssl_opt_type ssl_opt_sample("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type ssl_opt(*ssl_opt_sample.org_pointer());

		const ssl_opt_type& ssl_opt_cref = ssl_opt;
		yggr_test_assert(!ssl_opt_cref.empty());
		yggr_test_assert(ssl_opt_cref.var_pem_file() == "pem_file");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd() == "pem_passwd");
		yggr_test_assert(ssl_opt_cref.var_ca_file() == "ca_file");
		yggr_test_assert(ssl_opt_cref.var_ca_dir() == "ca_dir");
		yggr_test_assert(ssl_opt_cref.var_crl_file() == "crl_file");
		yggr_test_assert(ssl_opt_cref.var_weak_cert_validation());
		yggr_test_assert(ssl_opt_cref.var_allow_invalid_hostname());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_certificate_revocation_check());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_ocsp_endpoint_check());

		const ssl_opt_type::org_type& org_ssl_opt_cref = *(ssl_opt.org_pointer());
		yggr_test_assert((org_ssl_opt_cref.pem_file && 0 == strcmp(org_ssl_opt_cref.pem_file, "pem_file")));
		yggr_test_assert((org_ssl_opt_cref.pem_pwd && 0 == strcmp(org_ssl_opt_cref.pem_pwd, "pem_passwd")));
		yggr_test_assert((org_ssl_opt_cref.ca_file && 0 == strcmp(org_ssl_opt_cref.ca_file, "ca_file")));
		yggr_test_assert((org_ssl_opt_cref.ca_dir && 0 == strcmp(org_ssl_opt_cref.ca_dir, "ca_dir")));
		yggr_test_assert((org_ssl_opt_cref.crl_file && 0 == strcmp(org_ssl_opt_cref.crl_file, "crl_file")));

		yggr_test_assert(org_ssl_opt_cref.weak_cert_validation);
		yggr_test_assert(org_ssl_opt_cref.allow_invalid_hostname);

		yggr_test_assert(org_ssl_opt_cref.internal 
				&& org_ssl_opt_cref.internal 
					== reinterpret_cast<const void*>(boost::addressof(ssl_opt_cref.var_internal_tls_opts())));
	}

	{
		ssl_opt_type ssl_opt_sample("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type ssl_opt(boost::move(ssl_opt_sample));

		const ssl_opt_type& ssl_opt_cref = ssl_opt;
		yggr_test_assert(!ssl_opt_cref.empty());
		yggr_test_assert(ssl_opt_cref.var_pem_file() == "pem_file");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd() == "pem_passwd");
		yggr_test_assert(ssl_opt_cref.var_ca_file() == "ca_file");
		yggr_test_assert(ssl_opt_cref.var_ca_dir() == "ca_dir");
		yggr_test_assert(ssl_opt_cref.var_crl_file() == "crl_file");
		yggr_test_assert(ssl_opt_cref.var_weak_cert_validation());
		yggr_test_assert(ssl_opt_cref.var_allow_invalid_hostname());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_certificate_revocation_check());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_ocsp_endpoint_check());

		const ssl_opt_type::org_type& org_ssl_opt_cref = *(ssl_opt.org_pointer());
		yggr_test_assert((org_ssl_opt_cref.pem_file && 0 == strcmp(org_ssl_opt_cref.pem_file, "pem_file")));
		yggr_test_assert((org_ssl_opt_cref.pem_pwd && 0 == strcmp(org_ssl_opt_cref.pem_pwd, "pem_passwd")));
		yggr_test_assert((org_ssl_opt_cref.ca_file && 0 == strcmp(org_ssl_opt_cref.ca_file, "ca_file")));
		yggr_test_assert((org_ssl_opt_cref.ca_dir && 0 == strcmp(org_ssl_opt_cref.ca_dir, "ca_dir")));
		yggr_test_assert((org_ssl_opt_cref.crl_file && 0 == strcmp(org_ssl_opt_cref.crl_file, "crl_file")));

		yggr_test_assert(org_ssl_opt_cref.weak_cert_validation);
		yggr_test_assert(org_ssl_opt_cref.allow_invalid_hostname);

		yggr_test_assert(org_ssl_opt_cref.internal 
				&& org_ssl_opt_cref.internal 
					== reinterpret_cast<const void*>(boost::addressof(ssl_opt_cref.var_internal_tls_opts())));
	}

	{
		ssl_opt_type ssl_opt_sample("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type ssl_opt(ssl_opt_sample);

		const ssl_opt_type& ssl_opt_cref = ssl_opt;
		yggr_test_assert(!ssl_opt_cref.empty());
		yggr_test_assert(ssl_opt_cref.var_pem_file() == "pem_file");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd() == "pem_passwd");
		yggr_test_assert(ssl_opt_cref.var_ca_file() == "ca_file");
		yggr_test_assert(ssl_opt_cref.var_ca_dir() == "ca_dir");
		yggr_test_assert(ssl_opt_cref.var_crl_file() == "crl_file");
		yggr_test_assert(ssl_opt_cref.var_weak_cert_validation());
		yggr_test_assert(ssl_opt_cref.var_allow_invalid_hostname());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_certificate_revocation_check());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_ocsp_endpoint_check());

		const ssl_opt_type::org_type& org_ssl_opt_cref = *(ssl_opt.org_pointer());
		yggr_test_assert((org_ssl_opt_cref.pem_file && 0 == strcmp(org_ssl_opt_cref.pem_file, "pem_file")));
		yggr_test_assert((org_ssl_opt_cref.pem_pwd && 0 == strcmp(org_ssl_opt_cref.pem_pwd, "pem_passwd")));
		yggr_test_assert((org_ssl_opt_cref.ca_file && 0 == strcmp(org_ssl_opt_cref.ca_file, "ca_file")));
		yggr_test_assert((org_ssl_opt_cref.ca_dir && 0 == strcmp(org_ssl_opt_cref.ca_dir, "ca_dir")));
		yggr_test_assert((org_ssl_opt_cref.crl_file && 0 == strcmp(org_ssl_opt_cref.crl_file, "crl_file")));

		yggr_test_assert(org_ssl_opt_cref.weak_cert_validation);
		yggr_test_assert(org_ssl_opt_cref.allow_invalid_hostname);

		yggr_test_assert(org_ssl_opt_cref.internal 
				&& org_ssl_opt_cref.internal 
					== reinterpret_cast<const void*>(boost::addressof(ssl_opt_cref.var_internal_tls_opts())));
	}

	std::cout << "-----------------test_construct end-----------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_mongo_ssl_opt ssl_opt_type;

	{
		ssl_opt_type ssl_opt_sample("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type ssl_opt;
		ssl_opt = (*ssl_opt_sample.org_pointer());

		const ssl_opt_type& ssl_opt_cref = ssl_opt;
		yggr_test_assert(!ssl_opt_cref.empty());
		yggr_test_assert(ssl_opt_cref.var_pem_file() == "pem_file");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd() == "pem_passwd");
		yggr_test_assert(ssl_opt_cref.var_ca_file() == "ca_file");
		yggr_test_assert(ssl_opt_cref.var_ca_dir() == "ca_dir");
		yggr_test_assert(ssl_opt_cref.var_crl_file() == "crl_file");
		yggr_test_assert(ssl_opt_cref.var_weak_cert_validation());
		yggr_test_assert(ssl_opt_cref.var_allow_invalid_hostname());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_certificate_revocation_check());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_ocsp_endpoint_check());

		const ssl_opt_type::org_type& org_ssl_opt_cref = *(ssl_opt.org_pointer());
		yggr_test_assert((org_ssl_opt_cref.pem_file && 0 == strcmp(org_ssl_opt_cref.pem_file, "pem_file")));
		yggr_test_assert((org_ssl_opt_cref.pem_pwd && 0 == strcmp(org_ssl_opt_cref.pem_pwd, "pem_passwd")));
		yggr_test_assert((org_ssl_opt_cref.ca_file && 0 == strcmp(org_ssl_opt_cref.ca_file, "ca_file")));
		yggr_test_assert((org_ssl_opt_cref.ca_dir && 0 == strcmp(org_ssl_opt_cref.ca_dir, "ca_dir")));
		yggr_test_assert((org_ssl_opt_cref.crl_file && 0 == strcmp(org_ssl_opt_cref.crl_file, "crl_file")));

		yggr_test_assert(org_ssl_opt_cref.weak_cert_validation);
		yggr_test_assert(org_ssl_opt_cref.allow_invalid_hostname);

		yggr_test_assert(org_ssl_opt_cref.internal 
				&& org_ssl_opt_cref.internal 
					== reinterpret_cast<const void*>(boost::addressof(ssl_opt_cref.var_internal_tls_opts())));
	}

	{
		ssl_opt_type ssl_opt_sample("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type ssl_opt;
		ssl_opt = boost::move(boost::move(ssl_opt_sample));

		const ssl_opt_type& ssl_opt_cref = ssl_opt;
		yggr_test_assert(!ssl_opt_cref.empty());
		yggr_test_assert(ssl_opt_cref.var_pem_file() == "pem_file");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd() == "pem_passwd");
		yggr_test_assert(ssl_opt_cref.var_ca_file() == "ca_file");
		yggr_test_assert(ssl_opt_cref.var_ca_dir() == "ca_dir");
		yggr_test_assert(ssl_opt_cref.var_crl_file() == "crl_file");
		yggr_test_assert(ssl_opt_cref.var_weak_cert_validation());
		yggr_test_assert(ssl_opt_cref.var_allow_invalid_hostname());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_certificate_revocation_check());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_ocsp_endpoint_check());

		const ssl_opt_type::org_type& org_ssl_opt_cref = *(ssl_opt.org_pointer());
		yggr_test_assert((org_ssl_opt_cref.pem_file && 0 == strcmp(org_ssl_opt_cref.pem_file, "pem_file")));
		yggr_test_assert((org_ssl_opt_cref.pem_pwd && 0 == strcmp(org_ssl_opt_cref.pem_pwd, "pem_passwd")));
		yggr_test_assert((org_ssl_opt_cref.ca_file && 0 == strcmp(org_ssl_opt_cref.ca_file, "ca_file")));
		yggr_test_assert((org_ssl_opt_cref.ca_dir && 0 == strcmp(org_ssl_opt_cref.ca_dir, "ca_dir")));
		yggr_test_assert((org_ssl_opt_cref.crl_file && 0 == strcmp(org_ssl_opt_cref.crl_file, "crl_file")));

		yggr_test_assert(org_ssl_opt_cref.weak_cert_validation);
		yggr_test_assert(org_ssl_opt_cref.allow_invalid_hostname);

		yggr_test_assert(org_ssl_opt_cref.internal 
				&& org_ssl_opt_cref.internal 
					== reinterpret_cast<const void*>(boost::addressof(ssl_opt_cref.var_internal_tls_opts())));
	}

	{
		ssl_opt_type ssl_opt_sample("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type ssl_opt;
		ssl_opt = (ssl_opt_sample);

		const ssl_opt_type& ssl_opt_cref = ssl_opt;
		yggr_test_assert(!ssl_opt_cref.empty());
		yggr_test_assert(ssl_opt_cref.var_pem_file() == "pem_file");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd() == "pem_passwd");
		yggr_test_assert(ssl_opt_cref.var_ca_file() == "ca_file");
		yggr_test_assert(ssl_opt_cref.var_ca_dir() == "ca_dir");
		yggr_test_assert(ssl_opt_cref.var_crl_file() == "crl_file");
		yggr_test_assert(ssl_opt_cref.var_weak_cert_validation());
		yggr_test_assert(ssl_opt_cref.var_allow_invalid_hostname());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_certificate_revocation_check());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_ocsp_endpoint_check());

		const ssl_opt_type::org_type& org_ssl_opt_cref = *(ssl_opt.org_pointer());
		yggr_test_assert((org_ssl_opt_cref.pem_file && 0 == strcmp(org_ssl_opt_cref.pem_file, "pem_file")));
		yggr_test_assert((org_ssl_opt_cref.pem_pwd && 0 == strcmp(org_ssl_opt_cref.pem_pwd, "pem_passwd")));
		yggr_test_assert((org_ssl_opt_cref.ca_file && 0 == strcmp(org_ssl_opt_cref.ca_file, "ca_file")));
		yggr_test_assert((org_ssl_opt_cref.ca_dir && 0 == strcmp(org_ssl_opt_cref.ca_dir, "ca_dir")));
		yggr_test_assert((org_ssl_opt_cref.crl_file && 0 == strcmp(org_ssl_opt_cref.crl_file, "crl_file")));

		yggr_test_assert(org_ssl_opt_cref.weak_cert_validation);
		yggr_test_assert(org_ssl_opt_cref.allow_invalid_hostname);

		yggr_test_assert(org_ssl_opt_cref.internal 
				&& org_ssl_opt_cref.internal 
					== reinterpret_cast<const void*>(boost::addressof(ssl_opt_cref.var_internal_tls_opts())));
	}

	std::cout << "-----------------test_operator_set end-----------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_mongo_ssl_opt ssl_opt_type;

	{
		ssl_opt_type ssl_opt_sample("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type ssl_opt;
		ssl_opt.swap(boost::move(ssl_opt_sample));

		const ssl_opt_type& ssl_opt_cref = ssl_opt;
		yggr_test_assert(!ssl_opt_cref.empty());
		yggr_test_assert(ssl_opt_cref.var_pem_file() == "pem_file");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd() == "pem_passwd");
		yggr_test_assert(ssl_opt_cref.var_ca_file() == "ca_file");
		yggr_test_assert(ssl_opt_cref.var_ca_dir() == "ca_dir");
		yggr_test_assert(ssl_opt_cref.var_crl_file() == "crl_file");
		yggr_test_assert(ssl_opt_cref.var_weak_cert_validation());
		yggr_test_assert(ssl_opt_cref.var_allow_invalid_hostname());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_certificate_revocation_check());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_ocsp_endpoint_check());

		const ssl_opt_type::org_type& org_ssl_opt_cref = *(ssl_opt.org_pointer());
		yggr_test_assert((org_ssl_opt_cref.pem_file && 0 == strcmp(org_ssl_opt_cref.pem_file, "pem_file")));
		yggr_test_assert((org_ssl_opt_cref.pem_pwd && 0 == strcmp(org_ssl_opt_cref.pem_pwd, "pem_passwd")));
		yggr_test_assert((org_ssl_opt_cref.ca_file && 0 == strcmp(org_ssl_opt_cref.ca_file, "ca_file")));
		yggr_test_assert((org_ssl_opt_cref.ca_dir && 0 == strcmp(org_ssl_opt_cref.ca_dir, "ca_dir")));
		yggr_test_assert((org_ssl_opt_cref.crl_file && 0 == strcmp(org_ssl_opt_cref.crl_file, "crl_file")));

		yggr_test_assert(org_ssl_opt_cref.weak_cert_validation);
		yggr_test_assert(org_ssl_opt_cref.allow_invalid_hostname);

		yggr_test_assert(org_ssl_opt_cref.internal 
				&& org_ssl_opt_cref.internal 
					== reinterpret_cast<const void*>(boost::addressof(ssl_opt_cref.var_internal_tls_opts())));
	}

	{
		ssl_opt_type ssl_opt_sample("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type ssl_opt;
		ssl_opt.swap(ssl_opt_sample);

		const ssl_opt_type& ssl_opt_cref = ssl_opt;
		yggr_test_assert(!ssl_opt_cref.empty());
		yggr_test_assert(ssl_opt_cref.var_pem_file() == "pem_file");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd() == "pem_passwd");
		yggr_test_assert(ssl_opt_cref.var_ca_file() == "ca_file");
		yggr_test_assert(ssl_opt_cref.var_ca_dir() == "ca_dir");
		yggr_test_assert(ssl_opt_cref.var_crl_file() == "crl_file");
		yggr_test_assert(ssl_opt_cref.var_weak_cert_validation());
		yggr_test_assert(ssl_opt_cref.var_allow_invalid_hostname());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_certificate_revocation_check());
		yggr_test_assert(ssl_opt_cref.var_tls_disable_ocsp_endpoint_check());

		const ssl_opt_type::org_type& org_ssl_opt_cref = *(ssl_opt.org_pointer());
		yggr_test_assert((org_ssl_opt_cref.pem_file && 0 == strcmp(org_ssl_opt_cref.pem_file, "pem_file")));
		yggr_test_assert((org_ssl_opt_cref.pem_pwd && 0 == strcmp(org_ssl_opt_cref.pem_pwd, "pem_passwd")));
		yggr_test_assert((org_ssl_opt_cref.ca_file && 0 == strcmp(org_ssl_opt_cref.ca_file, "ca_file")));
		yggr_test_assert((org_ssl_opt_cref.ca_dir && 0 == strcmp(org_ssl_opt_cref.ca_dir, "ca_dir")));
		yggr_test_assert((org_ssl_opt_cref.crl_file && 0 == strcmp(org_ssl_opt_cref.crl_file, "crl_file")));

		yggr_test_assert(org_ssl_opt_cref.weak_cert_validation);
		yggr_test_assert(org_ssl_opt_cref.allow_invalid_hostname);

		yggr_test_assert(org_ssl_opt_cref.internal 
				&& org_ssl_opt_cref.internal 
					== reinterpret_cast<const void*>(boost::addressof(ssl_opt_cref.var_internal_tls_opts())));
	}

	{
		ssl_opt_type ssl_opt_sample1("pem_file1", "pem_passwd1", "ca_file1", "ca_dir1", "crl_file1",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));
		ssl_opt_type ssl_opt_sample2("pem_file2", "pem_passwd2", "ca_file2", "ca_dir2", "crl_file2",
										false, false, ssl_opt_type::internal_tls_opts_type(false, false));

		ssl_opt_type ssl_opt1(ssl_opt_sample1);
		ssl_opt_type ssl_opt2(ssl_opt_sample2);

		ssl_opt1.swap(boost::move(ssl_opt2));

		yggr_test_assert(!ssl_opt1.empty());
		yggr_test_assert(!ssl_opt2.empty());

		yggr_test_assert(ssl_opt1 == ssl_opt_sample2);
		yggr_test_assert(ssl_opt2 == ssl_opt_sample1);
	}

	{
		ssl_opt_type ssl_opt_sample1("pem_file1", "pem_passwd1", "ca_file1", "ca_dir1", "crl_file1",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));
		ssl_opt_type ssl_opt_sample2("pem_file2", "pem_passwd2", "ca_file2", "ca_dir2", "crl_file2",
										false, false, ssl_opt_type::internal_tls_opts_type(false, false));

		ssl_opt_type ssl_opt1(ssl_opt_sample1);
		ssl_opt_type ssl_opt2(ssl_opt_sample2);

		ssl_opt1.swap(ssl_opt2);

		yggr_test_assert(!ssl_opt1.empty());
		yggr_test_assert(!ssl_opt2.empty());

		yggr_test_assert(ssl_opt1 == ssl_opt_sample2);
		yggr_test_assert(ssl_opt2 == ssl_opt_sample1);
	}

	// std::swap
	{
		ssl_opt_type ssl_opt_sample1("pem_file1", "pem_passwd1", "ca_file1", "ca_dir1", "crl_file1",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));
		ssl_opt_type ssl_opt_sample2("pem_file2", "pem_passwd2", "ca_file2", "ca_dir2", "crl_file2",
										false, false, ssl_opt_type::internal_tls_opts_type(false, false));

		ssl_opt_type ssl_opt1(ssl_opt_sample1);
		ssl_opt_type ssl_opt2(ssl_opt_sample2);

		std::swap(ssl_opt1, boost::move(ssl_opt2));

		yggr_test_assert(!ssl_opt1.empty());
		yggr_test_assert(!ssl_opt2.empty());

		yggr_test_assert(ssl_opt1 == ssl_opt_sample2);
		yggr_test_assert(ssl_opt2 == ssl_opt_sample1);
	}

	{
		ssl_opt_type ssl_opt_sample1("pem_file1", "pem_passwd1", "ca_file1", "ca_dir1", "crl_file1",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));
		ssl_opt_type ssl_opt_sample2("pem_file2", "pem_passwd2", "ca_file2", "ca_dir2", "crl_file2",
										false, false, ssl_opt_type::internal_tls_opts_type(false, false));

		ssl_opt_type ssl_opt1(ssl_opt_sample1);
		ssl_opt_type ssl_opt2(ssl_opt_sample2);

		std::swap(boost::move(ssl_opt1), ssl_opt2);

		yggr_test_assert(!ssl_opt1.empty());
		yggr_test_assert(!ssl_opt2.empty());

		yggr_test_assert(ssl_opt1 == ssl_opt_sample2);
		yggr_test_assert(ssl_opt2 == ssl_opt_sample1);
	}

	{
		ssl_opt_type ssl_opt_sample1("pem_file1", "pem_passwd1", "ca_file1", "ca_dir1", "crl_file1",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));
		ssl_opt_type ssl_opt_sample2("pem_file2", "pem_passwd2", "ca_file2", "ca_dir2", "crl_file2",
										false, false, ssl_opt_type::internal_tls_opts_type(false, false));

		ssl_opt_type ssl_opt1(ssl_opt_sample1);
		ssl_opt_type ssl_opt2(ssl_opt_sample2);

		std::swap(ssl_opt1, ssl_opt2);

		yggr_test_assert(!ssl_opt1.empty());
		yggr_test_assert(!ssl_opt2.empty());

		yggr_test_assert(ssl_opt1 == ssl_opt_sample2);
		yggr_test_assert(ssl_opt2 == ssl_opt_sample1);
	}

	// boost::swap
	{
		ssl_opt_type ssl_opt_sample1("pem_file1", "pem_passwd1", "ca_file1", "ca_dir1", "crl_file1",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));
		ssl_opt_type ssl_opt_sample2("pem_file2", "pem_passwd2", "ca_file2", "ca_dir2", "crl_file2",
										false, false, ssl_opt_type::internal_tls_opts_type(false, false));

		ssl_opt_type ssl_opt1(ssl_opt_sample1);
		ssl_opt_type ssl_opt2(ssl_opt_sample2);

		boost::swap(ssl_opt1, boost::move(ssl_opt2));

		yggr_test_assert(!ssl_opt1.empty());
		yggr_test_assert(!ssl_opt2.empty());

		yggr_test_assert(ssl_opt1 == ssl_opt_sample2);
		yggr_test_assert(ssl_opt2 == ssl_opt_sample1);
	}

	{
		ssl_opt_type ssl_opt_sample1("pem_file1", "pem_passwd1", "ca_file1", "ca_dir1", "crl_file1",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));
		ssl_opt_type ssl_opt_sample2("pem_file2", "pem_passwd2", "ca_file2", "ca_dir2", "crl_file2",
										false, false, ssl_opt_type::internal_tls_opts_type(false, false));

		ssl_opt_type ssl_opt1(ssl_opt_sample1);
		ssl_opt_type ssl_opt2(ssl_opt_sample2);

		boost::swap(boost::move(ssl_opt1), ssl_opt2);

		yggr_test_assert(!ssl_opt1.empty());
		yggr_test_assert(!ssl_opt2.empty());

		yggr_test_assert(ssl_opt1 == ssl_opt_sample2);
		yggr_test_assert(ssl_opt2 == ssl_opt_sample1);
	}

	{
		ssl_opt_type ssl_opt_sample1("pem_file1", "pem_passwd1", "ca_file1", "ca_dir1", "crl_file1",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));
		ssl_opt_type ssl_opt_sample2("pem_file2", "pem_passwd2", "ca_file2", "ca_dir2", "crl_file2",
										false, false, ssl_opt_type::internal_tls_opts_type(false, false));

		ssl_opt_type ssl_opt1(ssl_opt_sample1);
		ssl_opt_type ssl_opt2(ssl_opt_sample2);

		boost::swap(ssl_opt1, ssl_opt2);

		yggr_test_assert(!ssl_opt1.empty());
		yggr_test_assert(!ssl_opt2.empty());

		yggr_test_assert(ssl_opt1 == ssl_opt_sample2);
		yggr_test_assert(ssl_opt2 == ssl_opt_sample1);
	}

	// yggr::swap
	{
		ssl_opt_type ssl_opt_sample1("pem_file1", "pem_passwd1", "ca_file1", "ca_dir1", "crl_file1",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));
		ssl_opt_type ssl_opt_sample2("pem_file2", "pem_passwd2", "ca_file2", "ca_dir2", "crl_file2",
										false, false, ssl_opt_type::internal_tls_opts_type(false, false));

		ssl_opt_type ssl_opt1(ssl_opt_sample1);
		ssl_opt_type ssl_opt2(ssl_opt_sample2);

		yggr::swap(ssl_opt1, boost::move(ssl_opt2));

		yggr_test_assert(!ssl_opt1.empty());
		yggr_test_assert(!ssl_opt2.empty());

		yggr_test_assert(ssl_opt1 == ssl_opt_sample2);
		yggr_test_assert(ssl_opt2 == ssl_opt_sample1);
	}

	{
		ssl_opt_type ssl_opt_sample1("pem_file1", "pem_passwd1", "ca_file1", "ca_dir1", "crl_file1",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));
		ssl_opt_type ssl_opt_sample2("pem_file2", "pem_passwd2", "ca_file2", "ca_dir2", "crl_file2",
										false, false, ssl_opt_type::internal_tls_opts_type(false, false));

		ssl_opt_type ssl_opt1(ssl_opt_sample1);
		ssl_opt_type ssl_opt2(ssl_opt_sample2);

		yggr::swap(boost::move(ssl_opt1), ssl_opt2);

		yggr_test_assert(!ssl_opt1.empty());
		yggr_test_assert(!ssl_opt2.empty());

		yggr_test_assert(ssl_opt1 == ssl_opt_sample2);
		yggr_test_assert(ssl_opt2 == ssl_opt_sample1);
	}

	{
		ssl_opt_type ssl_opt_sample1("pem_file1", "pem_passwd1", "ca_file1", "ca_dir1", "crl_file1",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));
		ssl_opt_type ssl_opt_sample2("pem_file2", "pem_passwd2", "ca_file2", "ca_dir2", "crl_file2",
										false, false, ssl_opt_type::internal_tls_opts_type(false, false));

		ssl_opt_type ssl_opt1(ssl_opt_sample1);
		ssl_opt_type ssl_opt2(ssl_opt_sample2);

		yggr::swap(ssl_opt1, ssl_opt2);

		yggr_test_assert(!ssl_opt1.empty());
		yggr_test_assert(!ssl_opt2.empty());

		yggr_test_assert(ssl_opt1 == ssl_opt_sample2);
		yggr_test_assert(ssl_opt2 == ssl_opt_sample1);
	}

	std::cout << "-----------------test_swap end-----------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_ssl_opt ssl_opt_type;

	// compare_eq
	{
		ssl_opt_type ssl_opt("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
								true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type ssl_opt_chk_same("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));
		
		ssl_opt_type ssl_opt_chk_diff;

		yggr_test_assert(ssl_opt.compare_eq(ssl_opt_chk_same));
		yggr_test_assert(!ssl_opt.compare_eq(ssl_opt_chk_diff));

		yggr_test_assert(ssl_opt == ssl_opt_chk_same);
		yggr_test_assert(ssl_opt != ssl_opt_chk_diff);

		// org_type

		yggr_test_assert(ssl_opt.compare_eq(*ssl_opt_chk_same.org_pointer()));
		yggr_test_assert(!ssl_opt.compare_eq(*ssl_opt_chk_diff.org_pointer()));

		yggr_test_assert(*ssl_opt.org_pointer() == ssl_opt_chk_same);
		yggr_test_assert(ssl_opt == *ssl_opt_chk_same.org_pointer());

		yggr_test_assert(*ssl_opt.org_pointer() != ssl_opt_chk_diff);
		yggr_test_assert(ssl_opt != *ssl_opt_chk_diff.org_pointer());
	}

	// empty
	{
		ssl_opt_type ssl_opt_empty;
		ssl_opt_type ssl_opt_non_empty("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
										true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		yggr_test_assert(ssl_opt_empty.empty());
		yggr_test_assert(!ssl_opt_non_empty.empty());
		
	}

	//var_pem_file
	{
		ssl_opt_type ssl_opt("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
								true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type& ssl_opt_ref(ssl_opt);
		const ssl_opt_type& ssl_opt_cref(ssl_opt);

		ssl_opt_ref.var_pem_file() = "pem_file2";

		yggr_test_assert(ssl_opt_ref.var_pem_file() == "pem_file2");
		yggr_test_assert(ssl_opt_cref.var_pem_file() == "pem_file2");

		yggr_test_assert(ssl_opt_cref.var_pem_file<ssl_opt_type::path_string_type>() == "pem_file2");
		yggr_test_assert(ssl_opt_cref.var_pem_file<ssl_opt_type::path_string_type>(ssl_opt_type::path_string_type().get_allocator()) == "pem_file2");

		yggr_test_assert(ssl_opt_cref.var_pem_file<std::string>() == "pem_file2");
		yggr_test_assert(ssl_opt_cref.var_pem_file<std::string>(std::string().get_allocator()) == "pem_file2");

		yggr_test_assert(ssl_opt_cref.var_pem_file<boost::container::string>() == "pem_file2");
		yggr_test_assert(ssl_opt_cref.var_pem_file<boost::container::string>(boost::container::string().get_allocator()) == "pem_file2");

		yggr_test_assert(ssl_opt_cref.var_pem_file<yggr::string>() == "pem_file2");
		yggr_test_assert(ssl_opt_cref.var_pem_file<yggr::string>(yggr::string().get_allocator()) == "pem_file2");

		yggr_test_assert(ssl_opt_cref.var_pem_file<yggr::utf8_string>() == "pem_file2");
		yggr_test_assert(ssl_opt_cref.var_pem_file<yggr::utf8_string>(yggr::utf8_string().get_allocator()) == "pem_file2");
	}

	//var_pem_pwd
	{
		ssl_opt_type ssl_opt("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
								true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type& ssl_opt_ref(ssl_opt);
		const ssl_opt_type& ssl_opt_cref(ssl_opt);

		ssl_opt_ref.var_pem_pwd() = "pem_passwd2";

		yggr_test_assert(ssl_opt_ref.var_pem_pwd() == "pem_passwd2");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd() == "pem_passwd2");

		yggr_test_assert(ssl_opt_cref.var_pem_pwd<ssl_opt_type::path_string_type>() == "pem_passwd2");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd<ssl_opt_type::path_string_type>(ssl_opt_type::path_string_type().get_allocator()) == "pem_passwd2");

		yggr_test_assert(ssl_opt_cref.var_pem_pwd<std::string>() == "pem_passwd2");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd<std::string>(std::string().get_allocator()) == "pem_passwd2");

		yggr_test_assert(ssl_opt_cref.var_pem_pwd<boost::container::string>() == "pem_passwd2");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd<boost::container::string>(boost::container::string().get_allocator()) == "pem_passwd2");

		yggr_test_assert(ssl_opt_cref.var_pem_pwd<yggr::string>() == "pem_passwd2");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd<yggr::string>(yggr::string().get_allocator()) == "pem_passwd2");

		yggr_test_assert(ssl_opt_cref.var_pem_pwd<yggr::utf8_string>() == "pem_passwd2");
		yggr_test_assert(ssl_opt_cref.var_pem_pwd<yggr::utf8_string>(yggr::utf8_string().get_allocator()) == "pem_passwd2");
	}

	//var_ca_file
	{
		ssl_opt_type ssl_opt("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
								true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type& ssl_opt_ref(ssl_opt);
		const ssl_opt_type& ssl_opt_cref(ssl_opt);

		ssl_opt_ref.var_ca_file() = "ca_file2";

		yggr_test_assert(ssl_opt_ref.var_ca_file() == "ca_file2");
		yggr_test_assert(ssl_opt_cref.var_ca_file() == "ca_file2");

		yggr_test_assert(ssl_opt_cref.var_ca_file<ssl_opt_type::path_string_type>() == "ca_file2");
		yggr_test_assert(ssl_opt_cref.var_ca_file<ssl_opt_type::path_string_type>(ssl_opt_type::path_string_type().get_allocator()) == "ca_file2");

		yggr_test_assert(ssl_opt_cref.var_ca_file<std::string>() == "ca_file2");
		yggr_test_assert(ssl_opt_cref.var_ca_file<std::string>(std::string().get_allocator()) == "ca_file2");

		yggr_test_assert(ssl_opt_cref.var_ca_file<boost::container::string>() == "ca_file2");
		yggr_test_assert(ssl_opt_cref.var_ca_file<boost::container::string>(boost::container::string().get_allocator()) == "ca_file2");

		yggr_test_assert(ssl_opt_cref.var_ca_file<yggr::string>() == "ca_file2");
		yggr_test_assert(ssl_opt_cref.var_ca_file<yggr::string>(yggr::string().get_allocator()) == "ca_file2");

		yggr_test_assert(ssl_opt_cref.var_ca_file<yggr::utf8_string>() == "ca_file2");
		yggr_test_assert(ssl_opt_cref.var_ca_file<yggr::utf8_string>(yggr::utf8_string().get_allocator()) == "ca_file2");
	}

	//var_ca_dir
	{
		ssl_opt_type ssl_opt("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
								true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type& ssl_opt_ref(ssl_opt);
		const ssl_opt_type& ssl_opt_cref(ssl_opt);

		ssl_opt_ref.var_ca_dir() = "ca_dir2";

		yggr_test_assert(ssl_opt_ref.var_ca_dir() == "ca_dir2");
		yggr_test_assert(ssl_opt_cref.var_ca_dir() == "ca_dir2");

		yggr_test_assert(ssl_opt_cref.var_ca_dir<ssl_opt_type::path_string_type>() == "ca_dir2");
		yggr_test_assert(ssl_opt_cref.var_ca_dir<ssl_opt_type::path_string_type>(ssl_opt_type::path_string_type().get_allocator()) == "ca_dir2");

		yggr_test_assert(ssl_opt_cref.var_ca_dir<std::string>() == "ca_dir2");
		yggr_test_assert(ssl_opt_cref.var_ca_dir<std::string>(std::string().get_allocator()) == "ca_dir2");

		yggr_test_assert(ssl_opt_cref.var_ca_dir<boost::container::string>() == "ca_dir2");
		yggr_test_assert(ssl_opt_cref.var_ca_dir<boost::container::string>(boost::container::string().get_allocator()) == "ca_dir2");

		yggr_test_assert(ssl_opt_cref.var_ca_dir<yggr::string>() == "ca_dir2");
		yggr_test_assert(ssl_opt_cref.var_ca_dir<yggr::string>(yggr::string().get_allocator()) == "ca_dir2");

		yggr_test_assert(ssl_opt_cref.var_ca_dir<yggr::utf8_string>() == "ca_dir2");
		yggr_test_assert(ssl_opt_cref.var_ca_dir<yggr::utf8_string>(yggr::utf8_string().get_allocator()) == "ca_dir2");
	}

	//var_crl_file
	{
		ssl_opt_type ssl_opt("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
								true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type& ssl_opt_ref(ssl_opt);
		const ssl_opt_type& ssl_opt_cref(ssl_opt);

		ssl_opt_ref.var_crl_file() = "crl_file2";

		yggr_test_assert(ssl_opt_ref.var_crl_file() == "crl_file2");
		yggr_test_assert(ssl_opt_cref.var_crl_file() == "crl_file2");

		yggr_test_assert(ssl_opt_cref.var_crl_file<ssl_opt_type::path_string_type>() == "crl_file2");
		yggr_test_assert(ssl_opt_cref.var_crl_file<ssl_opt_type::path_string_type>(ssl_opt_type::path_string_type().get_allocator()) == "crl_file2");

		yggr_test_assert(ssl_opt_cref.var_crl_file<std::string>() == "crl_file2");
		yggr_test_assert(ssl_opt_cref.var_crl_file<std::string>(std::string().get_allocator()) == "crl_file2");

		yggr_test_assert(ssl_opt_cref.var_crl_file<boost::container::string>() == "crl_file2");
		yggr_test_assert(ssl_opt_cref.var_crl_file<boost::container::string>(boost::container::string().get_allocator()) == "crl_file2");

		yggr_test_assert(ssl_opt_cref.var_crl_file<yggr::string>() == "crl_file2");
		yggr_test_assert(ssl_opt_cref.var_crl_file<yggr::string>(yggr::string().get_allocator()) == "crl_file2");

		yggr_test_assert(ssl_opt_cref.var_crl_file<yggr::utf8_string>() == "crl_file2");
		yggr_test_assert(ssl_opt_cref.var_crl_file<yggr::utf8_string>(yggr::utf8_string().get_allocator()) == "crl_file2");
	}

	// var_weak_cert_validation
	{
		ssl_opt_type ssl_opt("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
								true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type& ssl_opt_ref(ssl_opt);
		const ssl_opt_type& ssl_opt_cref(ssl_opt);

		ssl_opt_ref.var_weak_cert_validation() = false;

		yggr_test_assert(!ssl_opt_ref.var_weak_cert_validation());
		yggr_test_assert(!ssl_opt_cref.var_weak_cert_validation());
	}

	// var_allow_invalid_hostname
	{
		ssl_opt_type ssl_opt("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
								true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type& ssl_opt_ref(ssl_opt);
		const ssl_opt_type& ssl_opt_cref(ssl_opt);

		ssl_opt_ref.var_allow_invalid_hostname() = false;

		yggr_test_assert(!ssl_opt_ref.var_allow_invalid_hostname());
		yggr_test_assert(!ssl_opt_cref.var_allow_invalid_hostname());
	}

	// var_internal_tls_opts
	{
		ssl_opt_type ssl_opt("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
								true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type& ssl_opt_ref(ssl_opt);
		const ssl_opt_type& ssl_opt_cref(ssl_opt);

		ssl_opt_ref.var_internal_tls_opts() = ssl_opt_type::internal_tls_opts_type(false, false);

		yggr_test_assert(ssl_opt_ref.var_internal_tls_opts() == ssl_opt_type::internal_tls_opts_type(false, false));
		yggr_test_assert(ssl_opt_cref.var_internal_tls_opts() == ssl_opt_type::internal_tls_opts_type(false, false));
	}

	// var_tls_disable_certificate_revocation_check
	{
		ssl_opt_type ssl_opt("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
								true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type& ssl_opt_ref(ssl_opt);
		const ssl_opt_type& ssl_opt_cref(ssl_opt);

		ssl_opt_ref.var_tls_disable_certificate_revocation_check() = false;

		yggr_test_assert(!ssl_opt_ref.var_tls_disable_certificate_revocation_check());
		yggr_test_assert(!ssl_opt_cref.var_tls_disable_certificate_revocation_check());
	}

	// var_tls_disable_certificate_revocation_check
	{
		ssl_opt_type ssl_opt("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
								true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		ssl_opt_type& ssl_opt_ref(ssl_opt);
		const ssl_opt_type& ssl_opt_cref(ssl_opt);

		ssl_opt_ref.var_tls_disable_certificate_revocation_check() = false;

		yggr_test_assert(!ssl_opt_ref.var_tls_disable_certificate_revocation_check());
		yggr_test_assert(!ssl_opt_cref.var_tls_disable_certificate_revocation_check());
	}

	// mongoc_ssl_opt_get_default 
	{
		std::cout << ssl_opt_type::s_get_default() << std::endl;
	}

	// output
	{
		typedef yggr::nsql_database_system::bson_json_mode_def bson_json_mode_def_type;

		ssl_opt_type ssl_opt("pem_file", "pem_passwd", "ca_file", "ca_dir", "crl_file",
								true, true, ssl_opt_type::internal_tls_opts_type(true, true));

		std::cout << ssl_opt << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>(ssl_opt) << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>(ssl_opt) << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>(ssl_opt) << std::endl;

		// org_type

		std::cout << (*ssl_opt.org_pointer()) << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_LEGACY>((*ssl_opt.org_pointer())) << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_CANONICAL>((*ssl_opt.org_pointer())) << std::endl;

		std::cout <<
			yggr::nsql_database_system::make_bson_visit<
				bson_json_mode_def_type::E_BSON_JSON_MODE_RELAXED>((*ssl_opt.org_pointer())) << std::endl;
	}

	std::cout << "-----------------test_member_foo end-----------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_operator_set();
	test_swap();
	test_member_foo();

	wait_any_key(argc, argv);
	return 0;
}