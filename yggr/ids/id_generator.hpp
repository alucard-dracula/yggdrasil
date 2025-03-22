//id_generator.hpp

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

#ifndef __YGGR_IDS_GENERATOR_HPP__
#define __YGGR_IDS_GENERATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/random.h>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/utility/cast_pointer_to_int.hpp>

#include <ctime>
#include <memory>

namespace yggr
{
namespace ids
{

template<typename ID, typename Base_Generator = void>
class id_generator
	: private nonable::noncopyable
{
public:
	typedef ID id_type;

private:
	typedef Base_Generator base_genner_type;
	typedef id_generator this_type;

public:
	id_generator(const base_genner_type& genner)
		: _genner(genner)
	{
	}

	id_generator(const id_generator& right)
		: _genner(right._genner)
	{
	}

	~id_generator(void)
	{
	}

public:
	inline id_type operator()(void) const
	{
		return _genner();
	}

private:
	base_genner_type _genner;
};

template<typename ID, typename Base_Generator> inline
id_generator<ID, Base_Generator>
	create_id_generator(const Base_Generator& base_genner)
{
	typedef ID id_type;
	typedef Base_Generator base_genner_type;
	typedef id_generator<id_type, base_genner_type> id_generator_type;

	return id_generator_type(base_genner);
}

template<typename ID> inline
id_generator<ID, void> 
	create_id_generator(void)
{
	typedef ID id_type;
	typedef id_generator<id_type, void> id_generator_type;

	return id_generator_type();
}

template<typename ID = mplex::pointer_int::type>
struct pointer_id_generator
{
public:
	typedef ID id_type;

public:
	template<typename T> inline
	static id_type gen(const ::yggr::shared_ptr<T>& ptr)
	{
		return utility::cast_pointer_to_int::cast<id_type>(ptr.get());
	}

	template<typename T> inline
	static id_type gen(const T* ptr)
	{
		return utility::cast_pointer_to_int::cast<id_type>(ptr);
	}

	template<typename T> inline
	id_type operator()(const ::yggr::shared_ptr<T>& ptr) const
	{
		return utility::cast_pointer_to_int::cast<id_type>(ptr.get());
	}

	template<typename T> inline
	id_type operator()(const T* ptr) const
	{
		return utility::cast_pointer_to_int::cast<id_type>(ptr);
	}
};


} // namespace ids
} // namespace yggr

#endif // __YGGR_ID_GENERATOR_HPP__
