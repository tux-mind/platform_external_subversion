/* This file is automatically generated from wc-metadata.sql.
 * Do not edit this file -- edit the source and rerun gen-make.py */

#define STMT_CREATE_SCHEMA 0
#define STMT_0 \
  "CREATE TABLE REPOSITORY ( " \
  "  id INTEGER PRIMARY KEY AUTOINCREMENT, " \
  "  root  TEXT UNIQUE NOT NULL, " \
  "  uuid  TEXT NOT NULL " \
  "  ); " \
  "CREATE INDEX I_UUID ON REPOSITORY (uuid); " \
  "CREATE INDEX I_ROOT ON REPOSITORY (root); " \
  "CREATE TABLE WCROOT ( " \
  "  id  INTEGER PRIMARY KEY AUTOINCREMENT, " \
  "  local_abspath  TEXT UNIQUE " \
  "  ); " \
  "CREATE UNIQUE INDEX I_LOCAL_ABSPATH ON WCROOT (local_abspath); " \
  "CREATE TABLE PRISTINE ( " \
  "  checksum  TEXT NOT NULL PRIMARY KEY, " \
  "  compression  INTEGER, " \
  "  size  INTEGER NOT NULL, " \
  "  refcount  INTEGER NOT NULL, " \
  "  md5_checksum  TEXT NOT NULL " \
  "  ); " \
  "CREATE TABLE ACTUAL_NODE ( " \
  "  wc_id  INTEGER NOT NULL REFERENCES WCROOT (id), " \
  "  local_relpath  TEXT NOT NULL, " \
  "  parent_relpath  TEXT, " \
  "  properties  BLOB, " \
  "  conflict_old  TEXT, " \
  "  conflict_new  TEXT, " \
  "  conflict_working  TEXT, " \
  "  prop_reject  TEXT, " \
  "  changelist  TEXT, " \
  "  text_mod  TEXT, " \
  "  tree_conflict_data  TEXT, " \
  "  conflict_data  BLOB, " \
  "  older_checksum  TEXT REFERENCES PRISTINE (checksum), " \
  "  left_checksum  TEXT REFERENCES PRISTINE (checksum), " \
  "  right_checksum  TEXT REFERENCES PRISTINE (checksum), " \
  "  PRIMARY KEY (wc_id, local_relpath) " \
  "  ); " \
  "CREATE INDEX I_ACTUAL_PARENT ON ACTUAL_NODE (wc_id, parent_relpath); " \
  "CREATE INDEX I_ACTUAL_CHANGELIST ON ACTUAL_NODE (changelist); " \
  "CREATE TABLE LOCK ( " \
  "  repos_id  INTEGER NOT NULL REFERENCES REPOSITORY (id), " \
  "  repos_relpath  TEXT NOT NULL, " \
  "  lock_token  TEXT NOT NULL, " \
  "  lock_owner  TEXT, " \
  "  lock_comment  TEXT, " \
  "  lock_date  INTEGER, " \
  "  PRIMARY KEY (repos_id, repos_relpath) " \
  "  ); " \
  "CREATE TABLE WORK_QUEUE ( " \
  "  id  INTEGER PRIMARY KEY AUTOINCREMENT, " \
  "  work  BLOB NOT NULL " \
  "  ); " \
  "CREATE TABLE WC_LOCK ( " \
  "  wc_id  INTEGER NOT NULL  REFERENCES WCROOT (id), " \
  "  local_dir_relpath  TEXT NOT NULL, " \
  "  locked_levels  INTEGER NOT NULL DEFAULT -1, " \
  "  PRIMARY KEY (wc_id, local_dir_relpath) " \
  " ); " \
  "PRAGMA user_version = " \
  APR_STRINGIFY(SVN_WC__VERSION) \
  "; " \
  ""

