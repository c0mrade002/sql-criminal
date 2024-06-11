#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void print_color_text(const char *text, const char *color_code) {
    printf("%s%s\033[0m", color_code, text);
    fflush(stdout);
    usleep(50000);
}

void typewriter_effect(const char *text, const char *color_code) {
    for (size_t i = 0; i < strlen(text); i++) {
        printf("%s%c\033[0m", color_code, text[i]);
        fflush(stdout);
        usleep(50000);
    }
    printf("\n");
}

void input_color_text(const char *prompt, const char *color_code, char *buffer, size_t size) {
    printf("%s%s\033[0m", color_code, prompt);
    fflush(stdout);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; 
}

void save_to_txt(const char *hostname, const char *ip_address, const char **directories, size_t dir_count) {
    FILE *file = fopen("scan_results.txt", "w");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        return;
    }

    fprintf(file, "Hostname: %s\n", hostname);
    fprintf(file, "IP Address: %s\n", ip_address);
    fprintf(file, "\nAdmin Directories Found:\n");
    for (size_t i = 0; i < dir_count; i++) {
        fprintf(file, "  %s\n", directories[i]);
    }

    fclose(file);
    print_color_text("Risultati salvati nel file scan_results.txt\n", "\033[1;36m"); 
}

void get_ip_info(const char *hostname, char *ip_address, size_t size) {
    struct addrinfo hints, *res, *p;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
        typewriter_effect(gai_strerror(status), "\033[1;31m"); 
        return;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        const char *ipver;
        
        if (p->ai_family == AF_INET) { 
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { 
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }
        
        inet_ntop(p->ai_family, addr, ip_address, size);
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Indirizzo IP: %s\nFamiglia di indirizzi: %s\n", ip_address, ipver);
        typewriter_effect(buffer, "\033[1;33m"); 
    }

    freeaddrinfo(res);
}

void search_admin_directories(const char *hostname, const char *ip_address, const char **directories, size_t dir_count) {
    char url[256];
    char found_directories[3][256];
    size_t found_count = 0;

    for (size_t i = 0; i < dir_count; i++) {
        snprintf(url, sizeof(url), "http://%s/%s", ip_address, directories[i]);
        FILE *file = popen((char *)url, "r");
        if (file != NULL) {
            found_count++;
            strcpy(found_directories[found_count - 1], url);
            pclose(file);
        }
    }

    if (found_count > 0) {
        print_color_text("\nDirectory trovate\n", "\033[1;32m"); 
        for (size_t i = 0; i < found_count; i++) {
            print_color_text(found_directories[i], "\033[1;32m"); 
            printf("\n");
        }
    } else {
        print_color_text("\nNessuna directory di amministrazione trovata\n", "\033[1;33m"); 
    }

    save_to_txt(hostname, ip_address, (const char **)found_directories, found_count);
}

int main() {
    char hostname[256];
    char ip_address[256];
    const char *common_directories[] = {"admin", "login", "dashboard"};
    size_t dir_count = sizeof(common_directories) / sizeof(common_directories[0]);

    print_color_text("\nInserisci l'hostname da analizzare (digita 'exit' per uscire): ", "\033[1;36m"); 

    while (1) {
        input_color_text("", "\033[1;36m", hostname, sizeof(hostname));
        if (strcmp(hostname, "exit") == 0) {
            typewriter_effect("\nGrazie per aver provato questo tool! Visita t.me/VikingTerminal per provare altre utility.", "\033[1;36m"); 
            sleep(1);
            break;
        }

        get_ip_info(hostname, ip_address, sizeof(ip_address));

        if (strlen(ip_address) > 0) {
            search_admin_directories(hostname, ip_address, common_directories, dir_count);
            print_color_text("\nInserisci l'hostname da analizzare (digita 'exit' per uscire): ", "\033[1;36m"); 
        } else {
            print_color_text("Il dominio specificato non esiste. Riprova con un dominio esistente: ", "\033[1;31m"); 
        }
    }

    return 0;
}