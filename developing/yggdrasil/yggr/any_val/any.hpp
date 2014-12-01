// any.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_ANY_VAL_ANY_HPP__
#define __YGGR_ANY_VAL_ANY_HPP__

#include <boost/any.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/friend.hpp>
#include <yggr/serialization/nvp.hpp>

namespace yggr
{
	class any
	{
	public: // structors

		any()
			: content(0)
		{
		}

		template<typename ValueType>
		any(const ValueType & value)
			: content(new holder<ValueType>(value))
		{
		}

		any(const any & other)
			: content(other.content ? other.content->clone() : 0)
		{
		}

		~any()
		{
			delete content;
		}

	public: // modifiers

		any & swap(any & rhs)
		{
			if(this == &rhs)
			{
				return *this;
			}
			std::swap(content, rhs.content);
			return *this;
		}

		template<typename ValueType>
		any & operator=(const ValueType & rhs)
		{
			any(rhs).swap(*this);
			return *this;
		}

		any & operator=(any rhs)
		{
			if(this == &rhs)
			{
				return *this;
			}
			rhs.swap(*this);
			return *this;
		}

	public: // queries

		bool empty() const
		{
			return !content;
		}

		const std::type_info & type() const
		{
			return content ? content->type() : typeid(void);
		}

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
	private: // types
#else
	public: // types (public so any_cast can be non-friend)
#endif

		class placeholder
		{
		public: // structors

			virtual ~placeholder()
			{
			}

		public: // queries

			virtual const std::type_info & type() const = 0;

			virtual placeholder * clone() const = 0;

		private:
			friend class boost::serialization::access;
			template<typename Archive>
			void serialize(Archive& ar, const u32 version) {}
		};

	public:
		template<typename ValueType>
		class holder : public placeholder
		{
		private:
			typedef placeholder base_type;

		public: // structors

			holder(void)
			{
			}

			holder(const ValueType & value)
				: held(value)
			{
			}

			virtual ~holder(void)
			{
			}

		public: // queries

			virtual const std::type_info & type() const
			{
				return typeid(ValueType);
			}

			virtual placeholder * clone() const
			{
				return new holder(held);
			}

		private:
			friend class boost::serialization::access;
			template<typename Archive>
			void serialize(Archive& ar, const u32 version)
			{
				ar & YGGR_SERIALIZE_NAME_NVP("placeholder", boost::serialization::base_object< base_type >(*this));
				ar & YGGR_SERIALIZE_NVP(held);
			}

		public: // representation

			ValueType held;

		private: // intentionally left unimplemented
			holder & operator=(const holder &);
		};

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS

	private: // representation

		template<typename ValueType, typename Any>
		friend ValueType * any_cast(Any *);

		template<typename ValueType, typename Any>
		friend ValueType * unsafe_any_cast(Any *);

#else

	public: // representation (public so any_cast can be non-friend)

#endif

		placeholder * content;

	private:
		friend class boost::serialization::access;
		template<typename Archive>
		void serialize(Archive& ar, const u32 version)
		{
			ar & YGGR_SERIALIZE_NVP(content);
		}

	};

	class bad_any_cast : public std::bad_cast
	{
	public:
		virtual const char * what() const throw()
		{
			return "yggr::bad_any_cast: "
					"failed conversion using yggr::any_cast";
		}
	};

	template<typename ValueType, typename Any>
	ValueType * any_cast(Any * operand)
	{
		typedef Any any_type;
		return operand &&
#ifdef BOOST_AUX_ANY_TYPE_ID_NAME
			std::strcmp(operand->type().name(), typeid(ValueType).name()) == 0
#else
			operand->type() == typeid(ValueType)
#endif
			? &static_cast<typename any_type::template holder<ValueType> *>(operand->content)->held
			: 0;
	}

	template<typename ValueType>
	inline ValueType * any_cast(boost::any * operand)
	{
		return boost::any_cast<ValueType>(operand);
	}

	template<typename ValueType, typename Any>
	inline const ValueType * any_cast(const Any * operand)
	{
		typedef Any any_type;
		return any_cast<ValueType>(const_cast<any_type *>(operand));
	}

	template<typename ValueType, typename Any>
	ValueType any_cast(Any & operand)
	{
		typedef BOOST_DEDUCED_TYPENAME boost::remove_reference<ValueType>::type nonref;

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
		// If 'nonref' is still reference type, it means the user has not
		// specialized 'remove_reference'.

		// Please use BOOST_BROKEN_COMPILER_TYPE_TRAITS_SPECIALIZATION macro
		// to generate specialization of remove_reference for your class
		// See type traits library documentation for details
		BOOST_STATIC_ASSERT(!boost::is_reference<nonref>::value);
#endif

		nonref * result = any_cast<nonref, Any>(&operand);
		if(!result)
			boost::throw_exception(bad_any_cast());
		return *result;
	}

	template<typename ValueType>
	inline ValueType any_cast(boost::any & operand)
	{
		return boost::any_cast<ValueType>(operand);
	}

	template<typename ValueType, typename Any>
	inline ValueType any_cast(const Any & operand)
	{
		typedef Any any_type;
		typedef BOOST_DEDUCED_TYPENAME boost::remove_reference<ValueType>::type nonref;

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
		// The comment in the above version of 'any_cast' explains when this
		// assert is fired and what to do.
		BOOST_STATIC_ASSERT(!boost::is_reference<nonref>::value);
#endif

		return any_cast<const nonref &>(const_cast<any_type &>(operand));
	}

	template<typename ValueType>
	inline ValueType any_cast(const boost::any & operand)
	{
		return boost::any_cast<ValueType>(operand);
	}

	// Note: The "unsafe" versions of any_cast are not part of the
	// public interface and may be removed at any time. They are
	// required where we know what type is stored in the any and can't
	// use typeid() comparison, e.g., when our types may travel across
	// different shared libraries.
	template<typename ValueType, typename Any>
	inline ValueType * unsafe_any_cast(Any * operand)
	{
		typedef Any any_type;
		return &static_cast<typename any_type::template holder<ValueType> *>(operand->content)->held;
	}

	template<typename ValueType>
	inline ValueType * unsafe_any_cast(boost::any * operand)
	{
		return boost::unsafe_any_cast<ValueType>(operand);
	}

	template<typename ValueType, typename Any>
	inline const ValueType * unsafe_any_cast(const Any * operand)
	{
		typedef Any any_type;
		return unsafe_any_cast<ValueType>(const_cast<any_type *>(operand));
	}

} // namespace yggr

#endif //__YGGR_ANY_VAL_ANY_HPP__
