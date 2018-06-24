#!/bin/bash
tar cvf secrets.tar c/include/DB.h python/mysite/settings.py c/include/API.h python/JobCaravan/static/api.js

travis encrypt-file secrets.tar
