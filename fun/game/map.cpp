#include "map.hpp"
#include <iostream>
#include <bits/move.h>

coordinate::coordinate(unsigned int _x,unsigned int _y):x(_x),y(_y){}

bool coordinate::operator==(const coordinate &o) const {return x==o.x&&y==o.y;}

map::map():m_map() {}

map::map(unsigned int row,unsigned int column):m_map(row,std::vector<bool>(column,false)) {}

map::map(map &&m):m_map(std::move(m.m_map)) {}

void map::draw(char blank,char nblank) const {
    for(auto &&row:m_map){
        for(auto &&set:row){
            if(set)
                std::cout<<nblank;
            else
                std::cout<<blank;
        }
        std::cout<<std::endl;
    }
}

void map::clear() {m_map.clear();}

void map::set(const coordinate &crd) {m_map[crd.x][crd.y]=true;}

void map::unset(const coordinate &crd) {m_map[crd.x][crd.y]=false;}

void map::reverse(const coordinate &crd) {m_map[crd.x][crd.y]=!m_map[crd.x][crd.y];}

bool map::is_set(const coordinate &crd) const { return m_map[crd.x][crd.y];}

unsigned int map::row() const {return m_map.size();}

unsigned int map::column() const {return m_map[0].size();}

std::vector<bool>& map::operator[](unsigned int x) {return m_map[x];}

map& map::operator=(map &&m) {
    this->~map();
    m_map=std::move(m.m_map);
    return *this;
}
