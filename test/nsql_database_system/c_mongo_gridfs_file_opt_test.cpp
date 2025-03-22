// c_mongo_gridfs_file_opt_test.cpp

#include <cassert>

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/c_mongo_gridfs_file_opt.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)


void test_construct(void)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file_opt mongo_gridfs_file_opt_type;
	typedef mongo_gridfs_file_opt_type::bson_type bson_type;

	{
		mongo_gridfs_file_opt_type fopt;
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5().empty());
		yggr_test_assert(fopt_cref.var_file_name().empty());
		yggr_test_assert(fopt_cref.var_content().empty());
		yggr_test_assert(fopt_cref.var_aliases().empty());
		yggr_test_assert(fopt_cref.var_metadata().empty());
		yggr_test_assert(fopt_cref.var_chunk_size() == 0);
	}

	{
		mongo_gridfs_file_opt_type fopt = "file_name";
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5().empty());
		yggr_test_assert(fopt_cref.var_file_name() == "file_name");
		yggr_test_assert(fopt_cref.var_content().empty());
		yggr_test_assert(fopt_cref.var_aliases().empty());
		yggr_test_assert(fopt_cref.var_metadata().empty());
		yggr_test_assert(fopt_cref.var_chunk_size() == 0);
	}

#if !defined(YGGR_NO_CWCHAR)
	{
		mongo_gridfs_file_opt_type fopt = L"file_name";
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5().empty());
		yggr_test_assert(fopt_cref.var_file_name() == "file_name");
		yggr_test_assert(fopt_cref.var_content().empty());
		yggr_test_assert(fopt_cref.var_aliases().empty());
		yggr_test_assert(fopt_cref.var_metadata().empty());
		yggr_test_assert(fopt_cref.var_chunk_size() == 0);
	}
#endif // YGGR_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)
	{
		mongo_gridfs_file_opt_type fopt = u8"file_name";
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5().empty());
		yggr_test_assert(fopt_cref.var_file_name() == "file_name");
		yggr_test_assert(fopt_cref.var_content().empty());
		yggr_test_assert(fopt_cref.var_aliases().empty());
		yggr_test_assert(fopt_cref.var_metadata().empty());
		yggr_test_assert(fopt_cref.var_chunk_size() == 0);
	}
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	{
		mongo_gridfs_file_opt_type fopt = u"file_name";
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5().empty());
		yggr_test_assert(fopt_cref.var_file_name() == "file_name");
		yggr_test_assert(fopt_cref.var_content().empty());
		yggr_test_assert(fopt_cref.var_aliases().empty());
		yggr_test_assert(fopt_cref.var_metadata().empty());
		yggr_test_assert(fopt_cref.var_chunk_size() == 0);
	}
#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)
	{
		mongo_gridfs_file_opt_type fopt = U"file_name";
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5().empty());
		yggr_test_assert(fopt_cref.var_file_name() == "file_name");
		yggr_test_assert(fopt_cref.var_content().empty());
		yggr_test_assert(fopt_cref.var_aliases().empty());
		yggr_test_assert(fopt_cref.var_metadata().empty());
		yggr_test_assert(fopt_cref.var_chunk_size() == 0);
	}
#endif // YGGR_NO_CHAR32_T

	{
		mongo_gridfs_file_opt_type fopt(std::string("file_name"));
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5().empty());
		yggr_test_assert(fopt_cref.var_file_name() == "file_name");
		yggr_test_assert(fopt_cref.var_content().empty());
		yggr_test_assert(fopt_cref.var_aliases().empty());
		yggr_test_assert(fopt_cref.var_metadata().empty());
		yggr_test_assert(fopt_cref.var_chunk_size() == 0);
	}

#if !defined(YGGR_NO_CWCHAR)
	{
		mongo_gridfs_file_opt_type fopt(std::wstring(L"file_name"));
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5().empty());
		yggr_test_assert(fopt_cref.var_file_name() == "file_name");
		yggr_test_assert(fopt_cref.var_content().empty());
		yggr_test_assert(fopt_cref.var_aliases().empty());
		yggr_test_assert(fopt_cref.var_metadata().empty());
		yggr_test_assert(fopt_cref.var_chunk_size() == 0);
	}
