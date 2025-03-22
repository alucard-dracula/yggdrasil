//safe_map_test_part2_no_opt.cpp

#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <functional>

#include <boost/utility/enable_if.hpp>
#include <boost/bind.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/mplex/null_t.hpp>

#include <yggr/safe_container/safe_map.hpp>
#include <yggr/safe_container/safe_options_map.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)

#define THREAD_TEST_USING_MANUAL() 0

#define THREAD_TEST_THREAD_SIZE() 10
#define THREAD_TEST_THREAD_LOOP_SIZE() 10

//-----------------------test safe---------------------------------------------

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <cstdlib>
#include <yggr/thread/boost_thread.hpp>
#include <yggr/safe_container/safe_vector.hpp>

typedef yggr::safe_container::safe_vector<int, boost::mutex, std::allocator<int>, std::vector> hvt_type;

enum
{
	E_read = 0,
	E_write = 100,
	E_max_elem = 20000
};

template<typename Container>
bool push_handler(typename Container::base_type& base,
					const std::pair<typename Container::iterator, bool>& pair,
					int& count)
{
	if(pair.second)
	{
		++count;
	}

	return pair.second;
}

template<typename Container>
void ref_handler(typename Container::base_type& cont)
{
	if(cont.size() % 2 == 0)
	{
		cont.erase(cont.begin(), cont.end());
	}
}
template<typename Container>
void cref_handler(const typename Container::base_type& cont)
{
	int n = cont.size();
	std::cout << n <<std::endl;
}

template<typename Val>
std::pair<int, Val> create_val(void)
{
	typedef Val val_type;
	typedef typename val_type::element_type elem_type;
	return std::pair<int, val_type>(std::rand() % 100, val_type(new elem_type(std::rand() % 100)));
}

template<typename Container>
void read_op(const Container& cont, hvt_type& hvt)
{
	typedef Container container_type;
	typedef typename container_type::base_type container_base_type;
	typedef typename container_type::mapped_type val_type;

	if(E_max_elem < hvt.size())
	{
		hvt.clear();
	}
	int op = std::rand() %  10;
	switch(op)
	{
	case 0: // size
		hvt.push_back(0 + E_read);
		cont.size();
		break;
	case 1: // max_size
		hvt.push_back(1 + E_read);
		cont.max_size();
		break;
	case 2: // empty
		hvt.push_back(2 + E_read);
		cont.empty();
		break;
	case 3: // key_comp
		hvt.push_back(3 + E_read);
		cont.key_comp();
		break;
	case 4: // value_comp
		hvt.push_back(4 + E_read);
		cont.value_comp();
		break;
	case 5: // get_allocator
		hvt.push_back(5 + E_read);
		cont.get_allocator();
		break;
	case 6: // is_exisits
		{
			hvt.push_back(6 + E_read);
			cont.is_exists(std::rand() % 100);
			cont.is_exists(std::rand() % 100);
		}
		break;
	case 7: // using_handler const
		hvt.push_back(7 + E_read);
		cont.using_handler(boost::bind(&cref_handler<container_type>, _1));
		break;
	case 8: // get_value
		{
			hvt.push_back(8+ E_read);
			val_type p;
			cont.get_value(std::rand() % 100, p);
		}
		break;
	case 9:
		hvt.push_back(9 + E_read);
		cont.count(std::rand() % 100);
		break;
	default:
		yggr_test_assert(false);
	}
}

