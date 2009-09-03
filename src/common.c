/*
 *
 *  oFono - Open Source Telephony
 *
 *  Copyright (C) 2008-2009  Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE
#include <string.h>
#include <ctype.h>

#include <glib.h>

#include <ofono/types.h>
#include "common.h"

struct error_entry {
	int error;
	const char *str;
};

/* 0-127 from 24.011 Annex E2
 * 127-255 23.040 Section 9.2.3.22
 * Rest are from 27.005 Section 3.2.5
 */
struct error_entry cms_errors[] = {
	{ 1,	"Unassigned number" },
	{ 8,	"Operator determined barring" },
	{ 10,	"Call barred" },
	{ 21,	"Short message transfer rejected" },
	{ 27,	"Destination out of service" },
	{ 28,	"Unindentified subscriber" },
	{ 29,	"Facility rejected" },
	{ 30,	"Unknown subscriber" },
	{ 38,	"Network out of order" },
	{ 41,	"Temporary failure" },
	{ 42,	"Congestion" },
	{ 47,	"Recources unavailable" },
	{ 50,	"Requested facility not subscribed" },
	{ 69,	"Requested facility not implemented" },
	{ 81,	"Invalid short message transfer reference value" },
	{ 95,	"Invalid message, unspecified" },
	{ 96,	"Invalid mandatory information" },
	{ 97,	"Message type non existent or not implemented" },
	{ 98,	"Message not compatible with short message protocol state" },
	{ 99,	"Information element non-existent or not implemented" },
	{ 111,	"Protocol error, unspecified" },
	{ 127,	"Internetworking error, unspecified" },
	{ 128,	"Telematic internetworking not supported" },
	{ 129,	"Short message type 0 not supported" },
	{ 130,	"Cannot replace short message" },
	{ 143,	"Unspecified TP-PID error" },
	{ 144,	"Data code scheme not supported" },
	{ 145,	"Message class not supported" },
	{ 159,	"Unspecified TP-DCS error" },
	{ 160,	"Command cannot be actioned" },
	{ 161,	"Command unsupported" },
	{ 175,	"Unspecified TP-Command error" },
	{ 176,	"TPDU not supported" },
	{ 192,	"SC busy" },
	{ 193,	"No SC subscription" },
	{ 194,	"SC System failure" },
	{ 195,	"Invalid SME address" },
	{ 196,	"Destination SME barred" },
	{ 197,	"SM Rejected-Duplicate SM" },
	{ 198,	"TP-VPF not supported" },
	{ 199,	"TP-VP not supported" },
	{ 208,	"(U)SIM SMS Storage full" },
	{ 209,	"No SMS Storage capability in SIM" },
	{ 210,	"Error in MS" },
	{ 211,	"Memory capacity exceeded" },
	{ 212,	"Sim application toolkit busy" },
	{ 213,	"SIM data download error" },
	{ 255,	"Unspecified error cause" },
	{ 300,	"ME Failure" },
	{ 301,	"SMS service of ME reserved" },
	{ 302,	"Operation not allowed" },
	{ 303,	"Operation not supported" },
	{ 304,	"Invalid PDU mode parameter" },
	{ 305,	"Invalid Text mode parameter" },
	{ 310,	"(U)SIM not inserted" },
	{ 311,	"(U)SIM PIN required" },
	{ 312,	"PH-(U)SIM PIN required" },
	{ 313,	"(U)SIM failure" },
	{ 314,	"(U)SIM busy" },
	{ 315,	"(U)SIM wrong" },
	{ 316,	"(U)SIM PUK required" },
	{ 317,	"(U)SIM PIN2 required" },
	{ 318,	"(U)SIM PUK2 required" },
	{ 320,	"Memory failure" },
	{ 321,	"Invalid memory index" },
	{ 322,	"Memory full" },
	{ 330,	"SMSC address unknown" },
	{ 331,	"No network service" },
	{ 332,	"Network timeout" },
	{ 340,	"No +CNMA expected" },
	{ 500,	"Unknown error" },
};

