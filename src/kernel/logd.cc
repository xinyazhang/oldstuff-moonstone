#include <stdio.h>
#include <pal/xtdio.h>
#include "feedback.h"
#include "logd.h"

logd::logd()
{
	journal = xfopen("LOG.TXT", XTDIO_WRITE_TEXT_FLAG);
	if (!journal) {
		log().printf(LOG_ERROR, UT("COULD NOT OPEN LOG FILE\n"));
	}
	if (ftell(journal) == 0) {
		fwrite("\xFF\xFE", 2, 1, journal);
	}
	tg_ = globaltp()->allocate_threads(1, this);
}

logd::~logd()
{
	log().printf(TASKLET_QUIT, UT(""));
	delete tg_;
}

int logd::tp_working()
{
	while(true) {
		feedback_event le = log().next();
		if (le.evid >= LOG_FATAL && le.evid <= LOG_DEBUG) {
			/* print immediately */
			xfprintf(journal, UT("%s"), (unichar*)le.payload.get());
		}
	}
}
