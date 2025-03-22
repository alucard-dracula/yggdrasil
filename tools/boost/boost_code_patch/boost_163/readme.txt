file boost/multi_index/ranked_index.hpp // fix error C2385: ambiguous access of 'size' at indexed_by<..., ranked_index, ranked_index, ...> using boost_164 file
file boost/multi_index/detail/rnk_index_ops.hpp // fix error C2385: ambiguous access of 'size' at indexed_by<..., ranked_index, ranked_index, ...> using boost_164 file
file boost/archive/impl/basic_binary_iprimitive.ipp // line 118  #ifndef BOOST_NO_CWCHAR this should not be "load(char *)" and should be "load(wchar_t *)"
file boost/container/string.hpp // fix bug boost::container::string str = "abc"; str.replace(1, 1, str, 0, 3); assert(str != "aabcc") // not fix it yggr fixed
file boost/container/map.hpp // fix bug boost::container::map insert_or_assign hint version error C2664