#define STMT_CREATE_NODES 1
#define STMT_1 \
  "CREATE TABLE NODES ( " \
  "  wc_id  INTEGER NOT NULL REFERENCES WCROOT (id), " \
  "  local_relpath  TEXT NOT NULL, " \
  "  op_depth INTEGER NOT NULL, " \
  "  parent_relpath  TEXT, " \
  "  repos_id  INTEGER REFERENCES REPOSITORY (id), " \
  "  repos_path  TEXT, " \
  "  revision  INTEGER, " \
  "  presence  TEXT NOT NULL, " \
  "  moved_here  INTEGER, " \
  "  moved_to  TEXT, " \
  "  kind  TEXT NOT NULL, " \
  "  properties  BLOB, " \
  "  depth  TEXT, " \
  "  checksum  TEXT REFERENCES PRISTINE (checksum), " \
  "  symlink_target  TEXT, " \
  "  changed_revision  INTEGER, " \
  "  changed_date      INTEGER, " \
  "  changed_author    TEXT, " \
  "  translated_size  INTEGER, " \
  "  last_mod_time  INTEGER, " \
  "  dav_cache  BLOB, " \
  "  file_external  TEXT, " \
  "  PRIMARY KEY (wc_id, local_relpath, op_depth) " \
  "  ); " \
  "CREATE INDEX I_NODES_PARENT ON NODES (wc_id, parent_relpath, op_depth); " \
  "CREATE VIEW NODES_CURRENT AS " \
  "  SELECT * FROM nodes AS n " \
  "    WHERE op_depth = (SELECT MAX(op_depth) FROM nodes AS n2 " \
  "                      WHERE n2.wc_id = n.wc_id " \
  "                        AND n2.local_relpath = n.local_relpath); " \
  "CREATE VIEW NODES_BASE AS " \
  "  SELECT * FROM nodes " \
  "  WHERE op_depth = 0; " \
  ""

#define STMT_CREATE_NODES_TRIGGERS 2
#define STMT_2 \
  "CREATE TRIGGER nodes_insert_trigger " \
  "AFTER INSERT ON nodes " \
  "WHEN NEW.checksum IS NOT NULL " \
  "BEGIN " \
  "  UPDATE pristine SET refcount = refcount + 1 " \
  "  WHERE checksum = NEW.checksum; " \
  "END; " \
  "CREATE TRIGGER nodes_delete_trigger " \
  "AFTER DELETE ON nodes " \
  "WHEN OLD.checksum IS NOT NULL " \
  "BEGIN " \
  "  UPDATE pristine SET refcount = refcount - 1 " \
  "  WHERE checksum = OLD.checksum; " \
  "END; " \
  "CREATE TRIGGER nodes_update_checksum_trigger " \
  "AFTER UPDATE OF checksum ON nodes " \
  "WHEN NEW.checksum IS NOT OLD.checksum " \
  "BEGIN " \
  "  UPDATE pristine SET refcount = refcount + 1 " \
  "  WHERE checksum = NEW.checksum; " \
  "  UPDATE pristine SET refcount = refcount - 1 " \
  "  WHERE checksum = OLD.checksum; " \
  "END; " \
  ""

#define STMT_CREATE_EXTERNALS 3
#define STMT_3 \
  "CREATE TABLE EXTERNALS ( " \
  "  wc_id  INTEGER NOT NULL REFERENCES WCROOT (id), " \
  "  local_relpath  TEXT NOT NULL, " \
  "  parent_relpath  TEXT NOT NULL, " \
  "  repos_id  INTEGER NOT NULL REFERENCES REPOSITORY (id), " \
  "  presence  TEXT NOT NULL, " \
  "  kind  TEXT NOT NULL, " \
  "  def_local_relpath         TEXT NOT NULL, " \
  "  def_repos_relpath         TEXT NOT NULL, " \
  "  def_operational_revision  TEXT, " \
  "  def_revision              TEXT, " \
  "  PRIMARY KEY (wc_id, local_relpath) " \
  "); " \
  "CREATE INDEX I_EXTERNALS_PARENT ON EXTERNALS (wc_id, parent_relpath); " \
  "CREATE UNIQUE INDEX I_EXTERNALS_DEFINED ON EXTERNALS (wc_id, " \
  "                                                      def_local_relpath, " \
  "                                                      local_relpath); " \
  ""

