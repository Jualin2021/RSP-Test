#pragma once
#ifndef TempletFun
#define TempletFun

#include "Parameters.h"
#include "Classes.h"
//#include "DefGloVar.h"
#include "GlobalVar.h"

template <typename T>  T** Create2DAarray(const int dim1, const int dim2){
	T** array;
	array = new T*[dim1];
	for (int i = 0; i < dim1; i++) {
		array[i] = new T[dim2];
	}
	return (array);
}

template <typename T>
void Free2DArrey(T **Array, const int dim1) {

	for (int i = 0; i < dim1; i++) {
		delete[] Array[i];
	}
	delete[] Array;
}

template <typename T>
T *Create1DAarray(const int dim1) {
	T *array = new T[dim1];
	return (array);
}
template <typename T>
void Free1DArrey(T *Array) {

	delete[] Array;
}

template <typename T>
void DebugStop(T val, T tv){
	if (val >= tv - 0.1 && val < tv + 0.1)
	{
		cout << "debug stop" << endl;
		system("pause");
	}
}

template<typename T>
void PrintParaValue(char *Name, T Value)
{
	ParaSummary << left << setw(40) << Name;
	ParaSummary << right << Value << endl;
}

struct split
{
	enum empties_t { empties_ok, no_empties };
};

template <typename Container>
Container& splitf(
	Container&                            result,
	const typename Container::value_type& s,
	const typename Container::value_type& delimiters,
	split::empties_t                      empties = split::empties_ok)
{
	result.clear();
	size_t current;
	size_t next = (size_t)-1;
	do
	{
		if (empties == split::no_empties)
		{
			next = s.find_first_not_of(delimiters, next + 1);
			if (next == Container::value_type::npos) break;
			next -= 1;
		}
		current = next + 1;
		next = s.find_first_of(delimiters, current);
		result.push_back(s.substr(current, next - current));
	} while (next != Container::value_type::npos);
	return result;
}

template<typename T>
inline bool isEqual(const T &Rhs, const T &Lhs){
	if (std::abs(Rhs - Lhs) <= Zero) return true;
	else return false;
}
inline bool isEqual(const double &Rhs, const double &Lhs) {
	if (std::abs(Rhs - Lhs) <= Zero) return true;
	else return false;
}

#endif