#endif // YGGR_NO_CWCHAR

	{
		bson_type bs_aliases("{aligses_name : \"aligses_value\"}");
		bson_type bs_metadata("{metadata_name : \"metadata_value\"}");

		mongo_gridfs_file_opt_type fopt("md5_str",
										"fname",
										"content",
										bs_aliases,
										bs_metadata,
										1024);
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5() == "md5_str");
		yggr_test_assert(fopt_cref.var_file_name() == "fname");
		yggr_test_assert(fopt_cref.var_content() == "content");
		yggr_test_assert(fopt_cref.var_aliases() == bs_aliases);
		yggr_test_assert(fopt_cref.var_metadata()== bs_metadata);
		yggr_test_assert(fopt_cref.var_chunk_size() == 1024);
	}

	{
		bson_type bs_aliases("{aligses_name : \"aligses_value\"}");
		bson_type bs_metadata("{metadata_name : \"metadata_value\"}");

		mongo_gridfs_file_opt_type fopt_sample("md5_str",
												"fname",
												"content",
												bs_aliases,
												bs_metadata,
												1024);
		mongo_gridfs_file_opt_type fopt = static_cast<const mongo_gridfs_file_opt_type::base_type&>(fopt_sample);
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5() == "md5_str");
		yggr_test_assert(fopt_cref.var_file_name() == "fname");
		yggr_test_assert(fopt_cref.var_content() == "content");
		yggr_test_assert(fopt_cref.var_aliases() == bs_aliases);
		yggr_test_assert(fopt_cref.var_metadata()== bs_metadata);
		yggr_test_assert(fopt_cref.var_chunk_size() == 1024);
	}

	{
		bson_type bs_aliases("{aligses_name : \"aligses_value\"}");
		bson_type bs_metadata("{metadata_name : \"metadata_value\"}");

		mongo_gridfs_file_opt_type fopt_sample("md5_str",
												"fname",
												"content",
												bs_aliases,
												bs_metadata,
												1024);
		mongo_gridfs_file_opt_type fopt = boost::move(fopt_sample);
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5() == "md5_str");
		yggr_test_assert(fopt_cref.var_file_name() == "fname");
		yggr_test_assert(fopt_cref.var_content() == "content");
		yggr_test_assert(fopt_cref.var_aliases() == bs_aliases);
		yggr_test_assert(fopt_cref.var_metadata()== bs_metadata);
		yggr_test_assert(fopt_cref.var_chunk_size() == 1024);
	}

	{
		bson_type bs_aliases("{aligses_name : \"aligses_value\"}");
		bson_type bs_metadata("{metadata_name : \"metadata_value\"}");

		mongo_gridfs_file_opt_type fopt_sample("md5_str",
												"fname",
												"content",
												bs_aliases,
												bs_metadata,
												1024);
		mongo_gridfs_file_opt_type fopt = fopt_sample;
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5() == "md5_str");
		yggr_test_assert(fopt_cref.var_file_name() == "fname");
		yggr_test_assert(fopt_cref.var_content() == "content");
		yggr_test_assert(fopt_cref.var_aliases() == bs_aliases);
		yggr_test_assert(fopt_cref.var_metadata()== bs_metadata);
		yggr_test_assert(fopt_cref.var_chunk_size() == 1024);
	}

	std::cout << "-----------test_construct-------------" << std::endl;
}

