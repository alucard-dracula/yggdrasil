//static_mongodb_io_system.hpp

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_STATIC_MONGODB_IO_SYSTEM__
#define __YGGR_NSQL_DATABASE_SYSTEM_STATIC_MONGODB_IO_SYSTEM__

#include <yggr/base/yggrdef.h>
#include <yggr/nsql_database_system/mongo_accesser.hpp>
#include <boost/noncopyable.hpp>
#include <yggr/packet/packet.hpp>
#include <yggr/archive/bson_archive_partner.hpp>
#include <boost/any.hpp>
#include <boost/unordered_map.hpp>
#include <yggr/base/interface_ptr.hpp>
#include <yggr/thread/boost_thread_config.hpp>
#include <list>
#include <yggr/safe_struct/safe_buffered_object.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include <yggr/base/ctrl_center.hpp>

namespace yggr
{
namespace nsql_database_system
{

template<typename Connect, typename Sync_Mgr,
			typename Mutex = boost::mutex,
			typename Thread_Config = thread::boost_thread_config_type,
			template<typename _Connect, typename _Mutex> class Mongodb_Accesser = mongo_accesser>
class static_mongodb_io_system : protected Sync_Mgr, private boost::noncopyable
{
private:
	typedef Connect conn_type;
	//typedef typename conn_type::c_mongo_connection_ptr_type conn_ptr_type;
	//typedef typename conn_type::init_type init_type;
	typedef Mutex mutex_type;

	typedef Mongodb_Accesser<conn_type, mutex_type> accesser_type;

private:

	typedef Sync_Mgr sync_mgr_type;
	typedef sync_mgr_type base_type;
	typedef typename sync_mgr_type::helper_type helper_type;

	typedef Thread_Config thread_config_type;
	typedef typename thread_config_type::thread_type thread_type;
	typedef typename thread_config_type::thread_group_type thread_group_type;
	typedef typename thread_config_type::time_type time_type;

public:
	typedef typename thread_config_type::time_type time_type;

public:
	typedef class tag_init_sig
	{
	public:
		tag_init_sig(const std::string& nhost_port, size_type ntrd_count = 1, 
						const time_type& ntm_step = time_type(thread_config_type::E_def_sleep_time_sec, 
															thread_config_type::E_def_sleep_time_nsec))
			: host_port(nhost_port), trd_count(ntrd_count), tm_step(ntm_step)
		{
		}

		tag_init_sig(const tag_init_sig& right)
			: host_port(right.host_port), trd_count(right.trd_count), tm_step(right.tm_step)
		{
		}

		~tag_init_sig(void)
		{
		}

		std::string host_port;
		size_type trd_count;
		time_type tm_step;

	} init_sig_type;

	typedef class tag_init_sig_pwd
	{
	public:
		tag_init_sig_pwd(const std::string& nhost_port, 
							const std::string& nuser, const std::string& npwd,
							size_type ntrd_count = 1,
							const time_type& ntm_step = time_type(thread_config_type::E_def_sleep_time_sec, 
															thread_config_type::E_def_sleep_time_nsec))
			: host_port(nhost_port), user(nuser), pwd(npwd), trd_count(ntrd_count), tm_step(ntm_step)
		{
		}

		tag_init_sig_pwd(const tag_init_sig_pwd& right)
			: host_port(right.host_port), 
				user(right.user), pwd(right.pwd),
				trd_count(right.trd_count)
		{
		}

		~tag_init_sig_pwd(void)
		{
		}

		std::string host_port;
		std::string user;
		std::string pwd;
		size_type trd_count;
		time_type tm_step;

	} init_sig_pwd_type;

	typedef class tag_init_pair
	{
	public:
		tag_init_pair(const std::string& nhost_port1, const std::string& nhost_port2, 
						size_type ntrd_count = 1, 
						const time_type& ntm_step = time_type(thread_config_type::E_def_sleep_time_sec, 
															thread_config_type::E_def_sleep_time_nsec))
			: host_port1(nhost_port1), host_port2(nhost_port2), trd_count(ntrd_count), tm_step(ntm_step)
		{
		}

		tag_init_pair(const tag_init_pair& right)
			: host_port1(right.host_port1), host_port2(right.host_port2), 
				trd_count(right.trd_count)

		{
		}

		~tag_init_pair(void)
		{
		}

		std::string host_port1;
		std::string host_port2;
		size_type trd_count;
		time_type tm_step;

	} init_pair_type;

