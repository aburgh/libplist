//
//  print.c
//  libplist
//
//  Created by Aaron Burghardt on 8/25/14.
//
//

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <plist/plist.h>


static const char base64_str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char base64_pad = '=';

static char *base64encode(const unsigned char *buf, size_t size)
{
	if (!buf || !(size > 0)) return NULL;
	int outlen = (size / 3) * 4;
	char *outbuf = (char*)malloc(outlen+5); // 4 spare bytes + 1 for '\0'
	size_t n = 0;
	size_t m = 0;
	unsigned char input[3];
	unsigned int output[4];
	while (n < size) {
		input[0] = buf[n];
		input[1] = (n+1 < size) ? buf[n+1] : 0;
		input[2] = (n+2 < size) ? buf[n+2] : 0;
		output[0] = input[0] >> 2;
		output[1] = ((input[0] & 3) << 4) + (input[1] >> 4);
		output[2] = ((input[1] & 15) << 2) + (input[2] >> 6);
		output[3] = input[2] & 63;
		outbuf[m++] = base64_str[(int)output[0]];
		outbuf[m++] = base64_str[(int)output[1]];
		outbuf[m++] = (n+1 < size) ? base64_str[(int)output[2]] : base64_pad;
		outbuf[m++] = (n+2 < size) ? base64_str[(int)output[3]] : base64_pad;
		n+=3;
	}
	outbuf[m] = 0; // 0-termination!
	return outbuf;
}

static void plist_node_print_to_stream(plist_t node, int* indent_level, FILE* stream);

static void plist_array_print_to_stream(plist_t node, int* indent_level, FILE* stream)
{
	/* iterate over items */
	int i, count;
	plist_t subnode = NULL;

	count = plist_array_get_size(node);

	for (i = 0; i < count; i++) {
		subnode = plist_array_get_item(node, i);
		fprintf(stream, "%*s", *indent_level, "");
		fprintf(stream, "%d: ", i);
		plist_node_print_to_stream(subnode, indent_level, stream);
	}
}

static void plist_dict_print_to_stream(plist_t node, int* indent_level, FILE* stream)
{
	/* iterate over key/value pairs */
	plist_dict_iter it = NULL;

	char* key = NULL;
	plist_t subnode = NULL;
	plist_dict_new_iter(node, &it);
	plist_dict_next_item(node, it, &key, &subnode);
	while (subnode)
	{
		fprintf(stream, "%*s", *indent_level, "");
		fprintf(stream, "%s", key);
		if (plist_get_node_type(subnode) == PLIST_ARRAY)
			fprintf(stream, "[%d]: ", plist_array_get_size(subnode));
		else
			fprintf(stream, ": ");
		free(key);
		key = NULL;
		plist_node_print_to_stream(subnode, indent_level, stream);
		plist_dict_next_item(node, it, &key, &subnode);
	}
	free(it);
}

static void plist_node_print_to_stream(plist_t node, int* indent_level, FILE* stream)
{
	char *s = NULL;
	char *data = NULL;
	double d;
	uint8_t b;
	uint64_t u = 0;
	struct timeval tv = { 0, 0 };

	plist_type t;

	if (!node)
		return;

	t = plist_get_node_type(node);

	switch (t) {
		case PLIST_BOOLEAN:
			plist_get_bool_val(node, &b);
			fprintf(stream, "%s\n", (b ? "true" : "false"));
			break;

		case PLIST_UINT:
			plist_get_uint_val(node, &u);
			fprintf(stream, "%"PRIu64"\n", u);
			break;

		case PLIST_REAL:
			plist_get_real_val(node, &d);
			fprintf(stream, "%f\n", d);
			break;

		case PLIST_STRING:
			plist_get_string_val(node, &s);
			fprintf(stream, "%s\n", s);
			free(s);
			break;

		case PLIST_KEY:
			plist_get_key_val(node, &s);
			fprintf(stream, "%s: ", s);
			free(s);
			break;

		case PLIST_DATA:
			plist_get_data_val(node, &data, &u);
			if (u > 0) {
				s = base64encode((unsigned char*)data, u);
				free(data);
				if (s) {
					fprintf(stream, "%s\n", s);
					free(s);
				} else {
					fprintf(stream, "\n");
				}
			} else {
				fprintf(stream, "\n");
			}
			break;

		case PLIST_DATE:
			plist_get_date_val(node, (int32_t*)&tv.tv_sec, (int32_t*)&tv.tv_usec);
		{
			time_t ti = (time_t)tv.tv_sec;
			struct tm *btime = localtime(&ti);
			if (btime) {
				s = (char*)malloc(24);
				memset(s, 0, 24);
				if (strftime(s, 24, "%Y-%m-%dT%H:%M:%SZ", btime) <= 0) {
					free (s);
					s = NULL;
				}
			}
		}
			if (s) {
				fprintf(stream, "%s\n", s);
				free(s);
			} else {
				fprintf(stream, "\n");
			}
			break;

		case PLIST_ARRAY:
			fprintf(stream, "\n");
			(*indent_level)++;
			plist_array_print_to_stream(node, indent_level, stream);
			(*indent_level)--;
			break;

		case PLIST_DICT:
			fprintf(stream, "\n");
			(*indent_level)++;
			plist_dict_print_to_stream(node, indent_level, stream);
			(*indent_level)--;
			break;

		default:
			break;
	}
}

void plist_print_to_stream(plist_t plist, FILE* stream)
{
	int indent = 0;

	if (!plist || !stream)
		return;

	switch (plist_get_node_type(plist)) {
		case PLIST_DICT:
			plist_dict_print_to_stream(plist, &indent, stream);
			break;
		case PLIST_ARRAY:
			plist_array_print_to_stream(plist, &indent, stream);
			break;
		default:
			plist_node_print_to_stream(plist, &indent, stream);
	}
}