/* 27.007, Section 9 */
struct error_entry cme_errors[] = {
	{ 0,	"Phone failure" },
	{ 1,	"No connection to phone" },
	{ 2,	"Phone adapter link reserved" },
	{ 3,	"Operation not allowed" },
	{ 4,	"Operation not supported" },
	{ 5,	"PH_SIM PIN required" },
	{ 6,	"PH_FSIM PIN required" },
	{ 7,	"PH_FSIM PUK required" },
	{ 10,	"SIM not inserted" },
	{ 11,	"SIM PIN required" },
	{ 12,	"SIM PUK required" },
	{ 13,	"SIM failure" },
	{ 14,	"SIM busy" },
	{ 15,	"SIM wrong" },
	{ 16,	"Incorrect password" },
	{ 17,	"SIM PIN2 required" },
	{ 18,	"SIM PUK2 required" },
	{ 20,	"Memory full" },
	{ 21,	"Invalid index" },
	{ 22,	"Not found" },
	{ 23,	"Memory failure" },
	{ 24,	"Text string too long" },
	{ 25,	"Invalid characters in text string" },
	{ 26,	"Dial string too long" },
	{ 27,	"Invalid characters in dial string" },
	{ 30,	"No network service" },
	{ 31,	"Network timeout" },
	{ 32,	"Network not allowed, emergency calls only" },
	{ 40,	"Network personalization PIN required" },
	{ 41,	"Network personalization PUK required" },
	{ 42,	"Network subset personalization PIN required" },
	{ 43,	"Network subset personalization PUK required" },
	{ 44,	"Service provider personalization PIN required" },
	{ 45,	"Service provider personalization PUK required" },
	{ 46,	"Corporate personalization PIN required" },
	{ 47,	"Corporate personalization PUK required" },
	{ 48,	"PH-SIM PUK required" },
	{ 100,	"Unknown error" },
	{ 103,	"Illegal MS" },
	{ 106,	"Illegal ME" },
	{ 107,	"GPRS services not allowed" },
	{ 111,	"PLMN not allowed" },
	{ 112,	"Location area not allowed" },
	{ 113,	"Roaming not allowed in this location area" },
	{ 126,	"Operation temporary not allowed" },
	{ 132,	"Service operation not supported" },
	{ 133,	"Requested service option not subscribed" },
	{ 134,	"Service option temporary out of order" },
	{ 148,	"Unspecified GPRS error" },
	{ 149,	"PDP authentication failure" },
	{ 150,	"Invalid mobile class" },
	{ 256,	"Operation temporarily not allowed" },
	{ 257,	"Call barred" },
	{ 258,	"Phone is busy" },
	{ 259,	"User abort" },
	{ 260,	"Invalid dial string" },
	{ 261,	"SS not executed" },
	{ 262,	"SIM Blocked" },
	{ 263,	"Invalid block" },
	{ 772,	"SIM powered down" },
};

