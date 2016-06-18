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
    // A coordinate set whose origin is at the left-up most corner
    private:
        std::vector<std::vector<bool>> m_map;
    public:
        // An empty map
        map();
        
        // An row*column map
        map(unsigned int,unsigned int);
        
        // Move constructor
        map(map&&);
        
        // Draw the map, if one block is set then print `nblank`, not set then `blank`.
        void draw(char blank,char nblank) const;
        
        // Clear the map, reset to 0*0 map
        void clear();
        
        // Set coordinate is occupied
        void set(const coordinate&);
        
        // Set coordinate is not occupied
        void unset(const coordinate&);
        
        // Apply logical not to coordinate
        void reverse(const coordinate&);
        
        // Check if coordinate is set
        bool is_set(const coordinate&) const;
        
        // Return row
        unsigned int row() const;
        
        // Return column
        unsigned int column() const;
        
        std::vector<bool>& operator[](unsigned int);
        map& operator=(map&&);
};

#endif // __MAP__
