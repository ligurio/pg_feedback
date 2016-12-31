# pg_feedback

[![Build Status](https://travis-ci.org/ligurio/pg_feedback.svg?branch=master)](https://travis-ci.org/ligurio/pg_feedback)

This module generates a report about PostgreSQL usage.

```shell
$ psql -c 'CREATE EXTENSION pg_feedback;'
$ psql -c 'SELECT feedback();'
```

```json


{
  "uuid": "XXX-XXX-XXX-XXX",
  "name": "PostgreSQL",
  "version": "9.6.1",
  "pgpro_version": "9.6.1",
  "pgpro_edition": "standard",
  "feedback_version": "1.0",
  "settings": [
    {
      "name": "lc_collate",
      "setting": "en_US.UTF-8"
    },
    {
      "name": "lc_ctype",
      "setting": "en_US.UTF-8"
    },
    {
      "name": "shared_buffers",
      "setting": "16384"
    },
    {
      "name": "work_mem",
      "setting": "4096"
    }
  ],
  "extensions": [
    {
      "name": "adminpack",
      "comment": "administrative functions for PostgreSQL",
      "default_version": "1.0"
    },
    .....
    {
      "name": "fuzzystrmatch",
      "comment": "determine similarities and distance between strings",
      "default_version": "1.1"
    }
  ],
  "databases_size": {
    "size": 29047760
  }
}
```

Copyright (c) 2016, Sergey Bronnikov <sergeyb@bronevichok.ru>
