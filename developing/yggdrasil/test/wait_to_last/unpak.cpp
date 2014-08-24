//unpak.cpp

#include <iostream>
#include <yggr/base/yggrdef.h>
#include <yggr/packet/packet.hpp>
#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/network/network_packet.hpp>
#include <yggr/network/pak_back_id.hpp>
#include <yggr/encryption_tool/blowfish_tool.hpp>
#include <yggr/regular_parse/regex_parse.hpp>
#include <list>
#include <yggr/file_system/local_file_operator.hpp>
#include <utility>
#include <map>
#include <iomanip>
#include <boost/tuple/tuple.hpp>
#include <yggr/regular_parse/regex_parse.hpp>
#include <sstream>
#include <yggr/time/time.hpp>

typedef yggr::encryption_tool::blowfish_tool<> encryption_tool_type;

typedef yggr::network::network_packet::pak_back_id<yggr::u64, std::string> key_pak_type;

typedef yggr::network::network_packet::network_packet<yggr::archive::archive_partner::network_oarchive_partner> opak;
typedef yggr::network::network_packet::network_packet<yggr::archive::archive_partner::network_iarchive_partner> ipak;

const char* cfg_key = "cfg_key";
const char* cfg_val = "cfg_val";

const char* cfg_key_pak = "cfg_key_pak";
const char* cfg_key_unpak = "cfg_key_unpak";

const char* cfg_val_wpe = "cfg_val_wpe";
const char* cfg_val_normal = "cfg_val_normal";

const char* cfg_file = "cfg.txt";
const char* key_file = "key.txt";
const char* value_file = "value.txt";
//const char* value_dir = "./values";
//const char* open_value_dir = ".\\values";

const char* report_file = "report";
const char* help_str = "command: \r\n"
						"\t[\"setcfg\"] -- set config, cfg format (cfg_key = cfg_key_unpak | cfg_key_real) \r\n"
						"\t\tcfg_key_pak -- the key is paked state\r\n"
						"\t\tcfg_key_unpak    -- this key is unpaked state\r\n\r\n"
						"\t[\"setkey\"] -- set key data, key format support pak type and unpak type\r\n\r\n"
						"\t[\"setval\"] -- set value data, value format support normal type and wpe type\r\n\r\n"
						"\t[\"parse\"]  -- start parse \r\n\r\n"
						"\t[\"help\"] -- show help info \r\n";



class config
{
public:
	config(void)
	{
	}

	config(const std::string& nkey_cfg, const std::string& nval_cfg)
		: key_cfg(nkey_cfg), val_cfg(nval_cfg)
	{
	}

	config(const config& right)
		: key_cfg(right.key_cfg), val_cfg(right.val_cfg)
	{
	}

	~config(void)
	{
	}

	config& operator=(const config& right)
	{
		key_cfg = right.key_cfg;
		val_cfg = right.val_cfg;
		return *this;
	}

public:
	std::string key_cfg;
	std::string val_cfg;
};

yggr::u8 h_to_d(yggr::u8 h)
{
	if(h >= '0' && h <='9')
	{
		return h - '0';
	}

	if(h >= 'A' && h <= 'F')
	{
		return (h - 'A') + 10;
	}

	if(h >= 'a' && h <= 'f')
	{
		return (h - 'a') + 10;
	}

	return 0;
}

const std::string str_to_buf(const std::string& text)
{
	std::vector<std::string> vt;
	yggr::regular_parse::regex_parse::split(vt, text);

	std::string str(vt.size(), 0);

	for(yggr::size_type i = 0, isize = vt.size(); i != isize; ++i)
	{
		yggr::u8 l = h_to_d(vt[i][0]);
		l <<= 4;
		yggr::u8 r = h_to_d(vt[i][1]);
		r &= 0x0f;
		str[i] = l | r;
	}

	return str;
}

bool is_space(const std::string::value_type& val)
{
	return val == ' ';
}

std::pair<std::string, std::string> split_key_val(const std::string& str)
{
	std::string tstr(str);
	std::string::iterator new_end = std::remove_if(tstr.begin(), tstr.end(), is_space);
	tstr.erase(new_end, tstr.end());
	std::string str_re("=");

	std::vector<std::string> vt;
	yggr::regular_parse::regex_parse::split(vt, tstr, str_re);
	
	assert(vt.size() == 2);

	return std::pair<std::string, std::string>(vt[0], vt[1]);

}

const config get_cfg(const std::string& fname)
{
	config cfg;
	std::ifstream ifs;
	ifs.open(fname.c_str());

	for(;!ifs.eof();)
	{
		char tmp[1024] = {0};
		ifs.getline(tmp, 1023);
		std::pair<std::string, std::string> kv(split_key_val(tmp));
		if(kv.first == cfg_key)
		{
			cfg.key_cfg = kv.second;
			continue;
		}

		if(kv.first == cfg_val)
		{
			cfg.val_cfg = kv.second;
		}
	}
	ifs.close();
	return cfg;
}

