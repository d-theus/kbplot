#ifndef DATASET_HPP
#define DATASET_HPP

#include <iterator>
#include <typeinfo>
#include <vector>
#include <list>

#ifdef DEBUG
#include <iostream>
#endif

#include "rawdata.hpp"

class DataSet {
	/*
	 * Абстрактный класс, представляющий интерфейс работы с 
	 * набором данных.
	*/
	public:
		virtual void advance()=0;
		virtual void rewind()=0;
		virtual bool atEnd()const=0;
		virtual size_t size()const=0;
		virtual Txy current()const=0;

		virtual Txy operator[](size_t)=0;
		virtual ~DataSet(){};
};

template<class Container>
class DataSetT : public DataSet {
	/*
	 * Конкретные реализации существующими контейнерами интерфейса DataSet
	 */
	Container *data;
	typename Container::iterator cur;

public:

	DataSetT(Container *data):data(data){}

	virtual ~DataSetT (){}

	virtual void advance(){
		std::advance(cur,1);
	}
	virtual void rewind(){
		cur = data->begin();
	}
	virtual bool atEnd()const{
		return cur == data->end();
	}
	virtual Txy current()const{
		return *cur;
	}
	virtual Txy operator[](size_t id){
		typename Container::iterator i = data->begin();
		std::advance(i, id);
		return *i;
	}
	virtual size_t size()const{
		return data->size();
	}
};

typedef DataSetT<std::vector<Txy> > DSV;
typedef DataSetT<std::list<Txy> > DSL;
typedef DataSetT<RawData<double> > DSRd;
typedef DataSetT<RawData<int> > DSRi;
typedef DataSetT<RawData<unsigned short> > DSRus;

#endif //DATASET_HPP
