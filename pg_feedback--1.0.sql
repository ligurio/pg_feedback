-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_feedback" to load this file. \quit
CREATE OR REPLACE FUNCTION feedback()
RETURNS json AS $$
DECLARE
result json;
BEGIN
	SELECT
		json_build_object(
		'uuid', 'XXX-XXX-XXX-XXX',	-- FIXME
		'name', (SELECT SUBSTRING((SELECT version()), '([A-z]+)\s+.*')),
		'version', (SELECT SUBSTRING((SELECT version()), '[A-z]+\s+([0-9\.]+)\s+.*')),
		'feedback_version', (SELECT version FROM pg_available_extension_versions WHERE name='pg_feedback'),
		'settings', (SELECT json_agg(s) FROM
					(SELECT name,setting FROM pg_settings WHERE
							name='lc_collate' OR
							name='lc_ctype' OR
							name='shared_buffers' OR
							name='work_mem') s),
		'extensions', (SELECT json_agg(e) FROM
					(SELECT name,comment,default_version FROM pg_available_extensions) e),
		'databases_size', (SELECT row_to_json(s) FROM
					(SELECT SUM(pg_database_size(pg_database.datname)) AS size FROM pg_database) s)
	) INTO result;
RETURN result;
END;
$$ LANGUAGE sql;
