CREATE OR REPLACE FUNCTION isnull (element json)
RETURNS boolean AS $$
SELECT (element IS NOT NULL) AND (element::text = 'null');
$$ LANGUAGE SQL IMMUTABLE STRICT;

CREATE EXTENSION IF NOT EXISTS pg_feedback;

SELECT isnull((SELECT feedback())::json->'uuid');
SELECT isnull((SELECT feedback())::json->'datadir_fs');
SELECT isnull((SELECT feedback())::json->'postmaster_uptime');
--SELECT isnull((SELECT row_to_json(sysinfo())::json->'ncpu'));
--SELECT isnull((SELECT row_to_json(sysinfo())::json->'memsize'));
--SELECT isnull((SELECT row_to_json(sysinfo())::json->'uptime'));
--SELECT isnull((SELECT row_to_json(sysinfo())::json->'osversion'));
--SELECT isnull((SELECT row_to_json(sysinfo())::json->'osfamily'));
--SELECT isnull((SELECT row_to_json(sysinfo())::json->'osname'));
--SELECT isnull((SELECT row_to_json(sysinfo())::json->'osarch'));

DROP EXTENSION pg_feedback;
