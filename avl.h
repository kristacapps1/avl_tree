#ifndef _MAP_H_
#define _MAP_H_

#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <iostream>

namespace mystl {

////////////////////////////////////////////////////////////////////////////////
/// @brief avl ADT based on C++ avl implemented with binary search tree
/// @ingroup MySTL
/// @tparam Key Key type
/// @tparam Value Value type
///
/// Assumes the following: There is always enough memory for allocations (not a
/// good assumption, just good enough for our purposes); Functions not
/// well-defined on an empty container will exhibit undefined behavior.
////////////////////////////////////////////////////////////////////////////////
template<typename Key, typename Value>
class avl {

  class node;           ///< Forward declare node class
  template<typename>
  class map_iterator; ///< Forward declare iterator class

  public:

    ////////////////////////////////////////////////////////////////////////////
    /// @name Types
    /// @{

    typedef Key key_type;      ///< Public access to Key type
    typedef Value mapped_type; ///< Public access to Value type
    typedef std::pair<const key_type, mapped_type>
      value_type;              ///< Entry type
    typedef map_iterator<value_type>
      iterator;                ///< Bidirectional iterator
    typedef map_iterator<const value_type>
      const_iterator;          ///< Const bidirectional iterator
    typedef std::reverse_iterator<iterator>
      reverse_iterator;        ///< Reverse bidirectional iterator
    typedef std::reverse_iterator<const_iterator>
      const_reverse_iterator;  ///< Const reverse bidirectional iterator

    /// @}
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    /// @name Constructors
    /// @{

    /// @brief Constructor
    avl() : root(new node()), sz(0) {
      root->expand();
    }
    /// @brief Copy constructor
    /// @param m Other avl
    avl(const avl& m) : root(new node(*m.root)), sz(m.sz) {
    }
    /// @brief Destructor
    ~avl() {
      delete root;
    }

    /// @brief Copy assignment
    /// @param m Other avl
    /// @return Reference to self
    avl& operator=(const avl& m) {
      if(this != &m) {
        delete root;
        root = new node(*m.root);
        sz = m.sz;
      }
      return *this;
    }

    /// @}
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    /// @name Iterators
    /// @{

    /// @return Iterator to beginning
    iterator begin() {return iterator(root->leftmost());}
    /// @return Iterator to end
    iterator end() {return iterator(root);}
    /// @return Iterator to reverse beginning
    reverse_iterator rbegin() {return reverse_iterator(root);}
    /// @return Iterator to reverse end
    reverse_iterator rend() {return reverse_iterator(root->leftmost());}
    /// @return Iterator to beginning
    const_iterator cbegin() const {return const_iterator(root->leftmost());}
    /// @return Iterator to end
    const_iterator cend() const {return const_iterator(root);}
    /// @return Iterator to reverse beginning
    const_reverse_iterator crbegin() const {return const_reverse_iterator(root);}
    /// @return Iterator to reverse end
    const_reverse_iterator crend() const {return const_reverse_iterator(root->leftmost());}

    /// @}
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    /// @name Capacity
    /// @{

    /// @return Size of avl
    size_t size() const {return sz;}
    /// @return Does the avl contain anything?
    bool empty() const {return sz == 0;}

    /// @}
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    /// @name Element Access
    /// @{

    /// @param k Input key
    /// @return Value at given key
    ///
    /// If \c k matches an element in the container, the function will return a
    /// reference to its mapped value.
    ///
    /// If \c k is not found in the container, the function should insert a new
    /// element with that key and return a reference to its mapped value
    /// (constructed through default construction)
    Value& operator[](const Key& k) {
      value_type q = std::make_pair(k,Value());
      std::pair<node*,bool> i = inserter(q);
      node* n = i.first;
      return n->value.second;
    }

    /// @param k Input key
    /// @return Value at given key
    ///
    /// If \c k matches an element in the container, the function will return a
    /// reference to its mapped value.
    ///
    /// If \c k is not found in the container, the function throws an
    /// \c out_of_range exception.
    Value& at(const Key& k) {
      map_iterator<value_type> m = find(k);
      if(m == end()){
        throw std::out_of_range("out of range; not found");
      }
      else{
        value_type q = *m;
        node* n = new node(q);
        return n->value.second;
      }
    }

    /// @brief prints height, whether node is internal,  and value_type of tree
    /// preorder
    void prints() {
      prints(root->left);

    }
    void prints(node* n) {
        std::cout << n->value.first << " " << n->is_internal() << " "<<n->height<<std::endl;
        if(n->left != NULL)
          prints(n->left);
        if(n->right != NULL)
          prints(n->right);
    }
    /// @param k Input key
    /// @return Value at given key
    ///
    /// If \c k matches an element in the container, the function will return a
    /// reference to its mapped value.
    ///
    /// If \c k is not found in the container, the function throws an
    /// \c out_of_range exception.
    const Value& at(const Key& k) const {
	  map_iterator<value_type> m = find(k);
	  if(m == end()){
		  throw std::out_of_range("out of range; not found");
	  }
	  else{
		const value_type q = *m;
		node* n = new node(q);
		return n->value.second;
	  }
      return end()->second;
    }

