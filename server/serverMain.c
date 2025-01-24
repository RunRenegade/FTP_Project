#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctype.h>

// Function defintions
void valid_port_number_checker(char*);




int main() {

// Welcome message.
char server_mode[] = "Stream";
char server_file_structure[] = "File";
char server_representation_type[] = "ASCII Non-print";

char server_message [256];
snprintf(server_message, sizeof(server_message), "Welcome to the FTP server.\nServer is in '%s' mode, using structure type '%s', and using '%s' representation type.\n\n", server_mode, server_file_structure, server_representation_type);

printf("%s", server_message);
printf("What port would you like the server to listen on? 21 is standard.\n");

char command_port_number[6];
printf("Desired port: ");
scanf("%5s", command_port_number); // The %5s means only the first five characters are accepted, but the remaining go into the input buffer (which can cause issues later)
while (getchar() != '\n'); // This clears the input buffer, so the next scanf doesn't get the remaining characters from the previous input.



printf("Port selected was %s.\n", command_port_number); 
printf("Is this the correct port number? y/n\n");
char port_check[2];
scanf("%c", port_check);

if (port_check[0] != 'y' && port_check[0] != 'Y') {
    printf("Please enter port number correctly.\n");
    printf("Goodbye.\n");
    exit(5);
}

// Running a function to check the port number inputted is valid.
valid_port_number_checker(command_port_number);


// Creating a two socket (Control and Data).
// AF_INET = IPv4, SOCK_STREAM = TCP, 0 = IP

// Control channel socket created
int server_control_socket = socket(AF_INET, SOCK_STREAM, 0);
if (server_control_socket < 0) {
    perror("Error in creating control socket");
    exit(EXIT_FAILURE);
}


/* Instert some code here to create data sock later - should likely be in its own function. */



// struct sockaddr_in server_address_control;
// server_address_control.sin_family = AF_INET;
// server_address_control.sin_port = htons((uint16_t)atoi(command_port_number)); // Convert port number to integer
// server_address_control.sin_addr.s_addr = INADDR_ANY; // This means the server will listen on all interfaces.
// server_address_control.sin_addr.s_addr = INADDR_ANY; // This means the server will listen on all interfaces.

// // Defining the socket structure for the data channel
// struct sockaddr_in server_address_data;
// server_address_data.sin_family = AF_INET;
// // Port number is not set, as this is negotiated depending on the client's request.
// server_address_data.sin_addr.s_addr = server_address_control.sin_addr.s_addr; // Match the interface of the control channel.


return 0;
}

// A function to check if the port number is valid
void valid_port_number_checker(char* char_port) {
    
    // Turning char_port into an int
    int int_port = atoi(char_port);
    if (int_port < 0 || int_port > 65535) {
        printf("/n");
        printf("Invalid port number '%s'. Port must be between 0 and 65535.\n", char_port);
        exit(5);
    }
}
