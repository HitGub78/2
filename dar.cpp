#include <iostream>
#include <sstream>
#include <cassert>
#define CAP 100

class Printable {
public:
    virtual ~Printable() = default;
    virtual void print(std::ostream& os) const = 0;
};

std::ostream& operator<<(std::ostream& os, const Printable& p) {
    p.print(os);
    return os;
}

class Cloneable {
public:
    virtual ~Cloneable() = default;
    virtual Cloneable* clone() const = 0;
};

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

template <typename T>
class DynArr : public Printable , public Cloneable{
    private:
    int cap;
    void expand(){
        cap = (cap == 0) ? 1 : cap * 2;
        T *ar_new = new T[cap];
        for (int i = 0; i < size; i++){
            ar_new[i] = ar[i];
        }
        delete[] ar;
        ar = ar_new;
    }

    void shrink(){
        cap /= 2;
        T *ar_new = new T[cap];
        for (int i = 0; i < size; i++){
            ar_new[i] = ar[i];
        }
        delete[] ar;
        ar = ar_new; 
    }
        
    protected:
    T *ar;
    int size;
    public:
    DynArr(){
        ar = new T[CAP];
        size = 0;
        cap = CAP;
    }

    virtual ~DynArr(){delete[] ar;}

    DynArr(const DynArr& other)
    : ar(new T[other.cap]), size(other.size), cap(other.cap)
    {
        for (int i = 0; i < size; i++)
            ar[i] = other.ar[i];
    }

    int len() const{
        return size;
    }

    DynArr& operator=(const DynArr& other) {
        if (this == &other) return *this;
        delete[] ar;
        cap = other.cap;
        size = other.size;
        ar = new T[cap];
        for (int i = 0; i < size; i++)
            ar[i] = other.ar[i];
        return *this;
    }
    
    void push(int value) {
        if (size == cap) {
            expand();
        }
        ar[size++] = value;
    }

    T peek(){
        if (size == 0){
            throw Empty();
        } 
        else return ar[size-1];
    }

    T pop(){
        if (size == 0){
            throw Empty();
        } 
        size --;
        if (size > CAP && size < cap/4){
            shrink();
        }
        return ar[size];
    }

    T& operator[](int i){
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

    void print(std::ostream& os) const override {
        for (int i = 0; i < size; i++){   
            os << ar[i] << " ";
        }
    }

    DynArr* clone() const override {
        return new DynArr(*this);
    }

};

template <typename T>
class Stack : public Printable , public Cloneable{
    private:

    struct Node{
        struct Node *next;
        T val;
    };
    Node* head;

    public:

    Stack(): head(nullptr) {};
    void clear(){
        while(head){
            Node *to_del = head;
            head = head->next;
            delete to_del;
        };
    }
    ~Stack(){
        clear();
        delete head;
    }

    Stack& operator=(const Stack& other) {
        
        if (this == &other) return *this;
        clear();
        if (!other.head) return *this;
        else{
            head = new Node{nullptr, other.head->val};
            Node* copy = other.head->next;
            Node* tail = head;
            while(copy){
                tail->next = new Node{copy->next,copy->val};
                tail = tail->next;
                copy = copy->next;
            }
            tail->next = nullptr;
        }
        return *this;
    }

    Stack(const Stack& other) : head(nullptr) {

        if (!other.head) return;

        head = new Node{nullptr, other.head->val};
        Node* copy = other.head->next;
        Node* tail = head;
        while(copy){
            tail->next = new Node{copy->next,copy->val};
            tail = tail->next;
            copy = copy->next;
        }
        tail->next = nullptr;
    }


    void push(T el){
        Node* New = new Node;
        New->val = el;
        if (!head){
            head = New;
            head->next = nullptr;
            return;
        }
        
        New->next = head;
        head = New;
    }
    int pop(){
        if (head){
            int val = head->val;
            Node *to_del = head;
            head = head->next;
            delete to_del;
            return val;
        }
        else throw Empty();
    }   

    T peek(){
        if (head){
            return head->val;
        }
        else throw Empty();
    }
    void print(std::ostream& os) const override{
        Node *copy = head;
        while(copy){
            os << copy->val << " ";
            copy = copy->next;
        }
        delete copy;
    }

    Cloneable* clone() const override {
        return new Stack(*this);
    }
};

#include <sstream>


void test_dynarr_print() {
    DynArr<int> a;
    a.push(1); a.push(2); a.push(3);

    std::ostringstream os;
    os << a;                        
    assert(os.str() == "1 2 3 ");   
}

void test_dynarr_copy_deep() {
    DynArr<int> a;
    a.push(1); a.push(2);

    DynArr<int> b = a;                  
    b.push(3);                      

    std::ostringstream osa, osb;
    osa << a; osb << b;
    assert(osa.str() == "1 2 ");    
    assert(osb.str() == "1 2 3 ");  
}

void test_dynarr_assign_deep() {
    DynArr<int> a, b;
    a.push(10); a.push(20);
    b.push(99);                     

    b = a;                          
    std::ostringstream osb;
    osb << b;
    assert(osb.str() == "10 20 ");

    b.push(30);                     
    std::ostringstream osa;
    osa << a;
    assert(osa.str() == "10 20 ");
}

void test_dynarr_self_assign() {
    DynArr<int> a;
    a.push(5);
    a = a;                          
    std::ostringstream os;
    os << a;
    assert(os.str() == "5 ");
}

void test_stack_print() {
    Stack<int> s;
    s.push(1); s.push(2); s.push(3);

    std::ostringstream os;
    os << s;
    assert(os.str() == "3 2 1 ");
}

void test_stack_copy_deep() {
    Stack<int> s;
    s.push(1); s.push(2);

    Stack<int> c = s;                 
    c.push(3);

    std::ostringstream oss, osc;
    oss << s; osc << c;
    assert(oss.str() == "2 1 ");
    assert(osc.str() == "3 2 1 ");

    c.pop();
    std::ostringstream oss2, osc2;
    oss2 << s; osc2 << c;
    assert(oss2.str() == "2 1 ");
    assert(osc2.str() == "2 1 ");
}

void test_stack_assign_deep() {
    Stack<int> a, b;
    a.push(1); a.push(2);
    b.push(99);

    b = a;
    std::ostringstream osb;
    osb << b;
    assert(osb.str() == "2 1 ");

    b.pop();
    std::ostringstream osa;
    osa << a;
    assert(osa.str() == "2 1 ");    
}


int main(){

    test_dynarr_print();
    test_dynarr_copy_deep();
    test_dynarr_assign_deep();
    test_dynarr_self_assign();

    test_stack_print();
    test_stack_copy_deep();
    test_stack_assign_deep();

    std::cout << "all tests OK\n";

    Stack<int> S;
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
