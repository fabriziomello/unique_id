-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION unique_id" to load this file. \quit

-- unique_id_instagram
CREATE FUNCTION unique_id_instagram(seqname REGCLASS, shard_id INTEGER DEFAULT 0)
RETURNS bigint
AS 'MODULE_PATHNAME'
LANGUAGE C;

-- unique_id_sonyflake
CREATE FUNCTION unique_id_sonyflake(seqname REGCLASS, shard_id INTEGER DEFAULT 0)
RETURNS bigint
AS 'MODULE_PATHNAME'
LANGUAGE C;

