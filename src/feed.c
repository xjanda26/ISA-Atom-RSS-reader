/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "feed.h"

int process_author_node (xmlDocPtr doc, xmlNodePtr authorNode) {
    printf("\n");
    xmlChar *key;

    while (authorNode != NULL) {
        key = xmlNodeListGetString(doc, authorNode->children, 1);
        printf("\t\t%s, %s\n", authorNode->name, key);
        authorNode = authorNode->next;
    }

    xmlFree(key);
    return 0;
}

int process_entry_node(xmlDocPtr doc, xmlNodePtr node) {
    // spracuj uzly
    while (node != NULL) {
        printf("%s\n", node->name);
        if ((!xmlStrcmp(node->name, (const xmlChar *) "entry"))) {
            xmlNodePtr entryNode = node->children;
            while (entryNode != NULL) {
                xmlChar *key;
                printf("\t%s", entryNode->name);
                if ((!xmlStrcmp(entryNode->name, (const xmlChar *) "author"))) {
                    process_author_node(doc, entryNode->children);
                } else {
                    key = xmlNodeListGetString(doc, entryNode->children, 1);
                    printf(", %s\n", key);
                }
                
                xmlFree(key);
                entryNode = entryNode->next;
            }
        }
        
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
                printf("Empty XML\n");
            } else {
                process_entry_node(xmlTree, node->children);
            }
        } else {
            printf("Error, parsing xml.\n");
        }

        xmlFreeDoc(xmlTree);
    } else {
        printf("Error, converting string arr to xml arr\n");
    }

    return 0;
}

int get_and_print_feed(char *host, char *port, char *path, int is_testing) {
    int result = connect_to_host(host, port, 0);
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
    return result;
}
