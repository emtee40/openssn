// $Id: dstack.cpp,v 1.4 2003/04/14 05:51:03 mbridak Exp $
// Simple Stack Class                 

#include<iostream>
using std::cout;

#include "dstack.h"

DStack::DStack(int N)               // how to initalize a stack with
{                                   //   N items.
   bottom_ = new float[N];
   top_ = bottom_;
   size_ = N;
}

DStack::~DStack()                   // how to reclaim memory from local
{                                   //   stacks when exiting functions
   delete [] bottom_;
}

int DStack::num_items() const       // number of items currently in stack
{
    return (top_ - bottom_ );
}

void DStack::push(float val)        // push a new value 
{
    *top_ = val; 
    top_++;
}

float DStack::pop()                 // pop value from top
{
    top_--;
    return *top_;
}


int DStack::full() const            // 1 if full, 0 otherwise
{
    return (num_items() >= size_);
}


int DStack::empty() const           // 1 if empty, 0 otherwise
{
    return (num_items() <= 0);
}



void DStack::print() const
{

      cout << "Stack currently holds " << num_items() << " items: " ;
      for (float *element=bottom_; element<top_; element++)
      {
          cout << "  " << *element;
      }
      cout << "\n";

}

