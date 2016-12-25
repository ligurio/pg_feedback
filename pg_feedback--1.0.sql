-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_feedback" to load this file. \quit
CREATE FUNCTION feedback()
RETURNS json AS $$
	SELECT
		json_build_object(
		'uuid', 'XXX-XXX-XXX-XXX',	-- FIXME
		'version', (SELECT version()),
		'pgpro_version', (SELECT pgpro_version()),
		'pgpro_edition', (SELECT pgpro_edition()),
		'settings', (SELECT json_agg(s) FROM
					(SELECT name,setting FROM pg_settings WHERE
							name='lc_collate' OR
							name='lc_ctype' OR
							name='shared_buffers' OR
							name='work_mem') s),
		--'extensions', (SELECT json_agg(e) FROM (SELECT extname AS e FROM pg_extension) e),
		'extensions', (SELECT json_agg(e) FROM
					(SELECT name,comment,default_version FROM pg_available_extensions) e),
		'databases_size', (SELECT row_to_json(s) FROM
					(SELECT SUM(pg_database_size(pg_database.datname)) AS size FROM pg_database) s)
	);
  $$ LANGUAGE sql;
