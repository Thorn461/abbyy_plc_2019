#include <setjmp.h>
#include <deque>
#include <functional>
#include <iostream>

#define TRY switch( setjmp(env) ){ case 0:
#define CATCH(x) break; case x:
#define ADD_TO_DELETE(x) AddToDelete(x);
#define ETRY }\
        destructors.clear();
#define THROW(x) if (stack_cleaning) { \
                  std::cout << "Emergency termination of the program for re-exclusion during stack cleaning" << std::endl;\
                  exit(1); } \
                Throw();\
                longjmp(env, x)


#define EXCEPTION_1 (1)
#define EXCEPTION_2 (2)
#define EXCEPTION_3 (3)

jmp_buf env;
std::deque<std::function<void()>> destructors;
bool stack_cleaning = false;

template <typename T>
void AddToDelete(T& x) {
  destructors.push_front([&]() { x.~T(); });
}

void Throw(){
  stack_cleaning = true;
  for (auto& destrucor : destructors) {
    destrucor();
  }
  destructors.clear();
  stack_cleaning = false;
}

struct TestValue {
  TestValue(int a) : test_value_(a) {
    std::cout << "TestValue " << test_value_ << std::endl;
  }
  ~TestValue() {
    std::cout << "~TestValue " << test_value_ << std::endl;
  }
 private:
  int test_value_;
};

void f2(int a){
  TestValue x = TestValue(a);
}

void f3(int a){
  TestValue x = TestValue(a);
  ADD_TO_DELETE(x)
  THROW( EXCEPTION_1 );
}

void f4(int a){
  TestValue x = TestValue(a+1);
  ADD_TO_DELETE(x)
  f3(a);
}


int main(int argc, char** argv)
{
  TRY
      {
        std::cout << "Test 0" << std::endl;
        THROW( EXCEPTION_2 );
        std::cout << "I do not appear" << std::endl;
      }
        CATCH( EXCEPTION_1 )
      {
        std::cout << "Got EXCEPTION_1!" << std::endl;
      }
        CATCH( EXCEPTION_2 )
      {
        std::cout << "Got EXCEPTION_1!" << std::endl;
      }
        CATCH( EXCEPTION_3 )
      {
        std::cout << "Got EXCEPTION_3!" << std::endl;
      }
  ETRY

  TRY
    {
      std::cout << "Test 1" << std::endl;
      TestValue x1 = TestValue(1);
      ADD_TO_DELETE(x1)
      THROW( EXCEPTION_3 );
      std::cout << "I do not appear" << std::endl;
    }
      CATCH( EXCEPTION_1 )
    {
      std::cout << "Got EXCEPTION_1!" << std::endl;
    }
      CATCH( EXCEPTION_2 )
    {
      std::cout << "Got EXCEPTION_2!" << std::endl;
    }
      CATCH( EXCEPTION_3 )
    {
      std::cout << "Got EXCEPTION_3!" << std::endl;
    }
  ETRY

  TRY
    {
      std::cout << "Test 2" << std::endl;
      f2(2);
      THROW( EXCEPTION_2 );
      std::cout << "I do not appear" << std::endl;
    }
      CATCH( EXCEPTION_1 )
    {
      std::cout << "Got EXCEPTION_1!" << std::endl;
    }
      CATCH( EXCEPTION_2 )
    {
      std::cout << "Got EXCEPTION_2!" << std::endl;
    }
      CATCH( EXCEPTION_3 )
    {
      std::cout << "Got EXCEPTION_3!" << std::endl;
    }
  ETRY

  TRY
    {
      std::cout << "Test 3" << std::endl;
      f3(3);
      std::cout << "I do not appear" << std::endl;
    }
      CATCH( EXCEPTION_1 )
    {
      std::cout << "Got EXCEPTION_1!" << std::endl;
    }
      CATCH( EXCEPTION_2 )
    {
      std::cout << "Got EXCEPTION_2!" << std::endl;
    }
      CATCH( EXCEPTION_3 )
    {
      std::cout << "Got EXCEPTION_3!" << std::endl;
    }
  ETRY

  TRY
    {
      std::cout << "Test 4" << std::endl;
      f4(4);
      std::cout << "I do not appear" << std::endl;
    }
      CATCH( EXCEPTION_1 )
    {
      std::cout << "Got EXCEPTION_1!" << std::endl;
    }
      CATCH( EXCEPTION_2 )
    {
      std::cout << "Got EXCEPTION_2!" << std::endl;
    }
      CATCH( EXCEPTION_3 )
    {
      std::cout << "Got EXCEPTION_3!" << std::endl;
    }
  ETRY

  return 0;
}
