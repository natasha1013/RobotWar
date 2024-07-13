#include <iostream>
#include <iterator>
using namespace std;

template<class T>
class List{
private:
    struct Node{
        T data;
        Node* next = nullptr;
        Node(T data2, Node* next2 = nullptr){
            data = data2;
            next = next2;
        }
    };

    Node* head;
    int sz = 0;

public:

    List() { head = nullptr; }
    ~List() { clear(); };
    List(List& list2); // copy constructor
    List& operator=(const List& list2); // copy assignment operator
    List(List&& list2){ // move constructor
        std::swap(sz, list2.sz);
        std::swap(head, list2.head);
        list2.sz = 0;
        list2.head = nullptr;
    }

    List& operator=(List&& list2){ // move assignment operator
        //cout << "List move assignment operator" << endl;
        clear();
        std::swap(sz, list2.sz);
        std::swap(head, list2.head);
        list2.sz = 0;
        list2.head = nullptr;
        return *this;
    }

    Node* find(T data){
        Node* ptr = head;
        while (ptr != nullptr){
            if (ptr->data == data)
                return ptr;
            else
                ptr = ptr->next;
        }
        return nullptr;
    }

    friend ostream& operator<< (ostream& os, List<T>& lst){
        Node* ptr = lst.head;
        while( ptr != nullptr ){
            os << ptr->data << "->";
            ptr = ptr->next;
        }
        os << "\n";
        return os;
    }

    bool empty() { return head == nullptr; }
    int size() { return sz; }
    void clear(){
        while(head != nullptr){
            Node* cur = head;
            head = head->next;
            delete cur;
        }
        sz = 0;
    }

    T front() { return head->data; }
    T back()
    {
        Node* ptr = head;
        while(ptr->next != nullptr)
            ptr = ptr->next;
        return ptr->data;
    }

    class Iterator{
    private:
        Node* current;
    public:
        Iterator(Node* node) : current(node) {}
        Iterator& operator++() {
            if (current) current = current->next;
            return *this;
        }
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
        T& operator*() const {
            return current->data;
        }
    };

    Iterator begin() const {return Iterator(head);}
    Iterator end() const {return Iterator(nullptr);}

    T get(int pos) const ;
    bool insert(int pos, T data);
    void push_front(T data);
    void push_back(T data);
    bool remove(T data);
    void pop_front();
    void pop_back();
};


template<class T>
List<T>::List(List<T>& list2) // copy constructor
{
   if(list2.head == nullptr)
   {
       return;
   }
   else
   {
        sz = list2.sz;
        head = new Node(list2.head->data);
        Node* list1Obj = head;
        Node* nodeCopy = list2.head->next;
        while(nodeCopy != nullptr)
        {
            list1Obj->next = new Node(nodeCopy->data);
            list1Obj = list1Obj->next;
            nodeCopy = nodeCopy->next;
        }
   }
}

template<class T>
List<T>& List<T>::operator=(const List& list2) // copy assignment operator
{
   if(list2.head == nullptr)
   {
       return *this;
   }
   else
   {
        clear();
        sz = list2.sz;
        head = new Node(list2.head->data);
        Node* list1Obj = head;
        Node* nodeCopy = list2.head->next;
        while(nodeCopy != nullptr)
        {
            list1Obj->next = new Node(nodeCopy->data);
            list1Obj = list1Obj->next;
            nodeCopy = nodeCopy->next;
        }
   }
   return *this;
}

template<class T>
T List<T>::get(int pos) const
{
    if(pos < 0 || pos >= sz)
    {
        cout << "Error: Invalid position" << endl;
        exit(1);
    }

    Iterator it = begin();
    std::advance(it, pos);

    return *it;
}



template<class T>

bool List<T>::insert(int pos, T data) // insert starts from the index position zero

{

    if(pos > sz) return false;

    else

    {

        sz++;

        Node* newNode = new Node(data);

        if(pos == 0)

        {

            newNode->next = head;

            head = newNode;

        }

        else

        {

            Node* ptr = head;

            for(int i=0; i<pos-1; i++)

            {

                ptr = ptr->next;

            }

            newNode->next = ptr->next;

            ptr->next = newNode;

        }

    }

    return true;

}



template<class T>

void List<T>::push_front(T data)

{

    sz++;



    Node* newNode = new Node(data);



    newNode->next = head;

    head = newNode;

}



template<class T>

void List<T>::push_back(T data)

{

    sz++;



    if (head == nullptr)

    {

        head = new Node(data);

    }

    else

    {

        Node* ptr = head;

        while(ptr->next != nullptr)

        {

            ptr = ptr->next;

        }



        ptr->next = new Node(data);

    }

}



template<class T>

bool List<T>::remove(T data)

{

    Node* prev;

    Node* cur;



    // if the list is empty, nothing to remove

    if( !head )

    {

        return false;

    }



    // head node

    if( head->data == data )

    {

        cur = head;

        head = head->next;

        delete cur;

    }

    else

    {

        cur = head;

        while( cur != nullptr && cur->data != data )

        {

            prev = cur;

            cur = cur->next;

        }



        // middle node

        // link the previous node to the current next node then delete the current node

        if(cur)

        {

            prev->next = cur->next;

            delete cur;

        }

        else

        {

            return false;

        }

    }



    sz--;

    return true;

}



template<class T>

void List<T>::pop_front()

{

    if(head)

    {

        Node* delNode = head;

        head = head->next;

        delete delNode;

        sz--;

    }

}



template<class T>

void List<T>::pop_back()

{

    if(head == nullptr) // list is empty

    {

        return;

    }

    else if(head->next == nullptr) // only one node in the list

    {

        delete head;

        head = nullptr;

        sz = 0;

    }

    else // locate the second last node

    {

        Node* secondLastNode = head;

        while(secondLastNode->next->next != nullptr)

        {

            secondLastNode = secondLastNode->next;

        }

        delete (secondLastNode->next); // delete the last node

        secondLastNode->next = nullptr;

        sz--;

    }

}



//int main()
//{
//}
