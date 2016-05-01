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
        string    m_function;
        char_map  m_chars;
        state_map m_states;
        halt_map  m_halt;
    public:
        enum quintuple:unsigned int {CURRENT_STATE=0,CURRENT_CHAR,NEXT_STATE,NEW_CHAR,DIRECTION};
        
        static const state_map directions;
    public:
        // Default initialization, and add a blank character 'B' into m_chars
        universal_tm();
        
        // Add an action described in qunituple `list`, its order is defined in an enum above
        void add_action(const array &list);
        
        // Mark a halt state
        void mark_halt(const string&);
        
        // Import actions from given argument
        void import(const string&);
        
        // Read actions from file, quintuple's order is the same as `add_action(...)`, one qunituple per line
        void readfile(const string&);
        
        // Clear everything but the blank character
        void clear();
        
        // Make `check(...)` accept `const char*`
        // Do not use `const char*` because the string may get modified
        bool check(string&&);
        
        // The main check function
        bool check(string&);
        
        // Check if the automaton halt in state `state`
        bool halt(const string &state) const;
        
        const string& function() {return m_function;} // debug function
};
