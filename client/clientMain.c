#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // This is required for inet_addr

// Function Signatures
void create_control_channel(int, const char*, int);
void create_data_channel(int, const char*, int);

int main(int argc, char *argv[]) {

// Command is ran by user with two arguments; target IP and port number.
// Checking two arguments are provided.
if (argc != 3) {
    printf("Error in syntax.\n Usage: %s <Server IP> <Target Port>\n", argv[0]);
    exit(1);
}



// Checking the port number inputted in valid
const char *server_ip = argv[1];
char *endptr;
long port = strtol(argv[2], &endptr, 10);
if (*endptr != '\0' || port < 0 || port > 65535) {
    printf("Invalid port number. Port must be between 0 and 65535.\n");
    exit(5);
}
int target_control_port = (int)port;

// Need code to check the IP address inputted is valid.
printf("Put in code to check the IP inputted is valid.\n");


// Welcome message.
const char client_message[] = "Welcome to the FTP client";

printf("%s\n", client_message);

// Creating a two socket (Control and Data), which will reach out to server later.
// AF_INET = IPv4, SOCK_STREAM = TCP, 0 = IP

// Control channel socket created, though not bound to any address yet.
int client_control_socket;
client_control_socket = socket(AF_INET, SOCK_STREAM, 0);
if (client_control_socket < 0) {
    perror("Error in creating control socket");
    exit(EXIT_FAILURE);
}


// Data channel socket created, though not bound to any address yet.
int client_data_socket;
client_data_socket = socket(AF_INET, SOCK_STREAM, 0);
if (client_data_socket < 0) {
    perror("Error in creating data socket");
    exit(EXIT_FAILURE);
}

printf("Data channel: %i, Control socket: %i\n",client_control_socket, client_data_socket); //Debugging line

return 0;

// Connect to the control channel
create_control_channel(client_control_socket, server_ip, target_control_port);

/* Assuming target_data_port is the same as target_control_port for now
Will need to implement logic to grab the data port from the control channel later.
*/
int target_data_port = target_control_port;

// Connect to the data channel - This will need to be put inside its own FUNCTION to be called upon later when needed.
create_data_channel(client_data_socket, server_ip, target_data_port);

return 0;
}

// Function call to create and connect the control channel
void create_control_channel(int client_control_socket, const char* server_ip, int target_control_port) {

    // Specify an address for the control socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; // This defines the address type as IPv4
    server_address.sin_port = htons(target_control_port); // This defines the port number, htons converts the port (int) to a network byte order.
    server_address.sin_addr.s_addr = inet_addr(server_ip); // This defines the IP address of the server, inet_addr converts the IP address to a binary format.

    int control_connection_status = connect(client_control_socket, (struct sockaddr*) &server_address, sizeof(server_address)); // This connects the client to the server. Casting is used to change the variable type of the structure, as required by connect()

    // Error handling
    if (control_connection_status < 0) {
        perror("Error in connecting to the server's control channel.");
        exit(EXIT_FAILURE); // Note perror prints the error message to stderr, so function type can be ANYTHING.
}

printf("FAKE - Connected to the server's control channel.\n");
}

// Function call to create the data channel
void create_data_channel(int client_data_socket, const char* server_ip, int target_data_port) {

    // Specify an address for the data socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; // This defines the address type as IPv4
    server_address.sin_port = htons(target_data_port); 
    /* IMPORTANT NOTE
    The variable target_data_port is not yet defined or captured in this program, we need to somehow grab it from the data exchanged on the control channel.
    */
   server_address.sin_addr.s_addr = inet_addr(server_ip);

   int data_connection_status = connect(client_data_socket, (struct sockaddr*) &server_address, sizeof(server_address)); // This connects the client to the server. Casting is used to change the variable type of the structure, as required by connect()

    // Error handling
    if (data_connection_status < 0) {
        perror("Error in connecting to the server's data channel.");
        exit(EXIT_FAILURE); // Note perror prints the error message to stderr, so function type can be ANYTHING.
}
printf("FAKE - Connected to the server's data channel.\n");
}
