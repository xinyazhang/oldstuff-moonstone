/*
 * Log ``daemon'' which stores message of ring buffer(maybe)
 * to file
 */
#ifndef LOGD_H
#define LOGD_H

#include <stdio.h>
#include "eventd.h"

class EXPORT_TO_DLL logd
	:public eventd
{
public:
	logd();
	~logd();

	int dump(feedback_event);
private:
	volatile FILE* journal;
};

#endif
