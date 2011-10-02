#ifndef IPC_PACKET_TYPE_H
#define IPC_PACKET_TYPE_H

enum IPC_PACKET_TYPE
{
	/***** Underlying function messages *****/

	/* Send file descriptor accross processes */
	PT_WIN32_HANDLE = 0xED320001, /* rEserveD win32 */
	PT_LINUX_FD	= 0xED110001,	/* rEserveD llnux */

	/**** Local IPC requests to privileged process *****/
	/* NOTE: PR_PRIV_* should never be used in network processing */
	/* ALSO: No platform mark, requests are delivered into P.D. routines */
	PT_PRIV_JOURNAL_REQUEST = 0x15DC0001, /* pr1Vileged, Don't Care platforms */
	PT_PRIV_JOURNAL_ACK = 0x15DC0002, /* Journal processed */
	PT_PRIV_VOL_LIST_REQ = 0x15DC0003, /* Command to list seen or known partitions */
	PT_PRIV_VOL_LIST_REP = 0x15DC0004, /* Reply the seen or known partitions */
	PT_PRIV_TRACING_LIST_REQ = 0x15DC0005,
	PT_PRIV_TRACING_LIST_REP = 0x15DC0006,

	PT_PRIV_WIN32_VOLUME_HANDLE_REQ = 0x15320100, /* (UUID) */
	PT_PRIV_WIN32_VOLUME_HANDLE_REP = 0x15320101, /* (FD) */
	PT_PRIV_WIN32_VOLUME_HANDLE_ERR = 0x15320102, /* (FD) */
	PT_PRIV_WIN32_NTFS_TRACING_REPORT = 0x153201F1,

	PT_PRIV_LINUX_ADD_FILTERED_PATH = 0x15110100, /* (Path, cookie) (report) */
	PT_PRIV_LINUX_REMOVE_FILTERED_PATH = 0x15110101, /* (Path, cookie) (report) */
	PT_PRIV_LINUX_FILTERED_PATH_REPORT = 0x151101F1, /* (cookie, result) */
	PT_PRIV_LINUX_RECV_CHANGES = 0x15110102, /* () (FD) */

	/**** Basic IPC *****/
	PT_FIN = 0xBADC00FF, /* BAsic, Dont Care, Final Fin */

	PT_DEFINED_END
};

#endif