	typedef class tag_init_pair_pwd
	{
	public:
		tag_init_pair_pwd(const std::string& nhost_port1, const std::string& nhost_port2,
							const std::string& nuser, const std::string& npwd, size_type ntrd_count = 1,
							const time_type& ntm_step = time_type(thread_config_type::E_def_sleep_time_sec, 
															thread_config_type::E_def_sleep_time_nsec))
			: host_port1(nhost_port1), host_port2(nhost_port2), user(nuser), pwd(npwd),
				trd_count(ntrd_count), tm_step(ntm_step)
		{
		}

		tag_init_pair_pwd(const tag_init_pair_pwd& right)
			: host_port1(right.host_port1), host_port2(right.host_port2), 
			user(right.user), pwd(right.pwd), trd_count(right.trd_count)
		{
		}

		~tag_init_pair_pwd(void)
		{
		}

		std::string str_host_port1;
		std::string str_host_port2;
		std::string user;
		std::string pwd;
		size_type trd_count;
		time_type tm_step;

	} init_pair_pwd_type;

public:
	typedef typename sync_mgr_type::key_type key_type;

private:
	template<typename Base_Container>
	class database_container : public Base_Container
	{
	public:
		typedef Base_Container base_type;
	private:
		typedef	database_container this_type;

	public:
		database_container(accesser_type& accesser)
			: _accesser(accesser)
		{
		}

		template<typename Init>
		database_container(accesser_type& accesser, const Init& init)
			: base_type(init), _accesser(accesser)
		{
		}

		database_container(const this_type& right)
			: base_type(right), _accesser(right._accesser)
		{
		}

		~database_container(void)
		{
		}

		bool operator<<(helper_type& helper)
		{
			return base_type::save(_accesser, helper);
		}

		bool operator<<(helper_type& helper) const
		{
			return base_type::save(_accesser, helper);
		}

		bool operator>>(helper_type& helper)
		{
			return base_type::load(_accesser, helper);
		}

		bool operator>>(helper_type& helper) const
		{
			return base_type::load(_accesser, helper);
		}

		/*bool execute(void)
		{
			return base_type::execute(_accesser);
		}

		bool execute(void) const
		{
			return base_type::execute(_accesser);
		}*/

		bool execute(const helper_type& in_helper, helper_type& out_helper)
		{
			return base_type::execute(_accesser, in_helper, out_helper);
		}

		bool execute(const helper_type& in_helper, helper_type& out_helper) const
		{
			return base_type::execute(_accesser, in_helper, out_helper);
		}

	private:
		accesser_type& _accesser;
	};

	class base_back_handler_container
	{
	public:
		virtual ~base_back_handler_container(void)
		{
		}

		virtual void back(helper_type& helper) = 0;
	};

	typedef interface_ptr<base_back_handler_container> back_handler_container_ptr_type;

	template<typename Back_Val, typename Handler>
	class back_handler_container : public base_back_handler_container
	{
	public:
		typedef Back_Val back_val_type;
		typedef Handler handler_type;

	public:
		back_handler_container(const Handler& handler)
			: _handler(handler)
		{
		}

		virtual ~back_handler_container(void)
		{
		}

		virtual void back(helper_type& helper)
		{
			back_val_type val;
			val << helper;
			_handler(val);
		}

	private:
		handler_type _handler;
	};

	template<typename Command>
	class command_container
	{
	public:
		typedef Command command_type;
		typedef std::list<command_type> list_type;

	public:
		command_container(void)
		{
		}

		command_container(const command_container& right)
			: _list(right._list)
		{
		}

		~command_container(void)
		{
			clear();
		}

		command_container& operator=(const command_container& right)
		{
			_list = right._list;
			return *this;
		}

		void splice_to(command_container& right)
		{
			_list.splice(_list.end(), right._list);
			_list.swap(right._list);
		}

		void insert(const command_type& cmd)
		{
			_list.push_back(cmd);
		}

