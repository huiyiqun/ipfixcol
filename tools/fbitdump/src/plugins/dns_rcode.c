#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "plugin_header.h"

static char *messages[] = {
	"NoError",
	"FormErr",
	"ServFail",
	"NXDomain",
	"NotImp",
	"Refused",
	"YXDomain",
	"YXRRSet",
	"NXRRSet",
	"NotAuth",
	"NotZone",
	"",
	"",
	"",
	"",
	"",
	"BADVERS/BADSIG",
	"BADKEY",
	"BADTIME",
	"BADMODE",
	"BADNAME",
	"BADALG",
	"BADTRUNC"
};

#define MSG_CNT (sizeof(messages) / sizeof(messages[0]))

char *info()
{
	return \
"Converts DNS RCODE name to value and vice versa.\n \
e.g. \"BADKEY\" -> 17";
}


void format( const union plugin_arg * arg, int plain_numbers, char out[PLUGIN_BUFFER_SIZE])
{
	char *str = NULL;
	char num[PLUGIN_BUFFER_SIZE];

	if (plain_numbers) {
		snprintf(out, PLUGIN_BUFFER_SIZE, "%u", arg[0].uint8);
		return;
	}
	
	int size = MSG_CNT;
	
	if (arg[0].uint8 < size) {
		str = messages[arg[0].uint8];
	}
	
	if (arg[0].uint8 >= size || strlen(str) == 0) {
		/* out of bounds or unassigned rcode */
		snprintf(num, PLUGIN_BUFFER_SIZE, "%u", arg[0].uint8);
		str = num;
	}

	snprintf(out, PLUGIN_BUFFER_SIZE, "%s", str);
}

void parse(char *input, char out[PLUGIN_BUFFER_SIZE])
{
	int code, size = MSG_CNT;
	
	for (code = 0; code < size; ++code) {
		if (!strcasecmp(input, messages[code])) {
			break;
		}
	}
	
	if (code == size) {
		/* BADSIG and BADVERS have the same code */
		if (!strcasecmp(input, "BADSIG") || !strcasecmp(input, "BADVERS")) {
			code = 16;
		} else {
			snprintf(out, PLUGIN_BUFFER_SIZE, "");
			return;
		}
	}
	
	snprintf(out, PLUGIN_BUFFER_SIZE, "%d", code);
}
