//tool_of_skill_table.cpp

#include <iostream>
#include <string>
#include <base/yggrdef.h>
#include <regular_parse/regex_parse.hpp>
#include <list>
#include "text_parser.hpp"
#include <map>
#include <sstream>

#include <tinyxml.h>
#include <boost/shared_ptr.hpp>
#include <fstream>

#ifdef _DEBUG
#	pragma comment(lib, "tinyxmld.lib")
#else
#	pragma comment(lib, "tinyxml.lib") 
#endif // _DEBUG


class sp_align;
class s_group;
class skill_anim;

std::ostream& operator << (std::ostream& os, const std::vector<std::string> str_vt)
{
	yggr::size_type isize = str_vt.size();
	for(yggr::size_type i = 0; i != isize; ++i)
	{
		os << str_vt[i] << (i+1 != isize)?", ":"";
	}

	return os;
}

class sp_align
{
public:
	typedef std::string string_type;
	typedef std::vector<string_type> from_type;

	typedef std::map<const string_type, string_type> str_map_type;

public:
	sp_align(void)
	{
	}

	sp_align(const sp_align& right)
		: _id(right._id), _sp(right._sp), _pp(right._pp)
	{
	}

	~sp_align(void)
	{
	}

	const sp_align& operator=(const sp_align& right)
	{
		_id = right._id;
		_sp = right._sp;
		_pp = right._pp;
		return *this;
	}

	sp_align& operator << (const from_type& from)
	{
		if(from.size() != 3)
		{
			std::cerr << "sp_align err from: \n" << from << std::endl;
			return *this;
		}

		_id = from[0];
		//_sp = from[1];
		//_pp = from[2];
		_sp = str_map[from[1]];
		_pp = str_map[from[2]];
		return *this;
	}

	//bool fail(void) const
	//{
	//	return _id.empty() || _sp.empty() || _pp.empty();
	//}

public:
	string_type _id;
	string_type _sp;
	string_type _pp;

	static str_map_type str_map;
};

sp_align::str_map_type sp_align::str_map;

typedef std::map<const std::string, sp_align> sp_align_map_type;

class sp_align_table
{
public:
	typedef std::string string_type;
	typedef std::vector<string_type> from_type;

public:
	sp_align_table(void)
	{
	}

	~sp_align_table(void)
	{
		_sp_map.clear();
	}

	sp_align_table& operator<<(const from_type& from)
	{
		if(from.size() != 3)
		{
			std::cerr << "sp_align_table err from: \n" << from << std::endl;
			return *this;
		}

		_sp_map[from[0]] << from;
		return *this;
	}

public:
	sp_align_map_type _sp_map;
};

class s_group
{
public:
	typedef std::stringstream sstream_type;
	typedef std::string string_type;
	typedef std::vector<string_type> from_type;
public:
	s_group(void)
	{
	}

	s_group(const s_group& right)
		: _aid(right._aid), _sframe(right._sframe), _eframe(right._eframe), _stime(right._stime), _etime(right._etime)
	{
	}

	~s_group(void)
	{
	}

	const s_group& operator=(const s_group& right)
	{
		_aid = right._aid;
		_sframe = right._sframe;
		_eframe = right._eframe;
		_stime = right._stime;
		_etime = right._etime;

		return *this;
	}

	bool operator<(const s_group& right) const
	{
		sstream_type ss1, ss2;
		yggr::u32 s1, s2;

		ss1 << _stime;
		ss1 >> s1;
		ss2 << right._stime;
		ss2 >> s2;

		return s1 < s2;
	}

	s_group& operator<<(const from_type& from)
	{
		if(from.size() != 5)
		{
			std::cerr << "s_group_err from: \n" << from << std::endl;
			return *this;
		}

		_aid = from[0];
		_sframe = from[1];
		_eframe = from[2];
		_stime = from[3];
		_etime = from[4];

		return *this;
	}

	bool fail(void) const
	{
		return _aid.empty() || _sframe.empty() || _eframe.empty() || _stime.empty() || _etime.empty() || _sp.empty() || _pp.empty();
	}

	void merge_sp_align(sp_align_map_type& spmap)
	{
		_sp = spmap[_aid]._sp;
		_pp = spmap[_aid]._pp;
	}

	void write_xml(boost::shared_ptr<TiXmlElement>& proot_elm)
	{
		typedef boost::shared_ptr<TiXmlElement> elm_ptr_type;

		if(fail())
		{
			std::cerr << "s_group write xml err id =" << _aid << std::endl;
			return;
		}

		elm_ptr_type pnow(new elm_ptr_type::value_type("ani"));
		pnow->SetAttribute("actid", _aid.c_str());
		pnow->SetAttribute("sframe", _sframe.c_str());
		pnow->SetAttribute("eframe", _eframe.c_str());
		pnow->SetAttribute("stime", _stime.c_str());
		pnow->SetAttribute("etime", _etime.c_str());
		pnow->SetAttribute("spos", _sp.c_str());
		pnow->SetAttribute("ppos", _pp.c_str());

		proot_elm->InsertEndChild(*pnow);
	}

public:
	string_type _aid;
	string_type _sframe;
	string_type _eframe;
	string_type _stime;
	string_type _etime;
	string_type _sp;
	string_type _pp;
};