		template<typename Handler>
		void get(const Handler& handler)
		{
			handler(_list);
		}

		yggr::size_type size(void) const
		{
			return _list.size();
		}

		void clear(void)
		{
			list_type list;
			_list.swap(list);
			list.clear();
		}

		bool empty(void) const
		{
			return _list.empty();
		}

	private:
		list_type _list;
	};


	typedef key_type remove_command_type;
	typedef command_container<remove_command_type> remove_command_container_type;
	typedef typename remove_command_container_type::list_type remove_command_list_type;
	typedef typename remove_command_list_type::iterator remove_command_list_iter_type;
	typedef typename remove_command_list_type::const_iterator remove_command_list_citer_type;
	typedef safe_struct::safe_buffered_object<remove_command_container_type> remove_command_queue_type;

						// key    // input_val  // output foo
	typedef boost::tuple<key_type, helper_type, back_handler_container_ptr_type> command_type;
	typedef command_container<command_type> command_container_type;
	typedef command_container_type send_command_container_type;
	typedef typename command_container_type::list_type command_list_type;
	typedef command_list_type send_command_list_type;
	typedef typename command_list_type::iterator command_list_iter_type;
	typedef command_list_iter_type send_command_list_iter_type;
	typedef typename command_list_type::const_iterator command_list_citer_type;
	typedef command_list_citer_type send_command_list_citer_type;

	typedef safe_struct::safe_buffered_object<command_container_type> send_command_queue_type;
	typedef safe_struct::safe_buffered_object<command_container_type> command_queue_type;

	typedef boost::tuple<helper_type, back_handler_container_ptr_type> all_command_type;
	typedef command_container<all_command_type> all_command_container_type;
	typedef typename all_command_container_type::list_type all_command_container_list_type;
	typedef typename all_command_container_list_type::iterator all_command_container_list_iter_type;
	typedef typename all_command_container_list_type::const_iterator all_command_container_list_citer_type;
	typedef safe_struct::safe_buffered_object<all_command_container_type> all_command_queue_type;

	typedef static_mongodb_io_system this_type;

private:
	friend class system_controller::ctrl_center;
	template<typename Ctrl_Center>
	void register_controller(Ctrl_Center& cc)
	{
		_accesser.register_controller(cc);
	}
public:

	static_mongodb_io_system(void)
		: _brun(false), _trd_count(1), _tm_step(thread_config_type::E_def_sleep_time_sec,
														thread_config_type::E_def_sleep_time_nsec)
	{
	}

	static_mongodb_io_system(size_type trd_count,
						const time_type& tm_step = time_type(thread_config_type::E_def_sleep_time_sec, 
															thread_config_type::E_def_sleep_time_nsec))
		: _brun(false), _trd_count(trd_count), _tm_step(tm_step)
	{
	}

	static_mongodb_io_system(const std::string& str, size_type trd_count = 1, 
						const time_type& tm_step = time_type(thread_config_type::E_def_sleep_time_sec, 
															thread_config_type::E_def_sleep_time_nsec))
		: _brun(false), _accesser(str), _trd_count(trd_count), _tm_step(tm_step)
	{
	}

	static_mongodb_io_system(const init_sig_type& init)
		: _brun(false), _accesser(init.host_port), _trd_count(init.trd_count),
			_tm_step(init.tm_step)
	{
	}

	static_mongodb_io_system(const init_sig_pwd_type& init)
		: _brun(false), _accesser(init.host_port, init.user, init.pwd), 
			//_user(init.user), _pwd(init.pwd),
			_trd_count(init.trd_count),
			_tm_step(init.tm_step)
	{
	}

	static_mongodb_io_system(const init_pair_type& init)
		: _brun(false), _accesser(init.host_port1, init.host_port2),
		_trd_count(init.trd_count), _tm_step(init.tm_step)
	{
	}

	static_mongodb_io_system(const init_pair_pwd_type& init)
		: _brun(false), _accesser(init.host_port1, init.host_port2, init.user, init.pwd),
		_trd_count(init.trd_count), _tm_step(init.tm_step)
	{
	}

