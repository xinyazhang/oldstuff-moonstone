#ifndef DATABASE_ABSTRACT_LAYER_STATEMENT_INTERFACE_H
#define DATABASE_ABSTRACT_LAYER_STATEMENT_INTERFACE_H

#include "../pal/stdtype.h"

class EXPORT_TO_DLL sql_stmt_interface
{
public:
	~sql_stmt_interface() {}
	virtual void bind(int c) = 0; // bind void
	virtual void bind(int c, idx_t ) = 0;
	virtual void bind(int c, const unistr&) = 0;
	virtual void bind(int c, void*, int64_t size) = 0;
	virtual int execute() = 0;
	virtual bool step() = 0;
	virtual void col(int c, unistr& ) = 0;
	virtual void col(int c, idx_t& ) = 0;
};

#endif
