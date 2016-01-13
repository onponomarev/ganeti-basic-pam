# ganeti-basic-pam
PAM module providing the same authorization mechanism as the default basic

This module uses a standard ganeti remote API users file.

## installation
To install the module please clone the project and run 'make install'.

If necessary, modify GANETI_PATH and PAM_LIB_PATH variables in the Makefile.

## PAM module configuration
In order to enable the PAM module one should create a configuration file in
pam.d directory (usually /etc/pam.d) with a name matching the PAM service
name used in Ganeti (ganeti-rapi is a default value).

To create a simple config that asks only ganett_basic.so for permission,
run make pamd_install. But be careful! This will overwrite an existing
ganeti-rapi config file.

In order to remove the installed config, run make pamd_clean.