	static_mongodb_io_system(const std::string& str_host, u16 port, 
						size_type trd_count = 1, 
						const time_type& tm_step = time_type(thread_config_type::E_def_sleep_time_sec, 
															thread_config_type::E_def_sleep_time_nsec))
		: _brun(false), _accesser(str_host, port), _trd_count(trd_count), _tm_step(tm_step)
	{
	}

	static_mongodb_io_system(const std::string& str1, const std::string& str2, size_type trd_count = 1,
						const time_type& tm_step = time_type(thread_config_type::E_def_sleep_time_sec, 
															thread_config_type::E_def_sleep_time_nsec))
		: _brun(false), _accesser(str1, str2), _trd_count(trd_count), _tm_step(tm_step)
	{
	}

	static_mongodb_io_system(const std::pair<std::string, std::string>& init)
		: _brun(false), _accesser(init.first, init.second), _trd_count(init.trd_count),
		_tm_step(init.tm_step)
	{
	}

	static_mongodb_io_system(const std::string& str_host1, u16 port1, 
						const std::string& str_host2, u16 port2, size_type trd_count = 1,
						const time_type& tm_step = time_type(thread_config_type::E_def_sleep_time_sec, 
															thread_config_type::E_def_sleep_time_nsec))
		: _brun(false), _accesser(str_host1, port1, str_host2, port2), _trd_count(trd_count),
		_tm_step(init.tm_step)
	{
	}

	//template<typename Tuple>
	//static_mongodb_io_system(const Tuple& tuple)
	//	: _brun(false), _accesser(boost::str), _user(user), _pwd(pwd), _
	//{
	//}

	static_mongodb_io_system(const std::string& str, const std::string& user, const std::string& pwd, size_type trd_count = 1,
						const time_type& tm_step = time_type(thread_config_type::E_def_sleep_time_sec, 
															thread_config_type::E_def_sleep_time_nsec))
		: _brun(false), _accesser(str, user, pwd), _trd_count(trd_count), _tm_step(tm_step)
	{
	}

	static_mongodb_io_system(const std::string& str_host, u16 port, const std::string& user, 
						const std::string& pwd, size_type trd_count = 1,
						const time_type& tm_step = time_type(thread_config_type::E_def_sleep_time_sec, 
															thread_config_type::E_def_sleep_time_nsec))
		: _brun(false), _accesser(str_host, port, user, pwd), 
		_trd_count(trd_count), _tm_step(tm_step)
	{
	}

	static_mongodb_io_system(const std::string& str1, const std::string& str2, 
						const std::string& user, const std::string& pwd, size_type trd_count = 1,
						const time_type& tm_step = time_type(thread_config_type::E_def_sleep_time_sec, 
															thread_config_type::E_def_sleep_time_nsec))
		: _brun(false), _accesser(str1, str2, user, pwd), _trd_count(trd_count),
		_tm_step(tm_step)
	{
	}

	static_mongodb_io_system(const std::pair<std::string, std::string>& init, 
						const std::string& user, const std::string& pwd, size_type trd_count = 1,
						const time_type& tm_step = time_type(thread_config_type::E_def_sleep_time_sec, 
															thread_config_type::E_def_sleep_time_nsec))
		: _brun(false), _accesser(init.first, init.second, user, pwd), 
		_trd_count(trd_count), _tm_step(tm_step)
	{
	}

	static_mongodb_io_system(const std::string& str_host1, u16 port1, const std::string& str_host2, u16 port2, 
						const std::string& user, const std::string& pwd, size_type trd_count = 1,
						const time_type& tm_step = time_type(thread_config_type::E_def_sleep_time_sec, 
															thread_config_type::E_def_sleep_time_nsec))
		: _brun(false), _accesser(str_host1, port1, str_host2, port2, user, pwd), 
		_trd_count(trd_count), _tm_step(tm_step)
	{
	}


	~static_mongodb_io_system(void)
	{
		stop();
		//join();
	}

	void start(void)
	{
		if(_brun)
		{
			return;
		}
		_brun = true;

		for(size_type i = 0; i != _trd_count; ++i)
		{
			if((!_trd_group.create_thread(boost::bind(&this_type::run, this))) 
				|| (! _trd_group.create_thread(boost::bind(&this_type::run_load, this)))  )
			{
				_brun = false;
				break;
			}
		}
	}

