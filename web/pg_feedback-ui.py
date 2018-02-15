#!/usr/bin/env python

# pg_feedback-webui
# self-made python script with storing data in PostgreSQL

import json
import web

urls = (
    '/', 'index'
)

web.config.debug = True
db = web.database(dbn="postgres", db="sergeyb", user="sergeyb", pw="")


def cleanup():
    sqlq = 'DROP TABLE feedback'
    db.query(sqlq)


def init():
    sqlq = '''
        CREATE TABLE IF NOT EXISTS feedback (
          id serial primary key,
          created_at TIMESTAMP DEFAULT NOW(),
          data json
        );'''
    db.query(sqlq)


def addreport(data):
    db.insert('feedback', data=data)


def reports_number():
    results = db.query('SELECT COUNT(*) AS total_reports FROM feedback')
    return results[0].total_reports


class index:

    def POST(self):
        try:
            data = json.loads(web.data())
        except ValueError:
            return "not a json, sorry"
        uuid = data[1]["uuid"]      # FIXME
        addreport(web.data())
        return "Accepted usage report with DB UUID %s\n" % uuid

    def GET(self):
        return reports_number()

if __name__ == '__main__':
    app = web.application(urls, globals())
    t = db.transaction()
    try:
        cleanup()
        init()
    except:
        t.rollback()
        raise
    else:
        t.commit()
    app.run()