    /// @}
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    /// @name Modifiers
    /// @{

    /// @brief Insert element into avl, i.e., put(k, v) from the avl ADT
    /// @param v Key, Value pair
    /// @return pair of iterator and bool. Iterator pointing to found element or
    ///         already existing element. bool is true if a new element was
    ///         inserted and false if it existed.
    ///
    /// Insert is the "put(k, v)" function of the avl ADT. Remember that avls
    /// store unique elements, so if the element existed already it is returned.
    std::pair<iterator, bool> insert(const value_type& v) {
      std::pair<node*, bool> n = inserter(v);
      return std::make_pair(iterator(n.first), n.second);
    }
    /// @brief Remove element at specified position
    /// @param position Position
    /// @return Position of new location of element which was after eliminated
    ///         one
    iterator erase(const_iterator position) {
	  const value_type v = *position;
	  node* n = finder(v.first);
	  if(n->is_external()){
		  throw std::out_of_range("out of range; not found");
	  }
	  n = eraser(n);
      return iterator(n);
    }
    /// @brief Remove element at specified position
    /// @param k Key
    /// @return Number of elements removed (in this case it is at most 1)
    size_t erase(const Key& k) {
	  node* n = finder(k);
	  size_t removed = 0;
	  if(n->is_internal()){
		eraser(n);
		removed++;
	  }
      return removed;
    }
    /// @brief Removes all elements
    void clear() noexcept {
      delete root;
      root = new node();
      root->expand();
      sz = 0;
    }

    /// @}
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    /// @name Operations
    /// @{

    /// @brief Search the container for an element with key \c k
    /// @param k Key
    /// @return Iterator to position if found, end() otherwise
    iterator find(const Key& k) {
	  	  node* b = finder(k);
	  if(b->is_internal()){
		  return iterator(b);
	  }
	  else
        return end();
    }

    /// @brief Search the container for an element with key \c k
    /// @param k Key
    /// @return Iterator to position if found, cend() otherwise
    const_iterator find(const Key& k) const {
	  	node* b = finder(k);
	  if(b->is_internal()){
		 map_iterator<const value_type> m = iterator(b);
		 return m;
	  }
	  else
         return cend();
    }

    /// @brief Count elements with specific keys
    /// @param k Key
    /// @return Count of elements with key \c k
    ///
    /// Because all elements in a avl container are unique, the function will
    /// only return 1 or 0.
    size_t count(const Key& k) const {
	  node* b = finder(k);
	  if(b->is_internal()){
      return 1;
	  }
	  else
		  return 0;
    }

    /// @}
    ////////////////////////////////////////////////////////////////////////////

  private:

    ////////////////////////////////////////////////////////////////////////////
    /// @name Helpers
    /// @{

    /// @brief Utility for finding a node with Key \c k
    /// @param k Key
    /// @return Node pointer to either where node exists or where it should be
    ///         inserted
    ///
    /// ****Base your algorithm off of Code Fragment 10.9 on page 436
    node* finder(const Key& k) const {
	  node* v = root->left;
	  while(v->is_internal() && v->value.first != k){
		  if(k < v->value.first){
			v = v->left;
		  }
		  else			//k > v->value.first
			v = v->right;
	  }
		return v;
	}

    /// @brief Utility for inserting a new node into the data structure.
    /// @param v Key, Value pair
    /// @return pair of node and bool. node pointing to found element or
    ///         already existing element. bool is true if a new element was
    ///         inserted and false if it existed.
    ///
    /// Inserter is the "put(k, v)" function of the avl ADT. Remember that avls
    /// store unique elements, so if the element existed already it is returned.
    ///
    /// Base you algorithm off of Code Fragment 10.10 on page 436
    ///
    /// Hint: Will need to use functions node::replace and node::expand
    std::pair<node*, bool> inserter(const value_type& v) {
      node* n = finder(v.first);
      bool changed = false;
      if(n->is_external()){
        n->expand();
        n->replace(v);
        rebalance(n);
        sz++;
        changed = true;
      }
      return std::make_pair(n, changed);
    }

    /// @brief Erase a node from the tree
    /// @param n Node to erase
    /// @return Next inorder successor of \c n in tree
    ///
    /// Base your algorithm off of Code Fragment 10.11 on page 437
    ///
    /// Hint: will need to use functions node::inorder_next, node::replace, and
    /// node::remove_above_external

