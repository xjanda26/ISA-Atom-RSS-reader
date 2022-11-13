/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "rss.h"

char *itemAuthorName;
char *itemUpdatedAt;
char *itemTitle;
char *itemUri;

/// @brief Function prints saved data about an article if their were provided in response.
void print_item() {
    if (itemTitle) {
        printf("%s\n", itemTitle);
    }

    if (optFlags[A_FLAG] > 0 && itemAuthorName) {
        printf("%s: %s\n", (LANG ? AUTHOR_CZ : AUTHOR), itemAuthorName);
    }

    if (optFlags[U_FLAG] > 0 && itemUri) {
        printf("%s: %s\n", (LANG ? ENTRY_URI_CZ : ENTRY_URI), itemUri);
    }

    if (optFlags[T_FLAG] > 0 && itemUpdatedAt) {
        printf("%s: %s\n", (LANG ? UPDATED_AT_CZ : UPDATED_AT), itemUpdatedAt);
    }
}

/// @brief Function free dynamicly allocated memory.
void free_item_data() {
    free(itemTitle);
    if (optFlags[A_FLAG] > 0 && itemAuthorName) {
        free(itemAuthorName);
    }

    if (optFlags[U_FLAG] > 0 && itemUri) {
        free(itemUri);
    }

    if (optFlags[T_FLAG] > 0 && itemUpdatedAt) {
        free(itemUpdatedAt);
    }
}

/// @brief Function compares 2 strings.
/// @param node Pointer on XML node
/// @param string String to compare
/// @return Zero if name of node isn't equal to string or 
///         returns non-zero value if strings are equal.
int cmp_element_name(xmlNodePtr node, const char *string) {
    return xmlStrcmp(node->name, (const xmlChar *) string);
}

/// @brief General function that processes elements of RSS item element.
///        Processes only link, pubDate, author and title elements.
/// @param doc Parsed XML object
/// @param itemNode RSS item element
/// @param type Type of item element
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

/// @brief Function goes through every element of article. 
///        Picks only author, pubDate, link and title element.
/// @param doc Parsed XML object
/// @param node XML element within rss element
void process_item_nodes(xmlDocPtr doc, xmlNodePtr node) {       
    xmlNodePtr entryNode = node->children;
    while (entryNode != NULL) {
        if ((cmp_element_name(entryNode, "text"))) {
            if (optFlags[A_FLAG] > 0 && !cmp_element_name(entryNode, "author")) {
                process_item_node(doc, entryNode->children, "author");
            }

            if (optFlags[T_FLAG] > 0 && !cmp_element_name(entryNode, "pubDate")) {
                process_item_node(doc, entryNode, "pubDate");
            }

            if (optFlags[U_FLAG] > 0 && !cmp_element_name(entryNode, "link")) {
                process_item_node(doc, entryNode, "link");
            }
            
            if ((!cmp_element_name(entryNode, "title"))) {
                process_item_node(doc, entryNode, "title");
            }
        }
        
        entryNode = entryNode->next;
    }

    print_item();
    free_item_data();
}

/// @brief Function processes title of RSS element and prints it on STDOUT.
/// @param doc Parsed XML object
/// @param node XML element within rss element
void process_rss_title_node(xmlDocPtr doc, xmlNodePtr node) {
    xmlChar *key = xmlNodeListGetString(doc, node->children, 1);
    printf("*** %s ***\n", key);
    xmlFree(key);
}
