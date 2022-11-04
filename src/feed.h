/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "error.h"
#include "http_communication.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

int get_and_print_feed(char *host, char *port, char *path, int is_testing);
