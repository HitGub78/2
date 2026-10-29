#include <iostream>
#include <cassert>
#define CAP 100

struct Exception{
    virtual const char* str(){
        return "Error";
    }
};

struct OutOfBounds : Exception{
    const char* str(){
        return "Error: out of bounds";
    }
};

struct Empty : Exception{
    const char* str(){
        return "Error: array is empty";
    }
};



class Printable{
    public:
    virtual ~Printable() = default;
    virtual void print(std::ostream& os) = 0;
};

std::ostream& operator<<(std::ostream& os,Printable &p){
    p.print(os);
}

class Cloneable{
    public:
    virtual ~Cloneable() = default;
    virtual Cloneable* clone() const = 0;
};



class DynArr : public Printable{
    private:
        void expand(){
            cap = (cap == 0) ? 1 : cap * 2;
            int *ar_new = new int[cap];
            for (int i = 0; i < size; i++){
                ar_new[i] = ar[i];
            }
            delete[] ar;
            ar = ar_new;
        }

        void shrink(){
            if (size < cap/4 && cap > CAP){
                cap /= 2;
                int *ar_new = new int[cap];
                for (int i = 0; i < size; i++){
                    ar_new[i] = ar[i];
                }
                delete[] ar;
                ar = ar_new;
            }
        }
        int cap;
        
    protected:
        int *ar;
        int size;
    public:
        DynArr(){
            ar = new int[CAP];
            size = 0;
            cap = CAP;
        }

        virtual ~DynArr(){delete[] ar;}

        DynArr(const DynArr& other)
        : ar(new int[other.cap]), size(other.size), cap(other.cap)
        {
            for (int i = 0; i < size; i++)
                ar[i] = other.ar[i];
        }

        int len() const{
            return size;
        }
        DynArr& operator=(const DynArr& other) 
        {
            if (this == &other) return *this;
            delete[] ar;
            cap = other.cap;
            size = other.size;
            ar = new int[cap];
            for (int i = 0; i < size; i++)
                ar[i] = other.ar[i];
            return *this;
        }
        
        virtual void push(int value) {
            if (size == cap) {
                expand();
            }
            ar[size++] = value;
        }

        int peek(){
            if (size == 0){
                throw Empty();
            } 
            else return ar[size-1];
        }
        
        int pop(){
            if (size == 0){
                throw Empty();
            } 
            size --;
            if (size > CAP && size < cap/4){
                shrink();
            }
            return ar[size];
        }

        int& operator[](int i){
            if (i < 0 || i >= size){
                throw OutOfBounds();
            }
            return ar[i];
        }


        void clear(){
            delete[] ar;
            size = 0;
            cap = CAP;
            ar = new int[cap];
        }

        void print(std::ostream& os) override{
            for(int i = 0; i < size; i++){
                os << ar[i] << " ";
            }
        };

};


class Stack: public DynArr{
    protected:
    using DynArr::ar;
    using DynArr::size;
    public:
    Stack() : DynArr() {};
    using DynArr::push;
    using DynArr::pop;
    using DynArr::peek;
    using DynArr::clear;
    using DynArr::len;

    void print(std::ostream& os) override{
        for(int i = 0; i < size; i++){
            os << ar[i] << " ";
        }
    };
};

class LimStack: public Stack{
    int limit;
    protected:
    using Stack::ar;
    using Stack::size;

    public:
    LimStack(int lim) : Stack(), limit(lim)  {
        if (lim < 0){throw OutOfBounds();};
    };

    using Stack::pop;
    using Stack::peek;
    using Stack::clear;
    using Stack::len;
    void print(std::ostream& os) override{
        for(int i = 0; i < size; i++){
            os << ar[i] << " ";
        }
    };

    void push(int val) override{
        if (size >= limit){
            throw OutOfBounds();            
        }
        else{
            Stack::push(val);
        }
    }
};


#include <cassert>

void test_stack() {
    Stack s;

    assert(s.len() == 0);
    bool thrown = false;
    try { 
        s.pop(); 
    } 
    catch (const Empty&) {
        thrown = true; 
    }
    assert(thrown);

    s.push(10);
    s.push(20);
    s.push(30);
    assert(s.len() == 3);
    assert(s.peek() == 30);
    assert(s.pop() == 30);
    assert(s.pop() == 20);
    assert(s.len() == 1);
    assert(s.peek() == 10);


    s.clear();
    assert(s.len() == 0);
}

void test_limstack() {
    LimStack s(3);

    assert(s.len() == 0);

    s.push(1);
    s.push(2);
    s.push(3);
    assert(s.len() == 3);
    assert(s.peek() == 3);


    bool thrown = false;
    try { 
        s.push(4);
    } 
    catch (const OutOfBounds&) { 
        thrown = true; 
    }
    assert(thrown);
    assert(s.len() == 3);   

    assert(s.pop() == 3);
    s.push(4);             
    assert(s.peek() == 4);

    s.clear();
    assert(s.len() == 0);
    s.push(5);
    assert(s.peek() == 5);
}

std::ostream& operator<<(std::ostream& os, Printable& obj) {
    obj.print(os); 
    return os;     
}

int main(){


    test_stack();
    test_limstack();
    std::cout << "OK\n";

    int limit = 0;
    std::cin >> limit;
    LimStack S(limit);
    int input = -1, idx=0;
    int el;
    while(input){
        std::cin >> input;
        try{
            switch(input){
                case 1:
                    std::cin >> el;
                    S.push(el);
                    break;
                case 2:
                    std::cout << S.pop() << "\n";
                    break;
                case 3:
                    std::cout << S.peek() << "\n";
                    break;
                case 4:
                    S.clear();
                    break;
                case 5:
                    std::cout << S << "\n";
                    break;
                default:
                    break;
            }
        }
        catch (Exception& e) {
            std::cout << e.str() << "\n";
        }

    }

    // DynArr S;
    // int input = -1, idx=0;
    // int el;
    // while(input){
    //     std::cin >> input;
    //     try{
    //         switch(input){
    //             case 1:
    //                 std::cin >> el;
    //                 S.push(el);
    //                 break;
    //             case 2:
    //                 std::cout << S.pop() << "\n";
    //                 break;
    //             case 3:
    //                 std::cin >> idx;
    //                 std::cout << S[idx] << "\n";
    //                 break;
    //             case 4:
    //                 std::cin >> idx >> el;
    //                 S[idx] = el;
    //                 break;
    //             case 5:
    //                 S.clear();
    //                 break;
    //             case 6:
                    
    //                 std::cout << S << "\n";
    //                 break;
    //             default:
    //                 break;
    //         }
    //     }
    //     catch (Exception& e) {
    //         std::cout << e.str() << "\n";
    //     }

    // }

    return 0;
}