void test_operator_set(void)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file_opt mongo_gridfs_file_opt_type;
	typedef mongo_gridfs_file_opt_type::bson_type bson_type;

	{
		bson_type bs_aliases("{aligses_name : \"aligses_value\"}");
		bson_type bs_metadata("{metadata_name : \"metadata_value\"}");

		mongo_gridfs_file_opt_type fopt_sample("md5_str",
												"fname",
												"content",
												bs_aliases,
												bs_metadata,
												1024);
		mongo_gridfs_file_opt_type fopt;
		fopt = static_cast<const mongo_gridfs_file_opt_type::base_type&>(fopt_sample);
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5() == "md5_str");
		yggr_test_assert(fopt_cref.var_file_name() == "fname");
		yggr_test_assert(fopt_cref.var_content() == "content");
		yggr_test_assert(fopt_cref.var_aliases() == bs_aliases);
		yggr_test_assert(fopt_cref.var_metadata()== bs_metadata);
		yggr_test_assert(fopt_cref.var_chunk_size() == 1024);
	}

	{
		bson_type bs_aliases("{aligses_name : \"aligses_value\"}");
		bson_type bs_metadata("{metadata_name : \"metadata_value\"}");

		mongo_gridfs_file_opt_type fopt_sample("md5_str",
												"fname",
												"content",
												bs_aliases,
												bs_metadata,
												1024);
		mongo_gridfs_file_opt_type fopt;
		fopt = boost::move(fopt_sample);
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5() == "md5_str");
		yggr_test_assert(fopt_cref.var_file_name() == "fname");
		yggr_test_assert(fopt_cref.var_content() == "content");
		yggr_test_assert(fopt_cref.var_aliases() == bs_aliases);
		yggr_test_assert(fopt_cref.var_metadata()== bs_metadata);
		yggr_test_assert(fopt_cref.var_chunk_size() == 1024);
	}

	{
		bson_type bs_aliases("{aligses_name : \"aligses_value\"}");
		bson_type bs_metadata("{metadata_name : \"metadata_value\"}");

		mongo_gridfs_file_opt_type fopt_sample("md5_str",
												"fname",
												"content",
												bs_aliases,
												bs_metadata,
												1024);
		mongo_gridfs_file_opt_type fopt;
		fopt = fopt_sample;
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;
		
		yggr_test_assert(fopt_cref.var_md5() == "md5_str");
		yggr_test_assert(fopt_cref.var_file_name() == "fname");
		yggr_test_assert(fopt_cref.var_content() == "content");
		yggr_test_assert(fopt_cref.var_aliases() == bs_aliases);
		yggr_test_assert(fopt_cref.var_metadata()== bs_metadata);
		yggr_test_assert(fopt_cref.var_chunk_size() == 1024);
	}

	std::cout << "-----------test_operator_set-------------" << std::endl;
}

