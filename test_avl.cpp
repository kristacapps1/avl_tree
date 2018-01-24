//Series of tests for AVL
#include <algorithm>
#include <string>

#include "avl.h"

#include "unit_test.h"

using std::all_of;
using std::string;
using std::pair;
using std::make_pair;
using mystl::avl;

////////////////////////////////////////////////////////////////////////////////
/// @brief Testing of avl
/// @ingroup Testing
////////////////////////////////////////////////////////////////////////////////
class avl_test : public test_class {

  protected:

    void test() {
	  test_default_constructor();

	  std::cout<<"test_default_constructor() passed"<<std::endl;
	  //print_avl();

      test_element_access_operator_exists();

	  std::cout<<"test_element_access_operator_exists() passed"<<std::endl;

      test_element_access_operator_not_exists();

	  std::cout<<"test_element_access_operator_exists() passed"<<std::endl;

      test_element_access_at_exists();

	  std::cout<<"test_element_access_at_exists() passed"<<std::endl;

      test_element_access_at_not_exists();

	  std::cout<<"test_element_access_at_not_exists() passed"<<std::endl;

      test_find_exists();

	  std::cout<<"test_find_exists() passed"<<std::endl;

      test_find_not_exists();

	  std::cout<<"test_find_not_exists() passed"<<std::endl;

      test_count_exists();

	  std::cout<<"count_exists() passed"<<std::endl;

      test_count_not_exists();

	  std::cout<<"count_not_exists() passed"<<std::endl;

      test_insert_exists();

	  std::cout<<"insert_exists() passed"<<std::endl;

      test_insert_not_exists();

	  std::cout<<"insert_not_exists() passed"<<std::endl;

      test_erase_iterator();

	  std::cout<<"test_erase_iterator() passed"<<std::endl;

      test_erase_key();

	  std::cout<<"test_erase_key() passed"<<std::endl;

      test_copy_constructor();

	  std::cout<<"test_copy_constructor() passed"<<std::endl;

      test_copy_assign();

	  std::cout<<"test_copy_assign() passed"<<std::endl;
    }

  private:

    /// @brief Setup avl of integers to strings
    void setup_dummy_avl(avl<int, string>& m) {
      m[3] = "l";
      m[1] = "H";
      m[2] = "e";
      m[5] = "o";
      m[4] = "l";
    }
	///@brief print avl
	void print_avl(){
	  avl<int, string> m;
      setup_dummy_avl(m);
	  m.prints();
	}

    /// @brief Test default constructor generates avl of size 0
    void test_default_constructor() {
      avl<int, string> m;

      assert_msg(m.size() == 0 && m.empty(),
          "Default construction failed.");
    }

    /// @brief Test element access operator when element exists
    void test_element_access_operator_exists() {
      avl<int, string> m;
      setup_dummy_avl(m);

      string val = m[5];

      assert_msg(val == "o", "Element access operator exists failed");
    }

    /// @brief Test element access operator when element does not exist
    void test_element_access_operator_not_exists() {
      avl<int, string> m;
      setup_dummy_avl(m);

      string val = m[7];

      assert_msg(val == "", "Element access operator not exists failed");
    }

    /// @brief Test element access at when element exists
    void test_element_access_at_exists() {
      avl<int, string> m;
      setup_dummy_avl(m);

      string val = m.at(5);

      assert_msg(val == "o", "Element access at exists failed");
    }

    /// @brief Test element access at when element does not exist, ensure this
    ///        function will throw an error.
    void test_element_access_at_not_exists() {
      avl<int, string> m;
      setup_dummy_avl(m);

      try {
        string val = m.at(7);
        assert_msg(false, "Element access at not exists failed");
      }
      catch(std::out_of_range) {
        //test success!
      }
      catch(...) {
        assert_msg(false, "Element access at not exists failed");
      }
    }

    /// @brief Test find when element exists
    void test_find_exists() {
      avl<int, string> m;
      setup_dummy_avl(m);

      avl<int, string>::iterator i = m.find(5);

      assert_msg(i->first == 5 && i->second == "o", "Find exists failed.");
    }

    /// @brief Test find when element does not exist
    void test_find_not_exists() {
      avl<int, string> m;
      setup_dummy_avl(m);

      avl<int, string>::iterator i = m.find(7);

      assert_msg(i == m.end(), "Find exists failed.");
    }

    /// @brief Test count when element exists
    void test_count_exists() {
      avl<int, string> m;
      setup_dummy_avl(m);

      size_t i = m.count(5);

      assert_msg(i == 1, "Count exists failed.");
    }

    /// @brief Test count when element does not exist
    void test_count_not_exists() {
      avl<int, string> m;
      setup_dummy_avl(m);

      size_t i = m.count(7);

      assert_msg(i == 0, "Count exists failed.");
    }

    /// @brief Test insertion when element is already in avl
    void test_insert_exists() {
      avl<int, string> m;
      setup_dummy_avl(m);

      pair<avl<int, string>::iterator, bool> i = m.insert(make_pair(5, "o"));

      avl<int, string>::iterator j = m.begin();
      while(j != m.end() && i.first != j)
        ++j;

      assert_msg(m.size() == 5 && i.first == j && !i.second,
          "Insert exists failed.");
    }

    /// @brief Test insertion when element is not already in avl
    void test_insert_not_exists() {
      avl<int, string> m;
      setup_dummy_avl(m);

      pair<avl<int, string>::iterator, bool> i = m.insert(make_pair(7, "!"));

      avl<int, string>::iterator j = m.begin();
      while(j != m.end() && i.first != j)
        ++j;

      assert_msg(m.size() == 6 && i.first == j && i.second,
          "Insert not exists failed.");
    }

    /// @brief Test erase with an iterator
    void test_erase_iterator() {
      avl<int, string> m;
      setup_dummy_avl(m);
      avl<int, string>::iterator j = ++m.begin();

      avl<int, string>::iterator i = m.erase(m.begin());

      assert_msg(i == j && m.size() == 4, "Erase iterator failed.");
    }

    /// @brief Test erase with a key
    void test_erase_key() {
      avl<int, string> m;
      setup_dummy_avl(m);

      size_t i = m.erase(5);

      assert_msg(i == 1 && m.size() == 4, "Erase key failed.");
    }

    /// @brief Test copy constuction
    void test_copy_constructor() {
      avl<int, string> m1;
      setup_dummy_avl(m1);

      avl<int, string> m2(m1);

      for(auto&& x : m2)
        x.second = "w";

      assert_msg(m2.size() == m1.size() &&
          all_of(m1.begin(), m1.end(),
            [](const avl<int, string>::value_type& x) {
            return x.second != "w";}
            ) &&
          all_of(m2.begin(), m2.end(),
            [](const avl<int, string>::value_type& x) {
            return x.second == "w";}
            ),
          "Copy constructor failed.");
    }

    /// @brief Test copy assignment
    void test_copy_assign() {
      avl<int, string> m1;
      setup_dummy_avl(m1);

      avl<int, string> m2;
      m2[4] = "*";

      m2 = m1;

      for(auto&& x : m2)
        x.second = "w";

      assert_msg(m2.size() == m1.size() &&
          all_of(m1.begin(), m1.end(),
            [](const avl<int, string>::value_type& x) {
            return x.second != "w";}
            ) &&
          all_of(m2.begin(), m2.end(),
            [](const avl<int, string>::value_type& x) {
            return x.second == "w";}
            ),
          "Copy assign failed.");
    }
};

int main() {
  avl_test lt;

  if(lt.run())
    std::cout << "All tests passed." << std::endl;

  return 0;
}

