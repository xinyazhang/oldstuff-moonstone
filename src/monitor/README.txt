Monitor daemon/service (used daemon below) placed here.

Concepts:
	
	1. Partition
		A ``Partition'' here refers a file system built on any block devices
		(partitions on both static or dynamic disks, LVM volumes, and loop back
		devices, CD, DVD, or Blu-ray)

	2. Mount Point
		Like the definition in *nix.
		IMPORTANT: Windows also have the feature of mount point.

	3. Path
		The term has two meanings

		For the daemon's interface to other processes, the path is just like
		the path used by any user, no any other additional information should
		be required.

		For the internal usage of the daemon, the ``Path'' refers a
		combination of the directory relative to the root of the file system
		it relies on, and the Partition.

	4. Partition Locator
		The ``Locator'' refers any information may help in locating (both in
		the computer system and in the real world) a partition. It may
		contains (UPDATE THE LIST IF THERE BE MORE):
			- The device information, say:
				Device Model:     WDC WD20EURS-63S48Y0
				Serial Number:    WD-WCAZA5064261
			- The device path, say:
				/dev/sda1
				(WARNING: IT MAY CHANGES IF ADDITIONAL SATA/SCSI DEVICE IS
				INSTALLED)
				/dev/mapper/vg_lvroot
			- The partition label, say:
				Windows
				Ubuntu 10.04
				

Data Files:

	The daemon keeps following lists in files:
		- a list of known partitions and their locators
			* This list is also kept by UI program, the duplication is made
			  for r/w confliction
		- a list of tracing paths
	, and maintain following tables in memory
		- Mounted Partition

	The daemon write following data into journal:
		- File modifications in tracing paths
		- New discovered partitions and their locators

		NOTE: the daemon only keeps the journal

IPCs:
	
	The daemon provides following services:
		- Journal file file descriptor(or, file handle) request
			+ returns fd and offset
		- Add/Remove tracing paths requests

Monitors(UNIX):
	
	The daemon monitors following things:
		- fanotify on all tracing paths
		- fanotify on device changes (HOW?)

Monitors(WIN32):

	The service monitors following things:
		- All NTFS journals on which there be one path at least
		- RegisterDeviceNotification for SERVICE_CONTROL_DEVICEEVENT
		  notifications, to trace new discovered partitions

Initialization:

	- daemonize [unix]
	- load known partitions and locators
	- scan online partitions
		+ see /proc/self/mountinfo, or use libmount [linux]
		+ see MSDN: "Enumerating Volumes (Windows)" [win32]
	- RegisterDeviceNotification [win32]
	- load tracing paths
	- Create hooks (and thread) to record file modifications
		+ see fanotify [linux]
		+ see FSCTL_ENUM_USN_DATA and FSCTL_READ_USN_JOURNAL
	- Create IPC to provide service
		+ Unix Domain Socket[unix]
		+ Named Pipe [win32]
		+ A ``accept'' thread is required

Service Procedure:
	- Accept connection
		+ Unix Domain Socket is straightforward
		+ win32's named pipe is not like unix's, it provides connection alike
		methods
	- Frame and serialization.
		- Boost is good enough.
		- But boost (and other impl.s) don't provide the framing and object
		  type information, so a [msgtype][size][objid] header is required...
	- fd IPC
		- UDS provides the facility
		- Named Pipe require DuplicateHandle before send the Handle..