const std::string get_key(const config& cfg, const std::string& fname)
{
	std::string t;
	yggr::file_system::local_file_operator_type::read_file_of_binary(fname, t);
	std::string buf(str_to_buf(t));


	if(cfg.key_cfg == cfg_key_pak)
	{
		yggr::u32 len = 0;
		memcpy(&len, &buf[0], sizeof(yggr::u32));
		len = ntohl(len);

		if(len != buf.size() - 4)
		{
			return std::string();
		}

		buf.erase(buf.begin(), buf.begin() + 4);
		ipak in(buf);

		if(in.data_id() != 0x00008002 || in.data_ver() != 0x0001)
		{
			return std::string();
		}
		key_pak_type key_pak;
		in.load(key_pak);

		return key_pak.key;
	}

	if(cfg.key_cfg == cfg_key_unpak)
	{
		return buf;
	}

	return std::string();
}



std::pair<yggr::u32, yggr::u16> get_id_ver_of_buf(const std::string& key, const std::string& nbuf)
{
	encryption_tool_type tool;
	tool.init(key);

	std::string buf(nbuf);

	if(buf.size() < 4)
	{
		std::cerr << "parse error" << std::endl;
		return std::pair<yggr::u32, yggr::u16>(0xeeeeeee1, 0xeee1);
	}

	yggr::u32 len = 0;
	memcpy(&len, &buf[0], sizeof(yggr::u32));
	len = ntohl(len);

	if(len != buf.size() - 4)
	{
		std::cerr << "parse error" << std::endl;
		return std::pair<yggr::u32, yggr::u16>(0xeeeeeee2, 0xeee2);
	}

	buf.erase(buf.begin(), buf.begin() + 4);

	ipak in(buf);
	tool.decrypt(in);
	return std::pair<yggr::u32, yggr::u16>(in.data_id(), in.data_ver());
}

bool handler_match_hex(bool bright, const boost::smatch& m, std::string& buf)
{
	if(!bright || m.size() != 3)
	{
		return false;
	}

	buf = str_to_buf(m[1]);
	

	return true;
}

std::string parse_hex_values(const std::string text)
{
	std::string buf;
	std::string str_re("[\\da-f]{4}\\s{2}(([\\da-f][\\da-f]\\s{1})+)\\s+.*");
	yggr::regular_parse::regex_parse::match(text, str_re,
															yggr::regular_parse::regex_parse::regex_type::icase
															| yggr::regular_parse::regex_parse::regex_type::normal,
															boost::match_default,
															boost::bind(handler_match_hex, _1, _2, boost::ref(buf)));
	return buf;
}

void get_all_pak(const std::string& fname, std::list<std::string>& bufs)
{
	bufs.clear();

	std::ifstream ifs;
	ifs.open(fname);

	std::string l, r;
	for(;!ifs.eof();)
	{
		char tstr[1024] = {0};
		ifs.getline(tstr, 1023);
		std::string lstr(tstr);
		if(lstr.empty())
		{
			//list.push_back(std::pair<std::string, std::string>(l, r));
			l += r;
			//l.clear();
			r.clear();
			continue;
		}

		//l += lstr;
		//l += "\r\n";

		std::string rstr(parse_hex_values(lstr));
		if(rstr.empty())
		{
			continue;
		}

		r += rstr;
	}

	if(!r.empty())
	{
		l += r;
	}
	ifs.close();

	for(yggr::size_type i = 0, isize = l.size(); i  < isize;)
	{
		yggr::u32 size = 0;
		memcpy(&size, &l[i], sizeof(yggr::u32));
		size = ntohl(size);
		yggr::u32 now_buf_size = 4 + size;

		if(i + now_buf_size > isize)
		{
			std::cout << "lenght error!!!" << std::endl;
			break;
		}
		std::string pak_buf(now_buf_size, 0);
		memcpy(&pak_buf[0], &l[i], now_buf_size);
		bufs.push_back(pak_buf);

		i += now_buf_size;
	}

}

std::string parse_wpe_file(const std::string& key, const std::string& fname)
{
	//typedef std::list<boost::tuple<yggr::u32, yggr::u16, std::string> > wpe_data_list_type;
	typedef std::list < std::string > list_type;


	//wpe_data_list_type wpe_data_list;
	list_type list;

	get_all_pak(fname, list);

	yggr::time::time tm;
	std::string time_file(tm.to_local_string("%Y-%m-%d-%H-%M-%S"));
	std::string full_file(std::string(report_file) + std::string("-") + time_file + std::string(".txt"));

	//yggr::file_system::local_file_operator_type::create_path(full_dir);

	std::string report;
	yggr::size_type idx = 1;
	for(list_type::const_iterator i = list.begin(), isize = list.end(); i != isize; ++i, ++idx)
	{
		std::pair<yggr::u32, yggr::u16> id_ver(get_id_ver_of_buf(key, (*i)));
		if((id_ver.first & 0xeeeeeee0) != 0xeeeeeee0)
		{
			std::stringstream ss;
			ss << idx;
			ss << std::hex;
			ss << "\t" << std::setfill('0')
					<< "0x"<< std::setw(8) << id_ver.first 
					<< "-" 
					<< "0x"<< std::setw(4) << id_ver.second << std::endl;
			ss << std::dec;

			/*std::stringstream ss;
			ss <<full_dir << "/" << idx 
				<< "-" 
				<< std::hex 
				<< std::setfill('0')
				<< "0x"<< std::setw(8) << id_ver.first
				<< "-"
				<< "0x"<< std::setw(4) << id_ver.second
				<< ".txt"
				<< std::endl;*/

			//yggr::file_system::local_file_operator_type::write_file_of_binary(ss.str(), (*i));
			report += ss.str();
			report += std::string("\r\n");
		}
	}

	yggr::file_system::local_file_operator_type::write_file_of_binary(full_file, report);
	//return std::string(open_value_dir) + std::string("\\") + time_dir; 
	return full_file;
}

