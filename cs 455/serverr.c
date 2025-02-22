/* Name : Avinash
 * COURSE-SECTION : CS 440 SP25
 * Assignment : 01
 * Due Date : 02/10/2025
 * Collaborators : -
 * Resources : -
 * Description: Designing Skeleton for Assignment
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <time.h>

const int MAX_LINE = 512;
const int MAX_PENDING = 5;
const int SERVER_PORT = 11002;
//const int TOTAL_RESPONSES = 20;

const char possible_response[20][30] = {
    "It is certain", "It is decidedly so",
    "Without a doubt", "Yes, definitely",
    "You may rely on it", "As I see it, yes",
    "Most likely", "Outlook good",
    "Yes", "Signs point to yes",
    "Reply hazy try again", "Ask again later",
    "Better not tell you now", "Cannot predict now",
    "Concentrate and ask again", "Don't count on it",
    "My reply is no", "My sources say no",
    "Outlook not so good", "Very doubtful"
};

/*currently a socket has not been creaated so s = -1, value of s will change when a socket is created successfully*/
int s = -1;

/*handling SIGINT, (CTRL-C) pressed by the user */
void handle_sigint(int sig)
{   
    /*check if the socket has been created, if yes then close the socket*/
    if(s != -1) 
    {   
        close(s);
    }

    /*print informative message on the screen for the user after he chooses to exit*/
    printf("Exit Successfully \n");
    exit(EXIT_SUCCESS); 
}
    



int 
main(void)
{
	struct sockaddr_in sin;
	char buf[MAX_LINE];
	socklen_t new_len;
	int new_s, question_length;
            

        signal(SIGINT, handle_sigint);

	/* build address data structure */
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY; //EVERY INTERFACE ON THE SERVER FOR THIS SOCKET
        sin.sin_port = htons(SERVER_PORT) ; //what port i want to listen on
	

	//only do one time 
	/*setup passive open*/
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1 ) //if socket can not be opened
	    err(1, "Unable to open  the socket");

	if ((bind(s, (struct sockaddr*)&sin,sizeof sin)) == -1)
	{
		close(s);
	       err(2, "Unable to bind socket");
	}

	if ((listen(s, MAX_PENDING)) == -1)	
	{
		close(s);
		err(3, "Unable to listen on socket");
	}

	/*wait for a connection , then recieve and print message*/
	while (1)   
	     	//loop that never terminates
	{       
                
                memset(buf, 0, sizeof buf);
		new_len = sizeof sin;
		
                if ((new_s = accept(s, (struct sockaddr*)&sin, &new_len)) == -1)
		{
			close(s);
			err(4, "Accept error"); /*if accept fails keep running after logging a message*/
                    
		}

		question_length =  recv(new_s, buf, sizeof buf, 0);
                
                //check if
                if(question_length == - 1 || question_length == 0)
                {
                close(new_s);
                err(5, "Recieve error: No message Recieved");
                
                }
                
                if(question_length >= MAX_LINE - 1)
                {
                   char error_message[ ] = "Error: Question exceeds 512 bytes";
                    send(new_s, error_message, strlen(error_message), 0);
                    close(new_s);
                }
                
                
			
		    /* Randomly select a response from predefined responses */
                    unsigned int choice = arc4random_uniform(20);
                    char response[MAX_LINE];
                    
                    int len = strnlen(buf, sizeof buf);
                    buf[len - 1] = '\0';
                    
                    /* Format the response including the received question */
                    snprintf(response, MAX_LINE, "Q:\"%s\" " "\n" "R: \"%s\"", buf, possible_response[choice]);
                 
                /* Send response back to the client */
                     if (send(new_s, response, strlen(response), 0) < 0) 
                    {   
                        err(6, "Response send failed");
                    }
                
                    
                close(new_s);   
		


	}

	//not reached 
	close(s);
	return 0;

}
