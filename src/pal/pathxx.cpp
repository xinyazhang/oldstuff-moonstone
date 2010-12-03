#include "path.hpp"

using std::vector;

vector<unistr> split_path(const unistr& path)
{
	unistr elem;
	vector<unistr> ret;

	for(size_t i = remove_unc(path.c_str()); i < path.size(); i++)
	{
		if ( path[i] == UT('\\') || path[i] == UT('/') )
		{
			if ( !elem.empty() )
				ret.push_back(elem);
		} else
			elem.push_back(path[i]);
	}
	if ( !elem.empty() )
		ret.push_back(elem);
	return ret;
}

unistr agg_path(const vector<unistr>& path_list)
{
	if ( path_list.empty() )
		return unistr();
	unistr ret;
	vector<unistr>::const_iterator iter= path_list.end() - 1;
	/*
#ifdef _WIN32
	iter--; // skip null root
#endif
	*/
	for(;iter != path_list.begin(); iter--)
	{
		ret += *iter;
		ret += "/";
	}
	ret += path_list.front();

	return ret;
}

bool is_dir(const unistr& path)
{
	return ::isdir(path.native());
}

unistr abs_fullpath(const unistr& path)
{
	unichar* buf = abspath(path.native());
	if ( buf )
	{
		unistr ret(buf);
		abspath_dispose(buf);
		return ret;
	} else
		return unistr();
}

