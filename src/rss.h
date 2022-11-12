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

void process_item_nodes(xmlDocPtr doc, xmlNodePtr node);
void process_rss_title_node(xmlDocPtr doc, xmlNodePtr node);
