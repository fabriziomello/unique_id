/* -------------------------------------------------------------------------
 *
 * unique_id.c
 *      Postgres implementation of non-Standard Time-based K-sorted,
 *      Lexicographically Unique Identifiers:
 *      https://github.com/uuid6/uuid6-ietf-draft/blob/master/research/sortable-id-comparisons.md
 *
 * Copyright (C) 2021, Fabrízio de Royes Mello
 *
 * IDENTIFICATION
 *		unique_id.c
 *
 * -------------------------------------------------------------------------
 */

#include "postgres.h"

#include <math.h>

#include "commands/sequence.h"
#include "utils/timestamp.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(unique_id_instagram);

/* extracted from utils/adt/timestamp.c */
static int64
GetCurrentEpoch()
{
    Timestamp   epoch;
    TimestampTz timestamp;
    float8      result;

    epoch = SetEpochTimestamp();
    timestamp = GetCurrentTimestamp();

    /* try to avoid precision loss in subtraction */ 
    if (timestamp < (PG_INT64_MAX + epoch))
        result = (timestamp - epoch) / 1000000.0;
    else
        result = ((float8) timestamp - epoch) / 1000000.0;

    return (int64) floor(result);
}

/* 
 *  Instragam's Sharding ID (inspired by SnowFlake)
 *
 *  ID Format: timestamp|shardID|sequence
 *  Lenght: 64 bits
 *  - Timestamp: 41 bits
 *  - Shard id: 13 bits
 *  - Sequence: 10 bits
 *
 *  References:
 *  - https://github.com/uuid6/uuid6-ietf-draft/blob/master/research/sortable-id-comparisons.md#name-instagrams-sharding-id-inspired-by-snowflake
 *  - https://instagram-engineering.com/sharding-ids-at-instagram-1cf5a71e5a5c
 *
 */
Datum
unique_id_instagram(PG_FUNCTION_ARGS)
{
    Oid     seqoid = PG_GETARG_OID(0);
    uint32  shard_id = PG_GETARG_INT32(1);
    uint64  our_epoch = 1314220021721;
    uint64  result;

	result = ( (GetCurrentEpoch() * 1000) - our_epoch) << 23;
	result = result | (shard_id <<10);
    result = result | (nextval_internal(seqoid, true));

    PG_RETURN_INT64(result);
}