	void join(void)
	{
		_trd_group.join_all();
	}

	void stop(void)
	{
		if(!_brun)
		{
			return;
		}

		_brun = false;
		join();
	}

	template<typename Base_Container>
	bool reg_container(void)
	{
		typedef Base_Container base_container_type;
		typedef database_container<base_container_type> database_container_type;
		typedef boost::shared_ptr<database_container_type> database_container_ptr_type;

		database_container_ptr_type ptr(new database_container_type(_accesser));

		return base_type::template reg_container<database_container_type>(base_container_type::type_id(), ptr);
	}

	template<typename Base_Container>
	bool reg_container(const key_type& key)
	{
		typedef Base_Container base_container_type;
		typedef database_container<base_container_type> database_container_type;
		typedef boost::shared_ptr<database_container_type> database_container_ptr_type;

		database_container_ptr_type ptr(new database_container_type(_accesser));
		return base_type::template reg_container<database_container_type>(key, ptr);
	}

	template<typename Base_Container, typename Init>
	bool reg_container(const Init& init)
	{
		typedef Base_Container base_container_type;
		typedef database_container<base_container_type> database_container_type;
		typedef boost::shared_ptr<database_container_type> database_container_ptr_type;

		database_container_ptr_type ptr(new database_container_type(_accesser, init));
		return base_type::template reg_container<database_container_type>(base_container_type::type_id(), ptr);
	}

	template<typename Base_Container, typename Init>
	bool reg_container(const key_type& key, const Init& init)
	{
		typedef Base_Container base_container_type;
		typedef database_container<base_container_type> database_container_type;
		typedef boost::shared_ptr<database_container_type> database_container_ptr_type;

		database_container_ptr_type ptr(new database_container_type(_accesser, init));
		return base_type::template reg_container<database_container_type>(key, ptr);
	}

	bool remove_container(const key_type& key)
	{
		return _remove_cmd_queue.insert(key);
	}

	template<typename Val>
	bool save(const key_type& key, const Val& val)
	{
		helper_type helper(val);
		return _save_cmd_queue.insert(boost::make_tuple(key, helper, back_handler_container_ptr_type()));
	}

	template<typename Condition, typename Val>
	bool save(const key_type& key, const Condition& cdt, const Val& val)
	{
		helper_type helper(cdt, val);
		return _save_cmd_queue.insert(boost::make_tuple(key, helper, back_handler_container_ptr_type()));
	}

	template<typename Back_Val, typename Val, typename Handler>
	bool save(const key_type& key, const Val& val, const Handler& handler)
	{
		typedef Back_Val back_val_type;
		typedef Handler handler_type;
		typedef back_handler_container<back_val_type, handler_type> back_handler_container_type;

		helper_type helper(val);
		back_handler_container_ptr_type ptr(new back_handler_container_type(handler));
		return _save_cmd_queue.insert(boost::make_tuple(key, helper, ptr));
	}

	template<typename Back_Val, typename Condition, typename Val, typename Handler>
	bool save(const key_type& key, const Condition& cdt, const Val& val, const Handler& handler)
	{
		typedef Back_Val back_val_type;
		typedef Handler handler_type;
		typedef back_handler_container<back_val_type, handler_type> back_handler_container_type;

		helper_type helper(cdt, val);
		back_handler_container_ptr_type ptr(new back_handler_container_type(handler));
		return _save_cmd_queue.insert(boost::make_tuple(key, helper, ptr));
	}

	template<typename Back_Val, typename Handler>
	bool load(const key_type& key, const Handler& handler)
	{
		typedef Back_Val back_val_type;
		typedef Handler handler_type;
		typedef back_handler_container<back_val_type, handler_type> back_handler_container_type;

		back_handler_container_ptr_type ptr(new back_handler_container_type(handler));
		return _load_cmd_queue.insert(boost::make_tuple(key, helper_type(), ptr));
	}

	template<typename Back_Val, typename Condition, typename Handler>
	bool load(const key_type& key, const Condition& cdt, const Handler& handler)
	{
		typedef Back_Val back_val_type;
		typedef Handler handler_type;
		typedef back_handler_container<back_val_type,handler_type> back_handler_container_type;

		helper_type helper(cdt);
		back_handler_container_ptr_type ptr(new back_handler_container_type(handler));
		return _load_cmd_queue.insert(boost::make_tuple(key, helper, ptr));
	}