    node* eraser(node* n) {
	  node* w;
	  if(n->left->is_external()) w = n->left;
	  else if(n->right->is_external()) w = n->right;
	  else{
		  w = n->inorder_next();
		  n->replace(w->value);
		  w = w->left;
	  }
	  sz--;
      return w->remove_above_external();
    }

///@brief Rebalances tree by resetting heights and checking if tree is balanced

    void rebalance(node* v){
      node* z = v;
      while(!z->parent->is_root()){   //recheck height of all nodes along path until root
        z = z->parent;
        z->set_height();
        if(!is_balanced(z)){  //check balance of path
          node* x = tall_grandchild(z);
          z = restructure(x);  //restructure trinode where x is tall grandchild
          z->left->set_height();  //update heights of trinode
          z->right->set_height();
          z->set_height();
        }
      }
    }

    ///@brief find tall grandchild of original root of trinode

    node* tall_grandchild(const node* z){
      node* zl = z->left;	
      node* zr = z->right;
      if(zl->height >= zr->height){		//if left is greater
        if(zl->left->height >= zl->right->height)   //if left is greater again
          return zl->left;
        else							//otherwise left, right greater
          return zl->right;
      }
      else{
        if(zr->right->height >= zr->left->height)   //if right greater
          return zr->right;
        else
          return zr->left;				//otherwise right, left greater
      }
    }
    ///@brief check heiht of node
	size_t height(const node* v){
		return(v->is_external() ? 0 : v->height);
	}
    ///@brief check whether trinode is balanced given root of trinode
        bool is_balanced(const node* v){
          int bal = v->left->height - v->right->height;
          return ((-1 <= bal)&&(bal<=1));
        }
    ///@brief restructure trinode through pointer swaps
	node* restructure(node* x){   //where x is tall grandchild
		node* y = x->parent;
		node* z = x->parent->parent;
		node* a;                          //a,b,c inorder listing nodes
		node* b;
		node* c;
		if(z->right == y && y->right ==x){ //single rotation right
			a = z;
			b = y;
			c = x;
		}
		else if(z->left == y && y->left ==x){//single rotation left
			a = x;
			b = y;
			c = z;
		}
		else if(z->right == y && y->left ==x){//double rotation right
			a = z;
			b = x;
			c = y;
		}
		else { //z->left == y && y->right ==x double rotation left
			a = y;
			b = x;
			c = z;
		}
		if(z->parent->left == z) {  //includes super root case
			z->parent->left = b;
		}
		if(z->parent->right == z) {
			z->parent->right = b;
		}
		b->parent = z->parent; //new root for trinode complete
		a->parent = b;
		c->parent = b;
		if(b->left!=a){  //subtree is b->left
                        //b->left->parent = a;
			a->right = b->left;  //give subtree new parent
			b->left->parent = a;
		}
		b->left = a;   //now that subtree has been moved
		if(b->right != c){ //subtree is b->right
		        //b->right->parent = c;
                        c->left = b->right;   //give subtree new parent
		  	b->right->parent = c;
                }
		b->right = c;  //now that subtree was moved
		return b;
	}
    /// @}
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    /// @name Data
    /// @{

    node* root;     ///< Root of binary tree, the root will be a sentinel node
                    ///< for end iterator. root.left is the "true" root for the
                    ///< data
    size_t sz;      ///< Number of nodes

    /// @}
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    /// @name Types
    /// @{

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Internal structure for binary search tree
    ////////////////////////////////////////////////////////////////////////////
    struct node {
      public:

        ////////////////////////////////////////////////////////////////////////
        /// @name Constructors
        /// @{

        /// @brief Constructor
        /// @param v avl entry (Key, Value) pair
        node(const value_type& v = value_type()) :
          value(v), parent(nullptr), left(nullptr), right(nullptr), height(0) {}

        /// @brief Copy constructor
        /// @param n node to perform deep copy from
        node(const node& n) : value(n.value), parent(nullptr), left(nullptr), right(nullptr) {
		  	if (n.is_internal()){
                left = new node(*n.left);
                right = new node (*n.right);
                left->parent = this;
                right->parent = this;
            }
        }

        /// @brief Copy assignment - Deleted
        /// @param n Other node
        node& operator=(const node& n) = delete;

        /// @brief Destructor
        ~node() {
			delete left;
			delete right;
        }

        /// @}
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        /// @name Modifiers
        /// @{

        /// @brief Replace node with a new node of a different value
        /// @param v New value
        /// @return Pointer to new node
        void replace(const value_type& v) {
          const_cast<Key&>(value.first) = v.first;
          value.second = v.second;
        }

        /// @brief Expand external node to make it internal
        void expand() {
          left = new node;
          right = new node;
	      set_height();  //update height of node and all parents
          left->parent = this;
          right->parent = this;
        }

