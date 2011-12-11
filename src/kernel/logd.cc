#include <stdio.h>
#include <pal/xtdio.h>
#include "feedback.h"
#include "logd.h"

static int logd_dump(void* cookie, feedback_event le)
{
	logd* plogd = (logd*)cookie;
	return plogd->dump(le);
}

logd::logd()
	:eventd(log())
{
	journal = xfopen("LOG.TXT", XTDIO_WRITE_TEXT_FLAG);
	if (!journal) {
		log().printf(LOG_ERROR, UT("COULD NOT OPEN LOG FILE\n"));
	}
	if (ftell(journal) == 0) {
		fwrite("\xFF\xFE", 2, 1, journal);
	}
	register_receiver(logd_dump, this);
}

logd::~logd()
{
	FILE* todel = journal;
	journal = NULL;
	fclose(todel);
}

int logd::dump(feedback_event le)
{
	if (le.evid >= LOG_FATAL && le.evid <= LOG_DEBUG) {
		/* print immediately */
		if (journal)
			xfprintf(journal, UT("%s"), (unichar*)le.payload.get());
	}
}
