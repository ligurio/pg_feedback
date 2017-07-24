MODULE_big	= pg_feedback
EXTENSION 	= pg_feedback
MODULES 	= pg_feedback
DATA 		= ${EXTENSION}--1.0.sql
OBJS 		= pg_feedback.o helpers.o $(WIN32RES)

REGRESS 	= pg_feedback

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
