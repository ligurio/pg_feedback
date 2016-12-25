## pg_feedback

This module adds statistic report about PostgreSQL usage.

```
$ psql -c 'CREATE EXTENSION pg_feedback;'
$ psql -c 'SELECT feedback();'
```

```json
{
  "version": "PostgreSQL 9.6.1 on x86_64-apple-darwin15.6.0, compiled by Apple LLVM version 8.0.0 (clang-800.0.42.1), 64-bit",
  "pgpro_version": "PostgresPro 9.6.1.2 on x86_64-apple-darwin15.6.0, compiled by Apple LLVM version 8.0.0 (clang-800.0.42.1), 64-bit",
  "pgpro_edition": "standard",
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
  "databases_size": {
    "size": 29047760
  }
}
```

Copyright (c) 2016, Sergey Bronnikov <sergeyb@bronevichok.ru>
