#ifndef PAL_PATH_CPP_H
#define PAL_PATH_CPP_H

#include "unistr.h"
#include <vector>
#include <kernel/fso.h>
extern "C"
{
#include "path.h"
};

std::vector<unistr> split_path(const unistr& path);
unistr agg_path(const std::vector<unistr>& path_list);
unistr abs_fullpath(const unistr&);
bool is_dir(const unistr&);
void fso_read_entry(fso_t&, dir_entry_data);

#endif