template<typename Container>
void write_op(Container& cont, hvt_type& hvt)
{
	typedef Container container_type;
	typedef typename container_type::base_type container_base_type;
	typedef typename container_type::mapped_type val_type;
	typedef typename container_base_type::iterator iterator_type;
	typedef typename container_base_type::const_iterator const_iterator_type;

	typedef iterator_type (container_base_type::* begin_end_foo_type)(void);
	typedef const_iterator_type (container_base_type::* cbegin_end_foo_type)(void) const;

	if(E_max_elem < hvt.size())
	{
		hvt.clear();
	}

	int op = std::rand() % 15;
	//int op = 2;
	switch(op)
	{
	case 0: // operator= &&
		{
			hvt.push_back(0 + E_write);
			container_base_type tmp;
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			cont = boost::move(tmp);
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));

			typename container_type::value_type v1(create_val<val_type>());
			typename container_type::value_type v2(create_val<val_type>());
			typename container_type::value_type v3(create_val<val_type>());

			tmp.insert(v1);
			tmp.insert(v2);
			tmp.insert(v3);

			try
			{
				val_type val = cont[200];
				if(val)
				{
					*val += *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
				}
				else
				{
					typename val_type::element_type m = *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
					std::cout << m << std::endl;
				}
			}
			catch(const typename container_type::error_type&) {}
		}
		break;
	case 1: // operator = const &
		{
			hvt.push_back(1 + E_write);
			container_base_type tmp;
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			cont = tmp;
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));

			typename container_type::value_type v1(create_val<val_type>());
			typename container_type::value_type v2(create_val<val_type>());
			typename container_type::value_type v3(create_val<val_type>());

			tmp.insert(v1);
			tmp.insert(v2);
			tmp.insert(v3);

			try
			{
				val_type val = cont[200];
				if(val)
				{
					*val += *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
				}
				else
				{
					typename val_type::element_type m = *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
					std::cout << m << std::endl;
				}
			}
			catch(const typename container_type::error_type&) {}
		}
		break;
	case 2: // insert
		hvt.push_back(2 + E_write);
		cont.insert(create_val<val_type>());
		break;
	case 3: // insert(&&)
		hvt.push_back(3 + E_write);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		cont.insert(boost::move(create_val<val_type>()));
#else
        {
            typename container_type::value_type val(create_val<val_type>());
            cont.insert(boost::move(val));
        }
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		break;
	case 4: // insert(val&&, handler)
		{
			hvt.push_back(4 + E_write);
			int count = 0;
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
			bool bret = cont.insert(boost::move(create_val<val_type>()),
							boost::bind(&push_handler<container_type>, _1, _2, boost::ref(count)));
#else
            typename container_type::value_type val(create_val<val_type>());
            bool bret = cont.insert(boost::move(val),
							boost::bind(&push_handler<container_type>, _1, _2, boost::ref(count)));
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
			yggr_test_assert((count == 1 && bret == true) || (count == 0 && bret == false));
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
		}
		break;
	case 5: // // push_back(const val&, handler)
		{
			hvt.push_back(5 + E_write);
			int count = 0;
			bool bret =  cont.insert(create_val<val_type>(),
							boost::bind(&push_handler<container_type>, _1, _2, boost::ref(count)));
			yggr_test_assert((count == 1 && bret == true) || (count == 0 && bret == false));
		}
		break;
	case 6: // pop_back
		{
			hvt.push_back(6 + E_write);
			cont.erase(std::rand() % 100);
		}
		break;
	case 7: // swap(base&&)
		{
			hvt.push_back(7 + E_write);
			container_base_type tmp;
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			cont.swap(boost::move(tmp));
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));

			typename container_type::value_type v1(create_val<val_type>());
			typename container_type::value_type v2(create_val<val_type>());
			typename container_type::value_type v3(create_val<val_type>());

			tmp.insert(v1);
			tmp.insert(v2);
			tmp.insert(v3);

			try
			{
				val_type val = cont[200];
				if(val)
				{
					*val += *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
				}
				else
				{
					typename val_type::element_type m = *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
					std::cout << m << std::endl;
				}
			}
			catch(const typename container_type::error_type&) {}
		}
		break;
	case 8: // swap(base&)
		{
			hvt.push_back(8 + E_write);
			container_base_type tmp;
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			cont.swap(tmp);
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));

			typename container_type::value_type v1(create_val<val_type>());
			typename container_type::value_type v2(create_val<val_type>());
			typename container_type::value_type v3(create_val<val_type>());

			tmp.insert(v1);
			tmp.insert(v2);
			tmp.insert(v3);

			try
			{
				val_type val = cont[200];
				if(val)
				{
					*val += *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
				}
				else
				{
					typename val_type::element_type m = *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
					std::cout << m << std::endl;
				}
			}
			catch(const typename container_type::error_type&) {}
		}
		break;
	case 9: // swap(this&)
		{
			hvt.push_back(9 + E_write);
			container_type tmp;
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			cont.swap(tmp);
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));

			typename container_type::value_type v1(create_val<val_type>());
			typename container_type::value_type v2(create_val<val_type>());
			typename container_type::value_type v3(create_val<val_type>());

			tmp.insert(v1);
			tmp.insert(v2);
			tmp.insert(v3);

			try
			{
				val_type val = cont[200];
				if(val)
				{
					*val += *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
				}
				else
				{
					typename val_type::element_type m = *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
					std::cout << m << std::endl;
				}
			}
			catch(const typename container_type::error_type&) {}
		}
		break;
	case 10: //clear
		hvt.push_back(10 + E_write);
		cont.clear();
		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
		break;
	case 11: // use handler
		hvt.push_back(11 + E_write);
		cont.using_handler(boost::bind(&ref_handler<container_type>, _1));
		break;
	case 12: //set_value(const &)
		hvt.push_back(12 + E_write);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		cont.set_value(boost::move(create_val<val_type>()));
