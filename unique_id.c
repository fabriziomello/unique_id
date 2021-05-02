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

#define SNOWFLAKE_MAX_ELEMENTS  3

/* Generic SnowFlake configuration  */
typedef struct SnowFlakeConfig
{
    int     max_size;
    int     bits[SNOWFLAKE_MAX_ELEMENTS];
    int64   values[SNOWFLAKE_MAX_ELEMENTS];
} SnowFlakeConfig;

/* Generic SnowFlake id generator */
int64 snowflake_id(SnowFlakeConfig *config);

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(unique_id_instagram);
PG_FUNCTION_INFO_V1(unique_id_sonyflake);

/* extracted from utils/adt/timestamp.c */
static Datum
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

    return floor(result);
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
    Oid             seqoid = PG_GETARG_OID(0);
    int32           shard_id = PG_GETARG_INT32(1);
    int64           epoch = 1314220021721;
    int64           result;
    SnowFlakeConfig *config;

    config = (SnowFlakeConfig *) palloc(sizeof(SnowFlakeConfig));

    config->max_size = 64;

    config->bits[0] = 41;
    config->bits[1] = 13;
    config->bits[2] = 10;

    config->values[0] = (DatumGetInt64(GetCurrentEpoch()) * 1000) - epoch;
    config->values[1] = (int64) shard_id;
    config->values[2] = nextval_internal(seqoid, true);

    result = snowflake_id(config);
    pfree(config);

    PG_RETURN_INT64(result);
}

/*
 *  Sony's Sonyflake (inspired by Flake?)
 *
 *  ID Format: timestamp|sequence|machineID
 *  Lenght: 63 bits
 *  - Timestamp: 39 bits
 *  - Sequence: 8 bits
 *  - Shard id: 16 bits
 *
 *  References:
 *  - https://github.com/uuid6/uuid6-ietf-draft/blob/master/research/sortable-id-comparisons.md#name-sonys-sonyflake-inspired-by-flake
 *
 */
Datum
unique_id_sonyflake(PG_FUNCTION_ARGS)
{
    Oid             seqoid = PG_GETARG_OID(0);
    int32           shard_id = PG_GETARG_INT32(1);
    int64           epoch = 1409529600; /* 2014-09-01 00:00:00  */
    SnowFlakeConfig *config;

    config = (SnowFlakeConfig *) palloc(sizeof(SnowFlakeConfig));

    config->max_size = 63;

    config->bits[0] = 39;
    config->bits[1] = 8;
    config->bits[2] = 16;

    config->values[0] = (DatumGetInt64(GetCurrentEpoch()) * 1000) - epoch;
    config->values[1] = nextval_internal(seqoid, true);
    config->values[2] = (int64) shard_id;

    PG_RETURN_INT64( snowflake_id(config) );
}

/*
 * Generic implementation of SnowFlake int64 generation ID.
 */
int64
snowflake_id(SnowFlakeConfig *config)
{
    int     i;
    int     bits = 0;
    int64   result = 0;

    if (config->max_size > 64)
        elog(ERROR, "Maximum allowed size for SnowFlake is 64 bits");

    for (i = 0; i < SNOWFLAKE_MAX_ELEMENTS; i++)
    {
        bits += config->bits[i];
        result = result | (config->values[i] << (config->max_size - bits));
    }

    if (bits != config->max_size)
        elog(ERROR, "Expected %d bits but configured %d", config->max_size, bits);

    return result;
}
