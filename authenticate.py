#!/usr/bin/python

import sys
import os

PAM_ENV_USER = "GANETI_RAPI_USER"
PAM_ENV_PASSWORD = "GANETI_RAPI_PASSWORD"
PAM_ENV_AUTHTOK = "GANETI_RAPI_AUTHTOK"

sys.path.insert(0, os.getenv("GANETI_PATH"))

from ganeti.rapi.auth import users_file
from ganeti import pathutils

username = os.getenv(PAM_ENV_USER)
password = os.getenv(PAM_ENV_PASSWORD)

users = users_file.RapiUsers()
users.Load(pathutils.RAPI_USERS_FILE)
user = users.Get(username)

if not (user and password == user.password):
  sys.exit(1)

sys.exit(0)
