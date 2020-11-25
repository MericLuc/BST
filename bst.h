/*!
 * @file   bst.h
 * @brief  Header for home-made implementations of a Binary Search Tree
 * @author lhm
 * @date   16/10/2020
 */

#include <vector>
#include <cstdint>
#include <memory>
#include <iostream>
#include <climits>
#include <queue>
#include <iterator>
#include <sstream>

#ifndef BST_H
#define BST_H

namespace BST {

template< class T >
class BST {

private:
  T                    m_val;
  std::unique_ptr<BST> m_lft;
  std::unique_ptr<BST> m_rht;

public:
  BST(T val) : m_val(val), m_lft(nullptr), m_rht(nullptr) {}

  BST& insert(T val) {
        if      ( val < m_val  )
        {
            if ( !m_lft ) { m_lft = std::unique_ptr<BST>(new BST(val)); }
            else          { m_lft->insert(val);                         }
        }
        else
        {
            if ( !m_rht ) { m_rht = std::unique_ptr<BST>(new BST(val)); }
            else          { m_rht->insert(val);                         }
        }
    return *this;
  }

  BST& insert( const std::vector<T>& val ) {
      for ( auto it : val ) { insert(it); }
      return *this;
  }

  BST& insert( std::vector<T>&& val ) {
      for ( auto it : val ) { insert(it); }
      return *this;
  }

  bool contains(T val) const {
    if      ( m_val == val         ) { return true;			         }
    else if ( m_val > val && m_lft ) { return m_lft ->contains(val); }
    else if ( m_val < val && m_rht ) { return m_rht ->contains(val); }
    return false;
  } 

  T minVal(void) const { return m_lft ? m_lft->minVal() : m_val; }
  T maxVal(void) const { return m_rht ? m_rht->maxVal() : m_val; }

  BST& remove(T val, BST* p = nullptr) {
        if      ( val <  m_val && m_lft ){ m_lft->remove(val, this); }
        else if ( val >  m_val && m_rht ){ m_rht->remove(val, this); }
        else if ( m_val == val          )
        {
            if ( m_lft && m_rht )
            {
                m_val = m_rht->minVal();
                m_rht->remove(m_val, this);
            }
            else if ( !p )
            {
                if      ( m_lft ) { m_val = m_lft->m_val; m_rht = std::move(m_lft->m_rht); m_lft = std::move(m_lft->m_lft); }
                else if ( m_rht ) { m_val = m_rht->m_val; m_lft = std::move(m_rht->m_lft); m_rht = std::move(m_rht->m_rht); }
            }
            else if ( p->m_lft.get() == this ) { p->m_lft = m_lft ? std::move(m_lft) : std::move(m_rht); }
            else if ( p->m_rht.get() == this ) { p->m_rht = m_lft ? std::move(m_lft) : std::move(m_rht); }
        }
    return *this;
  }

  T closestValue(BST* cur, T tgt) {
      int  minDiff(INT_MAX), curDiff;
      BST* minNode(cur), *curNode(cur);

      while( curNode )
      {
          curDiff = std::abs(cur->m_val - tgt);

          if ( curDiff < minDiff ) { minDiff = curDiff; minNode = curNode; }

          if      ( curNode->m_val == tgt ) { break; }
          else if ( curNode->m_val >  tgt ) { curNode = curNode->m_lft.get(); }
          else                              { curNode = curNode->m_rht.get(); }
      }

    return minNode->m_val;
  }

  std::vector<T> branchSums(void) {
      std::vector<T> l_res;
      explore(this, l_res, 0);
      return l_res;
  }

  bool isValid(BST* cur) { return validHelper(cur, INT_MIN, INT_MAX); }

  void inOrderTraverse   (BST *tree) {
      std::vector<T> m;
      inOrder(tree, m);
      std::cout << toString(m) << std::endl;
  }

  void preOrderTraverse  (BST *tree) {
      std::vector<T> m;
      preOrder(tree, m);
      std::cout << toString(m) << std::endl;
  }

  void postOrderTraverse (BST *tree) {
      std::vector<T> m;
      postOrder(tree, m);
      std::cout << toString(m) << std::endl;
  }

  void levelOrderTraverse(BST* tree) {
      std::vector<T> m;
      levelOrder(tree, m);
      std::cout << toString(m) << std::endl;
  }

  std::string toString(const std::vector<T>& v, std::string delim = " " ) {
      std::stringstream ss;
      std::copy( std::begin(v), std::end(v), std::ostream_iterator<T>(ss, delim.c_str()) );
      std::string l_ret( ss.str() );

      return l_ret.size() > delim.size() ? std::string(l_ret.begin(), l_ret.end()-delim.size() ) : l_ret;
  }

protected:
  void explore( BST* cur, std::vector<T>& res, T sum ) {
      if ( cur )
      {
          sum += cur->m_val;
          if ( !cur->m_lft && !cur->m_rht ) { res.push_back(sum); }

          explore( cur->m_lft.get(), res, sum );
          explore( cur->m_rht.get(), res, sum );
      }
  }

  bool validHelper(BST* n, T min, T max )
  {
      if      ( !n                                ) { return true;  }
      else if ( n->m_val < min || n->m_val >= max ) { return false; }
      else
      {
          if ( n->m_lft && !validHelper(n->m_lft.get() , min     , n->m_val) ) { return false; }
          if ( n->m_rht && !validHelper(n->m_rht.get() , n->m_val, max     ) ) { return false; }
      }
      return true;
  }

  void inOrder(BST* cur, std::vector<T>& m) {
      if ( cur )
      {
          inOrder    (cur->m_lft.get(), m);
          m.push_back(cur->m_val         );
          inOrder    (cur->m_rht.get(), m);
      }
  }

  void preOrder(BST* cur, std::vector<T>& m) {
      if ( cur )
      {
          m.push_back(cur->m_val);
          preOrder( cur->m_lft.get(), m);
          preOrder( cur->m_rht.get(), m);
      }
  }

  void postOrder(BST* cur, std::vector<T>& m) {
      if ( cur )
      {
          postOrder( cur->m_lft.get(), m);
          postOrder( cur->m_rht.get(), m);
          m.push_back(cur->m_val);
      }
  }

  void levelOrder(BST* tree, std::vector<T>& m) {
      std::queue<BST*> stk({tree});

      while( !stk.empty() )
      {
          BST* cur = stk.front();
          stk.pop();

          if ( cur )
          {
              m.push_back( cur->m_val       );
              stk.push   ( cur->m_lft.get() );
              stk.push   ( cur->m_rht.get() );
          }
      }
  }

};

template <class T>
BST<T> *minHeightBst( std::vector<T> m, BST<T>* cur = nullptr ) {
    if ( m.empty() ) { return nullptr; }

    int l_idx = static_cast<int>(m.size())/2;
    int l_val = m[l_idx];

    if ( !cur ) { cur = new BST<T>(l_val); }
    else        { cur->insert(l_val)  ;    }

    if ( l_idx ) {
        minHeightBst( std::vector<T>(m.begin(),         m.begin()+l_idx ), cur );
        minHeightBst( std::vector<T>(m.begin()+l_idx+1, m.end()         ), cur );
    }
  return cur;
}

} // Namespace BST

#endif // BST_H