class skill_anim
{
public:
	typedef std::string string_type;
	typedef std::vector<string_type> from_type;
	typedef std::vector<s_group> s_group_vt_type;
public:
	
	skill_anim(void)
	{
	}

	skill_anim(const skill_anim& right)
		: _sid(right._sid), _gvt(right._gvt)
	{
	}

	~skill_anim(void)
	{
		_gvt.clear();
	}

	const skill_anim& operator=(const skill_anim& right)
	{
		_sid = right._sid;
		_gvt = right._gvt;

		return * this;
	}

	skill_anim& operator<<(const from_type& from)
	{
		if(from.size() != 6)
		{
			std::cerr << "skill_anim err from: \n" << from << std::endl;
			return *this;
		}

		if(_sid.empty())
		{
			_sid = from[0];
		}

		from_type tfrom(from.begin() + 1, from.end());
		s_group g;
		g << tfrom;
		_gvt.push_back(g);

		return *this;
	}

	bool fail(void) const
	{
		return _sid.empty() || _gvt.empty();
	}

	void merge_sp_align(sp_align_map_type& spmap)
	{
		yggr::size_type isize = _gvt.size();
		for(yggr::size_type i = 0; i != isize; ++i)
		{
			_gvt[i].merge_sp_align(spmap);
		}
	}

	void write_xml(boost::shared_ptr<TiXmlElement>& proot_elm)
	{
		typedef boost::shared_ptr<TiXmlElement> elm_ptr_type;

		std::sort(_gvt.begin(), _gvt.end());
		if(fail())
		{
			std::cerr << "skill_anim write xml err id = " << _sid << std::endl;
			return;
		}

		elm_ptr_type pnow(new elm_ptr_type::value_type("skill_anim"));
		pnow->SetAttribute("id", _sid.c_str());

		yggr::size_type isize = _gvt.size();
		for(yggr::size_type i = 0; i != isize; ++i)
		{
			_gvt[i].write_xml(pnow);
		}

		proot_elm->InsertEndChild(*pnow);
	}
public:
	string_type _sid;
	s_group_vt_type _gvt;
};

typedef std::map<const std::string, skill_anim> skill_anim_map_type;

class skill_anim_table
{
public:
	typedef std::string string_type;
	typedef std::vector<string_type> from_type;
public:
	skill_anim_table(void)
	{
	}

	~skill_anim_table(void)
	{
		_map.clear();
	}

	skill_anim_table& operator<<(const from_type& from)
	{
		if(from.size() != 6)
		{
			std::cerr << "skill anim table err from: \n" << from << std::endl;
			return *this;
		}
		
		/*if(_map.find(from[0]) != _map.end())
		{
			std::cerr << "now skill anim id redefined -- id =" << from[0] << std::endl; 
		}*/
		_map[from[0]] << from;

		return *this;
	}

	void merge_sp_align(sp_align_map_type& spmap)
	{
		skill_anim_map_type::iterator isize = _map.end();
		
		for(skill_anim_map_type::iterator i = _map.begin(); i != isize; ++i)
		{
			i->second.merge_sp_align(spmap);
		}
	}

	void write_xml(const std::string& fname)
	{
		typedef boost::shared_ptr<TiXmlElement> elm_ptr_type;
		
		TiXmlDocument xmldoc(fname.c_str()); 
		TiXmlDeclaration decl("1.0","utf-8", ""); 
		xmldoc.InsertEndChild(decl);

		elm_ptr_type pnow(new elm_ptr_type::value_type("skill_anim_list"));

		skill_anim_map_type::iterator isize = _map.end();
		
		for(skill_anim_map_type::iterator i = _map.begin(); i != isize; ++i)
		{
			i->second.write_xml(pnow);
		}

		xmldoc.InsertEndChild(*pnow);

		xmldoc.SaveFile();
	}
public:
	skill_anim_map_type _map;
};

class skill_rule
{
public:
	typedef std::string string_type;
	typedef std::vector<string_type> from_type;
public:
	skill_rule(void)
	{
	}

	skill_rule(const skill_rule& right)
		: _act(right._act), _dact(right._dact)
	{
	}

	~skill_rule(void)
	{
	}

	const skill_rule& operator=(const skill_rule& right)
	{
		_act = right._act;
		_dact = right._dact;
		return *this;
	}

