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

void print_entry();
void free_entry_data();
void process_author_node(xmlDocPtr doc, xmlNodePtr authorNode);
void process_updated_node(xmlDocPtr doc, xmlNodePtr entryNode);
void process_entry_node(xmlDocPtr doc, xmlNodePtr node);
void process_link_node(xmlDocPtr doc, xmlNodePtr entryNode);
void process_entry_title_node(xmlDocPtr doc, xmlNodePtr entryNode);
void process_entry_node(xmlDocPtr doc, xmlNodePtr node);
void process_feed_title_node(xmlDocPtr doc, xmlNodePtr node);