#else
        {
            std::pair<int, val_type> val(create_val<val_type>());
            cont.set_value(boost::move(val));
        }
#endif  //YGGR_NO_CXX11_RVALUE_REFERENCES
	case 13: //set_value(const &)
		hvt.push_back(13 + E_write);
		cont.set_value(create_val<val_type>());
		break;
	case 14: // replace
		hvt.push_back(14 + E_write);
		cont.replace(create_val<val_type>());
		break;
	default:
		yggr_test_assert(false);
	}

	if(cont.size() > E_max_elem)
	{
		container_base_type tmp;
		cont.swap(tmp);
	}
}

template<typename Container>
void thread_foo_of_mutex(Container& cont, int size, hvt_type& hvt)
{
	boost::thread::id tid = boost::this_thread::get_id();
	for(int i = 0; i != size; ++i)
	{
		if(std::rand() %  2)
		{
			write_op(cont, hvt);
		}
		else
		{
			read_op(cont, hvt);
		}

		//std::cout << "thread " << tid << " run " << i << std::endl;
	}
}

template<typename Cont>
void test_thread_safe_of_mutex(void)
{
	typedef Cont container_type;
	typedef typename container_type::value_type val_type;

	int loop_size = THREAD_TEST_THREAD_LOOP_SIZE();
	int trd_size = THREAD_TEST_THREAD_SIZE();

#if THREAD_TEST_USING_MANUAL()
	std::cout << "input loop_size:" << std::endl;
	std::cin >> loop_size;

	std::cout << "input thread_size:" << std::endl;
	std::cin >> trd_size;

#endif // THREAD_TEST_USING_MANUAL

	container_type cont;
	hvt_type hvt;

	boost::thread_group trds;

	for(int i = 0; i != trd_size; ++i)
	{
		trds.create_thread(boost::bind(thread_foo_of_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt)));
	}
	trds.join_all();

	std::cout << "test_thread_safe_of_mutex end" << std::endl;
}

template<typename Container>
void thread_foo_of_shard_mutex(Container& cont, int size, hvt_type& hvt, bool mode)
{
	boost::thread::id tid = boost::this_thread::get_id();
	for(int i = 0; i != size; ++i)
	{
		if(mode)
		{
			write_op(cont, hvt);
		}
		else
		{
			read_op(cont, hvt);
		}

		//std::cout << "thread " << tid << " run " << i << std::endl;
	}
}

