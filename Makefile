EXTENSION = pg_feedback
DATA = ${EXTENSION}--1.0.sql
REGRESS = pg_feedback

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
