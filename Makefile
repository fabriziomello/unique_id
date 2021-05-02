MODULES = unique_id

REGRESS = unique_id_instagram unique_id_sonyflake

EXTENSION = unique_id
DATA = unique_id--1.0.sql
PGFILEDESC = "unique_id - Implementation of non-Standard Time-based K-sorted, Lexicographically Unique Identifiers"

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