void test_swap(void)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file_opt mongo_gridfs_file_opt_type;
	typedef mongo_gridfs_file_opt_type::bson_type bson_type;

	bson_type bs_aliases1("{aligses_name : \"aligses_value\"}");
	bson_type bs_aliases2("{aligses_name : \"aligses_value2\"}");

	bson_type bs_metadata1("{metadata_name : \"metadata_value\"}");
	bson_type bs_metadata2("{metadata_name : \"metadata_value2\"}");

	mongo_gridfs_file_opt_type fopt_sample1("md5_str1",
											"fname1",
											"content1",
											bs_aliases1,
											bs_metadata1,
											1024);

	mongo_gridfs_file_opt_type fopt_sample2("md5_str2",
											"fname2",
											"content2",
											bs_aliases2,
											bs_metadata2,
											2048);

	mongo_gridfs_file_opt_type fopt1(fopt_sample1);
	mongo_gridfs_file_opt_type fopt2(fopt_sample2);

	{
		fopt1.swap(boost::move(fopt2));

		yggr_test_assert(fopt1.var_md5() == fopt_sample2.var_md5());
		yggr_test_assert(fopt1.var_file_name() == fopt_sample2.var_file_name());
		yggr_test_assert(fopt1.var_content() == fopt_sample2.var_content());
		yggr_test_assert(fopt1.var_aliases() == fopt_sample2.var_aliases());
		yggr_test_assert(fopt1.var_metadata()== fopt_sample2.var_metadata());
		yggr_test_assert(fopt1.var_chunk_size() == fopt_sample2.var_chunk_size());
	}

	{
		fopt1.swap(fopt2);

		yggr_test_assert(fopt1.var_md5() == fopt_sample1.var_md5());
		yggr_test_assert(fopt1.var_file_name() == fopt_sample1.var_file_name());
		yggr_test_assert(fopt1.var_content() == fopt_sample1.var_content());
		yggr_test_assert(fopt1.var_aliases() == fopt_sample1.var_aliases());
		yggr_test_assert(fopt1.var_metadata()== fopt_sample1.var_metadata());
		yggr_test_assert(fopt1.var_chunk_size() == fopt_sample1.var_chunk_size());
	}

	// std::swap
	{
		{
			std::swap(fopt1, fopt2);

			yggr_test_assert(fopt1.var_md5() == fopt_sample2.var_md5());
			yggr_test_assert(fopt1.var_file_name() == fopt_sample2.var_file_name());
			yggr_test_assert(fopt1.var_content() == fopt_sample2.var_content());
			yggr_test_assert(fopt1.var_aliases() == fopt_sample2.var_aliases());
			yggr_test_assert(fopt1.var_metadata()== fopt_sample2.var_metadata());
			yggr_test_assert(fopt1.var_chunk_size() == fopt_sample2.var_chunk_size());
		}

		{
			std::swap(boost::move(fopt1), fopt2);

			yggr_test_assert(fopt1.var_md5() == fopt_sample1.var_md5());
			yggr_test_assert(fopt1.var_file_name() == fopt_sample1.var_file_name());
			yggr_test_assert(fopt1.var_content() == fopt_sample1.var_content());
			yggr_test_assert(fopt1.var_aliases() == fopt_sample1.var_aliases());
			yggr_test_assert(fopt1.var_metadata()== fopt_sample1.var_metadata());
			yggr_test_assert(fopt1.var_chunk_size() == fopt_sample1.var_chunk_size());
		}

		{
			std::swap(fopt1, boost::move(fopt2));

			yggr_test_assert(fopt1.var_md5() == fopt_sample2.var_md5());
			yggr_test_assert(fopt1.var_file_name() == fopt_sample2.var_file_name());
			yggr_test_assert(fopt1.var_content() == fopt_sample2.var_content());
			yggr_test_assert(fopt1.var_aliases() == fopt_sample2.var_aliases());
			yggr_test_assert(fopt1.var_metadata()== fopt_sample2.var_metadata());
			yggr_test_assert(fopt1.var_chunk_size() == fopt_sample2.var_chunk_size());
		}
	}

	// boost::swap
	{
		{
			boost::swap(fopt1, fopt2);

			yggr_test_assert(fopt1.var_md5() == fopt_sample1.var_md5());
			yggr_test_assert(fopt1.var_file_name() == fopt_sample1.var_file_name());
			yggr_test_assert(fopt1.var_content() == fopt_sample1.var_content());
			yggr_test_assert(fopt1.var_aliases() == fopt_sample1.var_aliases());
			yggr_test_assert(fopt1.var_metadata()== fopt_sample1.var_metadata());
			yggr_test_assert(fopt1.var_chunk_size() == fopt_sample1.var_chunk_size());
		}

		{
			boost::swap(boost::move(fopt1), fopt2);

			yggr_test_assert(fopt1.var_md5() == fopt_sample2.var_md5());
			yggr_test_assert(fopt1.var_file_name() == fopt_sample2.var_file_name());
			yggr_test_assert(fopt1.var_content() == fopt_sample2.var_content());
			yggr_test_assert(fopt1.var_aliases() == fopt_sample2.var_aliases());
			yggr_test_assert(fopt1.var_metadata()== fopt_sample2.var_metadata());
			yggr_test_assert(fopt1.var_chunk_size() == fopt_sample2.var_chunk_size());
		}

		{
			boost::swap(fopt1, boost::move(fopt2));

			yggr_test_assert(fopt1.var_md5() == fopt_sample1.var_md5());
			yggr_test_assert(fopt1.var_file_name() == fopt_sample1.var_file_name());
			yggr_test_assert(fopt1.var_content() == fopt_sample1.var_content());
			yggr_test_assert(fopt1.var_aliases() == fopt_sample1.var_aliases());
			yggr_test_assert(fopt1.var_metadata()== fopt_sample1.var_metadata());
			yggr_test_assert(fopt1.var_chunk_size() == fopt_sample1.var_chunk_size());
		}
	}

	// yggr::swap
	{
		{
			yggr::swap(fopt1, fopt2);

			yggr_test_assert(fopt1.var_md5() == fopt_sample2.var_md5());
			yggr_test_assert(fopt1.var_file_name() == fopt_sample2.var_file_name());
			yggr_test_assert(fopt1.var_content() == fopt_sample2.var_content());
			yggr_test_assert(fopt1.var_aliases() == fopt_sample2.var_aliases());
			yggr_test_assert(fopt1.var_metadata()== fopt_sample2.var_metadata());
			yggr_test_assert(fopt1.var_chunk_size() == fopt_sample2.var_chunk_size());
		}

		{
			yggr::swap(boost::move(fopt1), fopt2);

			yggr_test_assert(fopt1.var_md5() == fopt_sample1.var_md5());
			yggr_test_assert(fopt1.var_file_name() == fopt_sample1.var_file_name());
			yggr_test_assert(fopt1.var_content() == fopt_sample1.var_content());
			yggr_test_assert(fopt1.var_aliases() == fopt_sample1.var_aliases());
			yggr_test_assert(fopt1.var_metadata()== fopt_sample1.var_metadata());
			yggr_test_assert(fopt1.var_chunk_size() == fopt_sample1.var_chunk_size());
		}

		{
			yggr::swap(fopt1, boost::move(fopt2));

			yggr_test_assert(fopt1.var_md5() == fopt_sample2.var_md5());
			yggr_test_assert(fopt1.var_file_name() == fopt_sample2.var_file_name());
			yggr_test_assert(fopt1.var_content() == fopt_sample2.var_content());
			yggr_test_assert(fopt1.var_aliases() == fopt_sample2.var_aliases());
			yggr_test_assert(fopt1.var_metadata()== fopt_sample2.var_metadata());
			yggr_test_assert(fopt1.var_chunk_size() == fopt_sample2.var_chunk_size());
		}
	}

	std::cout << "-----------test_swap-------------" << std::endl;
}

