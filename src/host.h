/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include <regex.h>
#include <stdint.h>

#include "error.h"

#define REGEX_URL "^(https?):\\/\\/((www\\.)?[a-zA-Z0-9]+([-\\.]{1}[a-zA-Z0-9]+)*\\.([a-z]{2,5}))(:([1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5]))?(\\/.*)?$"

extern char **hosts;
extern char **paths;
extern char **ports;
extern int destinations_counter;
extern int *secure;

void init_destinations(int dest_count);
void clear_destinations();
int parse_url (char *url, int is_getting_data, int is_testing);
int parse_url_from_file(char *file_name, int is_testing);
