\set VERBOSITY terse
CREATE EXTENSION unique_id;

-- Throw error because should exists a sequence
SELECT unique_id_sonyflake('sonyflake_seq');

CREATE SEQUENCE sonyflake_seq;

-- Individual transactions and default shard_id = 0
SELECT unique_id_sonyflake('sonyflake_seq') > unique_id_sonyflake('sonyflake_seq');
SELECT unique_id_sonyflake('sonyflake_seq') < unique_id_sonyflake('sonyflake_seq');
SELECT unique_id_sonyflake('sonyflake_seq') <> unique_id_sonyflake('sonyflake_seq');

-- Individual transactions and shard_id = 1
SELECT unique_id_sonyflake('sonyflake_seq', 1) > unique_id_sonyflake('sonyflake_seq', 1);
SELECT unique_id_sonyflake('sonyflake_seq', 1) < unique_id_sonyflake('sonyflake_seq', 1);
SELECT unique_id_sonyflake('sonyflake_seq', 1) <> unique_id_sonyflake('sonyflake_seq', 1);

DROP EXTENSION unique_id;
