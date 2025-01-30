#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h> // This is required for inet_pton and INET_ADDRSTRLEN
#include <ctype.h> // This is required for isdigit

// Function defintions
void valid_port_number_checker(char*);

int main() {

// Welcome message.
char server_mode[] = "Stream";
char server_file_structure[] = "File";
char server_representation_type[] = "ASCII Non-print";

char server_side_welcome_message [256];
snprintf(server_side_welcome_message, sizeof(server_side_welcome_message), "Welcome to the FTP server.\nServer is in '%s' mode, using structure type '%s', and using '%s' representation type.\n\n", server_mode, server_file_structure, server_representation_type);


printf("%s", server_side_welcome_message);
printf("What port would you like the server to listen on? 21 is standard.\n");

char command_port_number[6];
printf("Desired port: ");
scanf("%5s", command_port_number); // The %5s means only the first five characters are accepted, but the remaining go into the input buffer (which can cause issues later)
while (getchar() != '\n'); // This clears the input buffer, so the next scanf doesn't get the remaining characters from the previous input.



printf("\nPort selected was %s.\n", command_port_number); 
printf("Is this the correct port number? y/n\n");
char port_check[1];
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
int server_control_listener = socket(AF_INET, SOCK_STREAM, 0);
if (server_control_listener < 0) {
    perror("Error in creating control socket listener.\n");
    exit(EXIT_FAILURE);
}


/* Instert some code here to create data sock later - should likely be in its own function. */

// Defining the socket structure for the control channel
struct sockaddr_in server_address_control_port;
server_address_control_port.sin_family = AF_INET;
server_address_control_port.sin_port = htons((uint16_t)atoi(command_port_number)); // Convert port number to integer
server_address_control_port.sin_addr.s_addr = INADDR_ANY; // This means the server will listen on all interfaces.

// Binding the control channel to the socket
if (bind(server_control_listener, (struct sockaddr*) &server_address_control_port, sizeof(server_address_control_port)) < 0) {
    perror("Error in binding control socket");
    exit(EXIT_FAILURE);
}

// Listening on the control channel
if (listen(server_control_listener, 5) < 0) {
    perror("Error with listen() on control socket.");
    exit(EXIT_FAILURE);
}

// Accepting a client connection, maybe this needs to be made into a WHILE loop?
int client_control_connection_socket = -1;
int accept_fail_count = 0;

// Creating a structure to save the details of the client connecting to server.
struct sockaddr_in client_address;
int client_address_length = sizeof(client_address); // need to stop buffer overflows

printf("\nWaiting for client connection...\n");
while (client_control_connection_socket == -1) { // When client_connection_socket is successful it will become the next non-negative value for the FD (file descriptor) on the system
    client_control_connection_socket = accept(server_control_listener, (struct sockaddr *)&client_address, &client_address_length); 
    // Last two arguments of accept are us storing the incoming connection data (IP, port etc). We also needed to cast client_address as accept() expected sockaddr structure not sockaddr_in
    // accept() is known as a BLOCKING function, meaning it will wait until a client connects to the server before another itiration of the while loop starts.

    accept_fail_count += 1;

    if (client_control_connection_socket < 0) {
        perror("Error accepting connection from client.\n");
    }

    if (accept_fail_count > 3) {
        perror("Failed to accept client connection three times in a row.\n");
        exit(EXIT_FAILURE);
    }

accept_fail_count = 0; // Resetting the fail count for the next iteration of the while loop. <-- Good practice, but pointless until the above is inside its own loop
} // End of while client_control_connection_socket loop


// Send initial message to client, 220 means server is ready, 120 means server asks for client to wait.
// Currently, dont think I need to send a 120 message.
char welcome_client_message[256];
snprintf(welcome_client_message, sizeof(welcome_client_message), "220 Welcome to the FTP server!\r\n");

if (send(client_control_connection_socket, welcome_client_message, sizeof(welcome_client_message), 0) < 0) {
    perror("Error sending welcome message to client\n");
    exit(EXIT_FAILURE);
}

// Display IP of client connected to control socket
char client_ip[INET_ADDRSTRLEN]; //INET_ADDRSTRLEN is a constant from arpa/inet.h built specifically to represent the length of IPv4 address, and the null terminator.
inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN); // converts the IP address from binary to text form.

printf("Client connected from IP address: %s\n", client_ip);

// Close listerner socket
if (close(server_control_listener) != 0) {
    perror("Error closing control socket");
    exit(EXIT_FAILURE);
} else {
    printf("Command channel has closed.\n");
    printf("\n");
    printf("Goodbye.\n");
}

// Closer the control socket connected to client
if (close(client_control_connection_socket) != 0) {
    perror("Error closing control socket");
    exit(EXIT_FAILURE);
} else {
    printf("Command channel has closed.\n");
    printf("\n");
    printf("Goodbye.\n");
}

return 0;
} // end of main



// A function to check if the port number is valid
void valid_port_number_checker(char* char_port) {
    
    // Checking char_port is not empty.
    if (char_port[0] == '\0') {
        printf("\n");
        printf("Please enter a port number.\n");
        exit(EXIT_FAILURE);
    }

    // Checking char_port is a number
    for (int i = 0; char_port[i] != '\0'; i++) {
        if (!isdigit(char_port[i])) {
            printf("\n");
            printf("Invalid port number '%s'. Port must be a number.\n", char_port);
            exit(5);
        }
    }

    // Turning char_port into an int
    int int_port = atoi(char_port);
    if (int_port < 1 || int_port > 65535) {
        printf("\n");
        printf("Invalid port number '%s'. Port must be between 0 and 65535.\n", char_port);
        exit(5);
    }
}
