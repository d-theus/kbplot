#ifndef RAWDATA_HPP
#define RAWDATA_HPP

#include <exception>

template <typename T>
class RawData{
	/*
	 * Класс для извлечения данных из сложных пользовательских массивов
	 * Реализованы некоторые одноименные методы STL-контейнеров, чтобы использовать
	 * при конструировании DataSet
	 */
	unsigned char *data;
	size_t count, str_len, ox, oy;


	public:

	class iterator:public std::iterator<std::random_access_iterator_tag, Txy> {
		/*
		 * Необходим, чтобы использовать RawData как DataSet
		 */
		size_t strn;
		unsigned char *data;
		size_t count, str_len, ox, oy;


		public:
		iterator():
			strn(0),data(NULL), count(0),str_len(0),ox(0),oy(0){}

		iterator(unsigned char *data, size_t count, size_t str_len, size_t ox, size_t oy, size_t strn = 0):
			strn(strn),data(data), count(count),str_len(str_len),ox(ox),oy(oy){}

		iterator operator+(size_t n){
			size_t nstrn = strn + n;
			if(nstrn <= count){
				return iterator(data,
                                		count,
						str_len,
						ox,
						oy,
						strn+n
						)
			}
		}
		const iterator& operator++(){
			if (strn < count ){
				strn ++;
			}
			return *this;
		}
		bool operator==(const iterator &o)const{
		    	return this->strn == o.strn;
		}
		const iterator& operator+=(size_t n){
			size_t nstrn = strn + n;
			if(nstrn <= count){
				strn = nstrn;
			}
			return *this;
		}
		Txy operator* ()const{
                	if (data == NULL)
                        throw std::runtime_error("Dereferencing iterator pointing to NULL");
			size_t nstrn = strn == count ? count - 1 : strn;
			T *x = (T*)(data + nstrn*str_len + ox);
			T *y = (T*)(data + nstrn*str_len + oy);
			return Txy(*x,*y);
		}
		const iterator& operator= (const iterator &o){
			this->count = o.count;
			this->data = o.data;
			this->ox = o.ox;
			this->oy = o.oy;
			this->str_len = o.str_len;
			this->strn = o.strn;
			return *this;
		}
	};

	RawData(unsigned char *data, size_t count, size_t item_length, size_t o_x, size_t o_y):
		data(data), count(count), str_len(item_length), ox(o_x), oy(o_y) {};


	iterator begin(){return iterator(data,count,str_len,ox,oy,0);}

	iterator end(){return iterator(data,count,str_len,ox,oy,count);}

	size_t size()const{
		return count;
	}

};
#endif//RAWDATA_HPP
