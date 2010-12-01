#include "path.hpp"
extern "C"
{
#include "path.h"
};

using std::vector;

vector<unistr> split_path(const unistr& path)
{
	unistr elem;
	vector<unistr> ret;

	for(int i = remove_unc(path.c_str()); i < path.size(); i++)
	{
		if ( path[i] == UT('\\') || path[i] == UT('/') )
		{
			if ( !elem.empty() )
				ret.push_back(elem);
		} else
			elem.append(path[i]);
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
	unicode* buf = abspath(path.native());
	if ( buf )
	{
		unistr ret(buf);
		abspath_dispose(buf);
		return ret;
	} else
		return unistr();
}

vector<unistr> ls(const unistr& path) // ls all and not . and ..
{
	vector<unistr> ret;
	void* handle = open_dir(path.native());
	if ( !handle )
		goto bad_dir;

	unicode* buf = dir_buf(handle);
	if ( !buf )
		goto bad_buf;

	while ( dir_next(handle, buf) )
	{
		ret.push_back(unistr(buf));
	}
	close_dir(handle);

bad_buf:
bad_dir:
	return ret;
}