/* 24.008 Annex H */
struct error_entry ceer_errors[] = {
	{ 1,	"Unassigned number" },
	{ 3,	"No route to destination" },
	{ 6,	"Channel unacceptable" },
	{ 8,	"Operator determined barring" },
	{ 16,	"Normal call clearing" },
	{ 17,	"User busy" },
	{ 18,	"No user responding" },
	{ 19,	"User alerting, no answer" },
	{ 21,	"Call rejected" },
	{ 22,	"Number changed" },
	{ 25,	"Pre-emption" },
	{ 26,	"Non-selected user clearing" },
	{ 27,	"Destination out of order" },
	{ 28,	"Invalid number format (incomplete number)" },
	{ 29,	"Facility rejected" },
	{ 30,	"Response to STATUS ENQUIRY" },
	{ 31,	"Normal, unspecified" },
	{ 34,	"No circuit/channel available" },
	{ 38,	"Network out of order" },
	{ 41,	"Temporary failure" },
	{ 42,	"Switching equipment congestion" },
	{ 43,	"Access information discared" },
	{ 44,	"Requested circuit/channel not available" },
	{ 47,	"Resource unavailable (unspecified)" },
	{ 49,	"Quality of service unavailable" },
	{ 50,	"Requested facility not subscribed" },
	{ 55,	"Incoming calls barred within the CUG" },
	{ 57,	"Bearer capability not authorized" },
	{ 58,	"Bearar capability not presently available" },
	{ 63,	"Service or option not available, unspecified" },
	{ 65,	"Bearer service not implemented" },
	{ 68,	"ACM equal to or greater than ACMmax" },
	{ 69,	"Requested facility not implemented" },
	{ 70,	"Only restricted digital information bearer capability is available" },
	{ 79,	"Service or option not implemented, unspecified" },
	{ 81,	"Invalid transaction identifier value" },
	{ 87,	"User not member of CUG" },
	{ 88,	"Incompatible destination" },
	{ 91,	"Invalid transit network selection" },
	{ 95,	"Semantically incorrect message" },
	{ 96,	"Invalid mandatory information"},
	{ 97,	"Message type non-existent or not implemented" },
	{ 98,	"Message type not compatible with protocol state" },
	{ 99,	"Information element non-existent or not implemented" },
	{ 100,	"Conditional IE error" },
	{ 101,	"Message not compatible with protocol state" },
	{ 102,	"Recovery on timer expirty" },
	{ 111,	"Protocol error, unspecified" },
	{ 127,	"Interworking, unspecified" },
};

gboolean valid_phone_number_format(const char *number)
{
	int len = strlen(number);
	int begin = 0;
	int i;

	if (!len)
		return FALSE;

	if (number[0] == '+')
		begin = 1;

	if ((len - begin) > OFONO_MAX_PHONE_NUMBER_LENGTH)
		return FALSE;

	for (i = begin; i < len; i++) {
		if (number[i] >= '0' && number[i] <= '9')
			continue;

		if (number[i] == '*' || number[i] == '#')
			continue;

		return FALSE;
	}

	return TRUE;
}

const char *telephony_error_to_str(const struct ofono_error *error)
{
	struct error_entry *e;
	int maxentries;
	int i;

	switch (error->type) {
	case OFONO_ERROR_TYPE_CME:
		e = cme_errors;
		maxentries = sizeof(cme_errors) / sizeof(struct error_entry);
		break;
	case OFONO_ERROR_TYPE_CMS:
		e = cms_errors;
		maxentries = sizeof(cme_errors) / sizeof(struct error_entry);
		break;
	case OFONO_ERROR_TYPE_CEER:
		e = ceer_errors;
		maxentries = sizeof(ceer_errors) / sizeof(struct error_entry);
		break;
	default:
		return 0;
	}

	for (i = 0; i < maxentries; i++)
		if (e[i].error == error->error)
			return e[i].str;

	return 0;
}

