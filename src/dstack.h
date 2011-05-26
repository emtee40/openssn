// $Id: dstack.h,v 1.2 2003/04/14 05:51:03 mbridak Exp $
// dstack.h -- Dynamic stack (DStack) declaration and function prototypes.
//

class DStack
{
  private:
    float *bottom_;
    float *top_;
    int size_;

  public:
    DStack(int size=10);
    void push(float val);
    int num_items() const;
    float pop();
    int full() const;
    int empty() const;
    void print() const;
    ~DStack();
};
