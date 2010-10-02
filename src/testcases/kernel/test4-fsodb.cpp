#include "testKernels.h"
#include "../../kernel/common.h"
#include "../../dal/db_sqlite_impl.h"

void TestKernels::test700_daily_usage()
{
	/*
	 * Higest level API, used for daily life.
	 * 	However, it's nearly improssible to write a good unit test program.
	 */

	/*
	 * Register notifier
	 * 	Make plugin system could accept change notification from FSO sub-system
	 */
	fsodb_->register_notifier(pluginsys_);

	wq_->redirect_fsodb(fsodb_); //wq: copyer, mover, checksumer and other works which consume much time.
	// although wq could access fsodbman from db_, but we'd better add a redirect function for test

	pluginsys_->redirect_fsodb(fsodbman_); //same as wq_, redirect is necessary for test
	pluginsys_->set_wq(wq_); // this is necessary, mv/cp/ftp shall be done in wq.

	modsource_->redirect_fsodb(fsodbman_);
	modsource_->refresh(); // we can't test timer, so we test this...
}
/*
 * Things to do:
 * 1. Mod Source Subsystem:
 * 	this subsystem accept all high level modification from user and detect all modification from system.
 * 	In GUI, Add a directory or file to the system File System Database should use ModSource rather than FsodbMan
 *
 * 	ModSource have several works:
 * 	+ accept high level request "add directory recursively", read file system and convert to many
 * 		"add directory/file to directory" operations on FsodbMan
 * 	+ accept high level request "move/copy directory" and send request to Working queue "move/copy"
 * 		, and add a watching point(monitor process would handle it for us, nixixi)
 * 	+ watching directory modifications(using NTFS Journal or inotify), and synchronize with database.
 * 		- need a plugin system?
 * 		- load plugin
 * 			* plugin initialize several data structures for management
 * 			* plugin starts a thread to reading NTFS
 * 			* post events back to main thread
 * 			* a plugin should provides a shutdown function
 * 	+ accept high level request "refresh all", read file system and convert to many operations
 * 		- subdirectories in "recursive" directory shall be added also;
 * 		- file should be added/removed on any kind of directory.
 *
 * 	Warning: ModSource won't invoke Tagger PluginSys, it's done in FsodbMan, because
 * 	there may be other subsys would invoke FsodbMan to edit FSO db.
 *
 * 2. Plugin
 * 	This subsystem loads plugins and execute, currently we only considering about auto-tagger plugin.
 * 	When FsoMan subsystem trigger an add/delete/move actions
 *
 * 	must take placer into consideration, where the plugin can be placed?
 * 	What about the API?
 * 		- post message ( TYPE(MV,CP,...), path1, path2(optional), FLAG)
 * 		- send message (synchronized)
 * 		- call interface to tagging system, generally speaking the plugin should not 
 * 			take tag db into consideration, it should just give out tag names and attribute out
 * 			such as:
 * 				* tag splited from file name or path name
 * 				* bitrate
 * 		- call interface to working queue system, consider introduce tasklet system.
 *
 * 	Howto invoke?
 * 		- auto-tagger can be invoked from hook in FsodbMan::add
 * 		- auto-classifier can be invoked in working queue, as a timed tasklet
 * 		- manual invoke auto-plugin from ui event.
 *
 * 	Python interpreter can be delayed.
 * 	
 * 3. FsodbMan should be rewritten as overwrittable
 * 	FsodbMan should know nothing about working queue as it only work on database
 */
