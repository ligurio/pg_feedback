/*-------------------------------------------------------------------------
 *
 * pg_feedback.c
 *
 * Copyright (c) 2017-2019, Sergey Bronnikov
 *
 *-------------------------------------------------------------------------
 */

#include <inttypes.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <postgres.h>

#include <miscadmin.h>
#include <funcapi.h>
#include <utils/builtins.h>
#include <catalog/pg_control.h>
#include <common/controldata_utils.h>

#include "helpers.h"

static char *system_db_id()
{
	ControlFileData *ControlFile = NULL;
	char *sysident_str = NULL;
	sysident_str = (char *)malloc(32);
	if (!sysident_str) {
            perror("malloc");
            return NULL;
	}

	ControlFile = get_controlfile(DataDir, NULL);
        if (!ControlFile)
            ereport(ERROR,
              (errmsg("calculated CRC checksum does not match value stored in file")));
	snprintf(sysident_str, sizeof (sysident_str), UINT64_FORMAT, ControlFile->system_identifier);

	return sysident_str;
}

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(postmaster_uptime);

Datum
postmaster_uptime(PG_FUNCTION_ARGS)
{
	pid_t ppid = getppid();
	unsigned long long time = get_proc_uptime(ppid);
	char *uptime_str = NULL;
	snprintf(uptime_str, 256, "%lld", time);
	PG_RETURN_TEXT_P(cstring_to_text(uptime_str));
}

PG_FUNCTION_INFO_V1(db_sys_id);

Datum
db_sys_id(PG_FUNCTION_ARGS)
{
	char *id;
	id = system_db_id();
	PG_RETURN_TEXT_P(cstring_to_text(id));
}

PG_FUNCTION_INFO_V1(datadir_fs);

Datum
datadir_fs(PG_FUNCTION_ARGS)
{
	char *fs;
	fs = dirfs(DataDir);
	PG_RETURN_TEXT_P(cstring_to_text(fs));
}

PG_FUNCTION_INFO_V1(sysinfo);

Datum
sysinfo(PG_FUNCTION_ARGS)
{
	char **values;
	HeapTuple tuple;
	Datum result;
	TupleDesc tupdesc;
	AttInMetadata *attinmeta;
	hwinfo *hw;
	osinfo *os;

	if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
		ereport(ERROR,
		    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
		    errmsg("function returning record called in context "
		    "that cannot accept type record")));

	values = (char **)palloc(7 * sizeof (char *));
	values[0] = (char *)palloc(256 * sizeof (char));
	values[1] = (char *)palloc(256 * sizeof (char));
	values[2] = (char *)palloc(256 * sizeof (char));
	values[3] = (char *)palloc(256 * sizeof (char));
	values[4] = (char *)palloc(256 * sizeof (char));
	values[5] = (char *)palloc(256 * sizeof (char));
	values[6] = (char *)palloc(256 * sizeof (char));

	hw = get_hw_info();
	snprintf(values[0], 256, "%d", hw->ncpu);
	snprintf(values[1], 256, "%" PRId64, hw->memsize);
	snprintf(values[2], 256, "%llu", hw->uptime);
	free(hw);

	os = get_os_info();
	snprintf(values[3], 256, "%s", os->os_version);
	snprintf(values[4], 256, "%s", os->os_name);
	snprintf(values[5], 256, "%s", os->os_family);
	snprintf(values[6], 256, "%s", os->os_arch);
	free(os->os_version);
	free(os->os_name);
	free(os->os_family);
	free(os->os_arch);
	free(os);

	attinmeta = TupleDescGetAttInMetadata(tupdesc);
	tuple = BuildTupleFromCStrings(attinmeta, values);

	result = HeapTupleGetDatum(tuple);

	pfree(values[0]);
	pfree(values[1]);
	pfree(values[2]);
	pfree(values[3]);
	pfree(values[4]);
	pfree(values[5]);
	pfree(values[6]);
	pfree(values);

	PG_RETURN_DATUM(result);
}
