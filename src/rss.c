/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "rss.h"

int isFirstItem = 1;

char *itemAuthorName;
char *itemUpdatedAt;
char *itemTitle;
char *itemUri;

/// @brief Function prints saved data about an article if their were provided in response.
void print_item() {
    if (itemTitle) {
        printf("%s\n", itemTitle);
    }

    if (optFlags[A_FLAG] > 0) {
        if (itemAuthorName) {
            printf("%s: %s\n", AUTHOR, itemAuthorName);
        }
    }

    if (optFlags[U_FLAG] > 0) {
        if (itemUri) {
            printf("%s: %s\n", ENTRY_URI, itemUri);
        }
    }

    if (optFlags[T_FLAG] > 0) {
        if (itemUpdatedAt) {
            printf("%s: %s\n", UPDATED_AT,itemUpdatedAt);
        }
    }

}

/// @brief Function free dynamicly allocated memory
void free_item_data() {
    free(itemTitle);
    if (optFlags[A_FLAG] > 0) {
        if (itemAuthorName) {
            free(itemAuthorName);
        }
    }

    if (optFlags[U_FLAG] > 0) {
        if (itemUri) {
            free(itemUri);
        }
    }

    if (optFlags[T_FLAG] > 0) {
        if (itemUpdatedAt) {
            free(itemUpdatedAt);
        }
    }
}

///TODO: dokumentacia
void process_item_node(xmlDocPtr doc, xmlNodePtr itemNode, char *type) {
    xmlChar *key = xmlNodeListGetString(doc, itemNode->children, 1);
    int keyLen = strlen((char *) key);

    if (!strcmp(type, "link")) {
        itemUri = (char *) malloc((keyLen + 1) * sizeof(char));
        strcpy(itemUri, (char *) key);
    } else if (!strcmp(type, "pubDate")) {
        itemUpdatedAt = (char *) malloc((keyLen + 1) * sizeof(char));
        strcpy(itemUpdatedAt, (char *) key);
    } else if (!strcmp(type, "author")) {
        itemAuthorName = (char *) malloc((keyLen + 1) * sizeof(char));
        strcpy(itemAuthorName, (char *) key);
    } else if (!strcmp(type, "title")) {
        itemTitle = (char *) malloc((keyLen + 1) * sizeof(char));
        strcpy(itemTitle, (char *) key);
    }

    xmlFree(key);
}

/// @brief Function goes through every element of article. Pick only author, updated, link and title element.
/// @param doc Parsed XML object
/// @param node XML element within feed element
void process_item_nodes(xmlDocPtr doc, xmlNodePtr node) {
    if ((!xmlStrcmp(node->name, (const xmlChar *) "item"))) {

        if (isFirstItem) {
            isFirstItem = 0;
        } else {
            if (optFlags[A_FLAG] || optFlags[T_FLAG] || optFlags[U_FLAG]) {
                printf("\n");
            }
        }

        //printf("Doc children: %s\n", node->name);
        xmlNodePtr entryNode = node->children;
        while (entryNode != NULL) {
            if ((xmlStrcmp(entryNode->name, (const xmlChar *) "text"))) {
                if (optFlags[A_FLAG] > 0) {
                    if ((!xmlStrcmp(entryNode->name, (const xmlChar *) "author"))) {
                        process_item_node(doc, entryNode->children, "author");
                    }
                }

                if (optFlags[T_FLAG] > 0) {
                    if ((!xmlStrcmp(entryNode->name, (const xmlChar *) "updated"))) {
                        process_item_node(doc, entryNode, "updated");
                    }
                }

                if (optFlags[U_FLAG] > 0) {
                    if ((!xmlStrcmp(entryNode->name, (const xmlChar *) "link"))) {
                        process_item_node(doc, entryNode, "link");
                    }
                }

                if ((!xmlStrcmp(entryNode->name, (const xmlChar *) "title"))) {
                    process_item_node(doc, entryNode, "title");
                }
            }
            
            entryNode = entryNode->next;
        }

        print_item();
        free_item_data();
    }
}

/// @brief Function processes title of RSS element and prints it on STDOUT
/// @param doc Parsed XML object
/// @param node XML element within rss element
void process_rss_title_node(xmlDocPtr doc, xmlNodePtr node) {
    xmlChar *key;

    if ((!xmlStrcmp(node->name, (const xmlChar *) "title"))) {
        key = xmlNodeListGetString(doc, node->children, 1);
        printf("*** %s ***\n", key);
        xmlFree(key);
    }
}