void test_member_foo(void)
{
	typedef yggr::nsql_database_system::c_mongo_gridfs_file_opt mongo_gridfs_file_opt_type;
	typedef mongo_gridfs_file_opt_type::bson_type bson_type;

	{
		bson_type bs_aliases("{aligses_name : \"aligses_value\"}");
		bson_type bs_metadata("{metadata_name : \"metadata_value\"}");

		mongo_gridfs_file_opt_type fopt("md5_str",
										"fname",
										"content",
										bs_aliases,
										bs_metadata,
										1024);

		mongo_gridfs_file_opt_type& fopt_ref = fopt;
		const mongo_gridfs_file_opt_type& fopt_cref = fopt;

		
		yggr_test_assert(fopt_ref.var_md5() == "md5_str");
		fopt_ref.var_md5() = "md5_str2";
		yggr_test_assert(fopt_cref.var_md5() == "md5_str2");
		yggr_test_assert(fopt_cref.var_md5<std::string>() == "md5_str2");

		yggr_test_assert(fopt_ref.var_file_name() == "fname");
		fopt_ref.var_file_name() = "fname2";
		yggr_test_assert(fopt_cref.var_file_name() == "fname2");
		yggr_test_assert(fopt_cref.var_file_name<std::string>() == "fname2");

		yggr_test_assert(fopt_ref.var_content() == "content");
		fopt_ref.var_content() = "content2";
		yggr_test_assert(fopt_cref.var_content() == "content2");
		yggr_test_assert(fopt_cref.var_content<std::string>() == "content2");

		yggr_test_assert(fopt_ref.var_aliases() == bs_aliases);
		fopt_ref.var_aliases() = bson_type("{aligses_name : \"aligses_value2\"}");
		yggr_test_assert(fopt_cref.var_aliases() == bson_type("{aligses_name : \"aligses_value2\"}"));
		yggr_test_assert(fopt_cref.var_aliases<bson_t>() == bson_type("{aligses_name : \"aligses_value2\"}"));

		yggr_test_assert(fopt_ref.var_metadata() == bs_metadata);
		fopt_ref.var_metadata() = bson_type("{metadata_name : \"metadata_value2\"}");
		yggr_test_assert(fopt_cref.var_metadata() == bson_type("{metadata_name : \"metadata_value2\"}"));
		yggr_test_assert(fopt_cref.var_metadata<bson_t>() == bson_type("{metadata_name : \"metadata_value2\"}"));

		yggr_test_assert(fopt_ref.var_chunk_size() == 1024);
		fopt_ref.var_chunk_size() = 2048;
		yggr_test_assert(fopt_cref.var_chunk_size() == 2048);
	}

	std::cout << "-----------test_member_foo-------------" << std::endl;
}

int main(int argc, char *argv[])
{
	test_construct();
	test_operator_set();
	test_swap();
	test_member_foo();

	std::cout << "--------------all end-----------------" << std::endl;
	
	wait_any_key(argc, argv);
	return 0;
}