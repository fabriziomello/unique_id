\set VERBOSITY terse
CREATE EXTENSION unique_id;

-- Throw error because should exists a sequence
SELECT unique_id_instagram('instagram_seq');

CREATE SEQUENCE instagram_seq;

-- Individual transactions and default shard_id = 0
SELECT unique_id_instagram('instagram_seq') > unique_id_instagram('instagram_seq');
SELECT unique_id_instagram('instagram_seq') < unique_id_instagram('instagram_seq');
SELECT unique_id_instagram('instagram_seq') <> unique_id_instagram('instagram_seq');

-- Individual transactions and shard_id = 1
SELECT unique_id_instagram('instagram_seq', 1) > unique_id_instagram('instagram_seq', 1);
SELECT unique_id_instagram('instagram_seq', 1) < unique_id_instagram('instagram_seq', 1);
SELECT unique_id_instagram('instagram_seq', 1) <> unique_id_instagram('instagram_seq', 1);

DROP EXTENSION unique_id;