	bool commit(const key_type& key)
	{
		return _execute_cmd_queue.insert(boost::make_tuple(key, helper_type(), back_handler_container_ptr_type()));
	}

	template<typename Condition>
	bool commit(const key_type& key, const Condition& cdt)
	{
		helper_type helper(cdt);
		return _execute_cmd_queue.insert(boost::make_tuple(key, helper, back_handler_container_ptr_type()));
	}

	template<typename Back_Val, typename Condition, typename Handler>
	bool commit(const key_type& key, const Condition& cdt, const Handler& handler)
	{
		typedef Back_Val back_val_type;
		typedef Handler handler_type;
		typedef back_handler_container<back_val_type, handler_type> back_handler_container_type;

		helper_type helper(cdt);
		back_handler_container_ptr_type ptr(new back_handler_container_type(handler));
		return _execute_cmd_queue.insert(boost::make_tuple(key, helper, ptr));
	}

	bool commit_all(void)
	{
		return _execute_all_cmd_queue.insert(boost::make_tuple(helper_type(), back_handler_container_ptr_type()));
	}

	template<typename Condition>
	bool commit_all(const Condition& cdt)
	{
		helper_type helper(cdt);
		return _execute_all_cmd_queue.insert(boost::make_tuple(helper, back_handler_container_ptr_type()));
	}

	template<typename Back_Val, typename Condition, typename Handler>
	bool commit_all(const Condition& cdt, const Handler& handler)
	{
		typedef Back_Val back_val_type;
		typedef Handler handler_type;
		typedef back_handler_container<back_val_type, handler_type> back_handler_container_type;

		helper_type helper(cdt);
		back_handler_container_ptr_type ptr(new back_handler_container_type(handler));
		return _execute_all_cmd_queue.insert(boost::make_tuple(helper, ptr));
	}

	void clear(void)
	{
		_save_cmd_queue.clear();
		_load_cmd_queue.clear();
		_execute_cmd_queue.clear();
		_remove_cmd_queue.clear();

		base_type::clear();
	}

private:

	bool run_remove_command(void)
	{
		return run_remove_command_helper(boost::bind(&this_type::handler_run_remove_command, this, _1));
	}

	template<typename Handler>
	bool run_remove_command_helper(const Handler& handler)
	{
		typedef Handler handler_type;
		return _remove_cmd_queue.get(boost::bind(&remove_command_container_type::template get<handler_type>,
                                                    _1, boost::cref(handler)));
	}

	void handler_run_remove_command(remove_command_list_type& list)
	{
		for(remove_command_list_iter_type i = list.begin(), isize = list.end(); i != isize; ++i)
		{
			base_type::unreg_container(*i);
		}

		list.clear();
	}

	bool run_save_command(void)
	{
		return run_save_command_helper(boost::bind(&this_type::handler_run_save_command, this, _1));
	}

	template<typename Handler>
	bool run_save_command_helper(const Handler& handler)
	{
		typedef Handler handler_type;
		return _save_cmd_queue.get(boost::bind(&send_command_container_type::template get<handler_type>,
                                                _1, boost::cref(handler)));
	}

	void handler_run_save_command(send_command_list_type& list)
	{
		for(send_command_list_iter_type i = list.begin(), isize = list.end(); i != isize; ++i)
		{
			if(base_type::send(boost::get<0>(*i), boost::get<1>(*i)) && boost::get<2>(*i))
			{
				(boost::get<2>(*i))->back(boost::get<1>(*i));
			}
		}

		list.clear();
	}

	bool run_load_command(void)
	{
		return run_load_command_helper(boost::bind(&this_type::handler_run_load_command, this, _1));
	}

	template<typename Handler>
	bool run_load_command_helper(const Handler& handler)
	{
		typedef Handler handler_type;
		return _load_cmd_queue.get(boost::bind(&command_container_type::template get<handler_type>,
                                                _1, boost::cref(handler)));
	}

