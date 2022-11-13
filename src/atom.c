/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "atom.h"

char *authorName;
char *updatedAt;
char *entryTitle;
char *entryUri;

/// @brief Function prints saved data about an article 
///        if their were provided in response.
void print_entry() {
    if (entryTitle) {
        printf("%s\n", entryTitle);
    }

    if (optFlags[A_FLAG] > 0 && authorName) {
        printf("%s: %s\n",(LANG ? AUTHOR_CZ : AUTHOR), authorName);
    }

    if (optFlags[U_FLAG] > 0 && entryUri) {
        printf("%s: %s\n", (LANG ? ENTRY_URI_CZ : ENTRY_URI), entryUri);
    }

    if (optFlags[T_FLAG] > 0 && updatedAt) {
        printf("%s: %s\n", (LANG ? UPDATED_AT_CZ : UPDATED_AT), updatedAt);
    }
}

/// @brief Function frees dynamicly allocated memory.
void free_entry_data() {
    free(entryTitle);
    if (optFlags[A_FLAG] > 0 && authorName) {
        free(authorName);
    }

    if (optFlags[U_FLAG] > 0 && entryUri) {
        free(entryUri);
    }

    if (optFlags[T_FLAG] > 0 && updatedAt) {
        free(updatedAt);
    }
}

/// @brief Function compares 2 strings.
/// @param node Pointer on XML node
/// @param string String to compare
/// @return Zero if name of node isn't equal to string or 
///         returns non-zero value if strings are equal.
int cmp_node_name(xmlNodePtr node, const char *string) {
    return xmlStrcmp(node->name, (const xmlChar *) string);
}

/// @brief Function processes and saves data about author of article.
/// @param doc Parsed XML object
/// @param authorNode XML element within article
void process_author_node(xmlDocPtr doc, xmlNodePtr authorNode) {
    while (authorNode != NULL) {
        if (cmp_node_name(authorNode, "text") && !cmp_node_name(authorNode, "name")) {    
            xmlChar *key = xmlNodeListGetString(doc, authorNode->children, 1);
            int keyLen = strlen((char *) key);

            authorName = (char *) malloc((keyLen + 1) * sizeof(char));
            strcpy(authorName, (char *) key);

            xmlFree(key);
        }

        authorNode = authorNode->next;
    }
}

/// @brief Function processes and saves data about publishing date of arcticle.
/// @param doc Parsed XML object
/// @param entryNode Processed XML entry element (article node)
void process_updated_node(xmlDocPtr doc, xmlNodePtr entryNode) {
    xmlChar *key = xmlNodeListGetString(doc, entryNode->children, 1);
    int keyLen = strlen((char *) key);

    updatedAt = (char *) malloc((keyLen + 1) * sizeof(char));
    strcpy(updatedAt, (char *) key);

    xmlFree(key);
}

/// @brief Function processes and saves data about URI that refers to arcticle.
/// @param doc Parsed XML object
/// @param entryNode Processed XML entry element (article node)
void process_link_node(xmlDocPtr doc, xmlNodePtr entryNode) {
    xmlBufferPtr nodeBuffer = xmlBufferCreate();
    xmlNodeDump(nodeBuffer, doc, entryNode,0,0);

    char *href = strstr((char *) nodeBuffer->content, "href=\"");
    
    int j = 6, hrefLen = 0;

    while(href[j] != '\"') {
        j++;
        hrefLen++;
    }

    entryUri = (char *) malloc((hrefLen + 1) * sizeof(char));
    int i = 0;
    for (j = 6; j < hrefLen + 6; i++, j++) {
        entryUri[i] = href[j];
    }

    entryUri[i] = '\0';

    xmlBufferFree(nodeBuffer);
}

/// @brief Function processes and save data about title of arcticle.
/// @param doc Parsed XML object
/// @param entryNode Processed XML entry element (article node)
void process_entry_title_node(xmlDocPtr doc, xmlNodePtr entryNode) {
    xmlChar *key = xmlNodeListGetString(doc, entryNode->children, 1);
    int keyLen = strlen((char *) key);

    entryTitle = (char *) malloc((keyLen + 1) * sizeof(char));
    strcpy(entryTitle, (char *) key);
    
    xmlFree(key);
}

/// @brief Function goes through every element of article.
///        Pick only author, updated, link and title element.
/// @param doc Parsed XML object
/// @param node XML element within feed element
void process_entry_node(xmlDocPtr doc, xmlNodePtr node) {
    xmlNodePtr entryNode = node->children;
    while (entryNode != NULL) {
        if ((cmp_node_name(entryNode, "text"))) {
            if (optFlags[A_FLAG] > 0 && !cmp_node_name(entryNode, "author")) {
                process_author_node(doc, entryNode->children);
            }

            if (optFlags[T_FLAG] > 0 && !cmp_node_name(entryNode, "updated")) {
                process_updated_node(doc, entryNode);
            }

            if (optFlags[U_FLAG] > 0 && !cmp_node_name(entryNode, "link")) {
                process_link_node(doc, entryNode);
            }

            if ((!cmp_node_name(entryNode, "title"))) {
                process_entry_title_node(doc, entryNode);
            }
        }
        
        entryNode = entryNode->next;
    }

    print_entry();
    free_entry_data();
}

/// @brief Function processes title of feed and prints it on STDOUT.
/// @param doc Parsed XML object
/// @param node XML element within feed element
void process_feed_title_node(xmlDocPtr doc, xmlNodePtr node) {
    xmlChar *key = xmlNodeListGetString(doc, node->children, 1);
    printf("*** %s ***\n", key);
    xmlFree(key);
}
