//service_handler_shared_wrap.hpp

#ifndef __YGGR_NETWORK_SERVICE_HANDLER_SHARED_WRAP_HPP__
#define __YGGR_NETWORK_SERVICE_HANDLER_SHARED_WRAP_HPP__

#include <boost/shared_ptr.hpp>
#include <yggr/move/move.hpp>

namespace yggr
{
namespace network
{

template<typename Service_Handler>
class service_handler_wrap
{
public:
	typedef Service_Handler service_handler_type;
	typedef boost::shared_ptr<service_handler_type> service_handler_ptr_type;
	typedef service_handler_ptr_type init_type;

	typedef typename service_handler_type::signal_set_type signal_set_type;
	typedef typename service_handler_type::service_type service_type;
	typedef typename service_handler_type::io_service_pool_type io_service_pool_type;
	
private:
	typedef service_handler_wrap this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	service_handler_wrap(const service_handler_ptr_type& psh)
		: _psh(psh)
	{
		assert(_psh);
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	service_handler_wrap(BOOST_RV_REF(this_type) right)
		: _psh(boost::forward<service_handler_ptr_type>(right._psh))
	{
	}
#else
	service_handler_wrap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		_psh.swap(righr_ref._psh);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	service_handler_wrap(const this_type& right)
		: _psh(right._psh)
	{
	}

	~service_handler_wrap(void)
	{
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}
		_psh = right._psh;

		return *this;
	}

	io_service_pool_type& service_pool(void)
	{
		assert(_psh);
		return _psh->service_pool();
	}

	signal_set_type& signal_set(void)
	{
		assert(_psh);
		return _psh->signal_set();
	}

	service_type& get_service(void)
	{
		assert(_psh);
		return _psh->get_service();
	}

	void add_signal(u32 sig)
	{
		assert(_psh);
		_psh->add_signal(sig);
	}

	void join(void)
	{
		assert(_psh);
		_psh->join();
	}

	template<typename Handler>
	void set_signal_handler(const Handler& handler)
	{
		assert(_psh);
		_psh->set_signal_handler(handler);
	}

	bool is_running(void) const
	{
		assert(_psh);
		return _psh->is_running();
	}

	void run(void)
	{
		assert(_psh);
		_psh->run();
	}

	void stop(void)
	{
		assert(_psh);
		_psh->stop();
	}

private:
	service_handler_ptr_type _psh;
};

} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_SERVICE_HANDLER_SHARED_WRAP_HPP__