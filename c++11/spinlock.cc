#include <atomic>
#include <thread>

class SpinLock{
    std::atomic_flag flag;

public:
    SpinLock()
        :flag{ATOMIC_FLAG_INIT}
        {};

    SpinLock(const SpinLock& spinlock)=delete;
    SpinLock& operator=(const SpinLock& spinlock)=delete;

    ~SpinLock(){
        
    }

    void lock(){
        while(!flag.test_and_set())
        {
            //code
        }
    }

    void unlock(){
        flag.clear();
    }

}


SpinLock spin;

void workOnResource(){
  spin.lock();
  // shared resource
  spin.unlock();
}


int main(){

  std::thread t(workOnResource);
  std::thread t2(workOnResource);

  t.join();
  t2.join();

}