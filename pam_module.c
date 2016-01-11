#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <security/pam_modules.h>
#include <security/pam_appl.h>

static char password_prompt[] = "Password:";

#define MAX_STR_LENGTH 100000
#define PAM_ENV_URI "GANETI_RAPI_URI"
#define PAM_ENV_BODY "GANETI_REQUEST_BODY"
#define PAM_ENV_METHOD "GANETI_REQUEST_METHOD"
#define PAM_ENV_ACCESS "GANETI_RESOURCE_ACCESS"
#define PAM_ENV_USER "GANETI_RAPI_USER"
#define PAM_ENV_PASSWORD "GANETI_RAPI_PASSWORD"
#define PAM_ENV_AUTHTOK "GANETI_RAPI_AUTHTOK"

#define STR(str) (str) ? (str) : ""

int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc,
                        const char *argv[]) {

  int pam_err = PAM_SUCCESS;
  const char *user = NULL;
  if ((pam_err = pam_get_user(pamh, &user, NULL)) != PAM_SUCCESS)
    return pam_err;

  const void *ptr = NULL;
  pam_err = pam_get_item(pamh, PAM_CONV, &ptr);
  if (pam_err != PAM_SUCCESS)
    return PAM_SYSTEM_ERR;

  const struct pam_conv *conv = ptr;
  struct pam_message msg = {};
  msg.msg_style = PAM_PROMPT_ECHO_OFF;
  msg.msg = password_prompt;
  const struct pam_message *msgp = &msg;
  struct pam_response *resp = NULL;
  pam_err = (*conv->conv)(1, &msgp, &resp, conv->appdata_ptr);
  char *password = NULL;
  if (resp != NULL) {
    if (pam_err == PAM_SUCCESS)
      password = resp->resp;
    else
      free(resp->resp);
    free(resp);
  }
  if (pam_err == PAM_CONV_ERR)
    return pam_err;
  if (pam_err != PAM_SUCCESS)
    return PAM_AUTH_ERR;

  const char *authtok = NULL;
  pam_get_item(pamh, PAM_AUTHTOK, (const void**) &authtok);

  char buffer[MAX_STR_LENGTH * 4] = {};
  sprintf(buffer, "%s='%s' %s='%s' %s='%s' GANETI_PATH='%s'"
          " python " PAM_LIB_PATH "/ganeti_basic/authenticate.py",
          PAM_ENV_USER, STR(user), PAM_ENV_PASSWORD, STR(password),
          PAM_ENV_AUTHTOK, STR(authtok), GANETI_PATH);

  if (system(buffer))
    return PAM_AUTH_ERR;
  return PAM_SUCCESS;
}

int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc,
                   const char *argv[]) {
  return PAM_SUCCESS;
}

int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc,
                     const char *argv[]) {
  
  const char *user = NULL;
  int pam_err = PAM_SUCCESS;
  if ((pam_err = pam_get_user(pamh, &user, NULL)) != PAM_SUCCESS)
    return pam_err;
  
  const char *uri = pam_getenv(pamh, PAM_ENV_URI);
  const char *body = pam_getenv(pamh, PAM_ENV_BODY);
  const char *method = pam_getenv(pamh, PAM_ENV_METHOD);
  const char *access = pam_getenv(pamh, PAM_ENV_ACCESS);
  
  char buffer[MAX_STR_LENGTH * 6] = {};
  sprintf(buffer, "%s='%s' %s='%s' %s='%s' %s='%s' %s='%s', GANETI_PATH='%s'"
          " python " PAM_LIB_PATH "/ganeti_basic/authorize.py",
          PAM_ENV_USER, STR(user), PAM_ENV_URI, STR(uri),
          PAM_ENV_BODY, STR(body), PAM_ENV_METHOD, STR(method),
          PAM_ENV_ACCESS, STR(access), GANETI_PATH);

  if (system(buffer))
    return PAM_AUTH_ERR;
  return PAM_SUCCESS;
}

int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc,
                    const char *argv[]) {
  return PAM_SUCCESS;
}

int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc,
                         const char *argv[]) {
  return PAM_SUCCESS;
}

int pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc,
                     const char *argv[]) {
  return PAM_SERVICE_ERR;
}

