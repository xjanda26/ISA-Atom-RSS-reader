/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "dns_communication.h"

struct in_addr getIPv4Address(char *domain) {
    int sock;
    struct sockaddr_in servent;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        fprintf(stderr, "Unable to create socket\n");
        exit(1);
    }

    printf("Socket created\n");

    memset(&servent, 0, sizeof(servent));

    servent.sin_addr.s_addr = inet_addr(SERVERDNS);
    servent.sin_family = AF_INET;
    servent.sin_port = htons(DNSPORT);

    // Set up the DNS header
    dns_header_t header;
    memset(&header, 0, sizeof(dns_header_t));
    header.xid = htons(0x1234);   // Randomly chosen ID
    header.flags = htons(0x0100); // Q=0, RD=1
    header.qdcount = htons(1);    // Sending 1 question

    // Set up the DNS question
    dns_question_t question;
    question.dnstype = htons(1);  // QTYPE 1=A
    question.dnsclass = htons(1); // QCLASS 1=IN

    question.name = calloc(strlen(HOSTNAME) + 2, sizeof(char));

    printf("%s\n", HOSTNAME);
    // Leave the first byte blank for the first field length
    memcpy(question.name + 1, HOSTNAME, strlen(HOSTNAME));
    uint8_t *prev = question.name;
    uint8_t count = 0; // Used to count the bytes in a field

    // connect to DNS Server
    if (connect(sock, (struct sockaddr *)&servent, sizeof(servent)) == -1) {
        fprintf(stderr, "Unable to connect\n");
        exit(1);
    }

    printf("%s\n", question.name);
    printf("%ld\n", strlen(HOSTNAME));
    // Traverse through the name, looking for the . locations
    for (size_t i = 0; i < strlen(HOSTNAME); i++) {
        // A . indicates the end of a field
        if (HOSTNAME[i] == '.') {
            // Copy the length to the byte before this field, then update prev to the location of the .
            *prev = count;
            prev = (uint8_t *)question.name + i + 1;
            count = 0;
        } else {
            count++;
        }
    }
    *prev = count;

    // Copy all fields into a single, concatenated packet
    size_t packetlen = sizeof(header) + strlen(HOSTNAME) + 2 +
                       sizeof(question.dnstype) + sizeof(question.dnsclass);
    uint8_t *packet = calloc(packetlen, sizeof(uint8_t));
    uint8_t *p = (uint8_t *)packet;

    // Copy the header first
    memcpy(p, &header, sizeof(header));
    p += sizeof(header);

    // Copy the question name, QTYPE, and QCLASS fields
    memcpy(p, question.name, strlen(HOSTNAME) + 1);
    p += strlen(HOSTNAME) + 2; // includes 0 octet for end
    memcpy(p, &question.dnstype, sizeof(question.dnstype));
    p += sizeof(question.dnstype);
    memcpy(p, &question.dnsclass, sizeof(question.dnsclass));

    // Send the packet to OpenDNS, then request the response
    sendto(sock, packet, packetlen, 0, (struct sockaddr *)&servent,
           (socklen_t)sizeof(servent));

    // Receiving resnose
    socklen_t length = 0;
    uint8_t response[512];
    memset(&response, 0, 512);

    // Receive the response from OpenDNS into a local buffer
    ssize_t bytes = recvfrom(sock, response, 512, 0, (struct sockaddr *)&servent, &length);
    printf("Bytes: %ld\n", bytes);

    dns_header_t *response_header = (dns_header_t *)response;
    printf("Assert: %d\n", ((ntohs(response_header->flags) & 0xf) == 0));

    // Get a pointer to the start of the question name, and reconstruct it from the fields
    uint8_t *start_of_name = (uint8_t *)(response + sizeof(dns_header_t));
    uint8_t total = 0;
    uint8_t *field_length = start_of_name;
    while (*field_length != 0) {
        // Restore the dot in the name and advance to next length
        total += *field_length + 1;
        *field_length = '.';
        field_length = start_of_name + total;
        printf("Reconstruct question name\n");
    }
    printf("Total: %d\n", total);

    // Skip null byte, qtype, and qclass to get to first answer
    dns_record_a_t *records = (dns_record_a_t *)(field_length + 5);
    printf("%d\n", ntohs(response_header->ancount));
    for (int i = 0; i < ntohs(response_header->ancount); i++) {
        printf("TYPE: %d\n", ntohs(records[i].type));
        printf("CLASS: %d\n", ntohs(records[i].class));
        printf("TTL: %d\n", ntohl(records[i].ttl));
        // printf ("IPv4: %ld\n", ntohl (records[i].addr));
        printf("IPv4: %s\n", inet_ntoa(records[i].addr));
    }

    close(sock);
    return records[0].addr;
}