template<typename Cont>
void test_thread_safe_of_shared_mutex(void)
{
	typedef Cont container_type;
	typedef typename container_type::value_type val_type;

	int loop_size = THREAD_TEST_THREAD_LOOP_SIZE();
	int trd_size = THREAD_TEST_THREAD_SIZE();

#if THREAD_TEST_USING_MANUAL()
	std::cout << "input loop_size:" << std::endl;
	std::cin >> loop_size;

	std::cout << "input thread_size:" << std::endl;
	std::cin >> trd_size;

#endif // THREAD_TEST_USING_MANUAL

	container_type cont;
	hvt_type hvt;

	if(!trd_size)
	{
		return;
	}

	boost::thread_group trds;

	trds.create_thread(boost::bind(&thread_foo_of_shard_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt), true));

	for(int i = 0, isize = trd_size - 1; i != isize; ++i)
	{
		trds.create_thread(boost::bind(thread_foo_of_shard_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt), false));
	}
	trds.join_all();

	std::cout << "test_thread_safe_of_shared_mutex end" << std::endl;
}

int main(int argc, char* argv[])
{
	std::srand(std::time(0));

	// mutex
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_map<int, val_type> map_type;
		test_thread_safe_of_mutex<map_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef
			yggr::safe_container::safe_map
			<
				int, val_type,
	            boost::mutex,
	            std::less<int>,
	            std::allocator<std::pair<const int, val_type> >,
	            std::map
			> map_type;
		test_thread_safe_of_mutex<map_type>();
	}

#if BOOST_VERSION < 105600
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef
			yggr::safe_container::safe_map
			<
				int, val_type,
				boost::mutex,
				std::less<int>,
				std::allocator< std::pair<const int, val_type> >,
				boost::container::map
			> map_type;
		test_thread_safe_of_mutex<map_type>();
	}
#endif // BOOST_VERSION < 105600

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef
			yggr::safe_container::safe_map
			<
				int, val_type,
				boost::mutex,
				std::less<int>,
				std::allocator< std::pair<const int, val_type> >,
				yggr::map
			> map_type;
		test_thread_safe_of_mutex<map_type>();
	}

#if !(BOOST_VERSION < 105800)
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef
			yggr::safe_container::safe_map
			<
				int, val_type,
				boost::mutex,
				std::less<int>,
				boost::container::new_allocator< std::pair<const int, val_type> >,
				yggr::map
			> map_type;
		test_thread_safe_of_mutex<map_type>();
	}
#endif // !(BOOST_VERSION < 105800)

	// shared_mutex
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_map<int, val_type, boost::shared_mutex> map_type;
		test_thread_safe_of_shared_mutex<map_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef
			yggr::safe_container::safe_map
			<
				int, val_type,
	            boost::shared_mutex,
	            std::less<int>,
	            std::allocator<std::pair<const int, val_type> >,
	            std::map
			> map_type;
		test_thread_safe_of_shared_mutex<map_type>();
	}

#if BOOST_VERSION < 105600
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef
			yggr::safe_container::safe_map
			<
				int, val_type,
				boost::shared_mutex,
				std::less<int>,
				std::allocator< std::pair<const int, val_type> >,
				boost::container::map
			> map_type;
		test_thread_safe_of_shared_mutex<map_type>();
	}
#endif // BOOST_VERSION < 105600

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef
			yggr::safe_container::safe_map
			<
				int, val_type,
				boost::shared_mutex,
				std::less<int>,
				std::allocator< std::pair<const int, val_type> >,
				yggr::map
			> map_type;
		test_thread_safe_of_shared_mutex<map_type>();
	}

#if !(BOOST_VERSION < 105800)
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef
			yggr::safe_container::safe_map
			<
				int, val_type,
				boost::shared_mutex,
				std::less<int>,
				boost::container::new_allocator< std::pair<const int, val_type> >,
				yggr::map
			> map_type;
		test_thread_safe_of_shared_mutex<map_type>();
	}
#endif // !(BOOST_VERSION < 105800)

	wait_any_key(argc, argv);
	return 0;
}
