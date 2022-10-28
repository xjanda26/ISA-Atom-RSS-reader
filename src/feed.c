/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "feed.h"

int get_and_print_feed(char *url) {
    //int sock = 0;
    int connectRes = connect_to_host("www.theregister.com", "80", 0);
    if (connectRes) {
        return connectRes;
    }
    send_request(sock, "www.theregister.com", "80", "/headlines.atom");

    receive_data(sock);
    return 0;
}
