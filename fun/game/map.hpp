#ifndef __MAP__
#define __MAP__

#include <vector>

struct coordinate{
    unsigned int x;
    unsigned int y;
    
    coordinate(unsigned int,unsigned int);
    bool operator==(const coordinate&) const;
};

class map{
    public:
        // One point can represent as many as 255 types, that should be enough for everyone.
        typedef unsigned char content_type;
    private:
        // A coordinate system whose origin is at the top-left corner
        // The initial value of every point is 0
        std::vector<std::vector<content_type>> m_map;
    public:
        // An 0*0 map
        map();
        
        // A row*column map
        map(unsigned int,unsigned int);
        
        // Move constructor
        map(map&&);
        
        // Draw the map, displays every point according to the given list. 
        void draw(const std::vector<char>&) const;
        
        // Clear the map, reset to 0*0 map
        void clear();
        
        // Set coordinate to value
        void set(const coordinate&,content_type);
        
        // Return the value of coordinate
        content_type value(const coordinate&) const;
        
        // Return row
        unsigned int row() const;
        
        // Return column
        unsigned int column() const;
        
        std::vector<content_type>& operator[](unsigned int);
        map& operator=(map&&);
};

#endif // __MAP__