#define STMT_UPGRADE_TO_20 4
#define STMT_4 \
  "UPDATE BASE_NODE SET checksum=(SELECT checksum FROM pristine " \
  "                           WHERE md5_checksum=BASE_NODE.checksum) " \
  "WHERE EXISTS(SELECT 1 FROM pristine WHERE md5_checksum=BASE_NODE.checksum); " \
  "UPDATE WORKING_NODE SET checksum=(SELECT checksum FROM pristine " \
  "                           WHERE md5_checksum=WORKING_NODE.checksum) " \
  "WHERE EXISTS(SELECT 1 FROM pristine WHERE md5_checksum=WORKING_NODE.checksum); " \
  "INSERT INTO NODES ( " \
  "       wc_id, local_relpath, op_depth, parent_relpath, " \
  "       repos_id, repos_path, revision, " \
  "       presence, depth, moved_here, moved_to, kind, " \
  "       changed_revision, changed_date, changed_author, " \
  "       checksum, properties, translated_size, last_mod_time, " \
  "       dav_cache, symlink_target, file_external ) " \
  "SELECT wc_id, local_relpath, 0 , parent_relpath, " \
  "       repos_id, repos_relpath, revnum, " \
  "       presence, depth, NULL , NULL , kind, " \
  "       changed_rev, changed_date, changed_author, " \
  "       checksum, properties, translated_size, last_mod_time, " \
  "       dav_cache, symlink_target, file_external " \
  "FROM BASE_NODE; " \
  "INSERT INTO NODES ( " \
  "       wc_id, local_relpath, op_depth, parent_relpath, " \
  "       repos_id, repos_path, revision, " \
  "       presence, depth, moved_here, moved_to, kind, " \
  "       changed_revision, changed_date, changed_author, " \
  "       checksum, properties, translated_size, last_mod_time, " \
  "       dav_cache, symlink_target, file_external ) " \
  "SELECT wc_id, local_relpath, 2 , parent_relpath, " \
  "       copyfrom_repos_id, copyfrom_repos_path, copyfrom_revnum, " \
  "       presence, depth, NULL , NULL , kind, " \
  "       changed_rev, changed_date, changed_author, " \
  "       checksum, properties, translated_size, last_mod_time, " \
  "       NULL , symlink_target, NULL " \
  "FROM WORKING_NODE; " \
  "DROP TABLE BASE_NODE; " \
  "DROP TABLE WORKING_NODE; " \
  "PRAGMA user_version = 20; " \
  ""

#define STMT_UPGRADE_TO_21 5
#define STMT_5 \
  "PRAGMA user_version = 21; " \
  ""

#define STMT_UPGRADE_TO_22 6
#define STMT_6 \
  "UPDATE actual_node SET tree_conflict_data = conflict_data; " \
  "UPDATE actual_node SET conflict_data = NULL; " \
  "PRAGMA user_version = 22; " \
  ""

#define STMT_UPGRADE_TO_23 7
#define STMT_7 \
  "PRAGMA user_version = 23; " \
  ""

#define STMT_UPGRADE_TO_24 8
#define STMT_8 \
  "UPDATE pristine SET refcount = " \
  "  (SELECT COUNT(*) FROM nodes " \
  "   WHERE checksum = pristine.checksum ); " \
  "PRAGMA user_version = 24; " \
  ""

#define STMT_UPGRADE_TO_25 9
#define STMT_9 \
  "DROP VIEW IF EXISTS NODES_CURRENT; " \
  "CREATE VIEW NODES_CURRENT AS " \
  "  SELECT * FROM nodes " \
  "    JOIN (SELECT wc_id, local_relpath, MAX(op_depth) AS op_depth FROM nodes " \
  "          GROUP BY wc_id, local_relpath) AS filter " \
  "    ON nodes.wc_id = filter.wc_id " \
  "      AND nodes.local_relpath = filter.local_relpath " \
  "      AND nodes.op_depth = filter.op_depth; " \
  "PRAGMA user_version = 25; " \
  ""

#define STMT_UPGRADE_TO_26 10
#define STMT_10 \
  "DROP VIEW IF EXISTS NODES_BASE; " \
  "CREATE VIEW NODES_BASE AS " \
  "  SELECT * FROM nodes " \
  "  WHERE op_depth = 0; " \
  "PRAGMA user_version = 26; " \
  ""

#define STMT_UPGRADE_TO_27 11
#define STMT_11 \
  "PRAGMA user_version = 27; " \
  ""

#define STMT_UPGRADE_TO_28 12
#define STMT_12 \
  "UPDATE NODES SET checksum=(SELECT checksum FROM pristine " \
  "                           WHERE md5_checksum=nodes.checksum) " \
  "WHERE EXISTS(SELECT 1 FROM pristine WHERE md5_checksum=nodes.checksum); " \
  "PRAGMA user_version = 28; " \
  ""

