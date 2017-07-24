-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_feedback" to load this file. \quit

CREATE FUNCTION db_sys_id() RETURNS text
		AS 'MODULE_PATHNAME'
LANGUAGE C;

CREATE FUNCTION postmaster_uptime() RETURNS text
		AS 'MODULE_PATHNAME'
LANGUAGE C;

CREATE FUNCTION datadir_fs() RETURNS text
		AS 'MODULE_PATHNAME'
LANGUAGE C;

CREATE TYPE utype_system_info AS
(
        ncpu			varchar,
        memsize			varchar,
        uptime			varchar,
        osversion		varchar,
        osfamily		varchar,
        osname			varchar,
        osarch			varchar
);

CREATE OR REPLACE FUNCTION sysinfo() RETURNS utype_system_info
AS 'MODULE_PATHNAME', 'sysinfo'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION feedback()
RETURNS json AS $$
	SELECT
		json_build_object(
		'uuid', (SELECT SUBSTRING((SELECT db_sys_id()), '([0-9]*)')),
		'name', (SELECT SUBSTRING((SELECT version()), '([A-z]+)\s+.*')),
		'postmaster_uptime', (SELECT SUBSTRING((SELECT postmaster_uptime()), '[0-9]*')),
		'datadir_fs', (SELECT SUBSTRING((SELECT datadir_fs()), '.*')),
		'version', (SELECT SUBSTRING((SELECT version()), '[A-z]+\s+([0-9\.]+)\s+.*')),
		'feedback_version', (SELECT version FROM pg_available_extension_versions WHERE name='pg_feedback'),
		'settings', (SELECT json_agg(s) FROM
					(SELECT name,setting FROM pg_settings WHERE
							name='lc_collate' OR
							name='lc_ctype' OR
							name='shared_buffers' OR
							name='work_mem') s),
		-- pg_extension table?
		'extensions', (SELECT json_agg(e) FROM
					(SELECT name,comment,default_version FROM pg_available_extensions) e),
		'databases_size', (SELECT row_to_json(s) FROM
					(SELECT SUM(pg_database_size(pg_database.datname)) AS size FROM pg_database) s)
		--'system_info', (SELECT row_to_json(sysinfo(), true))
	);
$$ LANGUAGE sql;
