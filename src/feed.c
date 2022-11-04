/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "feed.h"

int get_and_print_feed(char *host, char *port, char *path, int is_testing) {
    int connectRes = connect_to_host(host, port, 0);
    if (connectRes) {
        return connectRes;
    }
    send_request(sock, host, port, path);

    receive_data(is_testing);
    free(xmlResponse);
    return 0;
}
