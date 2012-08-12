#include "decoder.h"
#include "input.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <set>

heap::heap( int size ){

		m_size = size;
		m_count = 0;
		m_bcnt = 0;
		init_heap();
}

heap::heap(){

		m_size = 0;
		m_count = 0;
		m_bcnt =0 ;
		m_array = NULL;
		m_backup = NULL;

}

heap::~heap(){

		for( int i=0; i < m_count; ++i ){
				delete (m_array[i]).ptr;
		}
		delete [] m_array;

		for( int i=0; i < m_bcnt; ++i ){
				delete (m_backup[i]).ptr;
		}

		delete [] m_backup;
		std::cout << "heap::~heap\n";
}

void heap::setsize( int size ){
		if( (m_size != 0) || ( m_count != 0 ) || 
			( m_bcnt != 0 ) || ( m_array != NULL ) || (m_backup != NULL) ){
				std::cout << "heap::setsize error!\n";
				abort();
		}
		m_size = size;
		init_heap();
}

void heap::init_heap(){

		m_array = new heapitem[m_size];
		if( !m_array ){
				std::cerr << "heap::init_heap new Failed\n";
				abort();
		}
		for( int i=0; i<m_size; ++i ){
				(m_array[i]).score = 0;
				(m_array[i]).ptr = NULL;
		}

		m_backup = new heapitem[m_size];
		if( !m_backup ){
				std::cerr << "heap::init_heap new failed\n";
				abort();
		}
		for( int i=0; i<m_size; ++i ){
				(m_backup[i]).score = 0;
				(m_backup[i]).ptr = NULL;
		}
}

bool heap::push( hypothesis* dat ){

		if( m_count >= m_size ){
				return false;
		}
  
		(m_array[m_count]).score = dat->getscore();
		(m_array[m_count]).ptr = dat;

		m_count += 1;
		reformulate();
		return true;
}

void heap::reformulate(){

		for( int i=m_count/2; i >=1; --i){
				heapify( i );
		}

}

void heap::heapify( int pos ){

		int left, right, root=pos, target;
		heapitem tmp;
		tmp = m_array[pos-1];
		while( root*2 <= m_count ){

				left = root*2;
				right = root*2 + 1;
				if( ( right <= m_count ) && 
					( (m_array[right-1].score) > (m_array[left-1]).score ) ){
						target = right;
				} else {
						target = left;
				}

				if( (m_array[target-1]).score > (m_array[root-1]).score ){
						m_array[root-1] = m_array[target -1];
						root = target;
				} else {
						break;
				}

		}

		m_array[root-1] = tmp;

}

void heap::del( hypothesis* dat ){

		int pos = -1;
		for( int i=0; i< m_count; ++i ){
				if( (m_array[i]).ptr == dat ){
						pos = i;
						break;
				}
		}

		if( -1 == pos )
				return;
		(m_array[pos]).score = 0;
		(m_array[pos]).ptr = NULL;
		if( pos != m_count -1 ){
				std::swap( m_array[pos], m_array[m_count-1] );
		}

		m_count -= 1;
		reformulate();
}

heapitem* heap::begin() const {
		return &( m_array[0] );
}

heapitem* heap::end() const {
		return &(m_array[m_count]);
}

bool heap::top( hypothesis*& ret) const {
		if( m_count > 0 ) {
				ret = (m_array[0]).ptr;
				return true;
		} else {
				ret = NULL;
				return false;
		}
}

void heap::push_bak( hypothesis* dat ){
		
		if( m_bcnt >= m_size ){
				std::cerr << "error, stack over flow\n";
				abort();
		}
		(m_backup[m_bcnt]).score = dat->getscore();
		(m_backup[m_bcnt]).ptr = dat;
		m_bcnt += 1;

}


