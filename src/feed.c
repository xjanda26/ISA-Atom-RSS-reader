/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "feed.h"

/// @brief Function goes through every element of ATOM XML
/// @param doc Parsed XML object
/// @param node Feed element
void process_feed_node(xmlDocPtr doc, xmlNodePtr node) {
    while (node != NULL) {
        process_feed_title_node(doc, node);
        process_entry_node(doc, node);
        
        node = node->next;
    }
}

/// @brief Function goes through every element of RSS XML
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
    
    return SUCCESS;
}
