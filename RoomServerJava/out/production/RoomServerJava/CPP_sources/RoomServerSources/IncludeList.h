#ifndef INCLUDE_LIST
#define INCLUDE_LIST
#include <iostream>

template <class T>
class List {
public:
	List<T>(int size = 1){
		m_values = new T[size];
		m_trueSize = size;
		m_size = 0;
	}

	List<T>(const List<T>& other) {
		m_size = other.m_size;
		m_trueSize = other.m_trueSize;
		m_values = new T[m_trueSize];
		for (int i = 0; i < m_size; i++)
		{
			m_values[i] = other.m_values[i];
		}
	}

	~List<T>() {
		delete[] m_values;
		m_values = NULL;
		m_trueSize = -1;
		m_size = -1;
	}

	void append(T value) {
		if(m_size >= m_trueSize){
			resize();
		}
		m_values[m_size] = value;
		m_size++;
	}

	void removeOne(T value) {
		for(int i = 0; i < m_size; i++){
			if(m_values[i] == value){
				for(int j = i+1; j < m_size; j++){
					m_values[j-1] = m_values[j];
				}
				m_size--;
				break;
			}
		}
	}

	int length() const{
		return m_size;
	}

	bool contains(T value) const{
		for(int i = 0; i < m_size; i++){
			if(m_values[i] == value){
				return true;
			}
		}
		return false;
	}

	void removeAt(int index){
		if(index >= 0 && index < m_size){
			for(int i = index+1; i < m_size; i++){
				m_values[i-1] = m_values[i];
			}
			m_size--;
		} else {
			throw 0x01;
		}
	}

	bool operator == (const List<T> &a) const{
		if(m_size != a.m_size){
			return false;
		}
		for(int i = 0; i < m_size; i++){
			if(m_values[i] != a.m_values[i]){
				return false;
			}
		}
		return true;
	}

	bool operator != (const List<T> &a) const{
		return !(*this == a);
	}

	List<T> deep_copy(){
		List<T> res(m_size);
		for(int i = 0; i < m_size; i++){
			res.m_values[i] = m_values[i];
		}
		res.m_size = m_size;
		res.m_trueSize = m_size;
		return res;
	}

	List<T> operator << (T value){
		append(value);
		return deep_copy();
	}

	T& operator [] (int index){
		if(index >= 0 && index < m_size){
			return m_values[index];
		} else{
			throw 0x01;
		}
	}

	void operator = (const List<T> &a) {
		this->m_size = a.m_size;
		this->m_trueSize = m_size;
		m_values = new T[m_trueSize];
		for (int i = 0; i < m_size; i++)
		{
			m_values[i] = a.m_values[i];
		}
	}

private:
	void resize(){
		m_trueSize *= 2;
		T* n_values = new T[m_trueSize];
		for(int i = 0; i < m_size; i++){
			n_values[i] = m_values[i];
		}
		delete[] m_values;
		m_values = n_values;
	}

	int m_size;
	int m_trueSize;
	T* m_values;
};
#define MafiaList List

#endif
