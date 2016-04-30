#include <array>
#include <string>
#include <unordered_set>
#include <unordered_map>

typedef std::string string;
typedef std::unordered_map<string,string> state_map;
typedef std::unordered_map<char,string> char_map;
typedef std::unordered_set<string> halt_map;
typedef std::array<string,5> array;

class universal_tm{
    private:
        //string m_buffer;
        string    m_function;
        char_map  m_chars;
        state_map m_states;
        halt_map  m_halt;
    public:
        static const state_map directions;
    public:
        universal_tm();
        void add_action(const array &list);
        void mark_halt(const string&);
        void import(const string&);
        void clear();
        bool check(string&&);
        bool check(string&);
        bool halt(const string&) const;
        
        const string& get_fun() {return m_function;} // debug function
};
