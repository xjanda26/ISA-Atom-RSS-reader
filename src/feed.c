/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "feed.h"

int isFirstEntry = 1;

void process_author_node (xmlDocPtr doc, xmlNodePtr authorNode) {
    xmlChar *key;

    while (authorNode != NULL) {
        if ((xmlStrcmp(authorNode->name, (const xmlChar *) "text"))) {
            if ((!xmlStrcmp(authorNode->name, (const xmlChar *) "name"))) {
                key = xmlNodeListGetString(doc, authorNode->children, 1);
                printf("Autor: %s\n", key);
                xmlFree(key);
            }
        }

        authorNode = authorNode->next;
    }
}

void process_entry_node(xmlDocPtr doc, xmlNodePtr node) {
    if ((!xmlStrcmp(node->name, (const xmlChar *) "entry"))) {
        xmlChar *key;

        if (isFirstEntry) {
            isFirstEntry = 0;
        } else {
            if (optFlags[A_FLAG] || optFlags[T_FLAG] || optFlags[U_FLAG]) {
                printf("\n");
            }
        }

        //printf("Doc children: %s\n", node->name);
        xmlNodePtr entryNode = node->children;
        while (entryNode != NULL) {
            //printf("Doc children: %s\n", entryNode->name);
            if ((xmlStrcmp(entryNode->name, (const xmlChar *) "text"))) {
                if (optFlags[A_FLAG] > 0) {
                    if ((!xmlStrcmp(entryNode->name, (const xmlChar *) "author"))) {
                        process_author_node(doc, entryNode->children);
                    }
                }

                if (optFlags[T_FLAG] > 0) {
                    if ((!xmlStrcmp(entryNode->name, (const xmlChar *) "updated"))) {
                        key = xmlNodeListGetString(doc, entryNode->children, 1);
                        printf("Aktualizace: %s\n", key);
                        xmlFree(key);
                    }
                }

                if (optFlags[U_FLAG] > 0) {
                    if ((!xmlStrcmp(entryNode->name, (const xmlChar *) "link"))) {
                        xmlBufferPtr nodeBuffer = xmlBufferCreate();
                        int dumpRes = xmlNodeDump(nodeBuffer, doc, entryNode,0,0);
                        
                        // TODO: dynamic parsing
                        if (dumpRes > 48) {
                            printf("URL: ");
                            for (int i = 45; i < dumpRes - 3 ;i++) {
                                printf("%c", nodeBuffer->content[i]);
                            }
                            printf("\n");
                        }

                        xmlBufferFree(nodeBuffer);
                    }
                }

                if ((!xmlStrcmp(entryNode->name, (const xmlChar *) "title"))) {
                    key = xmlNodeListGetString(doc, entryNode->children, 1);
                    printf("%s\n", key);
                    xmlFree(key);
                }
            }
            entryNode = entryNode->next;
        }
    }
}

void process_feed_title_node(xmlDocPtr doc, xmlNodePtr node) {
    xmlChar *key;

    if ((!xmlStrcmp(node->name, (const xmlChar *) "title"))) {
        key = xmlNodeListGetString(doc, node->children, 1);
        printf("*** %s ***\n", key);
        xmlFree(key);
    }
}

int process_feed_node(xmlDocPtr doc, xmlNodePtr node) {
    while (node != NULL) {
        process_feed_title_node(doc, node);
        process_entry_node(doc, node);
        
        node = node->next;
    }

    return 0;
}

int process_xml() {
    // ak nepracujeme s UTF-8, musíme convertovat do UTF-8
    xmlChar *xml = xmlCharStrdup(xmlResponse);
    xmlDocPtr xmlTree;

    if (xml) {
        xmlTree = xmlParseDoc(xml);

        if(xmlTree) {
            //ok
            xmlNodePtr node;
            node = xmlDocGetRootElement(xmlTree);
            if (node == NULL) {
                // TODO:
                printf("Empty XML\n");
            } else {
                // TODO: XML and ATOM parsing
                process_feed_node(xmlTree, node->children);
            }
        } else {
            // TODO:
            printf("Error, parsing xml.\n");
        }

        xmlFreeDoc(xmlTree);
    } else {
        // TODO:
        printf("Error, converting string arr to xml arr\n");
    }

    return 0;
}

int get_and_print_feed(char *host, char *port, char *path, int is_secure, int is_testing) {
    int result;

    if (is_secure) {
        printf("Is secured\n");
        result = 0;
    } else {
        result = connect_to_host(host, port, 0);
        if (result) {
            return result;
        }
        send_request(sock, host, port, path);

        result = receive_data(is_testing);
        if (result) {
            return result;
        }

        result = process_xml();
    
        free(xmlResponse);
    }
    
    return result;
}