int mmi_service_code_to_bearer_class(int code)
{
	int cls = 0;

	/* Teleservices according to 22.004
	 * 1 - Voice
	 * 2 - SMS
	 * 3,4,5 - Unallocated
	 * 6 - Fax
	 * 7 - All Data Async
	 * 8 - All Data Sync
	 * 12 - Voice Group
	 */

	switch (code) {
	/* 22.030: 1 to 6, 12 */
	case 10:
		cls = BEARER_CLASS_VOICE | BEARER_CLASS_FAX | BEARER_CLASS_SMS;
		break;
	/* 22.030: 1 */
	case 11:
		cls = BEARER_CLASS_VOICE;
		break;
	/* 22.030: 2-6 */
	case 12:
		cls = BEARER_CLASS_SMS | BEARER_CLASS_FAX;
		break;
	/* 22.030: 6 */
	case 13:
		cls = BEARER_CLASS_FAX;
		break;
	/* 22.030: 2 */
	case 16:
		cls = BEARER_CLASS_SMS;
		break;
	/* TODO: Voice Group Call & Broadcast VGCS & VBS */
	case 17:
	case 18:
		break;
	/* 22.030: 1, 3 to 6, 12 */
	case 19:
		cls = BEARER_CLASS_VOICE | BEARER_CLASS_FAX;
		break;

	/* 22.030: 7-11 */
	/* 22.004 only defines BS 7 (Data Sync) & BS 8 (Data Async) */
	case 20:
		cls = BEARER_CLASS_DATA_ASYNC | BEARER_CLASS_DATA_SYNC;
		break;
	/* According to 22.030: All Async */
	case 21:
	/* According to 22.030: All Data Async */
	case 25:
		cls = BEARER_CLASS_DATA_ASYNC;
		break;
	/* According to 22.030: All Sync */
	case 22:
	/* According to 22.030: All Data Sync */
	case 24:
		cls = BEARER_CLASS_DATA_SYNC;
		break;
	/* According to 22.030: Telephony & All Sync services */
	case 26:
		cls = BEARER_CLASS_VOICE | BEARER_CLASS_DATA_SYNC;
		break;
	default:
		break;
	}

	return cls;
}

const char *phone_number_to_string(const struct ofono_phone_number *ph)
{
	static char buffer[64];

	if (ph->type == 145 && (strlen(ph->number) > 0) &&
			ph->number[0] != '+') {
		buffer[0] = '+';
		strncpy(buffer + 1, ph->number, 62);
		buffer[63] = '\0';
	} else {
		strncpy(buffer, ph->number, 63);
		buffer[63] = '\0';
	}

	return buffer;
}

void string_to_phone_number(const char *str, struct ofono_phone_number *ph)
{
	if (strlen(str) && str[0] == '+') {
		strcpy(ph->number, str+1);
		ph->type = 145;	/* International */
	} else {
		strcpy(ph->number, str);
		ph->type = 129;	/* Local */
	}
}

int valid_ussd_string(const char *str)
{
	int len = strlen(str);

	if (!len)
		return FALSE;

	/* It is hard to understand exactly what constitutes a valid USSD string
	 * According to 22.090:
	 * Case a - 1, 2 or 3 digits from the set (*, #) followed by 1X(Y),
	 * where X=any number 0‑4, Y=any number 0‑9, then, optionally "*
	 * followed by any number of any characters", and concluding with #SEND
	 *
	 * Case b - 1, 2 or 3 digits from the set (*, #) followed by 1X(Y),
	 * where X=any number 5‑9, Y=any number 0‑9, then, optionally "*
	 * followed by any number of any characters", and concluding with #SEND
	 *
	 * Case c - 7(Y) SEND, where Y=any number 0‑9
	 *
	 * Case d - All other formats
	 *
	 * According to 22.030 Figure 3.5.3.2 USSD strings can be:
	 *
	 * Supplementary service control
	 * SIM control
	 * Manufacturer defined
	 * Terminated by '#'
	 * Short String - This can be any 2 digit short string.  If the string
	 *                starts with a '1' and no calls are in progress then
	 *                this string is treated as a call setup request
	 *
	 * Everything else is not a valid USSD string
	 */

	if (len != 2 && str[len-1] != '#')
		return FALSE;

	return TRUE;
}

const char *ss_control_type_to_string(enum ss_control_type type)
{
	switch (type) {
	case SS_CONTROL_TYPE_ACTIVATION:
		return "acivation";
	case SS_CONTROL_TYPE_REGISTRATION:
		return "registration";
	case SS_CONTROL_TYPE_QUERY:
		return "interrogation";
	case SS_CONTROL_TYPE_DEACTIVATION:
		return "deactivation";
	case SS_CONTROL_TYPE_ERASURE:
		return "erasure";
	}

	return NULL;
}

