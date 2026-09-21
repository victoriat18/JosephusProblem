//HEADER!!
#include "josephus.h"

#include <list>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <getopt.h>
#include <iterator>


//void print_underlined_string(const string &message);
//void print_list(const list<string> &collection, const unsigned &eliminations, int num_cols);
// Changed string and list to used std::
void print_underlined_string(const std::string &message);
void print_list(const std::list<std::string> &collection, const unsigned &eliminations, int num_cols);
/**
* Print a 'Usage' message and exit(1).
*
* @param a0 The name of the command to include in the usage message.
*****************************************************************************/
static void usage(const char *a0)
{
    std::cerr << "Usage: " << a0 << " [-n number of people] [-m modulus] [-p print frequency] [-c print columns]" << std::endl;
    exit(1);
}

//print a message with an underline
void print_underlined_string(const std::string &message)
{
    std::cout << message << std::endl;

    //print one dash for each character in the message
    for (std::string::size_type i = 0; i < message.length(); i++)
    {
        std::cout << "-";
    }

    std::cout << std::endl;
}

//Print the current list of people
void print_list(const std::list<std::string> &collection, const unsigned &eliminations, int num_cols)
{
    //print the correct heading
    if (eliminations == 0)
    {
        print_underlined_string("Initial group of people");
    }
    else
    {
        std::ostringstream heading;
        heading << "After eliminating " << eliminations << " people";
        print_underlined_string(heading.str());
    }

    int count = 0;

    //print the names in the list
for (std::list<std::string>::const_iterator it = collection.begin();
     it != collection.end(); ++it)
{
    std::cout << *it;
    count++;

    if (count < num_cols && std::next(it) != collection.end())
    {
        std::cout << ", ";
    }

    //start a new line after specified number of columns
    if (count == num_cols)
    {
        std::cout << std::endl;
        count = 0;
    }
}

    //print a new line if the last row was not full
    if (count != 0)
    {
        std::cout << std::endl;
    }
}


/**
* Create a std::list of prople with generated ID/names and reduce the
* list as per the Josephus problem algorithm.
*****************************************************************************/
int main(int argc, char **argv)
{
    unsigned num_people = 41;       // The number of people to start with
    unsigned modulus = 3;           // The count used to determine the elimination
    unsigned print_frequency = 13;  // How often to print the state of the system
    unsigned num_cols = 12;         // Number of colums to print per line

    int opt;
    while ((opt = getopt(argc, argv, "n:m:p:c:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            std::istringstream(optarg) >> num_people;
            break;
        case 'm':
            std::istringstream(optarg) >> modulus;
            break;
        case 'p':
            std::istringstream(optarg) >> print_frequency;
            break;
        case 'c':
            std::istringstream(optarg) >> num_cols;
            break;
        default:
            usage(argv[0]);
        }
    }

    if (optind < argc)
        usage(argv[0]); // If we get here, there was extra junk on command line

    //Create an empty list of people
    std::list<std::string> people;

    //Generate the names and add them to the list
    std::generate_n(std::back_inserter(people), num_people, SEQ(num_people));

    //print the initial list
    print_list(people, 0, num_cols);

    //start the iterator at the first person
    std::list<std::string>::iterator it = people.begin();

    unsigned eliminations = 0;

    //continue until only one person remains
    while (people.size() > 1)
    {
        //advance to the person to eliminate
        for (unsigned i = 1; i < modulus; i++)
        {
            ++it;

            //go back to beginning when the end is reached
            if (it == people.end())
            {
                it = people.begin();
            }
        }

        //remove the person and get the next iterator
        it = people.erase(it);

        //go back to the beginning if erase returned end()
        if (it == people.end())
        {
            it = people.begin();
        }

        eliminations++;

        //print the list at the required frequency
        if (eliminations % print_frequency == 0)
        {
            print_list(people, eliminations, num_cols);
        }
    }

    //print the final remaining person
    print_list(people, eliminations, num_cols);

    return 0;
}

//end 