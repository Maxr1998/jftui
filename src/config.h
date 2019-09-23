#ifndef _JF_CONFIG
#define _JF_CONFIG

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>

#include "linenoise.h"

#include "shared.h"
#include "net.h"


////////// CODE MACROS //////////
#define JF_CONFIG_KEY_IS(key) strncmp(line, key, JF_STATIC_STRLEN(key)) == 0

#define JF_CONFIG_FILL_VALUE(key)					\
do {												\
	value_len = strlen(value);						\
	if (value[value_len - 1] == '\n') value_len--;	\
	g_options.key = strndup(value, value_len);		\
} while (false)

#define JF_CONFIG_WRITE_VALUE(key) fprintf(config_file, #key "=%s\n", g_options.key)
/////////////////////////////////


////////// JF_OPTIONS //////////
// defaults
#define JF_CONFIG_SSL_VERIFYHOST_DEFAULT	true
#define JF_CONFIG_CLIENT_DEFAULT			"jftui"
#define JF_CONFIG_DEVICE_DEFAULT			"PC"
#define JF_CONFIG_DEVICEID_DEFAULT			"Linux"
#define JF_CONFIG_VERSION_DEFAULT			JF_VERSION

#define JF_OPTIONS_IS_INCOMPLETE() ()

typedef struct jf_options {
	char *server;
	size_t server_len;
	char *token;
	char *userid;
	bool ssl_verifyhost;
	char *client;
	char *device;
	char deviceid[JF_CONFIG_DEVICEID_MAX_LEN + 1];
	char *version;
} jf_options;


void jf_options_clear(void);
////////////////////////////////


////////// USER CONFIGURATION //////////
char *jf_config_get_default_dir(void);
void jf_config_read(const char *config_path);
void jf_config_write(const char *config_path);
void jf_config_ask_user_login(void);
void jf_config_ask_user(void);
////////////////////////////////////////

#endif
