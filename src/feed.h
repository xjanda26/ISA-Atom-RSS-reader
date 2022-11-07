/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "error.h"
#include "http_communication.h"
#include "https_communication.h"
#include "parameters.h"
#include "tcp_communication.h"

#define AUTHOR "Autor"
#define UPDATED_AT "Aktualizace"
#define ENTRY_URI "URI"

void process_author_node(xmlDocPtr doc, xmlNodePtr authorNode);
void process_entry_node(xmlDocPtr doc, xmlNodePtr node);
void process_feed_title_node(xmlDocPtr doc, xmlNodePtr node);
int process_feed_node(xmlDocPtr doc, xmlNodePtr node);
int process_xml();
int get_and_print_feed(char *host, char *port, char *path, int is_secure, int is_testing);