#define STMT_UPGRADE_TO_29 13
#define STMT_13 \
  "DROP TRIGGER IF EXISTS nodes_update_checksum_trigger; " \
  "DROP TRIGGER IF EXISTS nodes_insert_trigger; " \
  "DROP TRIGGER IF EXISTS nodes_delete_trigger; " \
  "CREATE TRIGGER nodes_update_checksum_trigger " \
  "AFTER UPDATE OF checksum ON nodes " \
  "WHEN NEW.checksum IS NOT OLD.checksum " \
  "BEGIN " \
  "  UPDATE pristine SET refcount = refcount + 1 " \
  "  WHERE checksum = NEW.checksum; " \
  "  UPDATE pristine SET refcount = refcount - 1 " \
  "  WHERE checksum = OLD.checksum; " \
  "END; " \
  "CREATE TRIGGER nodes_insert_trigger " \
  "AFTER INSERT ON nodes " \
  "WHEN NEW.checksum IS NOT NULL " \
  "BEGIN " \
  "  UPDATE pristine SET refcount = refcount + 1 " \
  "  WHERE checksum = NEW.checksum; " \
  "END; " \
  "CREATE TRIGGER nodes_delete_trigger " \
  "AFTER DELETE ON nodes " \
  "WHEN OLD.checksum IS NOT NULL " \
  "BEGIN " \
  "  UPDATE pristine SET refcount = refcount - 1 " \
  "  WHERE checksum = OLD.checksum; " \
  "END; " \
  "PRAGMA user_version = 29; " \
  "-- format: YYY " \
  ""

#define WC_METADATA_SQL_99 \
  "CREATE TABLE ACTUAL_NODE_BACKUP ( " \
  "  wc_id  INTEGER NOT NULL, " \
  "  local_relpath  TEXT NOT NULL, " \
  "  parent_relpath  TEXT, " \
  "  properties  BLOB, " \
  "  conflict_old  TEXT, " \
  "  conflict_new  TEXT, " \
  "  conflict_working  TEXT, " \
  "  prop_reject  TEXT, " \
  "  changelist  TEXT, " \
  "  text_mod  TEXT " \
  "  ); " \
  "INSERT INTO ACTUAL_NODE_BACKUP SELECT " \
  "  wc_id, local_relpath, parent_relpath, properties, conflict_old, " \
  "  conflict_new, conflict_working, prop_reject, changelist, text_mod " \
  "FROM ACTUAL_NODE; " \
  "DROP TABLE ACTUAL_NODE; " \
  "CREATE TABLE ACTUAL_NODE ( " \
  "  wc_id  INTEGER NOT NULL REFERENCES WCROOT (id), " \
  "  local_relpath  TEXT NOT NULL, " \
  "  parent_relpath  TEXT, " \
  "  properties  BLOB, " \
  "  conflict_old  TEXT, " \
  "  conflict_new  TEXT, " \
  "  conflict_working  TEXT, " \
  "  prop_reject  TEXT, " \
  "  changelist  TEXT, " \
  "  text_mod  TEXT, " \
  "  PRIMARY KEY (wc_id, local_relpath) " \
  "  ); " \
  "CREATE INDEX I_ACTUAL_PARENT ON ACTUAL_NODE (wc_id, parent_relpath); " \
  "CREATE INDEX I_ACTUAL_CHANGELIST ON ACTUAL_NODE (changelist); " \
  "INSERT INTO ACTUAL_NODE SELECT " \
  "  wc_id, local_relpath, parent_relpath, properties, conflict_old, " \
  "  conflict_new, conflict_working, prop_reject, changelist, text_mod " \
  "FROM ACTUAL_NODE_BACKUP; " \
  "DROP TABLE ACTUAL_NODE_BACKUP; " \
  ""

#define WC_METADATA_SQL_DECLARE_STATEMENTS(varname) \
  static const char * const varname[] = { \
    STMT_0, \
    STMT_1, \
    STMT_2, \
    STMT_3, \
    STMT_4, \
    STMT_5, \
    STMT_6, \
    STMT_7, \
    STMT_8, \
    STMT_9, \
    STMT_10, \
    STMT_11, \
    STMT_12, \
    STMT_13, \
    NULL \
  }