        void set_height(){
          size_t hl = left->height;
          size_t hr = right->height;
          height = (1+ std::max(hl,hr));	//height of node is increment max of children
        }

        /// @brief Remove above external node, assumes this is external node
        /// @return Sibling of \c n, who is promoted to n's parent's position
        node* remove_above_external() {
          node* par = parent;
          node* sib = this == par->left ? par->right : par->left;
          node* gpar = par->parent;
          if(par == gpar->left)
            gpar->left = sib;
          else
            gpar->right = sib;
          sib->parent = gpar;
          par->left = par->right = nullptr;
          delete this;
          delete par;
          return sib;
        }

        /// @}
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        /// @name Accessors

        /// @return If parent is null return true, else false
        bool is_root() const {return parent == nullptr;}
        /// @return If both children are null return true, else false
        bool is_external() const {return left == nullptr && right == nullptr;}
        /// @return If it is not external then it is internal
        bool is_internal() const {return !is_external();}
        /// @return Leftmost child of this node
        node* leftmost() const {
          const node* n = this;
          while(n->is_internal()) n = n->left;
          return n->parent;
        }


        /// @return Next node in the binary tree according to an inorder
        ///         traversal
        node* inorder_next() {
          //Here, I have a right child, so inorder successor is leftmost child
          //of right subtree
          if(right->is_internal()) {
            return right->leftmost();
          }
          //Otherwise, I am a right child myself and need to find an ancestor
          //who has a right child
          else {
            node* n = this;
            node* w = parent;
            while(n == w->right) {
              n = w;
              w = w->parent;
            }
            return w;
          }
        }

        /// @return Previous node in the binary tree according to an inorder
        ///         traversal
        node* inorder_prev() {
          //Here, I have a left child, so inorder predecessor is rightmost child
          //of left subtree
          if(left->is_internal()) {
            node* n = left;
            while(n->is_internal()) n = n->right;
            return n->parent;
          }
          //Otherwise, I am a left child myself and need to find an ancestor
          //who has a left child
          else {
            node* n = this;
            node* w = parent;
            while(n == w->left) {
              n = w;
              w = w->parent;
            }
            return w;
          }
        }

        /// @}
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        /// @name Data
        /// @{

        value_type value; ///< Value is pair(key, value)
        node* parent;     ///< Parent node
        node* left;       ///< Left node
        node* right;      ///< Right node
		size_t height; 	  ///< Height of node

        /// @}
        ////////////////////////////////////////////////////////////////////////
    };

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Bidirectional iterator for a linked binary tree
    /// @tparam U value_type of avl
    ////////////////////////////////////////////////////////////////////////////
    template<typename U>
      class map_iterator : public std::iterator<std::bidirectional_iterator_tag, U> {
        public:
          //////////////////////////////////////////////////////////////////////
          /// @name Constructors
          /// @{

          /// @brief Construction
          /// @param v Pointer to position in binary tree
          map_iterator(node* v = NULL) : n(v) {}

          /// @brief Copy construction
          /// @param i Other iterator
          map_iterator(const map_iterator<typename std::remove_const<U>::type>& i) : n(i.n) {}

          /// @}
          //////////////////////////////////////////////////////////////////////

          //////////////////////////////////////////////////////////////////////
          /// @name Comparison
          /// @{

          /// @brief Equality comparison
          /// @param i Iterator
          bool operator==(const map_iterator& i) const {return n == i.n;}
          /// @brief Inequality comparison
          /// @param i Iterator
          bool operator!=(const map_iterator& i) const {return n != i.n;}

          /// @}
          //////////////////////////////////////////////////////////////////////

          //////////////////////////////////////////////////////////////////////
          /// @name Dereference
          /// @{

          /// @brief Dereference operator
          U& operator*() const {return n->value;}
          /// @brief Dereference operator
          U* operator->() const {return &n->value;}

          /// @}
          //////////////////////////////////////////////////////////////////////

          //////////////////////////////////////////////////////////////////////
          /// @name Advancement
          /// @{

          /// @brief Pre-increment
          map_iterator& operator++() {n = n->inorder_next(); return *this;}
          /// @brief Post-increment
          map_iterator operator++(int) {map_iterator tmp(*this); ++(*this); return tmp;}
          /// @brief Pre-decrement
          map_iterator& operator--() {n = n->inorder_prev(); return *this;}
          /// @brief Post-decrement
          map_iterator operator--(int) {map_iterator tmp(*this); --(*this); return tmp;}

          /// @}
          //////////////////////////////////////////////////////////////////////

        private:
          node* n; ///< avl node

          friend class avl;
      };

    /// @}
    ////////////////////////////////////////////////////////////////////////////

};

}

#endif
