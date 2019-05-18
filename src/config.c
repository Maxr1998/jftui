#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "shared.h"


// NB return value will need to be free'd
const char *jf_config_get_path(void)
{
	char *str;
	if ((str = getenv("XDG_CONFIG_HOME")) == NULL) {
		str = jf_concat(2, getenv("HOME"), "/.config/jftui");
	} else {
		str = jf_concat(2, str, "/jftui");
	}
	return str;
}


// TODO: better error handling
jf_options *jf_config_read(const char *config_path)
{
	FILE *config_file;
	char *line;
	size_t line_size = 1024;
	char *value;
	size_t value_len;
	jf_options *opts;

	if ((opts = malloc(sizeof(jf_options))) == NULL) {
		return NULL;
	}
	*opts = (jf_options){ 0 }; // correctly initialize to empty, will NULL pointers

	if ((line = malloc(line_size)) == NULL) {
		free(opts);
		return NULL;
	}

	if ((config_file = fopen(config_path, "r")) == NULL) {
		free(opts);
		return NULL;
	}

	// read from file
	while (getline(&line, &line_size, config_file) != -1) {
		// allow comments
		if (line[0] == '#') continue;
		if ((value = strchr(line, '=')) == NULL) {
			// malformed line, consider fatal and return NULL
			JF_CONFIG_MALFORMED
		}
		value += 1; // digest '='
		// figure out which option key it is
		if JF_CONFIG_KEY_IS("server") {
			JF_CONFIG_FILL_VALUE(server);
			opts->server_len = value_len;
		} else if JF_CONFIG_KEY_IS("token") {
			JF_CONFIG_FILL_VALUE(token);
		} else if JF_CONFIG_KEY_IS("user") {
			JF_CONFIG_FILL_VALUE(user);
		} else if JF_CONFIG_KEY_IS("ssl_verifyhost") {
			if (strncmp(value, "false", sizeof("false")) == 0) opts->ssl_verifyhost = 0;
		} else if JF_CONFIG_KEY_IS("client") {
			JF_CONFIG_FILL_VALUE(client);
		} else if JF_CONFIG_KEY_IS("device") {
			JF_CONFIG_FILL_VALUE(device);
		} else if JF_CONFIG_KEY_IS("deviceid") {
			JF_CONFIG_FILL_VALUE(deviceid);
		} else if JF_CONFIG_KEY_IS("version") {
			JF_CONFIG_FILL_VALUE(version);
		} else {
			// unrecognized option key, consider fatal and return NULL
			JF_CONFIG_MALFORMED
		}
	}

	// apply defaults for values unread
	// TODO: missing some

	free(line);
	fclose(config_file);

	return opts;
}


