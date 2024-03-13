#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

// These helper function implementations should be ignored for the purpose of answering the questions.
char* getPassword()
{
    char* password = (char*)malloc( sizeof( char ) * 10 );
    strcpy( password, "password!" ); // Assume malloc doesn't fail
    return password;
}

// These helper function implementations should be ignored for the purpose of answering the questions.
void printMemoryData( char* pw, int length )
{
    printf( "Printing memory data.\n" );
    for (int i = 0; i < length; i++)
    {
        printf( "%c", pw[i] );
    }
    printf( "\nDone printing memory data.\n" );
}

// 1. Explain why the following function is vulnerable.
// Answer: TYPE EXPLAINATION HERE
// 
// 
// Additionally, adjust the code to fix the vulnerability.
void function1()
{
    char* password = getPassword();
    password = (char*)realloc( password, 64 );
    printMemoryData( password, 64 );
}

// 2. Discuss what sort of file content may raise a vulnerability in the following function? (Hint: Assume copy will not contain a null terminator).
// Answer: TYPE EXPLAINATION HERE
// 
// 
// Additionally, adjust the code to fix the vulnerability.
#define LENGTH 300
void function2()
{
    char* filename = "examplefile.txt";
    char prime[LENGTH];
    char copy[LENGTH];

    int handle = open( filename, O_RDONLY | O_CREAT, 0700 );
    read( handle, copy, LENGTH );
    close( handle );

    strcpy( prime, copy );
}

// 3. Discuss the issue with the following code.
// Answer: TYPE EXPLAINATION HERE
// 
// 
// Additionally, adjust the code to fix the vulnerability.
void function3()
{
    int index = 0;
    char* pointer;
    for (index = 0; index < 10; index++)
    {
        pointer = (char*)malloc( sizeof( char ) * 30 );
    }
    free( pointer );
    return index;
}

// 4. Explain a scenario in which the following code will not work (assuming the user is trying to provide the correct input). Discuss a solution. Also, discuss how your solution would handle an attacker.
// Answer: TYPE EXPLAINATION HERE
// 
// 
// Additionally, adjust the code to fix the vulnerability.
void function4()
{
    char cwid[9];
    printf( "Enter CWID:" );
    scanf( "%s", cwid );
    printf( "Entered CWID: %s\n", cwid );
    return 0;
    
}

// 5. What is the type of vulnerability this program may cause? How can this vulnerability be avoided? (Note: argv[0] refers to filename of program)
// Answer: TYPE EXPLAINATION HERE
// 
// 
// Additionally, adjust the code to fix the vulnerability.
void function5( int argc, char** argv)
{
    char var[10] = "";
    strcpy( var, argv[0] );
}

int main( int argc, char** argv )
{
    function1();
    function2();
    function3();
    function4();
    function5(argc, argv);
    return 0;
}
