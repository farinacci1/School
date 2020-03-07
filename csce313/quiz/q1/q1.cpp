#include <iostream>
#include <string.h>
using namespace std;

class Header{
private:
    char used;
    int payloadsize;
    char* data;
public:
    Header (){
        used = 0, payloadsize = -1, data = NULL;
    }
    Header (int ps, char initvalue = 0){
        used = 0;
        payloadsize = ps;
        data = new char [payloadsize];
        memset (data, initvalue, payloadsize);
    }
    int getsummation (){
        int sum = 0;
        for (int i=0; i<payloadsize; i++){
            sum += data [i];
        }
        return sum;
    }
};

int main (){
    Header h1;
    Header h2 (10);
    Header* h3 = new Header (20);
    cout << "Header type size " << sizeof (Header) << endl;  // 1. explain
    /* under base level the size of struct = size of struct indiviual members + any padding
    charr * = 0 (as it points to nothing ) char and int are both 8 bytes each
    8 + 8  + 0 = 16 */
    cout << "Header oject size " << sizeof (h1) << endl;      // 2. explain why
    /* answer is same as above utilizing the base initializiton of a struct
    itll be treated the same */
    cout << "Header object h2 size "<< sizeof (h2) << endl; // 3. explain why
    /* same as above with addition that char is now initialized
    but char size remains the same so long as char* is not set 
    it will remain 16 */
    cout << "Header object pointer size " << sizeof (h3) << endl; // 4. why
    /*
    using new with the parameters supplied only sets char and the other two values are null
    thus pointing no where and having size values of 0
    8  + 0 + 0 = 8
    */

    // 5. now allocate memory big enough to hold 10 instances of Header
   Header* heads =  new Header[10];
    // 6. Put 10 instances of Header in the allocated memory block, one after another wihtout overwriting
        // The instances should have payload size 10, 20,..., 100 respectively
        // and they should have initial values 1,2,...10 respectively
   for (int i =1; i <= 10; i++)
   {
     heads[i - 1] =  Header (i * 10, i);
   }


    // 7. now call getsummation() on each instance using a loop, output should be:
            // 10, 40, 90, ...., 1000 respectively
   for (int i =1; i <= 10; i++)
   {
    std::cout << heads[i - 1].getsummation() << std::endl;
    
   }



    Header* ptr = h3 + 100;
    cout <<"Printing pointer h3: " << h3 << endl;
    cout <<"Printing pointer ptr: " << ptr << endl;
    
    // 8. explain the output you see in the following
    cout << "Difference " << (ptr - h3) << " objects" << endl;
    cout << "Difference " << ((char*) ptr - (char *)h3) << " bytes" << endl;
    /*
    ptr - h3 was difference of units as the 100 is eesntially an offset so comparitevely it make sense
    for the difference to be 100
     */
    /*(char*) ptr - (char *)h3) was difference in adresses at both pointers
    1000 0101 1010 0010 1000 0000 - 1000 0101 1001 1100 0100 0000 = 0000 0110 0100 0000
    */
    
}

