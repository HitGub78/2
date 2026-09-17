#include <iostream>
#define CAP 100

template <typename T>

struct darray{
    T *ar;
    size_t size;
    size_t cap;

    void expand(){
        cap == 0 ? cap = 1 : cap *= 2;
        T *ar_new = new T[cap];
        for (int i = 0; i < size; i++){
            ar_new[i] = ar[i];
        }
        delete[] ar;
        ar = ar_new;
    }

    void shrink(){
        if (cap / 2 < size) return;
        cap /= 2;
        T *ar_new = new T[cap];
        for (int i = 0; i < size; i++){
            ar_new[i] = ar[i];
        }
        delete[] ar;
        ar = ar_new;
    }

    darray() {
        ar = new T[CAP];
        size = 0;
        cap = CAP;
    }
    ~darray(){delete[] ar;}

    public:
        void push(T value) {
            if (size == cap) {
                expand();
            }
            ar[size++] = value;
        }

        void pop(){
            if (size == 0) return;
            size --;
            if (size < cap/4){
                shrink();
            }
            
        }

        T index(int i){
            if (i >= size){
                std::cout << "Index out of range";
                return T{};
            }
            else return ar[i];
        }


        void change(int i, T val){
            if (i >= size){
                return;
            }
            else ar[i] = val;
        }

        void clear(){
            size = 0;
            cap = CAP;
        }

        void print(){
            for(int i = 0 ; i < size; i++){
                std::cout << ar[i] << " ";
            }
            std::cout << "\n";
        }
    
};


int main(){
    darray<char> S;
    int input = -1, idx=0;
    char el;
    while(input){
        std::cin >> input;
        switch(input){
            case 1:
                std::cin >> el;
                S.push(el);
                break;
            case 2:
                S.pop();
                break;
            case 3:
                std::cin >> idx;
                std::cout << S.index(idx) << "\n";
                break;
            case 4:
                std::cin >> idx >> el;
                S.change(idx,el);
                break;
            case 5:
                S.clear();
                break;
            case 6:
                S.print();
                break;
            default:
                break;
        }
    }
    
    return 0;
}