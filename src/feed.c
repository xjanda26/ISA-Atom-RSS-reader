/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "feed.h"

/// @brief Function goes through every element of ATOM XML.
/// @param doc Parsed XML object
/// @param node Feed element
void process_feed_node(xmlDocPtr doc, xmlNodePtr node) {
    int isFirstEntry = 1;
    while (node != NULL) {
        if ((xmlStrcmp(node->name, (const xmlChar *) "text"))) {
            if ((!xmlStrcmp(node->name, (const xmlChar *) "title"))) {
                process_feed_title_node(doc, node);
            } else if ((!xmlStrcmp(node->name, (const xmlChar *) "entry"))) {
                process_entry_node(doc, node);

                if (isFirstEntry) {
                    isFirstEntry = 0;
                } else if (optFlags[A_FLAG] || optFlags[T_FLAG] || optFlags[U_FLAG]) {
                    printf("\n");
                }
            }
        }
        node = node->next;
    }
}

/// @brief Function goes through every element of RSS XML.
/// @param doc Parsed XML object
/// @param node Rss element
void process_rss_node(xmlDocPtr doc, xmlNodePtr node) {
    while (node != NULL) {
        if ((!xmlStrcmp(node->name, (const xmlChar *) "channel"))) {
            xmlNodePtr channelChild = node->children;

            while (channelChild != NULL) {
                if ((xmlStrcmp(channelChild->name, (const xmlChar *) "text"))) {
                    process_rss_title_node(doc, channelChild);
                    process_item_nodes(doc, channelChild);
                }

                channelChild = channelChild->next;
            }
            
        }
        
        node = node->next;
    }
}

/// @brief Function converts received data into XML object and parses it.
///        Distinguishes between RSS and Atom feed types.
/// @return Zero value on success or any of XML error types defined in error.h
int process_xml() {
    ///TODO: ak nepracujeme s UTF-8, musíme convertovat do UTF-8
    xmlChar *xml = xmlCharStrdup(xmlResponse);
    xmlDocPtr xmlTree;

    if (xml) {
        xmlTree = xmlParseDoc(xml);

        if(xmlTree) {
            xmlNodePtr node;
            node = xmlDocGetRootElement(xmlTree);
            if (node == NULL) {
                error_msg(XML_EMPTY);
                return exit_value;
            } else {
                if ((!xmlStrcmp(node->name, (const xmlChar *) "rss"))) {
                    process_rss_node(xmlTree, node->children);
                } else {
                    process_feed_node(xmlTree, node->children);
                }
            }
        } else {
            error_msg(XML_PARSING_FAIL);
            return exit_value;
        }

        xmlFreeDoc(xmlTree);
    } else {
        error_msg(XML_CONVERT_FAIL);
        return exit_value;
    }

    return SUCCESS;
}

/// @brief Function creates socket, connection, receives data, parses data 
///        and prints it on STDOUT.
/// @param host Name of host
/// @param port Destination port
/// @param path Path to the feed on the remote host
/// @param is_secure Flag indicating whether the communication is secure or not
/// @return Zero value on success or any of error type defined in error.h
int get_and_print_feed(char *host, char *port, char *path, int is_secure) {
    if (connect_to_host(host, port)) {
        return exit_value;
    }
    
    if (is_secure) {
        if (init_ssl()) {
            return exit_value;
        }

        if (init_tls_connection(host)) {
            return exit_value;
        }

        if (verify_certificate()) {
            return exit_value;
        }

        send_https_request(host, port, path);

        if (receive_ssl_data()) {
            return exit_value;
        }
    } else {   
        send_http_request(sock, host, port, path);

        if (receive_data()) {
            return exit_value;
        }
    }

    process_xml();
    
    free(xmlResponse);
    
    return exit_value;
}