	void handler_run_load_command(command_list_type& list)
	{
		for(command_list_iter_type i = list.begin(), isize = list.end(); i != isize; ++i)
		{
			if(base_type::recv(boost::get<0>(*i), boost::get<1>(*i)) && boost::get<2>(*i))
			{
				(boost::get<2>(*i))->back(boost::get<1>(*i));
			}
		}

		list.clear();
	}

	bool run_execute_command(void)
	{
		return run_execute_command_helper(boost::bind(&this_type::handler_run_execute_command, this, _1));
	}

	template<typename Handler>
	bool run_execute_command_helper(const Handler& handler)
	{
		typedef Handler handler_type;
		return _execute_cmd_queue.get(boost::bind(&command_container_type::template get<handler_type>,
                                                    _1, boost::cref(handler)));
	}

	void handler_run_execute_command(command_list_type& list)
	{
		for(command_list_iter_type i = list.begin(), isize = list.end(); i != isize; ++i)
		{
			helper_type helper;
			if(base_type::execute(boost::get<0>(*i), boost::get<1>(*i), helper) && boost::get<2>(*i))
			{
				(boost::get<2>(*i))->back(helper);
			}
		}

		list.clear();
	}

	bool run_all_execute_command(void)
	{
		return run_all_execute_command_helper(boost::bind(&this_type::handler_run_all_execute_command_helper, this, _1));
	}

	template<typename Handler>
	bool run_all_execute_command_helper(const Handler& handler)
	{
		typedef Handler handler_type;
		return _execute_all_cmd_queue.get(boost::bind(&all_command_container_type::template get<handler_type>,
                                                        _1, boost::cref(handler)));
	}

	void handler_run_all_execute_command_helper(all_command_container_list_type& list)
	{
		for(all_command_container_list_iter_type i = list.begin(), isize = list.end(); i != isize; ++i)
		{
			helper_type helper;
			if(base_type::execute_all(boost::get<0>(*i), helper) && boost::get<1>(*i))
			{
				(boost::get<1>(*i))->back(helper);
			}
		}

		list.clear();
	}

	void run(void)
	{
		if(!connect())
		{
			_brun = false;
			return;
		}

		bool bfree = false;
		for(;_brun;)
		{
			bfree = false;
			//bfree |= run_reg_command();
			bfree = run_save_command() || bfree;
			//bfree = run_load_command() || bfree;
			bfree = run_execute_command() || bfree;
			bfree = run_all_execute_command() || bfree;
			bfree = run_remove_command() || bfree;

			if(!bfree)
			{
				thread_type::yield();
				time_type tm;
				tm += _tm_step;
				thread_type::sleep(tm);
			}
			//else
			//{
			//	int nnn = 0;
			//}
		}
	}

	void run_load(void)
	{
		if(!connect())
		{
			_brun = false;
			return;
		}

		bool bfree = false;
		for(;_brun;)
		{
			bfree = false;
			//bfree |= run_reg_command();
			//bfree = run_save_command() || bfree;
			bfree = run_load_command() || bfree;
			//bfree = run_execute_command() || bfree;
			//bfree = run_all_execute_command() || bfree;
			//bfree = run_remove_command() || bfree;

			if(!bfree)
			{
				thread_type::yield();
				time_type tm;
				tm += _tm_step;
				thread_type::sleep(tm);
			}
		/*	else
			{
				int nnn = 0;
			}*/
		}
	}

	bool connect(void)
	{	
		//return _accesser.connect() && (_user.empty() ||  _accesser.cmd_authenticate(std::string("admin"), _user, _pwd));
		return _accesser.connect();
	}

private:

	bool _brun;
	send_command_queue_type _save_cmd_queue;
	command_queue_type _load_cmd_queue;
	command_queue_type _execute_cmd_queue;
	all_command_queue_type _execute_all_cmd_queue;
	//reg_command_queue_type _reg_cmd_queue;
	remove_command_queue_type _remove_cmd_queue;
	accesser_type _accesser;
	thread_group_type _trd_group;
	size_type _trd_count;
	time_type _tm_step;
	//std::string _user;
	//std::string _pwd;
	
};

} // nsql_database_system
} // namespace yggr

#endif //__YGGR_NSQL_DATABASE_SYSTEM_STATIC_MONGODB_IO_SYSTEM__


