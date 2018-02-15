# pg_feedback-webui

there are two ways to store and view postgresql usage reports. First one is a
self-made script with ability to accept usage report via http, store data in
PostgreSQL and show interesting numbers and plots on a web page. Second one is
using of well-known tools Logstash and Kibana.

### How-to use:

- setup postgresql database
- install postgres extension [pg_feedback](https://github.com/ligurio/pg_feedback)
- update connection string in a script pg_feedback-ui.py
- python pg_feedback-ui.py
- ```psql -c 'SELECT feedback();' --tuples-only --no-align > report.json```
- ```curl -d "@report.json" -H "Content-Type: application/json" -X POST http://127.0.0.1:8080```
- point web browser to http://127.0.0.1:8080
