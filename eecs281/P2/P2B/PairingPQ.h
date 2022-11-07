// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

using namespace std;

// A specialized version of the priority queue ADT implemented as a pairing heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // initialize it here.
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }, parent{ nullptr }
            {}

            // Description: Allows access to the element at that Node's position.
			// There are two versions, getElt() and a dereference operator, use
			// whichever one seems more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data members of this
            // Node class from within the PairingPQ class. (ie: myNode.elt is a legal
            // statement in PairingPQ's add_node() function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            Node *parent;
    }; // Node


    // Description: Construct an empty pairing heap with an optional comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root { nullptr }, numNodes{ 0 }{
    } // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }{
        root = nullptr;
        numNodes = 0;
            
        while (start != end){
            push(*start);
            ++start;
        }
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare }, root{ nullptr }, numNodes{ 0 }{
            
        deque<Node*> dq;
        if (!other.root){
            return;
        }
        Node *temp = other.root;
        dq.push_back(temp);
        
        while (!dq.empty()){
            
            push(temp->elt);
            
            if (temp->child){
                dq.push_back(temp->child);
            }
            
            if (temp->sibling){
                dq.push_back(temp->sibling);
            }
        
            dq.pop_front();
            temp = dq.front();
        }
        // NOTE: The structure does not have to be identical to the original,
        //       but it must still be a valid pairing heap.
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        PairingPQ temp(rhs);
        //numNodes = temp.numNodes;
        swap(temp.numNodes, numNodes);
        swap(temp.root, root);

        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        
        if (!root){
            return;
        }
        
        deque<Node*> dq;
    
        dq.push_back(root);
        
        while (!dq.empty()){
            Node *temp = dq.front();
            
            if (temp->child){
                dq.push_back(temp->child);
            }
            
            if (temp->sibling){
                dq.push_back(temp->sibling);
            }
            
            dq.pop_front();
            delete temp;

        }
        
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out of order and
    //              'rebuilds' the pairing heap by fixing the pairing heap invariant.
    //              You CANNOT delete 'old' nodes and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        
        if (!root){
            return;
        }
        
        deque<Node*> dq;
        Node *temp_root = root;
        
        dq.push_back(root->child);
        root->child = nullptr;
                
        while (!dq.empty()){
            Node *temp = dq.front();

            if (temp->child){
                dq.push_back(temp->child);
                temp->child = nullptr;
            }
            if (temp->sibling){
                dq.push_back(temp->sibling);
                temp->sibling = nullptr;
            }
            temp->parent = nullptr;
            //melding process
            temp_root = meld(temp_root, temp);
            
            //updating next element to next element in deque
            dq.pop_front();
            //temp = dq.front();
        }
        root = temp_root;
        
    } // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already done.
    //              You should implement push functionality entirely in the addNode()
    //              function, and this function calls addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the pairing heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the pairing heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        Node *temp = root->child;
                
        if (!temp){ //if root is the only element
            delete root; //ADDED
            root = nullptr; //ADDED
            numNodes--;
            return;
        }
        
        deque<Node*> dq;
        
        while (temp){
            dq.push_back(temp);
            temp = temp->sibling;
        }
        
        while (dq.size() > 1){
            Node *left = dq.front();
            left->sibling = nullptr;
            left->parent = nullptr;
            dq.pop_front();
            
            Node *right = dq.front();
            right->sibling = nullptr;
            right->parent = nullptr;
            dq.pop_front();
            
            Node *to_add = meld(left, right);
            dq.push_back(to_add);
        }
        

        delete root;
        //root = nullptr;
        numNodes--;
        root = dq.front();
        
    } // pop()

    // Description: Return the most extreme (defined by 'compare') element of the
    //              pairing heap.  This should be a reference for speed.  It MUST
    //              be const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        return root->elt;
    } // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return numNodes;
    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return numNodes == 0;

    } // empty()


    // Description: Updates the priority of an element already in the pairing heap by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE &new_value) {
        //bool leftmost = false;
        node->elt = new_value;
        
        if (node->parent->child == node){
            node->parent = nullptr;
            node->parent->child = node->sibling;
            node->sibling = nullptr;
        }
        else{
            Node *temp = node->parent->child;
            if (!this->compare(node->parent->elt, node->elt)){
                return;
            }
            if (!node->parent){
                return;
            }

            while (true){
                if (temp->sibling == node){
                    break;
                }
                temp = temp->sibling;
            }
            temp->sibling = node->sibling;
            node->sibling = nullptr;
            node->parent = nullptr;
        }
        root = meld(root, node);
        
    } // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: O(1)
    // NOTE: Whenever you create a node, and thus return a Node *, you must be sure to
    //       never move or copy/delete that node in the future, until it is eliminated
    //       by the user calling pop().  Remember this when you implement updateElt() and
    //       updatePriorities().
    Node* addNode(const TYPE &val) {
        Node *newNode = new Node(val);
        numNodes++;

        if (root){
            root = meld(root, newNode);
        }
        else{
            root = newNode;
        }

        return newNode;
        
    } // addNode()


private:
    // TODO: Add any additional member variables or member functions you require here.
    Node* meld(Node *root1, Node *root2){
        
        //if root 1 is higher priority than root 2
        if (!this->compare(root1->elt, root2->elt)){
            //root = root1;
            root2->parent = root1;
            root2->sibling = root1->child;
            root1->child = root2;
            
            return root1;
        }
        
        //if root2 is higher priority than root 1
        else{
            //root = root2;
            root1->sibling = root2->child;
            root2->child = root1;
            root1->parent = root2;
            
            return root2;
        }
    }

    // NOTE: For member variables, you are only allowed to add a "root pointer"
    //       and a "count" of the number of nodes.  Anything else (such as a deque)
    //       should be declared inside of member functions as needed.
    Node *root;// = new Node;
    size_t numNodes = 0;
};


#endif // PAIRINGPQ_H
