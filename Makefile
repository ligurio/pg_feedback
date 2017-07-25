MODULE_big	= pg_feedback
EXTENSION 	= pg_feedback
MODULES 	= pg_feedback
DATA 		= ${EXTENSION}--1.0.sql
OBJS 		= pg_feedback.o helpers.o $(WIN32RES)

REGRESS 	= pg_feedback

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

CPPCHECK_OPT = --template "{file} ({line}): {severity} ({id}): {message}" \
				--enable=warning,portability,performance \
				--suppress=redundantAssignment \
				--suppress=uselessAssignmentPtrArg \
				--suppress=incorrectStringBooleanError \
				--std=c89 *.c *.h

fmt:
	@clang-format -i -style=file *.[ch]

static:
	@cppcheck ${CPPCHECK_OPT}

.PHONY: fmt static
