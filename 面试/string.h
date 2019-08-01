clasee String{
public:
    String(const char* str = nullptr);
    String(const String& other);
    String& operator=(const String& other);
    ~String();
private:
    char* m_data;
}

String::String(const char* str){
    if(str == nullptr)
    {
         m_data = new char[1];
        *m_data = '\0';
    }
    else{
        int len = strlen(str);
        m_data = new char[len+1];
        strcpy(m_data, str);
    }   
}


// 拷贝构造函数
String::String(const String &other) 
{ 
    // 允许操作other的私有成员m_data 
    int length = strlen(other.m_data); 
    m_data = new char[length+1]; 
    strcpy(m_data, other.m_data); 
} 
// 赋值函数
String & String::operate =(const String &other) 
{ 
    // (1) 检查自赋值
    if(this == &other) 
    return *this; 
    // (2) 释放原有的内存资源
    delete [] m_data; 
    // （3）分配新的内存资源，并复制内容
    int length = strlen(other.m_data); 
    m_data = new char[length+1]; 
    strcpy(m_data, other.m_data); 
    // （4）返回本对象的引用
    return *this; 
}

/*
    其他的写法：
    //2. copy and swap技术
    String& operator=(const String& rhs) {
        String temp(rhs);       //利用复制构造函数
        Swap(rhs);
        return *this;
    }
    //3. 传值方式
    String& operator=(String rhs) {
        Swap(rhs);
        return *this;
    }
    //4. c++11右值引用
    String& operator= (String&& rhs) {
        Swap(rhs);
        return *this;
    }
*/
