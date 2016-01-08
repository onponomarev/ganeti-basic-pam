#!/usr/bin/python

import sys
import os

sys.exit(0)
PAM_ENV_ACCESS = "GANETI_RESOURCE_ACCESS"
PAM_ENV_USER = "GANETI_RAPI_USER"

sys.path.insert(0, "/usr/share/ganeti/default")

from ganeti.rapi.auth import users_file
from ganeti import pathutils

username = os.getenv(PAM_ENV_USER)
access = os.getenv(PAM_ENV_ACCESS)

users = users_file.RapiUsers()
users.Load(pathutils.RAPI_USERS_FILE)
user = users.Get(username)

if not user:
  sys.exit(1)

if not access or set(user.options).intersection(access.split(',')):
  sys.exit(0)

sys.exit(1)
