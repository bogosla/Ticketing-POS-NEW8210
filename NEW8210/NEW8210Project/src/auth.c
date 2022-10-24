
#include "auth.h"
#include <seos.h>
#include <cjson/cJSON.h>
#include "common.h"
#include "wireless.h"




int auth_login(void)
{
	char url[250];
	char username[250];
	char password[250];
	char buffer[4096];
	int32_t retKey;
	
	//JSON
	cJSON *response_json;
	const cJSON *success = NULL;
	const cJSON *token = NULL;
	
	/* specify URL to get */
	memset(url, 0x00, sizeof(url));
	memset(buffer, 0x00, sizeof(buffer));
	memset(username, 0x00, sizeof(username));
	memset(password, 0x00, sizeof(password));
	
	show_message("LOGIN", "PLEASE ENTER USERNAME", 0);
	retKey = kbGetString(KB_BIG_ALPHA|KB_EN_PRESETSTR, 3, 11, -1, username);
	if(retKey == KB_CANCEL)
	{
		//exit
		exit(0);
	}
	else
	{
		show_message("LOGIN", "PLEASE ENTER PASSWORD", 0);
		retKey = kbGetString(KB_BIG_ALPHA|KB_BIG_PWD, 3, 11, -1, password);
		if(retKey == KB_CANCEL)
		{
			return -1;
		}
		else
		{
		
			sprintf(url, "%s/index.php/v1/accounts/login?username=%s&password=%s", BASEURL, username, password);
			if(get_url(url, buffer)<0)
			{
				show_message("COULD NOT FETCH DATA!", buffer, 1);
				return -1;
			}
			
			response_json = cJSON_Parse(buffer);
			if(response_json == NULL)
			{
				show_message("PARSE ERROR!", cJSON_GetErrorPtr(), 1);
			}
			else
			{
				success = cJSON_GetObjectItemCaseSensitive(response_json, "success");
				if(cJSON_IsBool(success) && cJSON_IsTrue(success))
				{
					//save token to file
					token = cJSON_GetObjectItemCaseSensitive(response_json, "token");
					if(cJSON_IsString(token) && (token->valuestring != NULL))
					{
						write_to_file(TOKEN_FILE, token->valuestring);
						
						//gen serial
						generate_ticket_serial();
						auth_load_stops(token->valuestring);
					}
				}
				else
				{
					show_message("LOGIN FAILED", "INVALID MOBILE OR PASSWORD!", 1);
					return -1;
				}
			}
		}
	}
	return 0;
}

void auth_load_stops(const char *token)
{
	char url[250];
	char data[4096];
	
	memset(url, 0x00, sizeof(url));
	memset(data, 0x00, sizeof(data));
	
	sprintf(url, "%s/index.php/v1/tickets/stops?beautify=1&access-token=%s", BASEURL, token);
	if(get_url(url, data)==0)
	{
		write_to_file(STOPS_FILE, data);
	}
}
