#include <deque>
#include <cstdlib>
#include <iostream>
#include "monitor.h"

using namespace std;

#define N 20 // buffer size

char getLetter()
{
    char c =  'A' + (random() % 26);
    return c;
}

class Buffer: Monitor
{
private:
    Condition prodA, prodB, consA, consB;

    class Elem
    {
    public:
        char letter;
        bool readA, readB; //readA readB

        Elem()
        {
            letter = '?';
            readA = readB = false;
        }
    };

    deque<Elem> buffer;

public:
  
    Buffer():Monitor()
    {
      Elem beg[3];
      beg[0].letter = 'A';
      beg[1].letter = 'B';
      beg[2].letter = 'C';
      buffer.push_back(beg[0]);
      buffer.push_back(beg[1]);
      buffer.push_back(beg[2]);
    }

    void produceA()
    {
        enter();
        //printf("ProdA\n");

        if(buffer.size() == N)
        {
            //std::cout << "ProdA waiting" << std::endl;
            wait(prodA);
        }

        std::cout << "ProducerA -> produce: ";
        pushLetter();
        printBuffer();

        if (buffer.size() > 0 && !buffer.front().readA) {
            //std::cout << "CA start" << std::endl;
            signal(consA);
        }

        if (buffer.size() > 0 && !buffer.front().readB) {
            //std::cout << "CB start" << std::endl;
            signal(consB);
        }

        leave();
    }

    void produceB()
    {
        enter();
        //printf("ProducerB\n");

        if(buffer.size()+2 > N)
        {
            //std::cout << "ProducerB waiting" << std::endl;
            wait(prodB);
        }

        std::cout << "ProducerB -> produce: ";
        pushLetter();
        pushLetter();
        printBuffer();

        if(buffer.size() > 0 && !buffer.front().readA)
        {
            //std::cout << "CA start" << std::endl;
            signal(consA);
        }

        if (buffer.size() > 0 && !buffer.front().readB)
        {
            //std::cout << "CB start" << std::endl;
            signal(consB);
        }
        leave();
    }

    void consumeA()
    {
        enter();
      //  printf("ConsumerA\n");
        if(buffer.size() <= 3 || buffer.front().readA)
        {
            //std::cout << "ConsumerA waiting" << std::endl;
            wait(consA);
        }

        Elem x = buffer.front();
        //std::cout << "ConsumerA read: " << x.letter << std::endl;
        buffer.front().readA = true;

        if (x.readB)
        {
            std::cout << "ConsumerA -> consume: " << x.letter;
            buffer.pop_front();
            printBuffer();

            if (buffer.size() <= N-2)
            {
                //std::cout << "PB start" << std::endl;
                signal(prodB);
            }

            if (buffer.size() <= N-1)
            {
                //std::cout << "PA start" << std::endl;
                signal(prodA);
            }
        }
        leave();
    }

    void consumeB()
    {
        enter();
    //    printf("ConsumerB\n");
        if (buffer.size() <= 3 || buffer.front().readB)
        {
          //  std::cout << "ConsumerB " << std::endl;
            wait(consB);
        }

        Elem x = buffer.front();
        //std::cout << "ConsumerB read: " << x.letter << std::endl;
        buffer.front().readB = true;

        if(x.readA)
        {
            std::cout << "ConsumerB -> consume: " << x.letter;

            buffer.pop_front();
            printBuffer();

            if ((int)buffer.size() <= N-2) {
                //std::cout << "PB start" << std::endl;
                signal(prodB);
            }

            if ((int)buffer.size() <= N-1) {
                //std::cout << "PA start" << std::endl;
                signal(prodA);
            }
        }
        leave();
    }

    void printBuffer()
    {
        std::cout << "\nBUFFER SIZE: " << buffer.size() << std::endl;

        for (auto it : buffer){
            std::cout<<it.letter<<" ";
        }
        std::cout<<std::endl<<std::endl;

    }

    void pushLetter()
    {
        Elem x;
        x.letter = getLetter();

        std::cout<<x.letter<<" ";

        buffer.push_back(x);
    }
};
