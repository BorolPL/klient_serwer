/*
    Simple udp client
*/
#include<stdio.h> 				//printf
#include<string.h> 				//memset
#include<stdlib.h> 				//exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>


#define BUFLEN 64  				//Maksymalna dlugosc bufora
#define PORT 666    			//Wybor portu

void die(char *s) //obsluga bledow
{
    perror(s);
    exit(1);
}


int main(int argc, char **argv)
{
	//>>wskaznik na zmienne
	char *server;
	//<<

    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    server = argv[4];

    //Wyswietlanie danych
    printf("\n\rWczytane parametry: \n\r");
    printf("\n\rZmienna A: %s" 		, argv[1]);
    printf("\n\rZmienna B: %s" 		, argv[2]);
    printf("\n\rOperacja  : %s"  	, argv[3]);
    printf("\n\rADR SERWERA : %s \n\r" , argv[4]); //tutaj localhost wpisywac

    //Umieszczenie zmiennych w payloadzie
    sprintf(message , "%s %s %s" , argv[1] , argv[2] , argv[3]);

    //Tworzenie gniazda
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(server , &si_other.sin_addr) == 0)			//inte_aton konwertuje adres IP na binerne dane i laduje do struktury in_adr
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    //wyslanie datagramu
    if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
    {
        die("sendto()");
    }

    printf("\n\rWait for server..."); //czekanie na odpowiedz

    //otrzymuje i wypisuje
    //czyszczenie bufora zerem
    memset(buf,'\0', BUFLEN);

    //blokowanie petli
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
    {
        die("recvfrom()");
    }

    printf("\n\rResponse:\n\r%s", buf);

    close(s);
    return 0;
}