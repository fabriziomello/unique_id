# unique_id
PostgreSQL extension that implements [non-Standard Time-based K-sorted, Lexicographically Unique Identifiers](https://github.com/uuid6/uuid6-ietf-draft/blob/master/research/sortable-id-comparisons.md).

## Supported PostgreSQL versions

The aim of the project is support as many community-supported major versions of Postgres as possible. Currently, the following versions of PostgreSQL are supported:

9.6, 10, 11, 12, 13 and master (pre-14).

## Installation

```sh
$ git clone https://github.com/fabriziomello/unique_id.git
$ cd unique_id
# Make sure your path includes the bin directory that contains the correct `pg_config`
$ PATH=/path/to/pg/bin:$PATH
$ USE_PGXS=1 make
$ USE_PGXS=1 make install
```

## Tests

```sh
$ USE_PGXS=1 make installcheck
```

## Examples

```
fabrizio=# CREATE EXTENSION unique_id;
CREATE EXTENSION
fabrizio=# \dx unique_id
                                     List of installed extensions
   Name    | Version | Schema |                              Description                               
-----------+---------+--------+------------------------------------------------------------------------
 unique_id | 1.0     | public | Non-Standard Time-based K-sorted, Lexicographically Unique Identifiers
(1 row)

fabrizio=# CREATE SEQUENCE instagram_seq;
CREATE SEQUENCE
fabrizio=# -- Using default shard_id = 0
fabrizio=# SELECT unique_id_instagram('instagram_seq');
 unique_id_instagram 
---------------------
 2563729919292997633
(1 row)

fabrizio=# -- Using default shard_id = 1
fabrizio=# SELECT unique_id_instagram('instagram_seq', 1);
 unique_id_instagram 
---------------------
 2563729919292998658
(1 row)
```
Please feel free to [open a PR](https://github.com/fabriziomello/unique_id/pull/new/master).

## Authors

- [Fabrízio de Royes Mello](mailto:fabriziomello@gmail.com)

## License

PostgreSQL server source code, used under the [PostgreSQL license](https://www.postgresql.org/about/licence/).<br>
Portions Copyright (c) 1996-2021, The PostgreSQL Global Development Group<br>
Portions Copyright (c) 1994, The Regents of the University of California

All other parts are licensed under the 3-clause BSD license, see LICENSE file for details.<br>
Copyright (c) 2021, Fabrízio de Royes Mello <fabriziomello@gmail.com>