	skill_rule& operator<<(const from_type& from)
	{
		if(from.size() != 2)
		{
			std::cerr << "skill rule err from: \n" << from << std::endl;
			return *this;
		}
		
		_act = from[0];
		_dact = from[1];

		return *this;
	}

	bool fail(void) const
	{
		return _act.empty() || _dact.empty();
	}

	bool check(const skill_anim_map_type& sam) const
	{
		//return false;
		return (sam.find(_act) != sam.end()) && (sam.find(_dact) != sam.end()); 
	}

	void write_xml(boost::shared_ptr<TiXmlElement>& proot_elm)
	{
		typedef boost::shared_ptr<TiXmlElement> elm_ptr_type;

		if(fail())
		{
			std::cerr << "skill rule write xml err" << std::endl;
			return;
		}

		elm_ptr_type pnow(new elm_ptr_type::value_type("skill_rule"));
		pnow->SetAttribute("act", _act.c_str());
		pnow->SetAttribute("dact", _dact.c_str());

		proot_elm->InsertEndChild(*pnow);
	}

public:
	string_type _act;
	string_type _dact;
};

class skill_aspect
{
public:
	typedef std::string string_type;
	typedef std::vector<string_type> from_type;
public:
	skill_aspect(void)
	{
	}

	skill_aspect(const skill_aspect& right)
		: _id(right._id), _sr(right._sr)
	{
	}

	~skill_aspect(void)
	{
	}
	
	const skill_aspect& operator=(const skill_aspect& right)
	{
		_id = right._id;
		_sr = right._sr;
		return *this;
	}

	skill_aspect& operator<<(const from_type& from)
	{
		if(from.size() != 3)
		{
			std::cerr << "skill aspect err from: \n" << from << std::endl;
			return *this;
		}

		_id = from[2];

		from_type tfrom(from.begin(), from.end() - 1);

		_sr << tfrom;

		return *this;
	}

	bool fail(void) const
	{
		return _id.empty() || _sr.fail();
	}

	bool check(const skill_anim_map_type& sam) const
	{
		return _sr.check(sam);
	}

	void write_xml(boost::shared_ptr<TiXmlElement>& proot_elm)
	{
		typedef boost::shared_ptr<TiXmlElement> elm_ptr_type;

		if(fail())
		{
			std::cerr << "skill aspect write xml err id = " << _id << std::endl;
			return;
		}

		elm_ptr_type pnow(new elm_ptr_type::value_type("skill_rule"));
		pnow->SetAttribute("id", _id.c_str());
		_sr.write_xml(pnow);

		proot_elm->InsertEndChild(*pnow);
	}

public:
	string_type _id;
	skill_rule _sr;
};

class skill
{
public:
	typedef std::string string_type;
	typedef std::vector<string_type> from_type;
public:
	skill(void)
	{
	}

	skill(const skill& right)
		: _id(right._id), _lsa(right._lsa), _rsa(right._rsa)
	{
	}

	~skill(void)
	{
	}
	
	const skill& operator=(const skill& right)
	{
		_id = right._id;
		_lsa = right._lsa;
		_rsa = right._rsa;
		return *this;
	}

	skill& operator<<(const from_type& from)
	{
		if(from.size() != 5)
		{
			std::cerr << "skill err from: \n" << from << std::endl;
			return *this;
		}

		_id = from[0];

		from_type lfrom(from.begin() + 1, from.begin() + 3);
		from_type rfrom(from.begin() + 3, from.end());

		lfrom.push_back("0");
		rfrom.push_back("1");

		_lsa << lfrom;
		_rsa << rfrom;

		return *this;
	}

	bool fail(void) const
	{
		return _id.empty() || _lsa.fail() || _rsa.fail();
	}

	bool check(const skill_anim_map_type& sam) const
	{
		if(_lsa.check(sam) && _rsa.check(sam))
		{
			
			return true;
		}
		else
		{
			std::cerr << "check err now skill anim id not exist -- skill id = " << _id << std::endl;
		}
		return false;
	}

	void write_xml(boost::shared_ptr<TiXmlElement>& proot_elm)
	{
		typedef boost::shared_ptr<TiXmlElement> elm_ptr_type;

		if(fail())
		{
			std::cerr << "skill write xml err id = " << _id << std::endl;
			return;
		}

		elm_ptr_type pnow(new elm_ptr_type::value_type("skill"));
		pnow->SetAttribute("id", _id.c_str());
		_lsa.write_xml(pnow);
		_rsa.write_xml(pnow);

		proot_elm->InsertEndChild(*pnow);
	}

public:
	string_type _id;
	skill_aspect _lsa;
	skill_aspect _rsa;
};

typedef std::map<const std::string, skill> skill_map_type;

