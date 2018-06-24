#!/bin/bash
tar cvf secrets.tar c/include/DB.h python/mysite/settings.py c/include/API.h

travis encrypt-file secrets.tar
