/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "tcp_communication.h"

int sock;
char *xmlResponse;

/// @brief Function gets host's IP address and creates TCP connection.
/// @param hostname Name of remote host
/// @param port Destination port
/// @return Zero value on success or TCP error type defined in error.h
int connect_to_host(char *hostname, char *port) {
    /**
     * doc. Petr Matousek, [a lecture] BUT FIT, 2022 
     * https://moodle.vut.cz/pluginfile.php/502879/mod_resource/content/2/isa-sockets.pdf
    */
    struct addrinfo hints;
    struct addrinfo *peer_address;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    
    if (getaddrinfo(hostname, port, &hints, &peer_address) != 0) {
        if (LANG) {
            fprintf(stderr, "Cílová destinace: %s\n", hostname);
        } else {
            fprintf(stderr, "Host name: %s\n", hostname);
        }
        return error_msg(TCP_UNKNOWN_ADDR);
    }

    if ((sock = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol)) == -1) {
        error_msg(TCP_CREATE_SOCK_FAIL);
        freeaddrinfo(peer_address);
        return exit_value;
    }

    if (connect(sock, peer_address->ai_addr, peer_address->ai_addrlen)) {
        error_msg(TCP_CONNECTION_FAIL);
        freeaddrinfo(peer_address);
        close(sock);
        return exit_value;
    }

    freeaddrinfo(peer_address);
    return SUCCESS;
}