class skill_table
{
public:
	typedef std::string string_type;
	typedef std::vector<string_type> from_type;
public:
	skill_table(void)
	{
	}

	~skill_table(void)
	{
	}

	skill_table& operator<<(const from_type& from)
	{
		if(from.size() != 5)
		{
			std::cerr << "skill table err from: \n" << from << std::endl;
			return *this;
		}

		if(_map.find(from[0]) != _map.end())
		{
			std::cerr << "skill id redefined -- id = " << from[0] << std::endl;
			return *this;
		}

		_map[from[0]] << from;

		return *this;
	}

	bool check(const skill_anim_map_type& sam) const
	{
		skill_map_type::const_iterator isize = _map.end();

		for(skill_map_type::const_iterator i = _map.begin(); i != isize; ++i)
		{
			if(!(i->second.check(sam)))
			{
				return false;
			}
		}

		return true;
	}

	void write_xml(const std::string& fname)
	{
		typedef boost::shared_ptr<TiXmlElement> elm_ptr_type;
		
		TiXmlDocument xmldoc(fname.c_str()); 
		TiXmlDeclaration decl("1.0","utf-8", ""); 
		xmldoc.InsertEndChild(decl);

		elm_ptr_type pnow(new elm_ptr_type::value_type("skill_list"));

		skill_map_type::iterator isize = _map.end();
		
		for(skill_map_type::iterator i = _map.begin(); i != isize; ++i)
		{
			i->second.write_xml(pnow);
		}

		xmldoc.InsertEndChild(*pnow);

		xmldoc.SaveFile();
	}

public:

	skill_map_type _map;
};



sp_align_table spt;
skill_anim_table skat;
skill_table skt;

typedef text_parser<std::string,std::vector<std::string>, yggr::regular_parse::regex_parse> text_parse_type;

std::string fixinfname(const std::string& fname)
{
	
	if(fname.substr(fname.size() - 4, 4) != ".txt")
	{
		return fname+".txt";
	}
}

bool read_sp(const std::string& fname)
{
	text_parse_type parser("\\s+");
	std::ifstream ifs;
	ifs.open(fname.c_str());

	if(!ifs.good())
	{
		std::cerr << fname <<" io bad" << std::endl;
		return false;
	}

	char str[1024] = {0};
	ifs.getline(str, 1023, '\n');

	
	for(;!ifs.eof();)
	{
		ifs.getline(str, 1023);
		parser(spt, str);
	}

	ifs.close();

	return true;
}

bool read_skill_anim(const std::string& fname)
{
	text_parse_type parser("\\s+");
	std::ifstream ifs;
	ifs.open(fname.c_str());

	if(!ifs.good())
	{
		std::cerr << fname <<" io bad" << std::endl;
		return false;
	}

	char str[1024] = {0};
	ifs.getline(str, 1023, '\n');


	for(;!ifs.eof();)
	{
		ifs.getline(str, 1023);
		parser(skat, str);
	}

	ifs.close();

	return true;
}

void merge(void)
{
	skat.merge_sp_align(spt._sp_map);
}

bool read_skill(const std::string& fname)
{
	text_parse_type parser("\\s+");
	std::ifstream ifs;
	ifs.open(fname.c_str());

	if(!ifs.good())
	{
		std::cerr << fname <<" io bad" << std::endl;
		return false;
	}

	char str[1024] = {0};
	ifs.getline(str, 1023, '\n');


	for(;!ifs.eof();)
	{
		ifs.getline(str, 1023);
		parser(skt, str);
	}

	ifs.close();

	return true;
}

bool check(void)
{
	return skt.check(skat._map);
}


int main(int argc, char* argv[])
{
	///std::string tfname_sp, tfname_skill, tfname;
	//std::cin >> 
	sp_align::str_map["LT"] = "0";
	sp_align::str_map["MT"] = "1";
	sp_align::str_map["RT"] = "2";
	sp_align::str_map["LM"] = "3";
	sp_align::str_map["MM"] = "4";
	sp_align::str_map["RM"] = "5";
	sp_align::str_map["LB"] = "6";
	sp_align::str_map["MB"] = "7";
	sp_align::str_map["RB"] = "8";

	std::string fname_sp("sp_tab.txt"), fname_sak("skill_anim_tab.txt"), fname_sk("skill_tab.txt"), fname_skat_xml("skill_anim.xml"), fname_skt_xml("skill.xml");

	if(!read_sp(fname_sp))
	{
		goto end;
	}
	
	if(!read_skill_anim(fname_sak))
	{
		goto end;
	}
	
	merge();

	read_skill(fname_sk);

	if(!check())
	{
		goto end;
	}

	skat.write_xml(fname_skat_xml);
	skt.write_xml(fname_skt_xml);

end:
	std::cout << "end" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}