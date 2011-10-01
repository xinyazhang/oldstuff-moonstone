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

	/**** Basic IPC *****/
	PT_FIN = 0xBADC00FF, /* BAsic, Dont Care, Final Fin */

	PT_DEFINED_COUNT
};

#endif
