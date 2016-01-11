CC=gcc
LDFLAGS=-fPIC -shared
LIBRARIES=-lpam

# Path to the pam libraries
PAM_LIB_PATH=/lib/security

all: ganeti_basic.so

ganeti_basic.so:
	$(CC) $(LDFLAGS) pam_module.c -o ganeti_basic.so $(LIBRARIES)

install: ganeti_basic.so
	cp ganeti_basic.so $(PAM_LIB_PATH)
	mkdir -p $(PAM_LIB_PATH)/ganeti_basic
	cp authenticate.py $(PAM_LIB_PATH)/ganeti_basic
	cp authorize.py $(PAM_LIB_PATH)/ganeti_basic

clean:
	rm -f ganeti_basic.so $(PAM_LIB_PATH)/ganetic_basic.so
	rm -rf $(PAM_LIB_PATH)/ganeti_basic
