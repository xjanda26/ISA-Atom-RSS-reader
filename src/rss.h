/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "parameters.h"

#define AUTHOR "Autor"
#define UPDATED_AT "Aktualizace"
#define ENTRY_URI "URI"

void process_item_nodes(xmlDocPtr doc, xmlNodePtr node);
void process_rss_title_node(xmlDocPtr doc, xmlNodePtr node);
