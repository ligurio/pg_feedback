# pg_feedback

[![Build Status](https://travis-ci.org/ligurio/pg_feedback.svg?branch=master)](https://travis-ci.org/ligurio/pg_feedback)
[![PGXN version](https://badge.fury.io/pg/pg_feedback.svg)](https://badge.fury.io/pg/pg_feedback)

This module generates a report about PostgreSQL usage.

```shell
$ psql -c 'CREATE EXTENSION pg_feedback;'
$ psql -c 'SELECT feedback();'
```

See example of usage report in Travis CI - https://travis-ci.org/ligurio/pg_feedback.

Copyright (c) 2016-2017, Sergey Bronnikov <sergeyb@bronevichok.ru>
