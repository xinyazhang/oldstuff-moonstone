#ifndef KERNEL_BINARY_LAYEROUT_H
#define KERNEL_BINARY_LAYEROUT_H

/*
 * Local Object Binary Layerout Type
 */
#define LOBLT_TAG		((uint8_t)1)
#define LOBLT_TNODE 	((uint8_t)2)
#define LOBLT_REL		((uint8_t)3)
#define LOBLT_FILE		((uint8_t)4)

#include "common_declare.h"
class QDataStream;

EXPORT_TO_DLL QDataStream& operator<< (QDataStream&, const tnode_t&);
EXPORT_TO_DLL QDataStream& operator>> (QDataStream&, tnode_t&);
                                      
EXPORT_TO_DLL QDataStream& operator<< (QDataStream&, const fso_t&);
EXPORT_TO_DLL QDataStream& operator>> (QDataStream&, fso_t&);

#endif
