# pg_feedback

[![PGXN version](https://badge.fury.io/pg/pg_feedback.svg)](https://badge.fury.io/pg/pg_feedback)

This module generates a report about PostgreSQL usage.

```shell
$ psql -c 'CREATE EXTENSION pg_feedback;'
$ psql -c 'SELECT feedback();'
```

Copyright (c) 2016-2023, Sergey Bronnikov <estetus@gmail.com>