std::pair<yggr::u32, yggr::u16> get_id_ver_of_normal(const std::string& key, const std::string& fname)
{
	std::string t;
	yggr::file_system::local_file_operator_type::read_file_of_binary(fname, t);
	std::string buf(str_to_buf(t));
	return get_id_ver_of_buf(key, buf);
}

void set_cfg(void)
{
	system((std::string("notepad ") + std::string(cfg_file)).c_str());
}

void set_key(void)
{
	system((std::string("notepad ") + std::string(key_file)).c_str());
}

void set_value(void)
{
	system((std::string("notepad ") + std::string(value_file)).c_str());
}

void parse_val(void)
{
	config cfg(get_cfg(cfg_file));
	std::string key(get_key(cfg, key_file));

	if(key.empty())
	{
		std::cerr << "key error" << std::endl;
		return;
	}

	if(cfg.val_cfg == cfg_val_normal)
	{
		std::pair<yggr::u32, yggr::u16> id_ver(get_id_ver_of_normal(key, value_file));

		std::cout << std::hex;
		std::cout << std::setfill('0')
					<< "0x"<< std::setw(8) << id_ver.first 
					<< "-" 
					<< "0x"<< std::setw(4) << id_ver.second << std::endl;
		std::cout << std::dec;

		return;
	}

	if(cfg.val_cfg == cfg_val_wpe)
	{
		std::string open_file(parse_wpe_file(key, value_file));
		system((std::string("notepad ") + open_file).c_str());
		return;
	}
}

void show_help(void)
{ 
	std::cout << help_str << std::endl;
}



void test_run(void)
{
	//std::string buf;
	//std::string text("0000  00 00 00 D0 87 A4 4E 86 57 5E 4A BD 0D 6B D1 31    ......N.W^J..k.1");
	////std::string str_re("([[\da-f]][[\da-f]]\s)+");
	//std::string str_re("[\\da-f]{4}\\s{2}(([\\da-f][\\da-f]\\s{1})+)\\s+.*");
	//std::cout << yggr::regular_parse::regex_parse::match(text, str_re,
	//														yggr::regular_parse::regex_parse::regex_type::icase
	//														| yggr::regular_parse::regex_parse::regex_type::normal,
	//														boost::match_default,
	//														boost::bind(handler_match_hex, _1, _2, boost::ref(buf)))
	//		<< std::endl;

	std::string text("0000  00 00 00 D0 87 A4 4E 86 57 5E 4A BD 0D 6B D1 31    ......N.W^J..k.1");
	std::string buf(parse_hex_values(text));
	int nnn = 0;
}

void real_run(void)
{
	typedef void(*cmd_ptr_type)(void);
	typedef std::map<std::string, cmd_ptr_type> cmd_map_type;
	typedef cmd_map_type::const_iterator cmd_map_citer_type;

	cmd_map_type cmd_map;
	cmd_map["setcfg"] = set_cfg;
	cmd_map["setkey"] = set_key;
	cmd_map["setval"] = set_value;
	cmd_map["parse"] = parse_val;
	cmd_map["help"] = show_help;
	
	std::string op;

	std::cout << "\t\t\t-----------Welcome-----------" << std::endl;
	std::cout << "\t\t\t-          unpak_1_00       -" << std::endl;
	std::cout << "\t\t\t-                           -" << std::endl;
	std::cout << "\t\t\t-  look info run [\"help\"]   -" << std::endl;
	std::cout << "\t\t\t-----------------------------" << std::endl;
	for(;;)
	{
		std::cout << ">";
		std::cin >> op;
		if(op == "exit")
		{
			break;
		}

		cmd_map_citer_type iter = cmd_map.find(op);
		if(iter == cmd_map.end())
		{
			std::cout << "bad command, show help text run [help] command" << std::endl;	
			continue;
		}

		(*(iter->second))();
	}
}



int main(int argc, char* argv)
{
	//test_run();
	real_run();

	//char cc = 0;
	//std::cin >> cc;

	return 0;
}