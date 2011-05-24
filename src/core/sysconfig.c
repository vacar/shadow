/**
 * The Shadow Simulator
 *
 * Copyright (c) 2010-2011 Rob Jansen <jansen@cs.umn.edu>
 * Copyright (c) 2006-2009 Tyson Malchow <tyson.malchow@gmail.com>
 *
 * This file is part of Shadow.
 *
 * Shadow is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Shadow is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Shadow.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "sysconfig.h"
#include "hashtable.h"

sysconfig_val_t sysconfig_defaults[] = {
		{"sim_nodetrack_hashsize", SYSCONFIG_INT, {.int_val=128}},
		{"sim_nodetrack_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"sim_modtrack_hashsize", SYSCONFIG_INT, {.int_val=128}},
		{"sim_modtrack_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"sim_nettrack_hashsize", SYSCONFIG_INT, {.int_val=128}},
		{"sim_nettrack_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"simnet_graph_hashsize", SYSCONFIG_INT, {.int_val=10}},
		{"simnet_graph_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"vci_network_hashsize", SYSCONFIG_INT, {.int_val=128}},
		{"vci_network_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"resolver_hashsize", SYSCONFIG_INT, {.int_val=128}},
		{"resolver_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"vci_remote_node_netmap_hashsize", SYSCONFIG_INT, {.int_val=128}},
		{"vci_remote_node_netmap_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"vci_mailbox_hashsize", SYSCONFIG_INT, {.int_val=65536}},
		{"vci_mailbox_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"vci_rcnn_hashsize", SYSCONFIG_INT, {.int_val=32}},
		{"vci_rcnn_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"vci_evtracker_size", SYSCONFIG_INT, {.int_val=65536}},
		{"vci_evtracker_granularity", SYSCONFIG_INT, {.int_val=1}},

		{"dtimer_evtracker_size", SYSCONFIG_INT, {.int_val=65536}},
		{"dtimer_evtracker_granularity", SYSCONFIG_INT, {.int_val=1}},

		{"dtimer_tset_hashsize", SYSCONFIG_INT, {.int_val=65536}},
		{"dtimer_tset_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"pipecloud_pp_size", SYSCONFIG_INT, {.int_val=131072}},

		{"event_tracker_size", SYSCONFIG_INT, {.int_val=65536}},
		{"event_tracker_granularity", SYSCONFIG_INT, {.int_val=1}},

		{"max_workers_per_slave", SYSCONFIG_INT, {.int_val=8}},

		/* force prevents vnetwork from adjusting size (autotuning) based on delay-bandwidth-product. see man tcp. */
		{"vnetwork_send_buffer_size_force", SYSCONFIG_INT, {.int_val=0}},
		{"vnetwork_send_buffer_size", SYSCONFIG_INT, {.int_val=131072}},
		{"vnetwork_recv_buffer_size", SYSCONFIG_INT, {.int_val=174760}},

		/* are we using shmemory cabinets as opposed to a pipecloud for IPC transfers */
		{"vnetwork_use_shmcabinet", SYSCONFIG_INT, {.int_val=1}},

		{"vpacketmgr_packets_per_shmcabinet", SYSCONFIG_INT, {.int_val=100}},
		{"vpacketmgr_packets_threshold_shmcabinet", SYSCONFIG_INT, {.int_val=10}},

		/* if using shmcabinets, possible types "custom", "pthread", "semaphore" */
		{"vpacketmgr_packets_cabinet_lock_type", SYSCONFIG_STRING, {.string_val=SYSCONFIG_LOCK_STR_CUSTOM}},
		{"vpacketmgr_packets_slot_lock_type", SYSCONFIG_STRING, {.string_val=SYSCONFIG_LOCK_STR_CUSTOM}},

		{"vpacketmgr_payloads_per_shmcabinet", SYSCONFIG_INT, {.int_val=100}},
		{"vpacketmgr_payloads_threshold_shmcabinet", SYSCONFIG_INT, {.int_val=10}},

		/* if using shmcabinets, possible types "custom", "pthread", "semaphore" */
		{"vpacketmgr_payloads_cabinet_lock_type", SYSCONFIG_STRING, {.string_val=SYSCONFIG_LOCK_STR_CUSTOM}},
		{"vpacketmgr_payloads_slot_lock_type", SYSCONFIG_STRING, {.string_val=SYSCONFIG_LOCK_STR_CUSTOM}},

		/* if not using shmcabinets, should packets be locked? */
		{"vpacketmgr_lock_regular_mem_packets", SYSCONFIG_INT, {.int_val=1}},

		/* if not using shmcabinets and we shuold lock packets, packets are locked with these locks */
		{"vpacketmgr_packets_lock_type", SYSCONFIG_STRING, {.string_val=SYSCONFIG_LOCK_STR_CUSTOM}},
		{"vpacketmgr_payloads_lock_type", SYSCONFIG_STRING, {.string_val=SYSCONFIG_LOCK_STR_CUSTOM}},

		{"vtcpserver_incomplete_hashsize", SYSCONFIG_INT, {.int_val=10}},
		{"vtcpserver_incomplete_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"vtcpserver_pending_hashsize", SYSCONFIG_INT, {.int_val=10}},
		{"vtcpserver_pending_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"vtcpserver_accepted_hashsize", SYSCONFIG_INT, {.int_val=10}},
		{"vtcpserver_accepted_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"vsockets_hashsize", SYSCONFIG_INT, {.int_val=10}},
		{"vsockets_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"vsocket_tcp_hashsize", SYSCONFIG_INT, {.int_val=10}},
		{"vsocket_tcp_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"vsocket_udp_hashsize", SYSCONFIG_INT, {.int_val=10}},
		{"vsocket_udp_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"vsocket_tcpserver_hashsize", SYSCONFIG_INT, {.int_val=10}},
		{"vsocket_tcpserver_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"vsocket_destroyed_descriptors_hashsize", SYSCONFIG_INT, {.int_val=10}},
		{"vsocket_destroyed_descriptors_hashgrowth", SYSCONFIG_FLOAT, {.float_val=0.9f}},

		{"use_wallclock_startup_time_offset", SYSCONFIG_INT, {.int_val=0}},

		{"loglevel", SYSCONFIG_STRING, {.string_val=SYSCONFIG_LOGLEVEL_STRING}},

		{"do_intercept_crypto", SYSCONFIG_INT, {.int_val=1}},

		{""}
};

sysconfig_t sysconfig;

void sysconfig_init(void)  {
	int i = 0;
	sysconfig.data = g_hash_table_new(g_int_hash, g_int_equal);

	/* load up all the defaults */
	while (sysconfig_defaults[i].name[0]) {
		switch(sysconfig_defaults[i].type) {
			case SYSCONFIG_INT:
				sysconfig_set_int(sysconfig_defaults[i].name, sysconfig_defaults[i].v.int_val);
				break;

			case SYSCONFIG_FLOAT:
				sysconfig_set_float(sysconfig_defaults[i].name, sysconfig_defaults[i].v.float_val);
				break;

			case SYSCONFIG_STRING:
				sysconfig_set_string(sysconfig_defaults[i].name, sysconfig_defaults[i].v.string_val);
				break;
		}
		i++;
	}
}

int sysconfig_get_int(char * param) {
        int key = adler32_hash(param);
	sysconfig_val_tp v = g_hash_table_lookup(sysconfig.data, &key);
	int rv = 0;

	if(!v)
		return 0;

	switch(v->type) {
		case SYSCONFIG_INT:
			rv = v->v.int_val;
			break;
		case SYSCONFIG_FLOAT:
			rv = ((int)v->v.float_val);
			break;
	}

	return rv;
}
float sysconfig_get_float(char * param) {
        int key = adler32_hash(param);
	sysconfig_val_tp v = g_hash_table_lookup(sysconfig.data, &key);
	float rv = 0.0f;

	if(!v)
		return 0;

	switch(v->type) {
		case SYSCONFIG_INT:
			rv = ((float)v->v.int_val);
			break;
		case SYSCONFIG_FLOAT:
			rv = v->v.float_val;
			break;
	}

	return rv;
}
char * sysconfig_get_string(char * param) {
	static char temp[64];
        int key = adler32_hash(param);
	sysconfig_val_tp v = g_hash_table_lookup(sysconfig.data, &key);
	char * rv = "";

	if(!v)
		return 0;

	switch(v->type) {
		case SYSCONFIG_INT:
			sprintf(temp, "%d", v->v.int_val);
			rv = temp;
			break;
		case SYSCONFIG_FLOAT:
			sprintf(temp, "%f", v->v.float_val);
			rv = temp;
			break;
		case SYSCONFIG_STRING:
			rv = v->v.string_val;
			break;
	}

	return rv;
}

void sysconfig_set_int(char * param, int v) {
	int key = adler32_hash(param);
	sysconfig_val_tp val = g_hash_table_lookup(sysconfig.data, &key);

	if(!val) {
		val = malloc(sizeof(*val));
		strncpy(val->name, param, sizeof(val->name)); val->name[sizeof(val->name)-1] = 0;
		g_hash_table_insert(sysconfig.data, int_key(key), val);
	}

	val->type = SYSCONFIG_INT;
	val->v.int_val = v;
}

void sysconfig_set_string(char * param, char * v) {
	int key = adler32_hash(param);
	sysconfig_val_tp val = g_hash_table_lookup(sysconfig.data, &key);

	if(!val) {
		val = malloc(sizeof(*val));
		strncpy(val->name, param, sizeof(val->name)); val->name[sizeof(val->name)-1] = 0;
		g_hash_table_insert(sysconfig.data, int_key(key), val);
	}

	strncpy(val->v.string_val, v, sizeof(val->v.string_val));
	val->v.string_val[sizeof(val->v.string_val)-1] = 0;
	val->type = SYSCONFIG_STRING;
}

void sysconfig_set_float(char * param, float v) {
	int key = adler32_hash(param);
	sysconfig_val_tp val = g_hash_table_lookup(sysconfig.data, &key);

	if(!val) {
		val = malloc(sizeof(*val));
		strncpy(val->name, param, sizeof(val->name)); val->name[sizeof(val->name)-1] = 0;
		g_hash_table_insert(sysconfig.data, int_key(key), val);
	}

	val->v.float_val = v;
	val->type = SYSCONFIG_FLOAT;
}

int sysconfig_determine_type(char * str) {
	char dot_found = 0;
	char * curletter = str;
	int len;

	while(*curletter) {
		if(*curletter == '.') {
			if(dot_found)
				return SYSCONFIG_STRING;
			dot_found  = 1;

		} else if(!isdigit(*curletter))
			return SYSCONFIG_STRING;
		curletter++;
	}

	if(dot_found) {
		if((len=strlen(str)) > 1 && str[len-1]!='.')
			return SYSCONFIG_FLOAT;
		else
			return SYSCONFIG_STRING;
	} else
		return SYSCONFIG_INT;
}

void sysconfig_import_config(char * in_config_data) {
	/* woohoo */
	static char line_delims[] = "\n\r";
	static char name_delims[] = " \t\r\n";
	char * config_data, * tok, * nameptr, * valptr;
	char * full_ptr, * line_ptr;
	int len, val_len;

	if(!in_config_data)
		return;

	len = strlen(in_config_data);
	config_data = malloc(len + 1);
	strcpy(config_data, in_config_data);

	tok = strtok_r(config_data, line_delims, &full_ptr);
	while(tok) {
		nameptr = strtok_r(tok, name_delims, &line_ptr);
		valptr = strtok_r(NULL, line_delims, &line_ptr);

		/* remove trailing \t and spaces from val .. */
		while(*valptr&&(*valptr==' '||*valptr=='\t')) valptr++;
		val_len = strlen(valptr);
		while(val_len > 0 && (valptr[val_len-1] == ' ' || valptr[val_len-1] == '\t'))
			valptr[--val_len] = 0;

		if(strlen(valptr) == 0)
			continue;

		/* figure out what the value is, then save it to the configuration */
		switch(sysconfig_determine_type(valptr)) {
			case SYSCONFIG_INT:
				sysconfig_set_int(nameptr, atoi(valptr));
				break;
			case SYSCONFIG_FLOAT:
				sysconfig_set_float(nameptr, atof(valptr));
				break;
			case SYSCONFIG_STRING:
				sysconfig_set_string(nameptr, valptr);
				break;
		}

		tok = strtok_r(NULL, line_delims, &full_ptr);
	}

	free(config_data);

	return;
}

void sysconfig_export_walk(int key, void * d, void *param) {
	static char temp_buffer[512];

	sysconfig_val_tp val = d;
	if(val) {
		switch(val->type) {
			case SYSCONFIG_INT:
				sysconfig.exported_config_size += snprintf(temp_buffer, sizeof(temp_buffer), "%-40s %d\n", val->name, val->v.int_val);
				break;
			case SYSCONFIG_FLOAT:
				sysconfig.exported_config_size += snprintf(temp_buffer, sizeof(temp_buffer), "%-40s %f\n", val->name, val->v.float_val);
				break;
			case SYSCONFIG_STRING:
				sysconfig.exported_config_size += snprintf(temp_buffer, sizeof(temp_buffer), "%-40s %s\n", val->name, val->v.string_val);
				break;
		}
		temp_buffer[sizeof(temp_buffer) - 1] = 0;
		strncat(sysconfig.exported_config, temp_buffer, sizeof(sysconfig.exported_config) - sysconfig.exported_config_size - 1);
		sysconfig.exported_config[sizeof(sysconfig.exported_config) - 1] = 0;
	}
}

char * sysconfig_export_config(void) {
	sysconfig.exported_config_size = 0;
	sysconfig.exported_config[0] = 0;
	g_hash_table_foreach(sysconfig.data, (GHFunc)sysconfig_export_walk, NULL);
	return sysconfig.exported_config;
}

void sysconfig_destroy_cb(int key, void* value, void *param) {
	free(value);
}

void sysconfig_cleanup(void) {
	g_hash_table_foreach(sysconfig.data, (GHFunc)sysconfig_destroy_cb, NULL);
	g_hash_table_destroy(sysconfig.data);
}