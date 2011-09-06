#ifndef PAL_PATH_CPP_H
#define PAL_PATH_CPP_H

#include "unistr.h"
#include <vector>
#include <kernel/fso.h>
extern "C"
{
#include "path.h"
};

std::vector<unistr> EXPORT_TO_DLL split_path(const unistr& path);
unistr EXPORT_TO_DLL agg_path(const std::vector<unistr>& path_list);
unistr EXPORT_TO_DLL abs_fullpath(const unistr&);
bool EXPORT_TO_DLL is_dir(const unistr&);
void EXPORT_TO_DLL fso_read_entry(fso_t&, dir_entry_data);

#endif
