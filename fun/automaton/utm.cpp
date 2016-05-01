#include "utm.hpp"
#include <fstream> // std::fstream
#include <bits/move.h> // std::move

// Encoding into '1'-string
static string lookup(const string &content,unsigned int &count,state_map &map){
    auto &&str=map[content];
    if(str.empty())
        str=string(++count,'1');
    return str;
}

// Encoding into '1'-string
static string lookup(const string &content,unsigned int &count,char_map &map){
    auto &&str=map[content[0]];
    if(str.empty())
        str=string(++count,'1');
    return str;
}

// Decoding to origin, this function may get optimized
static char lookup(const string &content,char_map &map){
    for(auto &i:map){
        if(content==i.second)
            return i.first;
    }
    return '\0';
}

static inline string split(const string &str,const char delim,string::size_type pos=0){
    return str.substr(pos,str.find_first_of(delim,pos)-pos);
}

const state_map universal_tm::directions{{"L","1"},{"R","11"},{"S","111"}};

universal_tm::universal_tm():m_function(),m_chars{{'B',"1"}},m_states(),m_halt() {}

// The given quintuple must follow the order below:
// (current state,current char,next state,new char,direction)
// The encoded quintuples are in this order:
// separator + (current state,current char,new char,direction,next state)
// 
// This requires the argument of the first call begins with the initial state
void universal_tm::add_action(const array &list) {
    unsigned int char_count=m_chars.size(),state_count=m_states.size(); // the blank character already exists
    
    string tmp("00"); // separator between actions
    tmp += lookup(list[CURRENT_STATE], state_count, m_states);
    tmp += '0';
    tmp += lookup(list[CURRENT_CHAR], char_count, m_chars);
    tmp += '0';
    tmp += lookup(list[NEW_CHAR], char_count, m_chars);
    tmp += '0';
    tmp += directions.at(list[DIRECTION]);
    tmp += '0';
    tmp += lookup(list[NEXT_STATE], state_count, m_states);
    m_function += tmp;
}

// `state` must exist in `m_states`, otherwise this will throw an exception
void universal_tm::mark_halt(const string &state) {m_halt.insert(m_states.at(state));}

void universal_tm::import(const string &func) {m_function=func;}

void universal_tm::readfile(const string &file_loc){
    std::fstream file(file_loc,std::ios::in);
    string current_state,current_char,next_state,new_char,direction;
    while(!file.eof()){
        file>>current_state>>current_char>>next_state>>new_char>>direction;
        if(direction.empty()) // file will read an empty line, all 5 strings are empty
            break;
        array &&arr={{std::move(current_state),std::move(current_char),std::move(next_state),std::move(new_char),std::move(direction)}};
        add_action(arr);
    }
    file.close();
}

void universal_tm::clear() {
    m_function.clear();
    m_chars.clear();
    m_states.clear();
    m_halt.clear();
    
    m_chars['B']="1";
}

bool universal_tm::halt(const string &state) const {return m_halt.find(state)!=m_halt.cend();}

// calls lvalue-reference version
bool universal_tm::check(string &&str) {return check(str);}

// The argument `str` must be un-encoded string
bool universal_tm::check(string &str){
    int str_cursor=0;
    constexpr unsigned int start=2; // `m_function` begins with "00"
    
    // get initial state
    string current_state=split(m_function,'0',start);
    string current_char("");
    
    for(;;){
        // get current encoded character on tape
        if(str_cursor<0 || str_cursor>=int(str.size()))
            current_char=m_chars.at('B');
        else
            current_char=m_chars.at(str[str_cursor]);
        
        // find the corresponding quintuple
        auto &&buffer="00"+current_state+'0'+std::move(current_char); // `current_char` is never used
        auto pos=m_function.find(buffer); // possible bug caused by encoding
        
        while(pos!=string::npos){
            if(m_function[pos+buffer.size()]=='0')
                break;
            pos=m_function.find(buffer,pos+buffer.size()); // find until meet a full match
        }
        
        if(pos==string::npos) // if not found or no full match
            return halt(current_state);
        
        // modify current_char to new_char
        auto new_char=lookup(split(m_function,'0',pos+=buffer.size()+1),m_chars); // `pos` jumps over separators and current state section and current char section
        if(new_char=='\0')
            return false;
        if(str_cursor<0 && new_char!='B')
            str=new_char+str;
        else if(str_cursor>=int(str.size()) && new_char!='B')
            str+=new_char;
        else
            str[str_cursor]=new_char;
        
        // move read position
        auto direction=split(m_function,'0',pos+=m_chars[new_char].size()+1); // `pos` jumps over a separator and character section
        if(direction=="1") // "L"
            --str_cursor;
        else if(direction=="11") // "R"
            ++str_cursor;
        // else ; // "S"
        
        // set next state
        current_state=split(m_function,'0',pos+=direction.size()+1);
    }
    return true; // dummy return to avoid warning
}

