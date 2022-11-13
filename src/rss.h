/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "parameters.h"

#define AUTHOR "Author"
#define AUTHOR_CZ "Autor"
#define UPDATED_AT "Updated"
#define UPDATED_AT_CZ "Aktualizace"
#define ENTRY_URI "URI"
#define ENTRY_URI_CZ "URI"

void print_item();
void free_item_data();
int cmp_element_name(xmlNodePtr node, const char *string);
void process_item_node(xmlDocPtr doc, xmlNodePtr itemNode, char *type);
void process_item_nodes(xmlDocPtr doc, xmlNodePtr node);
void process_rss_title_node(xmlDocPtr doc, xmlNodePtr node);
