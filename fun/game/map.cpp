#include "map.hpp"
#include <iostream>
#include <bits/move.h>

using content_type = map::content_type;

coordinate::coordinate(unsigned int _x,unsigned int _y):x(_x),y(_y){}

bool coordinate::operator==(const coordinate &o) const {return x==o.x&&y==o.y;}

map::map():m_map() {}

map::map(unsigned int row,unsigned int column):m_map(row,std::vector<content_type>(column,false)) {}

map::map(map &&m):m_map(std::move(m.m_map)) {}

void map::draw(const std::vector<char> &list) const {
    for(auto &&row:m_map){
        for(auto &&val:row){
            std::cout<<list[val];
        }
        std::cout<<std::endl;
    }
}

void map::clear() {m_map.clear();}

void map::set(const coordinate &crd,content_type val) {m_map[crd.x][crd.y]=val;}

content_type map::value(const coordinate &crd) const {return m_map[crd.x][crd.y];}

unsigned int map::row() const {return m_map.size();}

unsigned int map::column() const {return m_map[0].size();}

std::vector<content_type>& map::operator[](unsigned int x) {return m_map[x];}

map& map::operator=(map &&m) {
    this->~map();
    m_map=std::move(m.m_map);
    return *this;
}
