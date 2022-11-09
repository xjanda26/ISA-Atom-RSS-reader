/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "feed.h"

int isFirstEntry = 1;

char *authorName;
char *updatedAt;
char *entryTitle;
char *entryUri;

/// @brief Function prints saved data about an article if their were provided in reposnse.
void print_entry() {
    printf("%s\n", entryTitle);

    if (optFlags[A_FLAG] > 0) {
        if (authorName) {
            printf("%s: %s\n", AUTHOR, authorName);
        }
    }

    if (optFlags[U_FLAG] > 0) {
        if (entryUri) {
            printf("%s: %s\n", ENTRY_URI, entryUri);
        }
    }

    if (optFlags[T_FLAG] > 0) {
        if (updatedAt) {
            printf("%s: %s\n", UPDATED_AT,updatedAt);
        }
    }

}

/// @brief Function free dynamicly allocated memory
void free_entry_data() {
    free(entryTitle);
    if (optFlags[A_FLAG] > 0) {
        if (authorName) {
            free(authorName);
        }
    }

    if (optFlags[U_FLAG] > 0) {
        if (entryUri) {
            free(entryUri);
        }
    }

    if (optFlags[T_FLAG] > 0) {
        if (updatedAt) {
            free(updatedAt);
        }
    }
}

/// @brief Function processes and saves data about author of article
/// @param doc Parsed XML object
/// @param authorNode XML element within article
void process_author_node(xmlDocPtr doc, xmlNodePtr authorNode) {
    while (authorNode != NULL) {
        if ((xmlStrcmp(authorNode->name, (const xmlChar *) "text"))) {
            if ((!xmlStrcmp(authorNode->name, (const xmlChar *) "name"))) {
                
                xmlChar *key = xmlNodeListGetString(doc, authorNode->children, 1);
                int keyLen = strlen((char *) key);

                authorName = (char *) malloc((keyLen + 1) * sizeof(char));
                strcpy(authorName, (char *) key);

                xmlFree(key);
            }
        }

        authorNode = authorNode->next;
    }
}

/// @brief Function processes and saves data about publishing date of arcticle
/// @param doc Parsed XML object
/// @param entryNode XML element of article
void process_updated_node(xmlDocPtr doc, xmlNodePtr entryNode) {
    xmlChar *key = xmlNodeListGetString(doc, entryNode->children, 1);
    int keyLen = strlen((char *) key);

    updatedAt = (char *) malloc((keyLen + 1) * sizeof(char));
    strcpy(updatedAt, (char *) key);

    xmlFree(key);
}

/// @brief Function processes and saves data about URI that refers to arcticle
/// @param doc Parsed XML object
/// @param entryNode Processed XML element
void process_link_node(xmlDocPtr doc, xmlNodePtr entryNode) {
    xmlBufferPtr nodeBuffer = xmlBufferCreate();
    xmlNodeDump(nodeBuffer, doc, entryNode,0,0);

    char *href = strstr((char *) nodeBuffer->content, "href=\"");
    
    int j = 6;
    int hrefLen = 0;

    while(1) {
        if (href[j] == '\"') {
            break;
        }

        j++;
        hrefLen++;
    }

    entryUri = (char *) malloc((hrefLen + 1) * sizeof(char));
    int i = 0;
    for (j = 6; j < hrefLen + 6; j++) {
        entryUri[i] = href[j];
        i++;
    }

    entryUri[i] = '\0';

    xmlBufferFree(nodeBuffer);
}

/// @brief Function processes and save data about title of arcticle
/// @param doc Parsed XML object
/// @param entryNode Processed XML entry element (article node)
void process_entry_title_node(xmlDocPtr doc, xmlNodePtr entryNode) {
    xmlChar *key = xmlNodeListGetString(doc, entryNode->children, 1);
    int keyLen = strlen((char *) key);

    entryTitle = (char *) malloc((keyLen + 1) * sizeof(char));
    strcpy(entryTitle, (char *) key);
    
    xmlFree(key);
}

/// @brief Function goes through every element of article. Pick only author, updated, link and title element.
/// @param doc Parsed XML object
/// @param node XML element within feed element
void process_entry_node(xmlDocPtr doc, xmlNodePtr node) {
    if ((!xmlStrcmp(node->name, (const xmlChar *) "entry"))) {

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
                        process_updated_node(doc, entryNode);
                    }
                }

                if (optFlags[U_FLAG] > 0) {
                    if ((!xmlStrcmp(entryNode->name, (const xmlChar *) "link"))) {
                        process_link_node(doc, entryNode);
                    }
                }

                if ((!xmlStrcmp(entryNode->name, (const xmlChar *) "title"))) {
                    process_entry_title_node(doc, entryNode);
                }
            }
            
            entryNode = entryNode->next;
        }

        print_entry();
        free_entry_data();
    }
}

/// @brief Function processes title of feed and prints it on STDOUT
/// @param doc Parsed XML object
/// @param node XML element within feed element
void process_feed_title_node(xmlDocPtr doc, xmlNodePtr node) {
    xmlChar *key;

    if ((!xmlStrcmp(node->name, (const xmlChar *) "title"))) {
        key = xmlNodeListGetString(doc, node->children, 1);
        printf("*** %s ***\n", key);
        xmlFree(key);
    }
}


/// @brief Function goes through every element of ATOM XML
/// @param doc Parsed XML object
/// @param node Feed element
/// @return 
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
                // TODO: ATOM and RSS parsing
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
    
    result = connect_to_host(host, port, 0);
    if (result) {
        return result;
    }
    
    if (is_secure) {
        result = init_ssl(is_testing);
        if (result) {
            return error_msg(SSL_CTX_CONTEXT_FAIL, is_testing);
        }

        result = init_tls_connection(host, is_testing);
        if (result) {
            return result;
        }

        result = verify_certificate(is_testing);
        if (result) {
            return result;
        }

        send_https_request(host, port, path);

        result = receive_ssl_data(is_testing);
        if (result) {
            // TODO: 
            return result;
        }
    } else {       
        send_http_request(sock, host, port, path);

        result = receive_data(is_testing);
        if (result) {
            // TODO
            return result;
        }
    }

    result = process_xml();
    
    free(xmlResponse);
    
    return result;
}
