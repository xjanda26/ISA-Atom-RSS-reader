/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "feed.h"

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
                printf("Root name: %s\n", node->name);
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
