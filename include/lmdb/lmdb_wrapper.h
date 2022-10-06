#pragma once

/*! \file lmdb_wrapper.h

Wrap LMDB in a convenient interface.

Mock stub for this interface lets one
conditionally send lmdb changes based 
on the most recent block number reflected
in the database.  Helpful when
repairing from a crash (and i.e. some
lmdbs got persisted and some didn't).
*/

#include <cstdint>
#include <cstring>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include <lmdb.h>

#include "utils/cleanup.h"
#include "lmdb/lmdb_types.h"

namespace lmdb {

#define DEFAULT_LMDB_FLAGS MDB_WRITEMAP | MDB_NOSYNC // | MDB_NOTLS

/*! Utility methods around an LMDB instance.
    Beyond dbenv, tracks persisted round number and MDB_dbi for object data and metadata
    databases.
*/
class BaseLMDBInstance {

  dbenv env;
  bool env_open;

  //! Metadata DBI (currently just round number).
  MDB_dbi metadata_dbi;
  bool metadata_dbi_open;

  //uint64_t persisted_round_number; // state up to and including persisted_round_number has been persisted.
  //Starts at 0 - means that we can't have a block number 0, except as a base/bot instance

  void write_persisted_round_number(dbenv::wtxn& wtx, uint64_t round_number);

public:

  BaseLMDBInstance(size_t mapsize, size_t maxdbs = 50) 
    : env{mapsize, maxdbs}
    , env_open{false}
    , metadata_dbi{0}
    , metadata_dbi_open{false}
   // , persisted_round_number(0)
    {};

  //! Get the most recent round number 
  //! reflected on disk.
  uint64_t get_persisted_round_number() const;

  //! Open lmdb environment at a specified path.
  void open_env(const std::string path, unsigned flags = DEFAULT_LMDB_FLAGS, mdb_mode_t mode = 0666) {
    env.open(path.c_str(), flags, mode);
    env_open = true;
  }

  MDB_dbi create_db(const char* name);
  MDB_dbi open_db(const char* name);

  operator bool() const {
    return env_open;
  }

  dbenv::wtxn wbegin() const {
    if (!env_open) {
      throw std::runtime_error("can't access a db if env is unopened");
    }

    return env.wbegin();
  }

  dbenv::txn rbegin() const {
    return env.rbegin();
  }

  void sync() {
    env.sync();
  }

  const MDB_dbi get_metadata_dbi() const {
    return metadata_dbi;
  }

  //! Commits a write transaction to the database.
  //! Optionally performs an msync.  Updates persisted_round counter.
  void commit_wtxn(dbenv::wtxn& txn, uint64_t persisted_round, bool do_sync = true);
};

class LMDBInstance : public BaseLMDBInstance {

  //! Data DBI
  MDB_dbi dbi;
  bool dbi_valid;

public:

  LMDBInstance(size_t mapsize = 0x1000000000);

  const MDB_dbi& get_data_dbi() const;

  MDB_stat stat() const;

  void create_db(const char* name);

  void open_db(const char* name);
};

class SharedLMDBInstance {

  BaseLMDBInstance& base_lmdb;

  MDB_dbi local_dbi;
  bool local_dbi_valid;

public:

  SharedLMDBInstance(BaseLMDBInstance& base_lmdb);

  const MDB_dbi& get_data_dbi();

  void create_db(const char* name);

  void open_db(const char* name);

  uint64_t get_persisted_round_number() const {
    return base_lmdb.get_persisted_round_number();
  }

  operator bool() {
    return (bool)base_lmdb;
  }

  dbenv::txn rbegin() {
    return base_lmdb.rbegin();
  }
};

} /* lmdb */