#define NEXT_FIELD(str, dest)			\
	do {					\
		dest = str;			\
						\
		str = strchrnul(str, '*');	\
		if (*str) {			\
			*str = '\0';		\
			str += 1;		\
		}				\
	} while (0)				\

/* Note: The str will be modified, so in case of error you should
 * throw it away and start over
 */
gboolean parse_ss_control_string(char *str, int *ss_type,
					char **sc, char **sia,
					char **sib, char **sic,
					char **sid, char **dn)
{
	int len = strlen(str);
	int cur = 0;
	char *c;
	unsigned int i;
	gboolean ret = FALSE;

	/* Minimum is {*,#}SC# */
	if (len < 4)
		goto out;

	if (str[0] != '*' && str[0] != '#')
		goto out;

	cur = 1;

	if (str[1] != '*' && str[1] != '#' && str[1] > '9' && str[1] < '0')
		goto out;

	if (str[0] == '#' && str[1] == '*')
		goto out;

	if (str[1] == '#' || str[1] == '*')
		cur = 2;

	if (str[0] == '*' && str[1] == '*')
		*ss_type = SS_CONTROL_TYPE_REGISTRATION;
	else if (str[0] == '#' && str[1] == '#')
		*ss_type = SS_CONTROL_TYPE_ERASURE;
	else if (str[0] == '*' && str[1] == '#')
		*ss_type = SS_CONTROL_TYPE_QUERY;
	else if (str[0] == '*')
		*ss_type = SS_CONTROL_TYPE_ACTIVATION;
	else
		*ss_type = SS_CONTROL_TYPE_DEACTIVATION;

	/* Must have at least one other '#' */
	c = strrchr(str+cur, '#');

	if (!c)
		goto out;

	*dn = c+1;
	*c = '\0';

	if (strlen(*dn) > 0 && !valid_phone_number_format(*dn))
		goto out;

	c = str+cur;

	NEXT_FIELD(c, *sc);

	/* According to 22.030 SC is 2 or 3 digits, there can be
	 * an optional digit 'n' if this is a call setup string,
	 * however 22.030 does not define any SC of length 3
	 * with an 'n' present
	 */
	if (strlen(*sc) < 2 || strlen(*sc) > 3)
		goto out;

	for (i = 0; i < strlen(*sc); i++)
		if (!isdigit((*sc)[i]))
			goto out;

	NEXT_FIELD(c, *sia);
	NEXT_FIELD(c, *sib);
	NEXT_FIELD(c, *sic);
	NEXT_FIELD(c, *sid);

	if (*c == '\0')
		ret = TRUE;

out:
	return ret;
}

static const char *bearer_class_lut[] = {
	"Voice",
	"Data",
	"Fax",
	"Sms",
	"DataSync",
	"DataAsync",
	"DataPad",
	"DataPacket"
};

const char *bearer_class_to_string(enum bearer_class cls)
{
	switch (cls) {
	case BEARER_CLASS_VOICE:
		return bearer_class_lut[0];
	case BEARER_CLASS_DATA:
		return bearer_class_lut[1];
	case BEARER_CLASS_FAX:
		return bearer_class_lut[2];
	case BEARER_CLASS_SMS:
		return bearer_class_lut[3];
	case BEARER_CLASS_DATA_SYNC:
		return bearer_class_lut[4];
	case BEARER_CLASS_DATA_ASYNC:
		return bearer_class_lut[5];
	case BEARER_CLASS_PACKET:
		return bearer_class_lut[6];
	case BEARER_CLASS_PAD:
		return bearer_class_lut[7];
	case BEARER_CLASS_DEFAULT:
	case BEARER_CLASS_SS_DEFAULT:
		break;
	};

	return NULL;
}

gboolean is_valid_pin(const char *pin)
{
	unsigned int i;

	for (i = 0; i < strlen(pin); i++)
		if (pin[i] < '0' || pin[i] > '9')
			return FALSE;

	if (i > 8)
		return FALSE;

	return TRUE;
}
