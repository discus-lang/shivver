#pragma once

// bits in object tag
#define SV_STORE_BITS_TAG               8

// bits in name space identifier
#define SV_STORE_BITS_NAME_SPACE        8

// bits in name definition length
//  limits the number of bytes in a name.
#define SV_STORE_BITS_NAME_LENGTH       16

// bits in cold region index
//  limits the number of cold regions in a zone.
#define SV_STORE_BITS_COLD_REGION       24

// bits in store object offset
//  limits the number of objects in a region.
#define SV_STORE_BITS_OFFSET            32

// bits in argument count.
//  limits the length of an argument list.
#define SV_STORE_BITS_MMM_COUNT         24